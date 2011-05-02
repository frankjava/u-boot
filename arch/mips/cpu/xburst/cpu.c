/*
 * (C) Copyright 2003
 * Wolfgang Denk, DENX Software Engineering, <wd@denx.de>
 *
 * See file CREDITS for list of people who contributed to this
 * project.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

#include <common.h>
#include <command.h>
#include <netdev.h>
#include <asm/mipsregs.h>
#include <asm/cacheops.h>
#include <asm/reboot.h>
#include <asm/io.h>
#include <asm/jz4740.h>

#define cache_op(op,addr)						\
	__asm__ __volatile__(						\
	"	.set	push					\n"	\
	"	.set	noreorder				\n"	\
	"	.set	mips3\n\t				\n"	\
	"	cache	%0, %1					\n"	\
	"	.set	pop					\n"	\
	:								\
	: "i" (op), "R" (*(unsigned char *)(addr)))

#if !defined (CONFIG_NAND_SPL) && !defined (CONFIG_MSC_SPL)

void __attribute__((weak)) _machine_restart(void)
{
	struct jz4740_wdt *wdt = (struct jz4740_wdt *) JZ4740_WDT_BASE;
	struct jz4740_tcu *tcu = (struct jz4740_tcu *) JZ4740_TCU_BASE;

	writew((readw(&wdt->tcsr) &
		~(WDT_TCSR_EXT_EN | WDT_TCSR_RTC_EN | WDT_TCSR_PCK_EN))
	       | WDT_TCSR_EXT_EN, &wdt->tcsr);
	/* wdt_select_extalclk() */

	writew((readw(&wdt->tcsr) &
		~WDT_TCSR_PRESCALE_MASK) | WDT_TCSR_PRESCALE64, &wdt->tcsr);
	/* wdt_select_clk_div64() */

	writew(100, &wdt->tdr); /* wdt_set_data(100) */
	writew(0, &wdt->tcnt); /* wdt_set_count(0); */
	writew(TCU_TSSR_WDTSC, &tcu->tscr); /* tcu_start_wdt_clock */
	writeb(readb(&wdt->tcer) | WDT_TCER_TCEN, &wdt->tcer); /* wdt start */

	while (1)
		;
}

int do_reset(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	_machine_restart();

	fprintf(stderr, "*** reset failed ***\n");
	return 0;
}

void flush_cache(ulong start_addr, ulong size)
{
	unsigned long lsize = CONFIG_SYS_CACHELINE_SIZE;
	unsigned long addr = start_addr & ~(lsize - 1);
	unsigned long aend = (start_addr + size - 1) & ~(lsize - 1);

	for (;addr <= aend; addr += lsize) {
		cache_op(Hit_Writeback_Inv_D, addr);
		cache_op(Hit_Invalidate_I, addr);
	}
}

void flush_dcache_range(ulong start_addr, ulong stop)
{
	unsigned long lsize = CONFIG_SYS_CACHELINE_SIZE;
	unsigned long addr = start_addr & ~(lsize - 1);
	unsigned long aend = (stop - 1) & ~(lsize - 1);

	for (;addr <= aend; addr += lsize)
		cache_op(Hit_Writeback_Inv_D, addr);
}

void invalidate_dcache_range(ulong start_addr, ulong stop)
{
	unsigned long lsize = CONFIG_SYS_CACHELINE_SIZE;
	unsigned long addr = start_addr & ~(lsize - 1);
	unsigned long aend = (stop - 1) & ~(lsize - 1);

	for (;addr <= aend; addr += lsize)
		cache_op(Hit_Invalidate_D, addr);
}

void write_one_tlb(int index, u32 pagemask, u32 hi, u32 low0, u32 low1)
{
	write_c0_entrylo0(low0);
	write_c0_pagemask(pagemask);
	write_c0_entrylo1(low1);
	write_c0_entryhi(hi);
	write_c0_index(index);
	tlb_write_indexed();
}

#endif /* !CONFIG_NAND_SPL  !CONFIG_MSC_SPL */

void flush_icache_all(void)
{
	u32 addr, t = 0;

	asm volatile ("mtc0 $0, $28"); /* Clear Taglo */
	asm volatile ("mtc0 $0, $29"); /* Clear TagHi */

	for (addr = KSEG0; addr < KSEG0 + CONFIG_SYS_ICACHE_SIZE;
	     addr += CONFIG_SYS_CACHELINE_SIZE) {
		cache_op(Index_Store_Tag_I, addr);
	}

	/* invalidate btb */
	asm volatile (
		".set mips32\n\t"
		"mfc0 %0, $16, 7\n\t"
		"nop\n\t"
		"ori %0,2\n\t"
		"mtc0 %0, $16, 7\n\t"
		".set mips2\n\t"
		:
		: "r" (t));
}

void flush_dcache_all(void)
{
	u32 addr;

	for (addr = KSEG0; addr < KSEG0 + CONFIG_SYS_DCACHE_SIZE;
	     addr += CONFIG_SYS_CACHELINE_SIZE) {
		cache_op(Index_Writeback_Inv_D, addr);
	}

	asm volatile ("sync");
}

void flush_cache_all(void)
{
	flush_dcache_all();
	flush_icache_all();
}
