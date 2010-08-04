/*
 * (C) Copyright 2008  Ingenic Semiconductor
 * 
 *  Author: <lhhuang@ingenic.cn>
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

#ifndef __LEPUS_H__
#define __LEPUS_H__

#define DEBUG

#define CONFIG_MIPS32		1  /* MIPS32 CPU core */
#define CONFIG_JzRISC		1  /* JzRISC core */
#define CONFIG_JZSOC		1  /* Jz SoC */
#define CONFIG_JZ4760		1  /* Jz4760 SoC */
#define CONFIG_LEPUS		1  /* f4760 validation board */
#define CONFIG_DDRC
#define CONFIG_USE_DDR2
#define CONFIG_SDRAM_DDR2

#define CONFIG_SYS_EXTAL	12000000
#define CONFIG_SYS_CPU_SPEED	(144 * 1000000)
#define	CONFIG_SYS_HZ		(CONFIG_SYS_EXTAL / 256)/* incrementer freq */

#include "jz4760_common.h"

#define CONFIG_SYS_UART_BASE	UART1_BASE	/* Base of the UART channel */
#define CONFIG_BAUDRATE		57600
#define CONFIG_SYS_BAUDRATE_TABLE	{ 9600, 19200, 38400, 57600, 115200 }

/* allow to overwrite serial and ethaddr */
#define CONFIG_ENV_OVERWRITE

#define CONFIG_CMD_BOOTD	/* bootd			*/
#define CONFIG_CMD_CONSOLE	/* coninfo			*/
#define CONFIG_CMD_ECHO		/* echo arguments		*/

#define CONFIG_CMD_LOADB	/* loadb			*/
#define CONFIG_CMD_LOADS	/* loads			*/
#define CONFIG_CMD_MEMORY	/* md mm nm mw cp cmp crc base loop mtest */
#define CONFIG_CMD_MISC		/* Misc functions like sleep etc*/
#define CONFIG_CMD_RUN		/* run command in env variable	*/
#define CONFIG_CMD_ASKENV 
#define CONFIG_CMD_SAVEENV 
#define CONFIG_CMD_SETGETDCR	/* DCR support on 4xx		*/
#define CONFIG_CMD_SOURCE	/* "source" command support	*/

#define CONFIG_CMD_NAND
#define CONFIG_CMD_FAT
#define CONFIG_DOS_PARTITION	1
#define CONFIG_CMD_DFL
#define CONFIG_CMD_DHCP   
#define CONFIG_CMD_PING
#define CONFIG_BOOTP_MASK	( CONFIG_BOOTP_DEFAUL )

#define CONFIG_BOOTDELAY	5
#define CONFIG_BOOTFILE	        "uImage"	/* file to load */
#define CONFIG_BOOTCOMMAND      "nand read 0x80600000 0x400000 0x300000;bootm"
#define CONFIG_AUTOLOAD		"n"		/* No autoload */

#define CONFIG_NET_MULTI
#define CONFIG_ETHADDR		00:2a:c6:7a:ac:de

/*
 * Serial download configuration
 */
#define CONFIG_LOADS_ECHO	1 /* echo on for serial download */
#define CONFIG_LOADS_BAUD_CHANGE	1 /* allow baudrate change */

/*
 * Miscellaneous configurable options
 */
#define	CONFIG_SYS_LONGHELP				/* undef to save memory      */
#define	CONFIG_SYS_PROMPT	"LEPUS # "	/* Monitor Command Prompt */
#define	CONFIG_SYS_CBSIZE	256		/* Console I/O Buffer Size */
#define	CONFIG_SYS_PBSIZE	(CONFIG_SYS_CBSIZE+sizeof(CONFIG_SYS_PROMPT)+16)
				/* Print Buffer Size */
#define	CONFIG_SYS_MAXARGS	16		/* max number of command args*/

#define CONFIG_SYS_MALLOC_LEN	896*1024
#define CONFIG_SYS_BOOTPARAMS_LEN	128*1024

#define CONFIG_SYS_SDRAM_BASE	0x80000000     /* Cached addr */
#define CONFIG_SYS_INIT_SP_OFFSET	0x400000

#define	CONFIG_SYS_LOAD_ADDR		0x80600000     /* default load address	*/

#define CONFIG_SYS_MEMTEST_START	0x80100000
#define CONFIG_SYS_MEMTEST_END		0x80800000

#define CONFIG_SYS_RX_ETH_BUFFER	16	/* use 16 rx buffers on jz47xx eth */

#define CONFIG_NAND_BW8	1               /* Data bus width: 0-16bit, 1-8bit */
#define CONFIG_NAND_PAGE_SIZE   4096
#define CONFIG_NAND_ROW_CYCLE	3
#define CONFIG_NAND_BLOCK_SIZE	(512 << 10)	/* NAND chip block size	*/
#define CONFIG_NAND_BADBLOCK_PAGE	127	/* NAND bad block was marked
						 * at this page in a block,
						 * starting from 0 */
#define CONFIG_ENV_BLOCK_SIZE	CONFIG_NAND_BLOCK_SIZE

/*
 * Configurable options for zImage if SPL is to load zImage
 */
#define CONFIG_LOAD_UBOOT       /* If it's defined, then spl load u-boot instead of zImage, and following options isn't used */
#define PARAM_BASE		0x80004000      /* The base of parameters which will be sent to kernel zImage */
#define CONFIG_ZIMAGE_SIZE	(2 << 20)	/* Size of kernel zImage */
#define CONFIG_ZIMAGE_DST	0x80100000	/* Load kernel zImage to this addr */
#define CONFIG_ZIMAGE_START	CONFIG_ZIMAGE_DST	/* Start kernel zImage from this addr	*/
#define CONFIG_CMDLINE		CONFIG_BOOTARGS
#define CONFIG_NAND_ZIMAGE_OFFS	(CONFIG_NAND_BLOCK_SIZE*4) /* NAND offset of zImage being loaded */
#define CONFIG_SPI_ZIMAGE_OFFS	(256 << 10) /* NAND offset of zImage being loaded */

/*-----------------------------------------------------------------------
 * Environment
 *----------------------------------------------------------------------*/
#if !defined(CONFIG_NAND_U_BOOT) && !defined(CONFIG_NAND_SPL)
#define CONFIG_ENV_IS_IN_FLASH	1 /* use FLASH for environment vars */
#else
#define CONFIG_ENV_IS_IN_NAND	1 /* use NAND for environment vars  */
#endif

#define CONFIG_NAND_BCH_BIT        8  /* Specify the hardware BCH algorithm for 4760 (4|8) */
#define CONFIG_SYS_NAND_ECC_POS    3 /* Ecc offset position in oob area, its default value is 3 if it isn't defined. */
#define CONFIG_NAND_SMCR1          0x0d444400      /* 0x0fff7700 is slowest */
#define CONFIG_NAND_USE_PN         0               /* Use PN in jz4760 for TLC NAND */
#define CONFIG_NAND_BACKUP_NUM     1               /* TODO */

#define CONFIG_SYS_MAX_NAND_DEVICE     1
#define CONFIG_SYS_NAND_BASE           0xBA000000
#define CONFIG_NAND_SELECT_DEVICE  1       /* nand driver supports mutipl. chips   */
#define NAND_MAX_CHIPS          1
#define NAND_ADDR_OFFSET        0x00800000
#define NAND_CMD_OFFSET         0x00400000

/*
 * IPL (Initial Program Loader, integrated inside CPU)
 * Will load first 8k from NAND (SPL) into cache and execute it from there.
 *
 * SPL (Secondary Program Loader)
 * Will load special U-Boot version (NUB) from NAND and execute it. This SPL
 * has to fit into 8kByte. It sets up the CPU and configures the SDRAM
 * controller and the NAND controller so that the special U-Boot image can be
 * loaded from NAND to SDRAM.
 *
 * NUB (NAND U-Boot)
 * This NAND U-Boot (NUB) is a special U-Boot version which can be started
 * from RAM. Therefore it mustn't (re-)configure the SDRAM controller.
 *
 */
#define CONFIG_NAND_U_BOOT_DST	 0xa0100000  /* Load NUB to this addr	*/
#define CONFIG_NAND_U_BOOT_START 0x80100000

/*
 * Define the partitioning of the NAND chip (only RAM U-Boot is needed here)
 */
#define CONFIG_NAND_U_BOOT_OFFS	(CONFIG_NAND_BLOCK_SIZE * (CONFIG_NAND_BACKUP_NUM+1))	/* Offset to U-Boot image */

/* Size of U-Boot image */
#if CONFIG_NAND_BLOCK_SIZE > (512 << 10)
#define CONFIG_NAND_U_BOOT_SIZE	CONFIG_NAND_BLOCK_SIZE
#else
#define CONFIG_NAND_U_BOOT_SIZE	(512 << 10)
#endif

#ifdef CONFIG_ENV_IS_IN_NAND
#define CONFIG_ENV_SIZE	CONFIG_NAND_BLOCK_SIZE
#define CONFIG_ENV_OFFSET	(CONFIG_NAND_U_BOOT_OFFS + CONFIG_NAND_U_BOOT_SIZE)
#define CONFIG_ENV_OFFSET_REDUND	(CONFIG_ENV_OFFSET + CONFIG_ENV_BLOCK_SIZE)
#endif

/*-----------------------------------------------------------------------
 * SPI NOR FLASH configuration
 */
#define JZ4760_NORBOOT_CONFIG	JZ4760_NORBOOT_8BIT	/* NOR Boot config code */
#define CONFIG_SPI_MAX_FREQ	1000000
#define CONFIG_SPI_U_BOOT_DST	0x80100000	/* Load NUB to this addr	*/
#define CONFIG_SPI_U_BOOT_START	CONFIG_SPI_U_BOOT_DST
#define CONFIG_SPI_U_BOOT_OFFS	(8 << 10)
#define CONFIG_SPI_U_BOOT_SIZE	(256 << 10)

/*-----------------------------------------------------------------------
 * FLASH and environment organization
 */
#define CONFIG_SYS_MAX_FLASH_BANKS	1	/* max number of memory banks */
#define CONFIG_SYS_MAX_FLASH_SECT	(128)	/* max number of sectors on one chip */

#define PHYS_FLASH_1		0xB8000000 /* Flash Bank #1 */

/* The following #defines are needed to get flash environment right */
#define	CONFIG_SYS_MONITOR_BASE	TEXT_BASE
#define	CONFIG_SYS_MONITOR_LEN	(256*1024)  /* Reserve 256 kB for Monitor */

#define CONFIG_SYS_FLASH_BASE	PHYS_FLASH_1
/* Environment settings */
#ifdef CONFIG_ENV_IS_IN_FLASH

#define CONFIG_SYS_ENV_SECT_SIZE	0x20000 /* Total Size of Environment Sector */
#define CONFIG_ENV_SIZE		CONFIG_ENV_SECT_SIZE
#endif
#define CONFIG_ENV_ADDR		0xB8040000

#define CONFIG_DIRECT_FLASH_TFTP	1	/* allow direct tftp to flash */

/*-----------------------------------------------------------------------
 * Cache Configuration
 */
#define CONFIG_SYS_DCACHE_SIZE	16384
#define CONFIG_SYS_ICACHE_SIZE	16384
#define CONFIG_SYS_CACHELINE_SIZE	32

/*--------------------------------------------------------------------------------
 * DDR2 info
 */
/* Chip Select */
#define DDR_CS1EN 0 // CSEN : whether a ddr chip exists 0 - un-used, 1 - used
#define DDR_CS0EN 1
#define DDR_DW32 1 /* 0 - 16-bit data width, 1 - 32-bit data width */

/* DDR2 paramters */
#define DDR_ROW 13 /* ROW : 12 to 14 row address */
#define DDR_COL 10 /* COL :  8 to 10 column address */
#define DDR_BANK8 1 /* Banks each chip: 0-4bank, 1-8bank */
#define DDR_CL 3 /* CAS latency: 1 to 7 */

/*
 * DDR2 controller timing1 register
 */
#define DDR_tRAS 45 /*tRAS: ACTIVE to PRECHARGE command period to the same bank. */
#define DDR_tRTP 8 /* 7.5ns READ to PRECHARGE command period. */
#define DDR_tRP 42 /* tRP: PRECHARGE command period to the same bank */
#define DDR_tRCD 42 /* ACTIVE to READ or WRITE command period to the same bank. */
#define DDR_tRC 60 /* ACTIVE to ACTIVE command period to the same bank.*/
#define DDR_tRRD 8   /* ACTIVE bank A to ACTIVE bank B command period. */
#define DDR_tWR 15 /* WRITE Recovery Time defined by register MR of DDR2 memory */
#define DDR_tWTR 2 /* unit: tCK. WRITE to READ command delay. */

/*
 * DDR2 controller timing2 register
 */
#define DDR_tRFC 128 /* ns,  AUTO-REFRESH command period. */
#define DDR_tMINSR 6 /* Minimum Self-Refresh / Deep-Power-Down */
#define DDR_tXP 2 /* EXIT-POWER-DOWN to next valid command period: 1 to 8 tCK. */
#define DDR_tMRD 2 /* unit: tCK. Load-Mode-Register to next valid command period: 1 to 4 tCK */

/*
 * DDR2 controller refcnt register
 */
#define DDR_tREFI 7800	/* Refresh period: ns */

#define DDR_CLK_DIV 1    /* Clock Divider. auto refresh
			  * cnt_clk = memclk/(16*(2^DDR_CLK_DIV)) */

/* GPIO */
#define GPIO_LCD_PWM	(32*2+14)	/* GPE14 PWM4 */

#define  CONFIG_MTD_DEBUG
#define CONFIG_MTD_DEBUG_VERBOSE 2
#endif	/* __LEPUS_H */
