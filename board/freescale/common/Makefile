#
# (C) Copyright 2006
# Wolfgang Denk, DENX Software Engineering, wd@denx.de.
#
# See file CREDITS for list of people who contributed to this
# project.
#
# This program is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public License as
# published by the Free Software Foundation; either version 2 of
# the License, or (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place, Suite 330, Boston,
# MA 02111-1307 USA
#

include $(TOPDIR)/config.mk

ifneq ($(OBJTREE),$(SRCTREE))
$(shell mkdir -p $(obj)board/freescale/common)
endif

LIB	= $(obj)libfreescale.o

COBJS-$(CONFIG_FSL_CADMUS)	+= cadmus.o
COBJS-$(CONFIG_FSL_VIA)		+= cds_via.o
COBJS-$(CONFIG_FSL_PIXIS)	+= pixis.o
COBJS-$(CONFIG_FSL_NGPIXIS)	+= ngpixis.o
COBJS-$(CONFIG_PQ_MDS_PIB)	+= pq-mds-pib.o
COBJS-$(CONFIG_ID_EEPROM)	+= sys_eeprom.o
COBJS-$(CONFIG_FSL_SGMII_RISER)	+= sgmii_riser.o
COBJS-$(CONFIG_ENV_IS_IN_MMC)	+= sdhc_boot.o

COBJS-$(CONFIG_MPC8541CDS)	+= cds_pci_ft.o
COBJS-$(CONFIG_MPC8548CDS)	+= cds_pci_ft.o
COBJS-$(CONFIG_MPC8555CDS)	+= cds_pci_ft.o

COBJS-$(CONFIG_MPC8536DS)	+= ics307_clk.o
COBJS-$(CONFIG_MPC8572DS)	+= ics307_clk.o
COBJS-$(CONFIG_P1022DS)		+= ics307_clk.o
COBJS-$(CONFIG_P2020DS)		+= ics307_clk.o
COBJS-$(CONFIG_P3041DS)		+= ics307_clk.o
COBJS-$(CONFIG_P4080DS)		+= ics307_clk.o
COBJS-$(CONFIG_P5020DS)		+= ics307_clk.o

SRCS	:= $(SOBJS:.o=.S) $(COBJS-y:.o=.c)
OBJS	:= $(addprefix $(obj),$(COBJS-y))
SOBJS	:= $(addprefix $(obj),$(SOBJS))

$(LIB):	$(obj).depend $(OBJS)
	$(call cmd_link_o_target, $(OBJS))

clean:
	rm -f $(SOBJS) $(OBJS)

distclean:	clean
	rm -f $(LIB) core *.bak $(obj).depend

#########################################################################

# defines $(obj).depend target
include $(SRCTREE)/rules.mk

sinclude $(obj).depend

#########################################################################
