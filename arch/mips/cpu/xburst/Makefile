#
# (C) Copyright 2000-2006
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

LIB	= $(obj)lib$(CPU).a

START	= start.o
SOBJS-y	= 
COBJS-y = cpu.o interrupts.o jz4740.o jz_serial.o 

COBJS-y += jz_mmc.o
COBJS-$(CONFIG_NANONOTE) += nanonote_gpm940b0.o
COBJS-$(CONFIG_SAKC)	+= nanonote_gpm940b0.o

COBJS-$(CONFIG_JZ4760) += jz4760.o jz4760_nand.o jz4760_eth.o
COBJS-$(CONFIG_USE_MDDR) += jz4760_ddr.o jz_mddr_init.o
COBJS-$(CONFIG_USE_DDR1) += jz4760_ddr.o jz_ddr1_init.o
COBJS-$(CONFIG_USE_DDR2) += jz4760_ddr.o jz_ddr2_init.o
COBJS-$(CONFIG_USE_SDRAM) += jz4760_sdram.o


SRCS	:= $(START:.o=.S) $(SOBJS-y:.o=.S) $(COBJS-y:.o=.c)
OBJS	:= $(addprefix $(obj),$(SOBJS-y) $(COBJS-y))
START	:= $(addprefix $(obj),$(START))

all:	$(obj).depend $(START) $(LIB)

$(LIB):	$(OBJS)
	$(AR) $(ARFLAGS) $@ $(OBJS)

#########################################################################

# defines $(obj).depend target
include $(SRCTREE)/rules.mk

sinclude $(obj).depend

#########################################################################
