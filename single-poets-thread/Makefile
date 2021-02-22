# SPDX-License-Identifier: BSD-2-Clause
# Tinsel root
TINSEL_ROOT=../submodules/tinsel

DPD_INC=../inc
DPD_SRC=../src

include $(TINSEL_ROOT)/globals.mk

# Local compiler flags
CFLAGS = $(RV_CFLAGS) -O2 -I $(INC) -std=c++11
LDFLAGS = -melf32lriscv -G 0

.PHONY: all
all: code.v data.v run

code.v: dpd.elf
	checkelf.sh dpd.elf
	$(RV_OBJCOPY) -O verilog --only-section=.text $< $@

data.v: dpd.elf
	$(RV_OBJCOPY) -O verilog --remove-section=.text \
                --set-section-flags .bss=alloc,load,contents $< $@

dpd.elf: dpd.cpp dpd.h link.ld $(INC)/config.h $(INC)/tinsel.h entry.o Vector3D.o utils.o
	$(RV_CC) $(CFLAGS) -I $(DPD_INC) -Wall -c -o dpd.o $<
	$(RV_LD) $(LDFLAGS) -T link.ld -o $@ entry.o dpd.o Vector3D.o utils.o

Vector3D.o: $(DPD_SRC)/Vector3D.cpp $(DPD_INC)/Vector3D.hpp
	$(RV_CC) $(CFLAGS) -Wall -c -DTINSEL -I $(DPD_INC) $(LD_FLAGS) $< -o Vector3D.o

utils.o: $(DPD_SRC)/utils.cpp $(DPD_INC)/utils.hpp
	$(RV_CC) $(CFLAGS) -Wall -c -DTINSEL -I $(DPD_INC) $(LD_FLAGS) $< -o utils.o

entry.o:
	$(RV_CC) $(CFLAGS) -Wall -c -o entry.o entry.S

link.ld: genld.sh
	./genld.sh > link.ld

$(INC)/config.h: $(TINSEL_ROOT)/config.py
	make -C $(INC)

$(HL)/%.o:
	make -C $(HL)

run: run.cpp dpd.h $(HL)/*.o
	g++ -O2 -std=c++11 -I $(INC) -I $(HL) -I $(DPD_INC) -o run run.cpp $(HL)/*.o

sim: run.cpp dpd.h $(HL)/sim/*.o
	g++ -O2 -std=c++11 -I $(INC) -I $(HL) -o sim run.cpp $(HL)/sim/*.o

.PHONY: clean
clean:
	rm -f *.o *.elf link.ld *.v run sim *.ppm
