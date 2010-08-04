/*
 *  Startup Code for MIPS32 XBURST CPU-core
 *
 *  Copyright (c) 2010 Xiangfu Liu <xiangfu@sharism.cc>
 *
 * See file CREDITS for list of people who contributed to this
 * project.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 3 of
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
	
#include <config.h>
#include <version.h>
#include <asm/regdef.h>
#include <asm/mipsregs.h>
#include <asm/addrspace.h>
#include <asm/cacheops.h>

#include <asm/jz4740.h>

	.set noreorder
	
	.globl _start
	.text
_start:
#if defined(CONFIG_JZ4740)
	.word JZ4740_NANDBOOT_CFG /* fetched during NAND Boot */
#endif
#if defined(CONFIG_JZ4760)
	/* First 3 words fetched by CPU during NAND Boot
	 * First 4 flags fetched by CPU during NAND Boot
	 *	Flag name		Length(in byte)		Location(in byte)
	 * 	buswidth_flag		64			0
	 * 	rowcycle_flag		32			64
	 * 	pagesize_flag1		32			96
	 * 	pagesize_flag0		32			128
	 *
	 *	buswidth_flag	--	bus width(8/16bit)
	 * 	0x55			8bit
	 * 	0xaa			16bit
	 *	rowcycle_flag	--	row cycle(2/3)
	 *	0x55			2
	 *	0xaa			3
	 *	pagesize_flag1,		pagesize_flag0	--	pagesize(byte)
	 * 	0x55			0x55			512
	 *	0x55			0xaa			2048
	 *	0xaa			0x55			4096
	 *	0xaa			0xaa			8192
	 */

	/* buswidth_flag */
#if (CONFIG_NAND_BW8 == 1)
	.word	0x55555555
	.word	0x55555555
	.word	0x55555555
	.word	0x55555555
	.word	0x55555555
	.word	0x55555555
	.word	0x55555555
	.word	0x55555555
	.word	0x55555555
	.word	0x55555555
	.word	0x55555555
	.word	0x55555555
	.word	0x55555555
	.word	0x55555555
	.word	0x55555555
	.word	0x55555555
#else
	.word	0xaaaaaaaa
	.word	0xaaaaaaaa
	.word	0xaaaaaaaa
	.word	0xaaaaaaaa
	.word	0xaaaaaaaa
	.word	0xaaaaaaaa
	.word	0xaaaaaaaa
	.word	0xaaaaaaaa
	.word	0xaaaaaaaa
	.word	0xaaaaaaaa
	.word	0xaaaaaaaa
	.word	0xaaaaaaaa
	.word	0xaaaaaaaa
	.word	0xaaaaaaaa
	.word	0xaaaaaaaa
	.word	0xaaaaaaaa	
#endif
	/* rowcycle_flag */
#if (CONFIG_NAND_ROW_CYCLE == 3)
	.word	0xaaaaaaaa
	.word	0xaaaaaaaa
	.word	0xaaaaaaaa
	.word	0xaaaaaaaa
	.word	0xaaaaaaaa
	.word	0xaaaaaaaa
	.word	0xaaaaaaaa
	.word	0xaaaaaaaa	
#else
	.word	0x55555555
	.word	0x55555555
	.word	0x55555555
	.word	0x55555555
	.word	0x55555555
	.word	0x55555555
	.word	0x55555555
	.word	0x55555555
#endif
	/* pagesize_flag1 */
#if (CONFIG_NAND_PAGE_SIZE < 4096)
	.word	0x55555555
	.word	0x55555555
	.word	0x55555555
	.word	0x55555555
	.word	0x55555555
	.word	0x55555555
	.word	0x55555555
	.word	0x55555555	
#else
	.word	0xaaaaaaaa
	.word	0xaaaaaaaa
	.word	0xaaaaaaaa
	.word	0xaaaaaaaa
	.word	0xaaaaaaaa
	.word	0xaaaaaaaa
	.word	0xaaaaaaaa
	.word	0xaaaaaaaa	
#endif
	/* pagesize_flag0 */
#if ((CONFIG_NAND_PAGE_SIZE == 512) || (CONFIG_NAND_PAGE_SIZE == 4096))
	.word	0x55555555
	.word	0x55555555
	.word	0x55555555
	.word	0x55555555
	.word	0x55555555
	.word	0x55555555
	.word	0x55555555
	.word	0x55555555	
#else
	.word	0xaaaaaaaa
	.word	0xaaaaaaaa
	.word	0xaaaaaaaa
	.word	0xaaaaaaaa
	.word	0xaaaaaaaa
	.word	0xaaaaaaaa
	.word	0xaaaaaaaa
	.word	0xaaaaaaaa
#endif
#endif /* CONFIG_JZ4760 */
	
reset:	
	/* 
	 * STATUS register
	 * CU0=UM=EXL=IE=0, BEV=ERL=1, IP2~7=1
	 */
	li	t0, 0x0040FC04
	mtc0	t0, CP0_STATUS
	/* CAUSE register
	 * IV=1, use the specical interrupt vector (0x200) */
	li	t1, 0x00800000
	mtc0	t1, CP0_CAUSE
	
	bal     1f
	nop
	.word   _GLOBAL_OFFSET_TABLE_
1:
	move    gp, ra
	lw      t1, 0(ra)
	move	gp, t1

	la	sp, 0x80004000
	la	t9, nand_boot
	j	t9
	nop
