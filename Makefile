# Tinsel root
TINSEL_ROOT=submodules/tinsel

# directories
DPD_BIN=./bin
DPD_SRC=./src
DPD_INC=./inc
DPD_UTILS=./utils

include $(TINSEL_ROOT)/globals.mk

# Local compiler flags
CFLAGS = $(RV_CFLAGS) -O2 -I $(INC) -std=c++11 
LDFLAGS = -melf32lriscv -G 0  
DPD_OBJS = $(DPD_BIN)/Vector3D.o $(DPD_BIN)/utils.o 

.PHONY: all
all: $(DPD_BIN)/code.v $(DPD_BIN)/data.v $(DPD_BIN)/run $(DPD_BIN)

$(DPD_BIN):
	mkdir -p $(DPD_BIN) 

$(DPD_BIN)/%.o: $(DPD_SRC)/%.cpp $(DPD_INC)/%.hpp
	mkdir -p $(DPD_BIN)
	$(RV_CC) $(CFLAGS) -Wall -c -DTINSEL -I $(DPD_INC) $(LD_FLAGS) $< -o $@	

$(DPD_BIN)/code.v: $(DPD_BIN)/dpd.elf $(DPD_BIN)
	$(BIN)/checkelf.sh $(DPD_BIN)/dpd.elf
	$(RV_OBJCOPY) -O verilog --only-section=.text $(DPD_BIN)/dpd.elf $@

$(DPD_BIN)/data.v: $(DPD_BIN)/dpd.elf $(DPD_BIN)
	$(RV_OBJCOPY) -O verilog --remove-section=.text \
                --set-section-flags .bss=alloc,load,contents $(DPD_BIN)/dpd.elf $@

$(DPD_BIN)/dpd.elf: $(DPD_SRC)/dpd.cpp $(DPD_INC)/dpd.h $(DPD_BIN)/link.ld $(INC)/config.h $(INC)/tinsel.h $(DPD_BIN)/entry.o $(DPD_BIN) $(DPD_OBJS)
	$(RV_CC) $(CFLAGS) -Wall -c -DTINSEL -I $(DPD_INC) -o $(DPD_BIN)/dpd.o $<
	$(RV_LD) $(LDFLAGS) -T $(DPD_BIN)/link.ld -o $@ $(DPD_BIN)/entry.o $(DPD_BIN)/dpd.o $(DPD_OBJS)

$(DPD_BIN)/entry.o: $(DPD_BIN)
	$(RV_CC) $(CFLAGS) -Wall -c -o $(DPD_BIN)/entry.o $(DPD_UTILS)/entry.S

$(DPD_BIN)/link.ld: $(DPD_UTILS)/genld.sh $(DPD_BIN)
	$(DPD_UTILS)/genld.sh > $(DPD_BIN)/link.ld

$(INC)/config.h: $(TINSEL_ROOT)/config.py
	make -C $(INC)

$(HL)/%.o:
	make -C $(HL)

$(DPD_BIN)/run: $(DPD_SRC)/run.cpp $(DPD_INC)/dpd.h $(HL)/*.o $(DPD_BIN)
	g++ -O2 -std=c++98 -I $(INC) -I $(HL) -I $(DPD_INC) -o $(DPD_BIN)/run $(DPD_SRC)/run.cpp $(HL)/*.o \
          -ljtag_atlantic -ljtag_client -L $(QUARTUS_ROOTDIR)/linux64/ \
          -Wl,-rpath,$(QUARTUS_ROOTDIR)/linux64 -lmetis

.PHONY: tests
tests:
	make -C ./tests

.PHONY: clean
clean:
	rm -rf $(DPD_BIN) 
