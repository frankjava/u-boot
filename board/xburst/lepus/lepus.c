/*
 * (C) Copyright 2006
 * Ingenic Semiconductor, <jlwei@ingenic.cn>
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
#include <asm/jz4760.h>

void board_early_init(void)
{
	__gpio_as_nand_16bit(1);

#if CONFIG_SYS_UART_BASE == UART0_BASE
	__gpio_as_uart0();
#elif CONFIG_SYS_UART_BASE == UART1_BASE
	__gpio_as_uart1();
#elif CONFIG_SYS_UART_BASE == UART2_BASE
	__gpio_as_uart2();
#else 
	__gpio_as_uart3();
#endif
}

/* U-Boot common routines */
int checkboard (void)
{
	DECLARE_GLOBAL_DATA_PTR;

	printf("Board: Ingenic LEPUS(CPU Speed %d MHz)\n",
	       gd->cpu_clk/1000000);

	return 0;
}
