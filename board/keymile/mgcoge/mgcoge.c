/*
 * (C) Copyright 2007 - 2008
 * Heiko Schocher, DENX Software Engineering, hs@denx.de.
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
#include <mpc8260.h>
#include <ioports.h>
#include <malloc.h>
#include <asm/io.h>

#if defined(CONFIG_OF_BOARD_SETUP) && defined(CONFIG_OF_LIBFDT)
#include <libfdt.h>
#endif

#if defined(CONFIG_HARD_I2C) || defined(CONFIG_SOFT_I2C)
#include <i2c.h>
#endif

#include "../common/common.h"

/*
 * I/O Port configuration table
 *
 * if conf is 1, then that port pin will be configured at boot time
 * according to the five values podr/pdir/ppar/psor/pdat for that entry
 */
const iop_conf_t iop_conf_tab[4][32] = {

    /* Port A */
    {	/*	      conf	ppar psor pdir podr pdat */
	/* PA31 */ { 0,		 0,   0,   0,	0,   0 }, /* PA31	     */
	/* PA30 */ { 0,		 0,   0,   0,	0,   0 }, /* PA30	     */
	/* PA29 */ { 0,		 0,   0,   0,	0,   0 }, /* PA29	     */
	/* PA28 */ { 0,		 0,   0,   0,	0,   0 }, /* PA28	     */
	/* PA27 */ { 0,		 0,   0,   0,	0,   0 }, /* PA27	     */
	/* PA26 */ { 0,		 0,   0,   0,	0,   0 }, /* PA26	     */
	/* PA25 */ { 0,		 0,   0,   0,	0,   0 }, /* PA25	     */
	/* PA24 */ { 0,		 0,   0,   0,	0,   0 }, /* PA24	     */
	/* PA23 */ { 0,		 0,   0,   0,	0,   0 }, /* PA23	     */
	/* PA22 */ { 0,		 0,   0,   0,	0,   0 }, /* PA22	     */
	/* PA21 */ { 0,		 0,   0,   0,	0,   0 }, /* PA21	     */
	/* PA20 */ { 0,		 0,   0,   0,	0,   0 }, /* PA20	     */
	/* PA19 */ { 0,		 0,   0,   0,	0,   0 }, /* PA19	     */
	/* PA18 */ { 0,		 0,   0,   0,	0,   0 }, /* PA18	     */
	/* PA17 */ { 0,		 0,   0,   0,	0,   0 }, /* PA17	     */
	/* PA16 */ { 0,		 0,   0,   0,	0,   0 }, /* PA16	     */
	/* PA15 */ { 0,		 0,   0,   0,	0,   0 }, /* PA15	     */
	/* PA14 */ { 0,		 0,   0,   0,	0,   0 }, /* PA14	     */
	/* PA13 */ { 0,		 0,   0,   0,	0,   0 }, /* PA13	     */
	/* PA12 */ { 0,		 0,   0,   0,	0,   0 }, /* PA12	     */
	/* PA11 */ { 0,		 0,   0,   0,	0,   0 }, /* PA11	     */
	/* PA10 */ { 0,		 0,   0,   0,	0,   0 }, /* PA10	     */
	/* PA9	*/ { 1,		 1,   0,   1,	0,   0 }, /* SMC2 TxD	     */
	/* PA8	*/ { 1,		 1,   0,   0,	0,   0 }, /* SMC2 RxD	     */
	/* PA7	*/ { 0,		 0,   0,   0,	0,   0 }, /* PA7	     */
	/* PA6	*/ { 0,		 0,   0,   0,	0,   0 }, /* PA6	     */
	/* PA5	*/ { 0,		 0,   0,   0,	0,   0 }, /* PA5	     */
	/* PA4	*/ { 0,		 0,   0,   0,	0,   0 }, /* PA4	     */
	/* PA3	*/ { 0,		 0,   0,   0,	0,   0 }, /* PA3	     */
	/* PA2	*/ { 0,		 0,   0,   0,	0,   0 }, /* PA2	     */
	/* PA1	*/ { 0,		 0,   0,   0,	0,   0 }, /* PA1	     */
	/* PA0	*/ { 0,		 0,   0,   0,	0,   0 }  /* PA0	     */
    },

    /* Port B */
    {	/*	      conf	ppar psor pdir podr pdat */
	/* PB31 */ { 0,		 0,   0,   0,	0,   0 }, /* PB31	     */
	/* PB30 */ { 0,		 0,   0,   0,	0,   0 }, /* PB30	     */
	/* PB29 */ { 0,		 0,   0,   0,	0,   0 }, /* PB29	     */
	/* PB28 */ { 0,		 0,   0,   0,	0,   0 }, /* PB28	     */
	/* PB27 */ { 0,		 0,   0,   0,	0,   0 }, /* PB27	     */
	/* PB26 */ { 0,		 0,   0,   0,	0,   0 }, /* PB26	     */
	/* PB25 */ { 0,		 0,   0,   0,	0,   0 }, /* PB25	     */
	/* PB24 */ { 0,		 0,   0,   0,	0,   0 }, /* PB24	     */
	/* PB23 */ { 0,		 0,   0,   0,	0,   0 }, /* PB23	     */
	/* PB22 */ { 0,		 0,   0,   0,	0,   0 }, /* PB22	     */
	/* PB21 */ { 0,		 0,   0,   0,	0,   0 }, /* PB21	     */
	/* PB20 */ { 0,		 0,   0,   0,	0,   0 }, /* PB20	     */
	/* PB19 */ { 0,		 0,   0,   0,	0,   0 }, /* PB19	     */
	/* PB18 */ { 0,		 0,   0,   0,	0,   0 }, /* PB18	     */
	/* PB17 */ { 0,		 0,   0,   0,	0,   0 }, /* non-existent    */
	/* PB16 */ { 0,		 0,   0,   0,	0,   0 }, /* non-existent    */
	/* PB15 */ { 0,		 0,   0,   0,	0,   0 }, /* non-existent    */
	/* PB14 */ { 0,		 0,   0,   0,	0,   0 }, /* non-existent    */
	/* PB13 */ { 0,		 0,   0,   0,	0,   0 }, /* non-existent    */
	/* PB12 */ { 0,		 0,   0,   0,	0,   0 }, /* non-existent    */
	/* PB11 */ { 0,		 0,   0,   0,	0,   0 }, /* non-existent    */
	/* PB10 */ { 0,		 0,   0,   0,	0,   0 }, /* non-existent    */
	/* PB9	*/ { 0,		 0,   0,   0,	0,   0 }, /* non-existent    */
	/* PB8	*/ { 0,		 0,   0,   0,	0,   0 }, /* non-existent    */
	/* PB7	*/ { 0,		 0,   0,   0,	0,   0 }, /* non-existent    */
	/* PB6	*/ { 0,		 0,   0,   0,	0,   0 }, /* non-existent    */
	/* PB5	*/ { 0,		 0,   0,   0,	0,   0 }, /* non-existent    */
	/* PB4	*/ { 0,		 0,   0,   0,	0,   0 }, /* non-existent    */
	/* PB3	*/ { 0,		 0,   0,   0,	0,   0 }, /* non-existent    */
	/* PB2	*/ { 0,		 0,   0,   0,	0,   0 }, /* non-existent    */
	/* PB1	*/ { 0,		 0,   0,   0,	0,   0 }, /* non-existent    */
	/* PB0	*/ { 0,		 0,   0,   0,	0,   0 }  /* non-existent    */
    },

    /* Port C */
    {	/*	      conf	ppar psor pdir podr pdat */
	/* PC31 */ { 0,		 0,   0,   0,	0,   0 }, /* PC31	     */
	/* PC30 */ { 0,		 0,   0,   0,	0,   0 }, /* PC30	     */
	/* PC29 */ { 0,		 0,   0,   0,	0,   0 }, /* PC29	     */
	/* PC28 */ { 0,		 0,   0,   0,	0,   0 }, /* PC28	     */
	/* PC27 */ { 0,		 0,   0,   0,	0,   0 }, /* PC27	     */
	/* PC26 */ { 0,		 0,   0,   0,	0,   0 }, /* PC26	     */
	/* PC25 */ { 1,		 1,   0,   0,	0,   0 }, /* SCC4 RxClk      */
	/* PC24 */ { 1,		 1,   0,   0,	0,   0 }, /* SCC4 TxClk      */
	/* PC23 */ { 0,		 0,   0,   0,	0,   0 }, /* PC23	     */
	/* PC22 */ { 0,		 0,   0,   0,	0,   0 }, /* PC22	     */
	/* PC21 */ { 0,		 0,   0,   0,	0,   0 }, /* PC21	     */
	/* PC20 */ { 0,		 0,   0,   0,	0,   0 }, /* PC20	     */
	/* PC19 */ { 0,		 0,   0,   0,	0,   0 }, /* PC19	     */
	/* PC18 */ { 0,		 0,   0,   0,	0,   0 }, /* PC18	     */
	/* PC17 */ { 0,		 0,   0,   0,	0,   0 }, /* PC17	     */
	/* PC16 */ { 0,		 0,   0,   0,	0,   0 }, /* PC16	     */
	/* PC15 */ { 0,		 0,   0,   0,	0,   0 }, /* PC15	     */
	/* PC14 */ { 0,		 0,   0,   0,	0,   0 }, /* PC14	     */
	/* PC13 */ { 0,		 0,   0,   0,	0,   0 }, /* PC13	     */
	/* PC12 */ { 0,		 0,   0,   0,	0,   0 }, /* PC12	     */
	/* PC11 */ { 0,		 0,   0,   0,	0,   0 }, /* PC11	     */
	/* PC10 */ { 0,		 0,   0,   0,	0,   0 }, /* PC10	     */
	/* PC9	*/ { 1,		 1,   0,   0,	0,   0 }, /* SCC4: CTS	     */
	/* PC8	*/ { 1,		 1,   0,   0,	0,   0 }, /* SCC4: CD	     */
	/* PC7	*/ { 0,		 0,   0,   0,	0,   0 }, /* PC7	     */
	/* PC6	*/ { 0,		 0,   0,   0,	0,   0 }, /* PC6	     */
	/* PC5	*/ { 0,		 0,   0,   0,	0,   0 }, /* PC5	     */
	/* PC4	*/ { 0,		 0,   0,   0,	0,   0 }, /* PC4	     */
	/* PC3	*/ { 0,		 0,   0,   0,	0,   0 }, /* PC3	     */
	/* PC2	*/ { 0,		 0,   0,   0,	0,   0 }, /* PC2	     */
	/* PC1	*/ { 0,		 0,   0,   0,	0,   0 }, /* PC1	     */
	/* PC0	*/ { 0,		 0,   0,   0,	0,   0 }, /* PC0	     */
    },

    /* Port D */
    {	/*	      conf	ppar psor pdir podr pdat */
	/* PD31 */ { 0,		 0,   0,   0,	0,   0 }, /* PD31	     */
	/* PD30 */ { 0,		 0,   0,   0,	0,   0 }, /* PD30	     */
	/* PD29 */ { 0,		 0,   0,   0,	0,   0 }, /* PD29	     */
	/* PD28 */ { 0,		 0,   0,   0,	0,   0 }, /* PD28	     */
	/* PD27 */ { 0,		 0,   0,   0,	0,   0 }, /* PD27	     */
	/* PD26 */ { 0,		 0,   0,   0,	0,   0 }, /* PD26	     */
	/* PD25 */ { 0,		 0,   0,   0,	0,   0 }, /* PD25	     */
	/* PD24 */ { 0,		 0,   0,   0,	0,   0 }, /* PD24	     */
	/* PD23 */ { 0,		 0,   0,   0,	0,   0 }, /* PD23	     */
	/* PD22 */ { 1,		 1,   0,   0,	0,   0 }, /* SCC4: RXD	     */
	/* PD21 */ { 1,		 1,   0,   1,	0,   0 }, /* SCC4: TXD	     */
	/* PD20 */ { 1,		 1,   0,   1,	0,   0 }, /* SCC4: RTS	     */
	/* PD19 */ { 0,		 0,   0,   0,	0,   0 }, /* PD19	     */
	/* PD18 */ { 0,		 0,   0,   0,	0,   0 }, /* PD18	     */
	/* PD17 */ { 0,		 0,   0,   0,	0,   0 }, /* PD17	     */
	/* PD16 */ { 0,		 0,   0,   0,	0,   0 }, /* PD16	     */
#if defined(CONFIG_HARD_I2C)
	/* PD15 */ { 1,		 1,   1,   0,	1,   0 }, /* I2C SDA	     */
	/* PD14 */ { 1,		 1,   1,   0,	1,   0 }, /* I2C SCL	     */
#else
	/* PD15 */ { 1,		 0,   0,   0,	1,   1 }, /* PD15	     */
	/* PD14 */ { 1,		 0,   0,   1,	1,   1 }, /* PD14	     */
#endif
	/* PD13 */ { 0,		 0,   0,   0,	0,   0 }, /* PD13	     */
	/* PD12 */ { 0,		 0,   0,   0,	0,   0 }, /* PD12	     */
	/* PD11 */ { 0,		 0,   0,   0,	0,   0 }, /* PD11	     */
	/* PD10 */ { 0,		 0,   0,   0,	0,   0 }, /* PD10	     */
	/* PD9	*/ { 0,		 0,   0,   0,	0,   0 }, /* PD9	     */
	/* PD8	*/ { 0,		 0,   0,   0,	0,   0 }, /* PD8	     */
	/* PD7	*/ { 0,		 0,   0,   0,	0,   0 }, /* PD7	     */
	/* PD6	*/ { 0,		 0,   0,   0,	0,   0 }, /* PD6	     */
	/* PD5	*/ { 0,		 0,   0,   0,	0,   0 }, /* PD5	     */
	/* PD4	*/ { 0,		 0,   0,   0,	0,   0 }, /* PD4	     */
	/* PD3	*/ { 0,		 0,   0,   0,	0,   0 }, /* non-existent    */
	/* PD2	*/ { 0,		 0,   0,   0,	0,   0 }, /* non-existent    */
	/* PD1	*/ { 0,		 0,   0,   0,	0,   0 }, /* non-existent    */
	/* PD0	*/ { 0,		 0,   0,   0,	0,   0 }  /* non-existent    */
    }
};

/*
 * Try SDRAM initialization with P/LSDMR=sdmr and ORx=orx
 *
 * This routine performs standard 8260 initialization sequence
 * and calculates the available memory size. It may be called
 * several times to try different SDRAM configurations on both
 * 60x and local buses.
 */
static long int try_init(memctl8260_t *memctl, ulong sdmr,
				  ulong orx, uchar *base)
{
	uchar c = 0xff;
	ulong maxsize, size;
	int i;

	/*
	 * We must be able to test a location outsize the maximum legal size
	 * to find out THAT we are outside; but this address still has to be
	 * mapped by the controller. That means, that the initial mapping has
	 * to be (at least) twice as large as the maximum expected size.
	 */
	maxsize = (1 + (~orx | 0x7fff))/* / 2*/;

	out_be32(&memctl->memc_or1, orx);

	/*
	 * Quote from 8260 UM (10.4.2 SDRAM Power-On Initialization, 10-35):
	 *
	 * "At system reset, initialization software must set up the
	 *  programmable parameters in the memory controller banks registers
	 *  (ORx, BRx, P/LSDMR). After all memory parameters are configured,
	 *  system software should execute the following initialization sequence
	 *  for each SDRAM device.
	 *
	 *  1. Issue a PRECHARGE-ALL-BANKS command
	 *  2. Issue eight CBR REFRESH commands
	 *  3. Issue a MODE-SET command to initialize the mode register
	 *
	 *  The initial commands are executed by setting P/LSDMR[OP] and
	 *  accessing the SDRAM with a single-byte transaction."
	 *
	 * The appropriate BRx/ORx registers have already been set when we
	 * get here. The SDRAM can be accessed at the address CONFIG_SYS_SDRAM_BASE.
	 */

	out_be32(&memctl->memc_psdmr, sdmr | PSDMR_OP_PREA);
	out_8(base, c);

	out_be32(&memctl->memc_psdmr, sdmr | PSDMR_OP_CBRR);
	for (i = 0; i < 8; i++)
		out_8(base, c);

	out_be32(&memctl->memc_psdmr, sdmr | PSDMR_OP_MRW);
	/* setting MR on address lines */
	out_8((uchar *)(base + CONFIG_SYS_MRS_OFFS), c);

	out_be32(&memctl->memc_psdmr, sdmr | PSDMR_OP_NORM | PSDMR_RFEN);
	out_8(base, c);

	size = get_ram_size((long *)base, maxsize);
	out_be32(&memctl->memc_or1, orx | ~(size - 1));

	return (size);
}

phys_size_t initdram(int board_type)
{
	immap_t *immap = (immap_t *) CONFIG_SYS_IMMR;
	memctl8260_t *memctl = &immap->im_memctl;

	long psize;

	out_8(&memctl->memc_psrt, CONFIG_SYS_PSRT);
	out_be16(&memctl->memc_mptpr, CONFIG_SYS_MPTPR);

#ifndef CONFIG_SYS_RAMBOOT
	/* 60x SDRAM setup:
	 */
	psize = try_init(memctl, CONFIG_SYS_PSDMR, CONFIG_SYS_OR1,
				  (uchar *) CONFIG_SYS_SDRAM_BASE);
#endif /* CONFIG_SYS_RAMBOOT */

	icache_enable();

	return (psize);
}

int checkboard(void)
{
#if defined(CONFIG_MGCOGE)
	puts("Board: Keymile mgcoge");
#else
	puts("Board: Keymile mgcoge2ne");
#endif
	if (ethernet_present())
		puts(" with PIGGY.");
	puts("\n");
	return 0;
}

#define DIPSWITCH_OFFSET 0x89
#define DIPSWITCH_MASK   0x0f

int last_stage_init(void)
{
	u8 dip_switch;
	/* Dip switch */
	dip_switch = readb(CONFIG_SYS_BFTICU_BASE + DIPSWITCH_OFFSET);
	dip_switch &= DIPSWITCH_MASK;
	/* dip switch 'full reset' or 'db erase' */
	if (dip_switch & 0x1 || dip_switch & 0x2) {
		/* start bootloader */
		puts("DIP:   Enabled\n");
		setenv("actual_bank", "0");
	}
	set_km_env();
	return 0;
}

/*
 * Early board initalization.
 */
int board_early_init_r(void)
{
	struct km_bec_fpga *base =
		(struct km_bec_fpga *)CONFIG_SYS_KMBEC_FPGA_BASE;

	/* setup the UPIOx */
	/* General Unit Reset disabled, Flash Bank enabled, UnitLed on */
	out_8(&base->oprth, (WRG_RESET | H_OPORTS_14 | WRG_LED));
	/* SCC4 enable, halfduplex, FCC1 powerdown */
	out_8(&base->oprtl, (H_OPORTS_SCC4_ENA | H_OPORTS_SCC4_FD_ENA |
		H_OPORTS_FCC1_PW_DWN));

	return 0;
}

int hush_init_var(void)
{
	ivm_read_eeprom();
	return 0;
}

#if defined(CONFIG_OF_BOARD_SETUP) && defined(CONFIG_OF_LIBFDT)
void ft_board_setup(void *blob, bd_t *bd)
{
	ft_cpu_setup(blob, bd);
}
#endif /* defined(CONFIG_OF_BOARD_SETUP) && defined(CONFIG_OF_LIBFDT) */
