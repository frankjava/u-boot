/*
 * (C) Copyright 2007
 * Sascha Hauer, Pengutronix
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
#include <asm/arch/imx-regs.h>
#include <asm/io.h>

static u32 mx31_decode_pll(u32 reg, u32 infreq)
{
	u32 mfi = (reg >> 10) & 0xf;
	u32 mfn = reg & 0x3ff;
	u32 mfd = (reg >> 16) & 0x3ff;
	u32 pd =  (reg >> 26) & 0xf;

	mfi = mfi <= 5 ? 5 : mfi;
	mfd += 1;
	pd += 1;

	return ((2 * (infreq >> 10) * (mfi * mfd + mfn)) /
		(mfd * pd)) << 10;
}

static u32 mx31_get_mpl_dpdgck_clk(void)
{
	u32 infreq;

	if ((__REG(CCM_CCMR) & CCMR_PRCS_MASK) == CCMR_FPM)
		infreq = CONFIG_MX31_CLK32 * 1024;
	else
		infreq = CONFIG_MX31_HCLK_FREQ;

	return mx31_decode_pll(__REG(CCM_MPCTL), infreq);
}

static u32 mx31_get_mcu_main_clk(void)
{
	/* For now we assume mpl_dpdgck_clk == mcu_main_clk
	 * which should be correct for most boards
	 */
	return mx31_get_mpl_dpdgck_clk();
}

u32 mx31_get_ipg_clk(void)
{
	u32 freq = mx31_get_mcu_main_clk();
	u32 pdr0 = __REG(CCM_PDR0);

	freq /= ((pdr0 >> 3) & 0x7) + 1;
	freq /= ((pdr0 >> 6) & 0x3) + 1;

	return freq;
}

void mx31_dump_clocks(void)
{
	u32 cpufreq = mx31_get_mcu_main_clk();
	printf("mx31 cpu clock: %dMHz\n",cpufreq / 1000000);
	printf("ipg clock     : %dHz\n", mx31_get_ipg_clk());
}

void mx31_gpio_mux(unsigned long mode)
{
	unsigned long reg, shift, tmp;

	reg = IOMUXC_BASE + (mode & 0x1fc);
	shift = (~mode & 0x3) * 8;

	tmp = __REG(reg);
	tmp &= ~(0xff << shift);
	tmp |= ((mode >> IOMUX_MODE_POS) & 0xff) << shift;
	__REG(reg) = tmp;
}

void mx31_set_pad(enum iomux_pins pin, u32 config)
{
	u32 field, l, reg;

	pin &= IOMUX_PADNUM_MASK;
	reg = (IOMUXC_BASE + 0x154) + (pin + 2) / 3 * 4;
	field = (pin + 2) % 3;

	l = __REG(reg);
	l &= ~(0x1ff << (field * 10));
	l |= config << (field * 10);
	__REG(reg) = l;

}

struct mx3_cpu_type mx31_cpu_type[] = {
	{ .srev = 0x00,	.v = "1.0"  },
	{ .srev = 0x10,	.v = "1.1"  },
	{ .srev = 0x11,	.v = "1.1"  },
	{ .srev = 0x12,	.v = "1.15" },
	{ .srev = 0x13,	.v = "1.15" },
	{ .srev = 0x14,	.v = "1.2"  },
	{ .srev = 0x15,	.v = "1.2"  },
	{ .srev = 0x28,	.v = "2.0"  },
	{ .srev = 0x29,	.v = "2.0"  },
};

char *get_cpu_rev(void)
{
	u32 i, srev;

	/* read SREV register from IIM module */
	struct iim_regs *iim = (struct iim_regs *)MX31_IIM_BASE_ADDR;
	srev = readl(&iim->iim_srev);

	for (i = 0; i < ARRAY_SIZE(mx31_cpu_type); i++)
		if (srev == mx31_cpu_type[i].srev)
			return mx31_cpu_type[i].v;
		return "unknown";
}

char *get_reset_cause(void)
{
	/* read RCSR register from CCM module */
	struct clock_control_regs *ccm =
		(struct clock_control_regs *)CCM_BASE;

	u32 cause = readl(&ccm->rcsr) & 0x07;

	switch (cause) {
	case 0x0000:
		return "POR";
		break;
	case 0x0001:
		return "RST";
		break;
	case 0x0002:
		return "WDOG";
		break;
	case 0x0006:
		return "JTAG";
		break;
	default:
		return "unknown reset";
	}
}

#if defined(CONFIG_DISPLAY_CPUINFO)
int print_cpuinfo (void)
{
	printf("CPU:   Freescale i.MX31 rev %s at %d MHz.",
			get_cpu_rev(), mx31_get_mcu_main_clk() / 1000000);
	printf("Reset cause: %s\n", get_reset_cause());
	return 0;
}
#endif
