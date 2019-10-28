# Tinsel root
TINSEL_ROOT=submodules/tinsel

# Tinsel lib
TINSEL_LIB=$(TINSEL_ROOT)/lib

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
HOST_OBJS = $(DPD_BIN)/universe.o $(DPD_BIN)/ExternalClient.o $(DPD_BIN)/ExternalServer.o

SOCAT_SCRIPT = ./scripts/socat_script

.PHONY: all
all: DFLAGS=-DVISUALISE
all: run

run: $(DPD_BIN) $(DPD_BIN)/code.v $(DPD_BIN)/data.v $(DPD_BIN)/run

bridge: $(DPD_BIN)/dpd-bridge

# ~~~~~~~~~~~~~~~ Client side setup ~~~~~~~~~~~~~~~~~~~~~~~~~
LOCAL_SOCKET=./_external.sock
REMOTE_FULL=jrbeaumont@fielding.cl.cam.ac.uk
REMOTE_SOCKET=/home/jrbeaumont/dpd-baremetal/bin/_external.sock
# ~~~~~~~~~~~~~~~ Client side run ~~~~~~~~~~~~~~~~~~~~~~~~~~~
client_run: bridge
	$(SOCAT_SCRIPT) $(LOCAL_SOCKET) $(REMOTE_FULL) $(REMOTE_SOCKET)
	touch state.json
	./bin/dpd-bridge | nodejs submodules/dpd-vis/visualiser.js
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

$(DPD_BIN):
	mkdir -p $(DPD_BIN)

# -------------- Host Object files --------------------------
$(DPD_BIN)/ExternalClient.o: $(DPD_SRC)/ExternalClient.cpp $(DPD_INC)/ExternalClient.hpp
	mkdir -p $(DPD_BIN)
	g++ -O2 -std=c++11 $(DFLAGS) -I $(INC) -I $(HL) -I $(DPD_INC) -c -o $(DPD_BIN)/ExternalClient.o $(DPD_SRC)/ExternalClient.cpp

$(DPD_BIN)/ExternalServer.o: $(DPD_SRC)/ExternalServer.cpp $(DPD_INC)/ExternalServer.hpp
	mkdir -p $(DPD_BIN)
	g++ -O2 -std=c++11 $(DFLAGS) -I $(INC) -I $(HL) -I $(DPD_INC) -c -o $(DPD_BIN)/ExternalServer.o $(DPD_SRC)/ExternalServer.cpp

$(DPD_BIN)/universe.o: $(DPD_SRC)/universe.cpp $(DPD_INC)/universe.hpp
	mkdir -p $(DPD_BIN)
	g++ -O2 -std=c++11 $(DFLAGS) -I $(INC) -I $(HL) -I $(DPD_INC) -c -o $(DPD_BIN)/universe.o $(DPD_SRC)/universe.cpp

# -------------- Tinsel Object files --------------------------
$(DPD_BIN)/Vector3D.o: $(DPD_SRC)/Vector3D.cpp $(DPD_INC)/Vector3D.hpp
	mkdir -p $(DPD_BIN)
	$(RV_CC) $(CFLAGS) -Wall -c -DTINSEL -I $(DPD_INC) $(LD_FLAGS) $< -o $@

$(DPD_BIN)/utils.o: $(DPD_SRC)/utils.cpp $(DPD_INC)/utils.hpp
	mkdir -p $(DPD_BIN)
	$(RV_CC) $(CFLAGS) -Wall -c -DTINSEL -I $(DPD_INC) $(LD_FLAGS) $< -o $@

# -------------- elf --------------------------
$(DPD_BIN)/code.v: $(DPD_BIN)/dpd.elf $(DPD_BIN)
	$(BIN)/checkelf.sh $(DPD_BIN)/dpd.elf
	$(RV_OBJCOPY) -O verilog --only-section=.text $(DPD_BIN)/dpd.elf $@

$(DPD_BIN)/data.v: $(DPD_BIN)/dpd.elf $(DPD_BIN)
	$(RV_OBJCOPY) -O verilog --remove-section=.text \
                --set-section-flags .bss=alloc,load,contents $(DPD_BIN)/dpd.elf $@

$(DPD_BIN)/dpd.elf: $(DPD_SRC)/dpd.cpp $(DPD_INC)/dpd.h $(DPD_BIN)/link.ld $(INC)/config.h $(INC)/tinsel.h $(DPD_BIN)/entry.o $(DPD_BIN) $(DPD_OBJS)
	$(RV_CC) $(CFLAGS) -Wall -c -DTINSEL $(DFLAGS) -I $(DPD_INC) -o $(DPD_BIN)/dpd.o $<
	$(RV_LD) $(LDFLAGS) -T $(DPD_BIN)/link.ld -o $@ $(DPD_BIN)/entry.o $(DPD_BIN)/dpd.o $(TINSEL_LIB_INC) $(DPD_OBJS)

$(DPD_BIN)/entry.o: $(DPD_BIN)
	$(RV_CC) $(CFLAGS) -Wall -c -o $(DPD_BIN)/entry.o $(DPD_UTILS)/entry.S

$(DPD_BIN)/link.ld: $(DPD_UTILS)/genld.sh $(DPD_BIN)
	$(DPD_UTILS)/genld.sh > $(DPD_BIN)/link.ld

$(INC)/config.h: $(TINSEL_ROOT)/config.py
	make -C $(INC)

$(HL)/%.o:
	make -C $(HL)

# -------------- host program --------------------------
$(DPD_BIN)/run: $(DPD_SRC)/run.cpp $(DPD_INC)/dpd.h $(HL)/*.o $(DPD_BIN) $(HOST_OBJS)
	g++ -O2 -std=c++11 $(DFLAGS) -I $(INC) -I $(HL) -I $(DPD_INC) -c -o $(DPD_BIN)/run.o $(DPD_SRC)/run.cpp
	g++ -O2 -std=c++11 -o $(DPD_BIN)/run $(HOST_OBJS) $(HL)/*.o $(DPD_BIN)/run.o \
	  -static-libgcc -static-libstdc++ \
          -ljtag_atlantic -ljtag_client -L$(QUARTUS_ROOTDIR)/linux64 \
          -Wl,-rpath,$(QUARTUS_ROOTDIR)/linux64 -lmetis -lpthread -lboost_program_options -lboost_filesystem -lboost_system

# ------------ the external client ----------------
$(DPD_BIN)/dpd-bridge: $(DPD_SRC)/dpd-bridge.cpp $(HOST_OBJS)
	g++ -O2 -std=c++17 -o $(DPD_BIN)/dpd-bridge -I $(INC) -I $(HL) -I $(DPD_INC) $(HOST_OBJS) $(DPD_SRC)/dpd-bridge.cpp \
		-lboost_program_options -lboost_filesystem -lboost_system -lpthread -lstdc++fs

# ------------- Run with wallclock timer ------------
timed-run: DFLAGS=-DTIMER
timed-run: run

# ------------- Do local calculations one bead at a time ------------
onebyone: DFLAGS=-DTIMER -DONE_BY_ONE
onebyone: run

test-onebyone: DFLAGS=-DTESTING -DONE_BY_ONE
test-onebyone: $(INC)/config.h $(HL)/*.o $(HOST_OBJS) $(DPD_BIN)/code.v $(DPD_BIN)/data.v
	g++ -O2 -std=c++11 $(DFLAGS) -I $(INC) -I $(HL) -I $(DPD_INC) -c -o $(DPD_BIN)/test.o $(DPD_SRC)/test.cpp
	g++ -O2 -std=c++11 -o $(DPD_BIN)/test $(HOST_OBJS) $(HL)/*.o $(DPD_BIN)/test.o \
	  -static-libgcc -static-libstdc++ \
          -ljtag_atlantic -ljtag_client -L$(QUARTUS_ROOTDIR)/linux64 \
          -Wl,-rpath,$(QUARTUS_ROOTDIR)/linux64 -lmetis -lpthread -lboost_program_options -lboost_filesystem -lboost_system

# ------------ Send beads to self to reduce local calculation memory space -------------------
sendtoself: DFLAGS=-DTIMER -DSEND_TO_SELF
sendtoself: run

test-sendtoself: DFLAGS=-DTESTING -DSEND_TO_SELF
test-sendtoself: $(INC)/config.h $(HL)/*.o $(HOST_OBJS) $(DPD_BIN)/code.v $(DPD_BIN)/data.v
	g++ -O2 -std=c++11 $(DFLAGS) -I $(INC) -I $(HL) -I $(DPD_INC) -c -o $(DPD_BIN)/test.o $(DPD_SRC)/test.cpp
	g++ -O2 -std=c++11 -o $(DPD_BIN)/test $(HOST_OBJS) $(HL)/*.o $(DPD_BIN)/test.o \
	  -static-libgcc -static-libstdc++ \
          -ljtag_atlantic -ljtag_client -L$(QUARTUS_ROOTDIR)/linux64 \
          -Wl,-rpath,$(QUARTUS_ROOTDIR)/linux64 -lmetis -lpthread -lboost_program_options -lboost_filesystem -lboost_system

# ------------- Run to output statistics generated by POLite ----------

clean-tinsel:
	make clean -C $(TINSEL_ROOT)

$(TINSEL_LIB)/lib.o:
	make -C $(TINSEL_LIB)

stats-run: DFLAGS=-DSTATS
stats-run: TINSEL_LIB_INC=$(TINSEL_LIB)/lib.o
stats-run: clean clean-tinsel $(TINSEL_LIB)/lib.o run

stats-obo: DFLAGS=-DSTATS -DONE_BY_ONE
stats-obo: TINSEL_LIB_INC=$(TINSEL_LIB)/lib.o
stats-obo: clean clean-tinsel $(TINSEL_LIB)/lib.o run

stats-sts: DFLAGS=-DSTATS -DSEND_TO_SELF
stats-sts: TINSEL_LIB_INC=$(TINSEL_LIB)/lib.o
stats-sts: clean clean-tinsel $(TINSEL_LIB)/lib.o run

print-stats: $(DPD_BIN)/stats.txt
	./$(TINSEL_ROOT)/apps/POLite/util/sumstats.awk < $(DPD_BIN)/stats.txt

# ------------ TESTING ---------------------------------
.PHONY: test
test: DFLAGS=-DTESTING
test: $(INC)/config.h $(HL)/*.o $(HOST_OBJS) $(DPD_BIN)/code.v $(DPD_BIN)/data.v
	g++ -O2 -std=c++11 $(DFLAGS) -I $(INC) -I $(HL) -I $(DPD_INC) -c -o $(DPD_BIN)/test.o $(DPD_SRC)/test.cpp
	g++ -O2 -std=c++11 -o $(DPD_BIN)/test $(HOST_OBJS) $(HL)/*.o $(DPD_BIN)/test.o \
	  -static-libgcc -static-libstdc++ \
          -ljtag_atlantic -ljtag_client -L$(QUARTUS_ROOTDIR)/linux64 \
          -Wl,-rpath,$(QUARTUS_ROOTDIR)/linux64 -lmetis -lpthread -lboost_program_options -lboost_filesystem -lboost_system

# ------------ BONDS TESTING----------------------------
test-bonds: DFLAGS=-DTESTING -DBOND_TESTING
test-bonds: $(INC)/config.h $(HL)/*.o $(HOST_OBJS) $(DPD_BIN)/code.v $(DPD_BIN)/data.v
	g++ -O2 -std=c++11 $(DFLAGS) -I $(INC) -I $(HL) -I $(DPD_INC) -c -o $(DPD_BIN)/test.o $(DPD_SRC)/test.cpp
	g++ -O2 -std=c++11 -o $(DPD_BIN)/test $(HOST_OBJS) $(HL)/*.o $(DPD_BIN)/test.o \
	  -static-libgcc -static-libstdc++ \
          -ljtag_atlantic -ljtag_client -L$(QUARTUS_ROOTDIR)/linux64 \
          -Wl,-rpath,$(QUARTUS_ROOTDIR)/linux64 -lmetis -lpthread -lboost_program_options -lboost_filesystem -lboost_system


# ------------- FILE PARSING AND RUNNING ---------------
$(DPD_BIN)/parsedCode.v: $(DPD_BIN)/parsedDPD.elf $(DPD_BIN)
	$(BIN)/checkelf.sh $(DPD_BIN)/parsedDPD.elf
	$(RV_OBJCOPY) -O verilog --only-section=.text $(DPD_BIN)/parsedDPD.elf $@

$(DPD_BIN)/parsedData.v: $(DPD_BIN)/parsedDPD.elf $(DPD_BIN)
	$(RV_OBJCOPY) -O verilog --remove-section=.text \
                --set-section-flags .bss=alloc,load,contents $(DPD_BIN)/parsedDPD.elf $@

$(DPD_BIN)/parsedDPD.elf: $(DPD_SRC)/parsedDPD.cpp $(DPD_INC)/parsedDPD.h $(DPD_BIN)/link.ld $(INC)/config.h $(INC)/tinsel.h $(DPD_BIN)/entry.o $(DPD_BIN) $(DPD_OBJS)
	$(RV_CC) $(CFLAGS) -Wall -c -DTINSEL $(DFLAGS) -I $(DPD_INC) -o $(DPD_BIN)/parsedDPD.o $<
	$(RV_LD) $(LDFLAGS) -T $(DPD_BIN)/link.ld -o $@ $(DPD_BIN)/entry.o $(DPD_BIN)/parsedDPD.o $(TINSEL_LIB_INC) $(DPD_OBJS)

PARSE_OBJS = $(DPD_BIN)/ExternalClient.o $(DPD_BIN)/ExternalServer.o

$(DPD_BIN)/parserRun: $(DPD_SRC)/parserRun.cpp $(PARSE_OBJS) $(DPD_SRC)/parseUniverse.cpp $(DPD_INC)/parseUniverse.hpp $(DPD_SRC)/parser.cpp $(DPD_INC)/parser.hpp $(DPD_INC)/parsedDPD.h $(HL)/*.o $(DPD_BIN) $(PARSE_OBJS)
	g++ -O2 -std=c++11 $(DFLAGS) -I $(INC) -I $(HL) -I $(DPD_INC) -c -o $(DPD_BIN)/parserRun.o $(DPD_SRC)/parserRun.cpp
	g++ -O2 -std=c++11 -o $(DPD_BIN)/parserRun $(PARSE_OBJS) $(HL)/*.o $(DPD_BIN)/parserRun.o \
	  -static-libgcc -static-libstdc++ \
          -ljtag_atlantic -ljtag_client -L$(QUARTUS_ROOTDIR)/linux64 \
          -Wl,-rpath,$(QUARTUS_ROOTDIR)/linux64 -lmetis -lpthread -lboost_program_options -lboost_filesystem -lboost_system

parse: DFLAGS=-DVISUALISE
parse: $(DPD_BIN) $(DPD_BIN)/parsedCode.v $(DPD_BIN)/parsedData.v $(DPD_BIN)/parserRun

ptime: DFLAGS=-DTIMER
ptime: $(DPD_BIN) $(DPD_SRC)/timer.cpp $(DPD_INC)/timer.h $(DPD_BIN)/parsedCode.v $(DPD_BIN)/parsedData.v $(DPD_BIN)/parserRun

pobo: DFLAGS=-DTIMER -DONE_BY_ONE
pobo: $(DPD_BIN) $(DPD_SRC)/timer.cpp $(DPD_INC)/timer.h $(DPD_BIN)/parsedCode.v $(DPD_BIN)/parsedData.v $(DPD_BIN)/parserRun

pobov: DFLAGS=-DVISUALISE -DONE_BY_ONE
pobov: $(DPD_BIN) $(DPD_BIN)/parsedCode.v $(DPD_BIN)/parsedData.v $(DPD_BIN)/parserRun

# ----------------- GALS implementation ----------------------------

$(DPD_BIN)/galsCode.v: $(DPD_BIN)/dpdGALS.elf $(DPD_BIN)
	$(BIN)/checkelf.sh $(DPD_BIN)/dpdGALS.elf
	$(RV_OBJCOPY) -O verilog --only-section=.text $(DPD_BIN)/dpdGALS.elf $@

$(DPD_BIN)/galsData.v: $(DPD_BIN)/dpdGALS.elf $(DPD_BIN)
	$(RV_OBJCOPY) -O verilog --remove-section=.text \
                --set-section-flags .bss=alloc,load,contents $(DPD_BIN)/dpdGALS.elf $@

$(DPD_BIN)/dpdGALS.elf: $(DPD_SRC)/dpdGALS.cpp $(DPD_INC)/dpdGALS.h $(DPD_BIN)/link.ld $(INC)/config.h $(INC)/tinsel.h $(DPD_BIN)/entry.o $(DPD_BIN) $(DPD_OBJS)
	$(RV_CC) $(CFLAGS) -Wall -c -DTINSEL $(DFLAGS) -I $(DPD_INC) -o $(DPD_BIN)/dpdGALS.o $<
	$(RV_LD) $(LDFLAGS) -T $(DPD_BIN)/link.ld -o $@ $(DPD_BIN)/entry.o $(DPD_BIN)/dpdGALS.o $(TINSEL_LIB_INC) $(DPD_OBJS)

gals: DFLAGS=-DVISUALISE -DGALS
gals: $(DPD_BIN) $(DPD_BIN)/galsCode.v $(DPD_BIN)/galsData.v $(DPD_BIN)/run
	cp $(DPD_BIN)/galsCode.v $(DPD_BIN)/code.v
	cp $(DPD_BIN)/galsData.v $(DPD_BIN)/data.v
	cp $(DPD_BIN)/dpdGALS.elf $(DPD_BIN)/dpd.elf

test-gals: DFLAGS=-DTESTING -DGALS
test-gals: $(INC)/config.h $(HL)/*.o $(HOST_OBJS) $(DPD_BIN)/galsCode.v $(DPD_BIN)/galsData.v
	cp $(DPD_BIN)/galsCode.v $(DPD_BIN)/code.v
	cp $(DPD_BIN)/galsData.v $(DPD_BIN)/data.v
	cp $(DPD_BIN)/dpdGALS.elf $(DPD_BIN)/dpd.elf
	g++ -O2 -std=c++11 $(DFLAGS) -I $(INC) -I $(HL) -I $(DPD_INC) -c -o $(DPD_BIN)/test.o $(DPD_SRC)/test.cpp
	g++ -O2 -std=c++11 -o $(DPD_BIN)/test $(HOST_OBJS) $(HL)/*.o $(DPD_BIN)/test.o \
	  -static-libgcc -static-libstdc++ \
          -ljtag_atlantic -ljtag_client -L$(QUARTUS_ROOTDIR)/linux64 \
          -Wl,-rpath,$(QUARTUS_ROOTDIR)/linux64 -lmetis -lpthread -lboost_program_options -lboost_filesystem -lboost_system

timed-gals: DFLAGS=-DTIMER -DGALS
timed-gals: $(DPD_BIN) $(DPD_BIN)/galsCode.v $(DPD_BIN)/galsData.v $(DPD_BIN)/run
	cp $(DPD_BIN)/galsCode.v $(DPD_BIN)/code.v
	cp $(DPD_BIN)/galsData.v $(DPD_BIN)/data.v
	cp $(DPD_BIN)/dpdGALS.elf $(DPD_BIN)/dpd.elf

stats-gals: DFLAGS=-DSTATS -DGALS
stats-gals: TINSEL_LIB_INC=$(TINSEL_LIB)/lib.o
stats-gals: clean clean-tinsel $(TINSEL_LIB)/lib.o $(DPD_BIN) $(DPD_BIN)/galsCode.v $(DPD_BIN)/galsData.v $(DPD_BIN)/run
	cp $(DPD_BIN)/galsCode.v $(DPD_BIN)/code.v
	cp $(DPD_BIN)/galsData.v $(DPD_BIN)/data.v
	cp $(DPD_BIN)/dpdGALS.elf $(DPD_BIN)/dpd.elf

.PHONY: clean
clean:
	rm -rf $(DPD_BIN) *.sock state.json
	rm -rf _frames
	rm -rf _meta.json
	rm -rf _state.json
	rm -rf node_modules
