/*
 * Platform independend driver for JZ4760.
 *
 * Copyright (c) 2007 Ingenic Semiconductor Inc.
 * Author: <jlwei@ingenic.cn>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 */

#include <common.h>

#if defined(CONFIG_CMD_NAND) && defined(CONFIG_JZ4760)
#include <nand.h>
#include <asm/jz4760.h>
#include <asm/io.h>

#define NAND_DATA_PORT1	       0xBA000000	/* read-write area in static bank 1 */
#define NAND_DATA_PORT2	       0xB4000000	/* read-write area in static bank 2 */
#define NAND_DATA_PORT3	       0xAC000000	/* read-write area in static bank 3 */
#define NAND_DATA_PORT4	       0xA8000000	/* read-write area in static bank 4 */

#define NAND_ADDR_OFFSET0       0x00800000      /* address port offset for share mode */
#define NAND_CMD_OFFSET0        0x00400000      /* command port offset for share mode */
#define NAND_ADDR_OFFSET1       0x00000008      /* address port offset for unshare mode */
#define NAND_CMD_OFFSET1        0x00000004      /* command port offset for unshare mode */

#define JZ_NAND_CTRL_ENABLE_CHIP(x) BIT(x << 1)
#define JZ_NAND_CTRL_ASSERT_CHIP(x) BIT((x << 1) + 1)

#define JZ_NAND_DATA_ADDR ((void __iomem *)0xBA000000)
#define JZ_NAND_CMD_ADDR (JZ_NAND_DATA_ADDR + 0x400000)
#define JZ_NAND_ADDR_ADDR (JZ_NAND_DATA_ADDR + 0x800000)

static int par_size;

struct nand_ecclayout ecclayout_2gb = {
	.eccbytes = 104,
	.eccpos = {
		3,  4,  5,  6,  7,  8,  9,  10,
		11, 12, 13, 14, 15, 16, 17, 18,
		19, 20, 21, 22, 23, 24, 25, 26,
		27, 28, 29, 30, 31, 32, 33, 34,
		35, 36, 37, 38, 39, 40, 41, 42,
		43, 44, 45, 46, 47, 48, 49, 50,
		51, 52, 53, 54 ,55, 56, 57, 58,
		59, 60, 61, 62, 63, 64, 65, 66,
		67, 68, 69, 70, 71, 72, 73, 74,
		75, 76, 77, 78, 79, 80, 81, 82,
		83, 84, 85, 86, 87, 88, 89, 90,
		91, 92, 93, 94, 95, 96, 97, 98, 
		99, 100, 101, 102, 103, 104, 105, 106},
	.oobfree = {{2, 1}, {107, 21}}
};

static void jz_hwcontrol(struct mtd_info *mtd, int cmd, unsigned int ctrl)
{
	struct jz_nand *nand =  mtd->priv;
	struct nand_chip *chip = mtd->priv;
	uint32_t reg;

	if (ctrl & NAND_CTRL_CHANGE) {
		if (ctrl & NAND_ALE)
			chip->IO_ADDR_W = JZ_NAND_ADDR_ADDR;
		else if (ctrl & NAND_CLE)
			chip->IO_ADDR_W = JZ_NAND_CMD_ADDR;
		else
			chip->IO_ADDR_W = JZ_NAND_DATA_ADDR;

		if (ctrl & NAND_NCE)
			REG_NEMC_NFCSR |= NEMC_NFCSR_NFCE1;
		else
			REG_NEMC_NFCSR &= ~NEMC_NFCSR_NFCE1;
	}

	if (cmd != NAND_CMD_NONE)
		writeb(cmd, chip->IO_ADDR_W);
}

static int jz_device_ready(struct mtd_info *mtd)
{
	int ready;
	udelay(20);	/* FIXME: add 20us delay */
	ready = (REG_GPIO_PXPIN(0) & 0x00100000)? 1 : 0;
	return ready;
}

/*
 * EMC setup
 */
static void jz_device_setup(void)
{
	/* Set NFE bit */
	REG_NEMC_NFCSR |= NEMC_NFCSR_NFE1;
#if CONFIG_NAND_BW8 == 1
	REG_NEMC_SMCR1 = CONFIG_NAND_SMCR1;
#else /* 16 bit */
	REG_NEMC_SMCR1 = CONFIG_NAND_SMCR1 | 0x40;
#endif
}

void board_nand_select_device(struct nand_chip *nand, int chip)
{
	/*
	 * Don't use "chip" to address the NAND device,
	 * generate the cs from the address where it is encoded.
	 */
}

static int jzsoc_nand_calculate_bch_ecc(struct mtd_info *mtd, const u_char * dat, u_char * ecc_code)
{
	struct nand_chip *this = (struct nand_chip *)(mtd->priv);
	volatile u8 *paraddr = (volatile u8 *)BCH_PAR0;
	short i;
#ifdef CONFIG_NAND_BCH_WITH_OOB
	/* Write data to REG_BCH_DR */
	for (i = 0; i < this->ecc.size; i++) {
		REG_BCH_DR = ((struct buf_be_corrected *)dat)->data[i];
	}

	/* Write oob to REG_BCH_DR */
	for (i = 0; i < CONFIG_NAND_ECC_POS / this->eccsteps; i++) {
		REG_BCH_DR = ((struct buf_be_corrected *)dat)->oob[i];
	}
#else
	/* Write data to REG_BCH_DR */
	for (i = 0; i < this->ecc.size; i++) {
		REG_BCH_DR = dat[i];
	}
#endif
	__ecc_encode_sync();
	__ecc_disable();

	for (i = 0; i < (par_size+1)/2; i++) {
		ecc_code[i] = *paraddr++;
	}

	return 0;
}

static void jzsoc_nand_enable_bch_hwecc(struct mtd_info* mtd, int mode)
{
	struct nand_chip *this = (struct nand_chip *)(mtd->priv);

	REG_BCH_INTS = 0xffffffff;

	if (mode == NAND_ECC_READ) {
		if (CONFIG_NAND_BCH_BIT == 8)
			__ecc_decoding_8bit();
		else
			__ecc_decoding_4bit();
#ifdef CONFIG_NAND_BCH_WITH_OOB
		__ecc_cnt_dec(2*(this->ecc.size + CONFIG_NAND_ECC_POS / this->eccsteps) + par_size);
#else
		__ecc_cnt_dec(2*this->ecc.size + par_size);
#endif
	}

	if (mode == NAND_ECC_WRITE) {
		if (CONFIG_NAND_BCH_BIT == 8)
			__ecc_encoding_8bit();
		else
			__ecc_encoding_4bit();
#ifdef CONFIG_NAND_BCH_WITH_OOB
		__ecc_cnt_enc(2*(this->ecc.size + CONFIG_NAND_ECC_POS / this->eccsteps));
#else
		__ecc_cnt_enc(2*(this->ecc.size));
#endif
	}
}

/**
 * bch_correct
 * @dat:        data to be corrected
 * @idx:        the index of error bit in an ecc.size
 */
static void bch_correct(struct mtd_info *mtd, u8 * dat, int idx)
{
	struct nand_chip *this = (struct nand_chip *)(mtd->priv);
	int i, bit;		/* the 'bit' of i byte is error */
	i = (idx - 1) >> 3;
	bit = (idx - 1) & 0x7;

#ifdef CONFIG_NAND_BCH_WITH_OOB
	if (i < this->ecc.size)
		((struct buf_be_corrected *)dat)->data[i] ^= (1 << bit);
	else if (i <  (this->ecc.size + CONFIG_NAND_ECC_POS / this->eccsteps))
		((struct buf_be_corrected *)dat)->oob[i - this->ecc.size] ^= (1 << bit);
#else
	if (i < this->ecc.size)
		dat[i] ^= (1 << bit);
#endif
}

/**
 * jzsoc_nand_bch_correct_data:  calc_ecc points to oob_buf for us
 * @mtd:	mtd info structure
 * @dat:        data to be corrected
 * @read_ecc:   pointer to ecc buffer calculated when nand writing
 * @calc_ecc:   no used
 */
static int jzsoc_nand_bch_correct_data(struct mtd_info *mtd, u_char * dat, u_char * read_ecc, u_char * calc_ecc)
{
	struct nand_chip *this = (struct nand_chip *)(mtd->priv);
	short k;
	u32 stat;

#ifdef CONFIG_NAND_BCH_WITH_OOB
	/* Write data to REG_BCH_DR */
	for (k = 0; k < this->ecc.size; k++) {
		REG_BCH_DR = ((struct buf_be_corrected *)dat)->data[k];
	}
	/* Write oob to REG_BCH_DR */
	for (k = 0; k < CONFIG_NAND_ECC_POS / this->eccsteps; k++) {
		REG_BCH_DR = ((struct buf_be_corrected *)dat)->oob[k];
	}
#else
	/* Write data to REG_BCH_DR */
	for (k = 0; k < this->ecc.size; k++) {
		REG_BCH_DR = dat[k];
	}
#endif

	/* Write parities to REG_BCH_DR */
	for (k = 0; k < (par_size+1)/2; k++) {
		REG_BCH_DR = read_ecc[k];
	}

	/* Wait for completion */
	__ecc_decode_sync();
	__ecc_disable();

	/* Check decoding */
	stat = REG_BCH_INTS;

	if (stat & BCH_INTS_ERR) {
		/* Error occurred */
		if (stat & BCH_INTS_UNCOR) {
			printk("NAND: Uncorrectable ECC error--\n");
			return -1;
		} else {
			u32 errcnt = (stat & BCH_INTS_ERRC_MASK) >> BCH_INTS_ERRC_BIT;
			switch (errcnt) {
			case 8:
			  bch_correct(mtd, dat, (REG_BCH_ERR3 & BCH_ERR_INDEX_ODD_MASK) >> BCH_ERR_INDEX_ODD_BIT);
				/* FALL-THROUGH */
			case 7:
			  bch_correct(mtd, dat, (REG_BCH_ERR3 & BCH_ERR_INDEX_EVEN_MASK) >> BCH_ERR_INDEX_EVEN_BIT);
				/* FALL-THROUGH */
			case 6:
			  bch_correct(mtd, dat, (REG_BCH_ERR2 & BCH_ERR_INDEX_ODD_MASK) >> BCH_ERR_INDEX_ODD_BIT);
				/* FALL-THROUGH */
			case 5:
			  bch_correct(mtd, dat, (REG_BCH_ERR2 & BCH_ERR_INDEX_EVEN_MASK) >> BCH_ERR_INDEX_EVEN_BIT);
				/* FALL-THROUGH */
			case 4:
			  bch_correct(mtd, dat, (REG_BCH_ERR1 & BCH_ERR_INDEX_ODD_MASK) >> BCH_ERR_INDEX_ODD_BIT);
				/* FALL-THROUGH */
			case 3:
			  bch_correct(mtd, dat, (REG_BCH_ERR1 & BCH_ERR_INDEX_EVEN_MASK) >> BCH_ERR_INDEX_EVEN_BIT);
				/* FALL-THROUGH */
			case 2:
			  bch_correct(mtd, dat, (REG_BCH_ERR0 & BCH_ERR_INDEX_ODD_MASK) >> BCH_ERR_INDEX_ODD_BIT);
				/* FALL-THROUGH */
			case 1:
			  bch_correct(mtd, dat, (REG_BCH_ERR0 & BCH_ERR_INDEX_EVEN_MASK) >> BCH_ERR_INDEX_EVEN_BIT);;
			  break;
			default:
				break;
			}
		}
	}

	return 0;
}

/*
 * Main initialization routine
 */
int board_nand_init(struct nand_chip *nand)
{
	jz_device_setup();

        /* Set address of NAND IO lines */
        nand->IO_ADDR_R = (void __iomem *) CONFIG_SYS_NAND_BASE;
        nand->IO_ADDR_W = (void __iomem *) CONFIG_SYS_NAND_BASE;

        nand->cmd_ctrl = jz_hwcontrol;
        nand->dev_ready = jz_device_ready;
	
	nand->ecc.correct  = jzsoc_nand_bch_correct_data;
	nand->ecc.hwctl  = jzsoc_nand_enable_bch_hwecc;
	nand->ecc.calculate = jzsoc_nand_calculate_bch_ecc;
	nand->ecc.mode = NAND_ECC_HW_OOB_FIRST;
	nand->ecc.size = 512;
	nand->ecc.bytes = 13;
	nand->ecc.layout = &ecclayout_2gb;
	par_size = 26;

        /* 20 us command delay time */
        nand->chip_delay = 50;

	nand->options &= ~NAND_BUSWIDTH_16;
#if CONFIG_NAND_BW8 == 0
	nand->options |= NAND_BUSWIDTH_16;
#endif
	return 0;

}
#endif
