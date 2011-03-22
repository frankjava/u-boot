#
# (C) Copyright 2006
# Stefan Roese, DENX Software Engineering, sr@denx.de.
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

LDSCRIPT= $(TOPDIR)/nand_spl/board/$(BOARDDIR)/u-boot.lds
LDFLAGS	= -Bstatic -T $(LDSCRIPT) -Ttext $(CONFIG_NAND_SPL_TEXT_BASE)
AFLAGS	+= -DCONFIG_NAND_SPL
CFLAGS	+= -DCONFIG_NAND_SPL -O2

SOBJS	= start.o usbboot.o
COBJS	= nand_boot.o cpu.o jz4740.o jz_serial.o jz4740_nand.o

SRCS	:= $(addprefix $(obj),$(SOBJS:.o=.S) $(COBJS:.o=.c))
OBJS	:= $(addprefix $(obj),$(SOBJS) $(COBJS))
__OBJS	:= $(SOBJS) $(COBJS)
LNDIR	:= $(OBJTREE)/nand_spl/board/$(BOARDDIR)

nandobj	:= $(OBJTREE)/nand_spl/

ALL	= $(nandobj)u-boot-spl $(nandobj)u-boot-spl.bin $(nandobj)u-boot-spl-16k.bin
all:	$(obj).depend $(ALL)

#The boot program can load two areas of data from NAND flash to internal SRAM, one is the normal
#area up to 8KB starting from NAND flash address 0, the other is the backup area up to 8KB starting
#from NAND flash address 0x2000. After reset, the boot program will first read the normal area data
#from NAND flash using hardware Reed-Solomon ECC. If no ECC error is detected or ECC error is
#correctable, the boot program then branches to internal SRAM at 4 bytes offset. If it detects an
#uncorrectable ECC error, it will continue to read the backup area of data from NAND flash using
#hardware Reed-Solomon ECC.
$(nandobj)u-boot-spl-16k.bin: $(nandobj)u-boot-spl.bin
	dd bs=1024 count=8 if=/dev/zero of=$(nandobj)junk1
	cat $< $(nandobj)junk1 > $(nandobj)junk2
	dd bs=1024 count=8 if=$(nandobj)junk2 of=$(nandobj)junk3
	cat $(nandobj)junk3 $(nandobj)junk3 > $(nandobj)junk4
	dd bs=1024 count=256 if=/dev/zero of=$(nandobj)junk5
	cat $(nandobj)junk4 $(nandobj)junk5 > $(nandobj)junk6
	dd bs=1024 count=256 if=$(nandobj)junk6 of=$@
	rm -f $(nandobj)junk*

$(nandobj)u-boot-spl.bin:	$(nandobj)u-boot-spl
	$(OBJCOPY) ${OBJCFLAGS} -O binary $< $@

$(nandobj)u-boot-spl:	$(OBJS)
	cd $(LNDIR) && $(LD) $(LDFLAGS) $$UNDEF_SYM $(__OBJS) \
		-Map $(nandobj)u-boot-spl.map \
		-o $(nandobj)u-boot-spl

# create symbolic links for common files
$(obj)start.S:
	@rm -f $@
	ln -s $(SRCTREE)/arch/mips/cpu/xburst/start_spl.S $@

$(obj)usbboot.S:
	@rm -f $@
	ln -s $(SRCTREE)/arch/mips/cpu/xburst/usbboot.S $@

$(obj)cpu.c:
	@rm -f $@
	ln -s $(SRCTREE)/arch/mips/cpu/xburst/cpu.c $@

$(obj)jz4740.c:
	@rm -f $@
	ln -s $(SRCTREE)/arch/mips/cpu/xburst/jz4740.c $@

$(obj)jz_serial.c:
	@rm -f $@
	ln -s $(SRCTREE)/arch/mips/cpu/xburst/jz_serial.c $@

$(obj)nand_boot.c:
	@rm -f $@
	ln -s $(SRCTREE)/nand_spl/nand_boot.c $@

$(obj)jz4740_nand.c:
	@rm -f $@
	@ln -s $(TOPDIR)/drivers/mtd/nand/jz4740_nand.c $@

$(obj)%.o:	$(obj)%.S
	$(CC) $(AFLAGS) -c -o $@ $<

$(obj)%.o:	$(obj)%.c
	$(CC) $(CFLAGS) -c -o $@ $<

include $(SRCTREE)/rules.mk

sinclude $(obj).depend
