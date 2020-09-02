# Tinsel root
TINSEL_ROOT=submodules/tinsel

# Tinsel lib
TINSEL_LIB=$(TINSEL_ROOT)/lib

# directories
DPD_BIN=./bin
DPD_SRC=./src
DPD_INC=./inc
DPD_UTILS=./utils
DPD_EXAMPLES=./examples

include $(TINSEL_ROOT)/globals.mk

# Local compiler flags
CFLAGS = $(RV_CFLAGS) -O2 -I $(INC) -std=c++11
LDFLAGS = -melf32lriscv -G 0
DPD_OBJS = $(DPD_BIN)/Vector3D.o $(DPD_BIN)/utils.o
HOST_OBJS = $(DPD_BIN)/universe.o $(DPD_BIN)/ExternalClient.o $(DPD_BIN)/ExternalServer.o

SOCAT_SCRIPT = ./scripts/socat_script

run: $(DPD_BIN) $(DPD_BIN)/code.v $(DPD_BIN)/data.v $(DPD_BIN)/run

bridge: $(INC)/config.h $(DPD_BIN)/dpd-bridge

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
	$(RV_CC) $(CFLAGS) -Wall -c -DTINSEL $(DFLAGS) -I $(DPD_INC) $(LD_FLAGS) $< -o $@

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
          -ljtag_atlantic -ljtag_client -lscotch -L$(QUARTUS_ROOTDIR)/linux64 \
          -Wl,-rpath,$(QUARTUS_ROOTDIR)/linux64 -lmetis -lpthread -lboost_program_options -lboost_filesystem -lboost_system -fopenmp

# ------------ the external client ----------------
$(DPD_BIN)/dpd-bridge: $(DPD_SRC)/dpd-bridge.cpp $(HOST_OBJS)
	g++ -O2 -std=c++17 -o $(DPD_BIN)/dpd-bridge -I $(INC) -I $(HL) -I $(DPD_INC) $(HOST_OBJS) $(DPD_SRC)/dpd-bridge.cpp \
		-lboost_program_options -lboost_filesystem -lboost_system -lpthread -lstdc++fs

# ------------- Run with live visualisations
visual: DFLAGS=-DVISUALISE -DSEND_TO_SELF
visual: run

visual-dram: DFLAGS=-DDRAM -DVISUALISE
visual-dram: run

# ------------- Run with wallclock timer ------------
timed-run: DFLAGS=-DTIMER
timed-run: run

timed-dram-run: DFLAGS=-DTIMER -DDRAM
timed-dram-run: run

# ------------- Output mapping for analysis ---------
output-mapping: DFLAGS=-DOUTPUT_MAPPING -DDRAM
output-mapping: run

# ------------- Cells output messages sent (not including number of edges) ---------
count-messages: DFLAGS=-DMESSAGE_COUNTER
count-messages: run

count-messages-dram: DFLAGS=-DMESSAGE_COUNTER -DDRAM
count-messages-dram: run

# ------------- Link messages analysis util -------------------------
$(DPD_BIN)/linkAnalysis: $(DPD_BIN) $(DPD_UTILS)/linkAnalysis.cpp
	g++ -O2 -std=c++11 $(DPD_UTILS)/linkAnalysis.cpp -o $(DPD_BIN)/linkAnalysis -lboost_filesystem

link-analysis: $(DPD_BIN)/linkAnalysis


# ------------- Do local calculations one bead at a time ------------
onebyone: DFLAGS=-DTIMER -DONE_BY_ONE
onebyone: run

onebyone-dram: DFLAGS=-DTIMER -DONE_BY_ONE -DDRAM
onebyone-dram: run

onebyone-double-sqrt: DFLAGS=-DTIMER -DONE_BY_ONE -DDOUBLE_SQRT
onebyone-double-sqrt: run

test-onebyone: DFLAGS=-DTESTING -DONE_BY_ONE
test-onebyone: $(INC)/config.h $(HL)/*.o $(HOST_OBJS) $(DPD_BIN)/code.v $(DPD_BIN)/data.v
	g++ -O2 -std=c++11 $(DFLAGS) -I $(INC) -I $(HL) -I $(DPD_INC) -c -o $(DPD_BIN)/test.o $(DPD_SRC)/test.cpp
	g++ -O2 -std=c++11 -o $(DPD_BIN)/test $(HOST_OBJS) $(HL)/*.o $(DPD_BIN)/test.o \
	  -static-libgcc -static-libstdc++ \
          -ljtag_atlantic -ljtag_client -lscotch -L$(QUARTUS_ROOTDIR)/linux64 \
          -Wl,-rpath,$(QUARTUS_ROOTDIR)/linux64 -lmetis -lpthread -lboost_program_options -lboost_filesystem -lboost_system -fopenmp

test-onebyone-large: DFLAGS=-DTESTING -DONE_BY_ONE -DLARGE_TEST
test-onebyone-large: $(INC)/config.h $(HL)/*.o $(HOST_OBJS) $(DPD_BIN)/code.v $(DPD_BIN)/data.v
	g++ -O2 -std=c++11 $(DFLAGS) -I $(INC) -I $(HL) -I $(DPD_INC) -c -o $(DPD_BIN)/test.o $(DPD_SRC)/test.cpp
	g++ -O2 -std=c++11 -o $(DPD_BIN)/test $(HOST_OBJS) $(HL)/*.o $(DPD_BIN)/test.o \
	  -static-libgcc -static-libstdc++ \
          -ljtag_atlantic -ljtag_client -lscotch -L$(QUARTUS_ROOTDIR)/linux64 \
          -Wl,-rpath,$(QUARTUS_ROOTDIR)/linux64 -lmetis -lpthread -lboost_program_options -lboost_filesystem -lboost_system -fopenmp

# ------------ Send beads to self to reduce local calculation memory space -------------------
sendtoself: DFLAGS=-DTIMER -DSEND_TO_SELF
sendtoself: run

sendtoself-dram: DFLAGS=-DTIMER -DSEND_TO_SELF -DDRAM
sendtoself-dram: run

test-sendtoself: DFLAGS=-DTESTING -DSEND_TO_SELF
test-sendtoself: $(INC)/config.h $(HL)/*.o $(HOST_OBJS) $(DPD_BIN)/code.v $(DPD_BIN)/data.v
	g++ -O2 -std=c++11 $(DFLAGS) -I $(INC) -I $(HL) -I $(DPD_INC) -c -o $(DPD_BIN)/test.o $(DPD_SRC)/test.cpp
	g++ -O2 -std=c++11 -o $(DPD_BIN)/test $(HOST_OBJS) $(HL)/*.o $(DPD_BIN)/test.o \
	  -static-libgcc -static-libstdc++ \
          -ljtag_atlantic -ljtag_client -lscotch -L$(QUARTUS_ROOTDIR)/linux64 \
          -Wl,-rpath,$(QUARTUS_ROOTDIR)/linux64 -lmetis -lpthread -lboost_program_options -lboost_filesystem -lboost_system -fopenmp

# TODO: 46x46x46 is too larger for send to self. Make smaller test for this, or smaller for all
test-sendtoself-large: DFLAGS=-DTESTING -DSEND_TO_SELF -DLARGE_TEST
test-sendtoself-large: $(INC)/config.h $(HL)/*.o $(HOST_OBJS) $(DPD_BIN)/code.v $(DPD_BIN)/data.v
	g++ -O2 -std=c++11 $(DFLAGS) -I $(INC) -I $(HL) -I $(DPD_INC) -c -o $(DPD_BIN)/test.o $(DPD_SRC)/test.cpp
	g++ -O2 -std=c++11 -o $(DPD_BIN)/test $(HOST_OBJS) $(HL)/*.o $(DPD_BIN)/test.o \
	  -static-libgcc -static-libstdc++ \
          -ljtag_atlantic -ljtag_client -lscotch -L$(QUARTUS_ROOTDIR)/linux64 \
          -Wl,-rpath,$(QUARTUS_ROOTDIR)/linux64 -lmetis -lpthread -lboost_program_options -lboost_filesystem -lboost_system -fopenmp


# ------------ Use message manager to reduce NoC strain -------------------

# Standard timed with message management
timed-msg-mgmt: DFLAGS=-DTIMER -DMESSAGE_MANAGEMENT
timed-msg-mgmt: run

# Standard with message management
msg-mgmt: DFLAGS=-DMESSAGE_MANAGEMENT -DVISUALISE
msg-mgmt: run

# Standard timed with message management and vertices mapped to DRAM
timed-msg-mgmt-dram: DFLAGS=-DTIMER -DMESSAGE_MANAGEMENT -DDRAM
timed-msg-mgmt-dram: run

# Test standard with message management
test-msg-mgmt: DFLAGS=-DTESTING -DMESSAGE_MANAGEMENT
test-msg-mgmt: $(INC)/config.h $(HL)/*.o $(HOST_OBJS) $(DPD_BIN)/code.v $(DPD_BIN)/data.v
	g++ -O2 -std=c++11 $(DFLAGS) -I $(INC) -I $(HL) -I $(DPD_INC) -c -o $(DPD_BIN)/test.o $(DPD_SRC)/test.cpp
	g++ -O2 -std=c++11 -o $(DPD_BIN)/test $(HOST_OBJS) $(HL)/*.o $(DPD_BIN)/test.o \
	  -static-libgcc -static-libstdc++ \
          -ljtag_atlantic -ljtag_client -lscotch -L$(QUARTUS_ROOTDIR)/linux64 \
          -Wl,-rpath,$(QUARTUS_ROOTDIR)/linux64 -lmetis -lpthread -lboost_program_options -lboost_filesystem -lboost_system -fopenmp

# Large test of standard with message management
test-msg-mgmt-large: DFLAGS=-DTESTING -DMESSAGE_MANAGEMENT -DLARGE_TEST
test-msg-mgmt-large: $(INC)/config.h $(HL)/*.o $(HOST_OBJS) $(DPD_BIN)/code.v $(DPD_BIN)/data.v
	g++ -O2 -std=c++11 $(DFLAGS) -I $(INC) -I $(HL) -I $(DPD_INC) -c -o $(DPD_BIN)/test.o $(DPD_SRC)/test.cpp
	g++ -O2 -std=c++11 -o $(DPD_BIN)/test $(HOST_OBJS) $(HL)/*.o $(DPD_BIN)/test.o \
	  -static-libgcc -static-libstdc++ \
          -ljtag_atlantic -ljtag_client -lscotch -L$(QUARTUS_ROOTDIR)/linux64 \
          -Wl,-rpath,$(QUARTUS_ROOTDIR)/linux64 -lmetis -lpthread -lboost_program_options -lboost_filesystem -lboost_system -fopenmp

# One by one timed with message management
timed-obo-msg-mgmt: DFLAGS=-DTIMER -DONE_BY_ONE -DMESSAGE_MANAGEMENT
timed-obo-msg-mgmt: run

# One by one with message management
obo-msg-mgmt: DFLAGS=-DONE_BY_ONE -DMESSAGE_MANAGEMENT -DVISUALISE
obo-msg-mgmt: run

# One by one timed with message management and vertices mapped to DRAM
timed-obo-msg-mgmt-dram: DFLAGS=-DTIMER -DONE_BY_ONE -DMESSAGE_MANAGEMENT -DDRAM
timed-obo-msg-mgmt-dram: run

# Test one by one with message management
test-obo-msg-mgmt: DFLAGS=-DTESTING -DONE_BY_ONE -DMESSAGE_MANAGEMENT
test-obo-msg-mgmt: $(INC)/config.h $(HL)/*.o $(HOST_OBJS) $(DPD_BIN)/code.v $(DPD_BIN)/data.v
	g++ -O2 -std=c++11 $(DFLAGS) -I $(INC) -I $(HL) -I $(DPD_INC) -c -o $(DPD_BIN)/test.o $(DPD_SRC)/test.cpp
	g++ -O2 -std=c++11 -o $(DPD_BIN)/test $(HOST_OBJS) $(HL)/*.o $(DPD_BIN)/test.o \
	  -static-libgcc -static-libstdc++ \
          -ljtag_atlantic -ljtag_client -lscotch -L$(QUARTUS_ROOTDIR)/linux64 \
          -Wl,-rpath,$(QUARTUS_ROOTDIR)/linux64 -lmetis -lpthread -lboost_program_options -lboost_filesystem -lboost_system -fopenmp

# Large test of one by one with message management
test-obo-msg-mgmt-large: DFLAGS=-DTESTING -DONE_BY_ONE -DMESSAGE_MANAGEMENT -DLARGE_TEST
test-obo-msg-mgmt-large: $(INC)/config.h $(HL)/*.o $(HOST_OBJS) $(DPD_BIN)/code.v $(DPD_BIN)/data.v
	g++ -O2 -std=c++11 $(DFLAGS) -I $(INC) -I $(HL) -I $(DPD_INC) -c -o $(DPD_BIN)/test.o $(DPD_SRC)/test.cpp
	g++ -O2 -std=c++11 -o $(DPD_BIN)/test $(HOST_OBJS) $(HL)/*.o $(DPD_BIN)/test.o \
	  -static-libgcc -static-libstdc++ \
          -ljtag_atlantic -ljtag_client -lscotch -L$(QUARTUS_ROOTDIR)/linux64 \
          -Wl,-rpath,$(QUARTUS_ROOTDIR)/linux64 -lmetis -lpthread -lboost_program_options -lboost_filesystem -lboost_system -fopenmp

# Send to self timed with message management
timed-sts-msg-mgmt: DFLAGS=-DTIMER -DSEND_TO_SELF -DMESSAGE_MANAGEMENT
timed-sts-msg-mgmt: run

# Send to self with message management
sts-msg-mgmt: DFLAGS=-DSEND_TO_SELF -DMESSAGE_MANAGEMENT -DVISUALISE
sts-msg-mgmt: run

# Send to self timed with message management and vertices mapped to DRAM
timed-sts-msg-mgmt-dram: DFLAGS=-DTIMER -DSEND_TO_SELF -DMESSAGE_MANAGEMENT -DDRAM
timed-sts-msg-mgmt-dram: run

# Test send to self with message management
test-sts-msg-mgmt: DFLAGS=-DTESTING -DSEND_TO_SELF -DMESSAGE_MANAGEMENT
test-sts-msg-mgmt: $(INC)/config.h $(HL)/*.o $(HOST_OBJS) $(DPD_BIN)/code.v $(DPD_BIN)/data.v
	g++ -O2 -std=c++11 $(DFLAGS) -I $(INC) -I $(HL) -I $(DPD_INC) -c -o $(DPD_BIN)/test.o $(DPD_SRC)/test.cpp
	g++ -O2 -std=c++11 -o $(DPD_BIN)/test $(HOST_OBJS) $(HL)/*.o $(DPD_BIN)/test.o \
	  -static-libgcc -static-libstdc++ \
          -ljtag_atlantic -ljtag_client -lscotch -L$(QUARTUS_ROOTDIR)/linux64 \
          -Wl,-rpath,$(QUARTUS_ROOTDIR)/linux64 -lmetis -lpthread -lboost_program_options -lboost_filesystem -lboost_system -fopenmp

# Large send to self with message management
test-sts-msg-mgmt-large: DFLAGS=-DTESTING -DSEND_TO_SELF -DMESSAGE_MANAGEMENT -DLARGE_TEST
test-sts-msg-mgmt-large: $(INC)/config.h $(HL)/*.o $(HOST_OBJS) $(DPD_BIN)/code.v $(DPD_BIN)/data.v
	g++ -O2 -std=c++11 $(DFLAGS) -I $(INC) -I $(HL) -I $(DPD_INC) -c -o $(DPD_BIN)/test.o $(DPD_SRC)/test.cpp
	g++ -O2 -std=c++11 -o $(DPD_BIN)/test $(HOST_OBJS) $(HL)/*.o $(DPD_BIN)/test.o \
	  -static-libgcc -static-libstdc++ \
          -ljtag_atlantic -ljtag_client -lscotch -L$(QUARTUS_ROOTDIR)/linux64 \
          -Wl,-rpath,$(QUARTUS_ROOTDIR)/linux64 -lmetis -lpthread -lboost_program_options -lboost_filesystem -lboost_system -fopenmp

gals-msg-mgmt: DFLAGS=-DVISUALISE -DGALS -DMESSAGE_MANAGEMENT
gals-msg-mgmt: $(DPD_BIN) $(DPD_BIN)/galsCode.v $(DPD_BIN)/galsData.v $(DPD_BIN)/run
	cp $(DPD_BIN)/galsCode.v $(DPD_BIN)/code.v
	cp $(DPD_BIN)/galsData.v $(DPD_BIN)/data.v
	cp $(DPD_BIN)/dpdGALS.elf $(DPD_BIN)/dpd.elf

timed-gals-msg-mgmt: DFLAGS=-DTIMER -DGALS -DMESSAGE_MANAGEMENT
timed-gals-msg-mgmt: $(DPD_BIN) $(DPD_BIN)/galsCode.v $(DPD_BIN)/galsData.v $(DPD_BIN)/run
	cp $(DPD_BIN)/galsCode.v $(DPD_BIN)/code.v
	cp $(DPD_BIN)/galsData.v $(DPD_BIN)/data.v
	cp $(DPD_BIN)/dpdGALS.elf $(DPD_BIN)/dpd.elf

timed-dram-gals-msg-mgmt: DFLAGS=-DTIMER -DGALS -DDRAM -DMESSAGE_MANAGEMENT
timed-dram-gals-msg-mgmt: $(DPD_BIN) $(DPD_BIN)/galsCode.v $(DPD_BIN)/galsData.v $(DPD_BIN)/run
	cp $(DPD_BIN)/galsCode.v $(DPD_BIN)/code.v
	cp $(DPD_BIN)/galsData.v $(DPD_BIN)/data.v
	cp $(DPD_BIN)/dpdGALS.elf $(DPD_BIN)/dpd.elf

timed-gals-double-sqrt: DFLAGS=-DTIMER -DGALS -DDOUBLE_SQRT
timed-gals-double-sqrt: $(DPD_BIN) $(DPD_BIN)/galsCode.v $(DPD_BIN)/galsData.v $(DPD_BIN)/run
	cp $(DPD_BIN)/galsCode.v $(DPD_BIN)/code.v
	cp $(DPD_BIN)/galsData.v $(DPD_BIN)/data.v
	cp $(DPD_BIN)/dpdGALS.elf $(DPD_BIN)/dpd.elf

test-gals-msg-mgmt: DFLAGS=-DTESTING -DGALS -DMESSAGE_MANAGEMENT
test-gals-msg-mgmt: $(INC)/config.h $(HL)/*.o $(HOST_OBJS) $(DPD_BIN)/galsCode.v $(DPD_BIN)/galsData.v
	cp $(DPD_BIN)/galsCode.v $(DPD_BIN)/code.v
	cp $(DPD_BIN)/galsData.v $(DPD_BIN)/data.v
	cp $(DPD_BIN)/dpdGALS.elf $(DPD_BIN)/dpd.elf
	g++ -O2 -std=c++11 $(DFLAGS) -I $(INC) -I $(HL) -I $(DPD_INC) -c -o $(DPD_BIN)/test.o $(DPD_SRC)/test.cpp
	g++ -O2 -std=c++11 -o $(DPD_BIN)/test $(HOST_OBJS) $(HL)/*.o $(DPD_BIN)/test.o \
	  -static-libgcc -static-libstdc++ \
          -ljtag_atlantic -ljtag_client -lscotch -L$(QUARTUS_ROOTDIR)/linux64 \
          -Wl,-rpath,$(QUARTUS_ROOTDIR)/linux64 -lmetis -lpthread -lboost_program_options -lboost_filesystem -lboost_system -fopenmp

test-gals-msg-mgmt-large: DFLAGS=-DTESTING -DGALS -DLARGE_TEST -DMESSAGE_MANAGEMENT -DLARGE_TEST
test-gals-msg-mgmt-large: $(INC)/config.h $(HL)/*.o $(HOST_OBJS) $(DPD_BIN)/galsCode.v $(DPD_BIN)/galsData.v
	cp $(DPD_BIN)/galsCode.v $(DPD_BIN)/code.v
	cp $(DPD_BIN)/galsData.v $(DPD_BIN)/data.v
	cp $(DPD_BIN)/dpdGALS.elf $(DPD_BIN)/dpd.elf
	g++ -O2 -std=c++11 $(DFLAGS) -I $(INC) -I $(HL) -I $(DPD_INC) -c -o $(DPD_BIN)/test.o $(DPD_SRC)/test.cpp
	g++ -O2 -std=c++11 -o $(DPD_BIN)/test $(HOST_OBJS) $(HL)/*.o $(DPD_BIN)/test.o \
	  -static-libgcc -static-libstdc++ \
          -ljtag_atlantic -ljtag_client -lscotch -L$(QUARTUS_ROOTDIR)/linux64 \
          -Wl,-rpath,$(QUARTUS_ROOTDIR)/linux64 -lmetis -lpthread -lboost_program_options -lboost_filesystem -lboost_system -fopenmp

test-gals-double-sqrt: DFLAGS=-DTESTING -DGALS -DDOUBLE_SQRT
test-gals-double-sqrt: $(INC)/config.h $(HL)/*.o $(HOST_OBJS) $(DPD_BIN)/galsCode.v $(DPD_BIN)/galsData.v
	cp $(DPD_BIN)/galsCode.v $(DPD_BIN)/code.v
	cp $(DPD_BIN)/galsData.v $(DPD_BIN)/data.v
	cp $(DPD_BIN)/dpdGALS.elf $(DPD_BIN)/dpd.elf
	g++ -O2 -std=c++11 $(DFLAGS) -I $(INC) -I $(HL) -I $(DPD_INC) -c -o $(DPD_BIN)/test.o $(DPD_SRC)/test.cpp
	g++ -O2 -std=c++11 -o $(DPD_BIN)/test $(HOST_OBJS) $(HL)/*.o $(DPD_BIN)/test.o \
	  -static-libgcc -static-libstdc++ \
          -ljtag_atlantic -ljtag_client -lscotch -L$(QUARTUS_ROOTDIR)/linux64 \
          -Wl,-rpath,$(QUARTUS_ROOTDIR)/linux64 -lmetis -lpthread -lboost_program_options -lboost_filesystem -lboost_system -fopenmp

test-gals-obo: DFLAGS=-DTESTING -DGALS -DONE_BY_ONE
test-gals-obo: $(INC)/config.h $(HL)/*.o $(HOST_OBJS) $(DPD_BIN)/galsCode.v $(DPD_BIN)/galsData.v
	cp $(DPD_BIN)/galsCode.v $(DPD_BIN)/code.v
	cp $(DPD_BIN)/galsData.v $(DPD_BIN)/data.v
	cp $(DPD_BIN)/dpdGALS.elf $(DPD_BIN)/dpd.elf
	g++ -O2 -std=c++11 $(DFLAGS) -I $(INC) -I $(HL) -I $(DPD_INC) -c -o $(DPD_BIN)/test.o $(DPD_SRC)/test.cpp
	g++ -O2 -std=c++11 -o $(DPD_BIN)/test $(HOST_OBJS) $(HL)/*.o $(DPD_BIN)/test.o \
	  -static-libgcc -static-libstdc++ \
          -ljtag_atlantic -ljtag_client -lscotch -L$(QUARTUS_ROOTDIR)/linux64 \
          -Wl,-rpath,$(QUARTUS_ROOTDIR)/linux64 -lmetis -lpthread -lboost_program_options -lboost_filesystem -lboost_system -fopenmp

test-gals-obo-large: DFLAGS=-DTESTING -DGALS -DONE_BY_ONE -DLARGE_TEST
test-gals-obo-large: $(INC)/config.h $(HL)/*.o $(HOST_OBJS) $(DPD_BIN)/galsCode.v $(DPD_BIN)/galsData.v
	cp $(DPD_BIN)/galsCode.v $(DPD_BIN)/code.v
	cp $(DPD_BIN)/galsData.v $(DPD_BIN)/data.v
	cp $(DPD_BIN)/dpdGALS.elf $(DPD_BIN)/dpd.elf
	g++ -O2 -std=c++11 $(DFLAGS) -I $(INC) -I $(HL) -I $(DPD_INC) -c -o $(DPD_BIN)/test.o $(DPD_SRC)/test.cpp
	g++ -O2 -std=c++11 -o $(DPD_BIN)/test $(HOST_OBJS) $(HL)/*.o $(DPD_BIN)/test.o \
	  -static-libgcc -static-libstdc++ \
          -ljtag_atlantic -ljtag_client -lscotch -L$(QUARTUS_ROOTDIR)/linux64 \
          -Wl,-rpath,$(QUARTUS_ROOTDIR)/linux64 -lmetis -lpthread -lboost_program_options -lboost_filesystem -lboost_system -fopenmp

test-gals-new-verlet: DFLAGS=-DTESTING -DGALS -DBETTER_VERLET
test-gals-new-verlet: $(INC)/config.h $(HL)/*.o $(HOST_OBJS) $(DPD_BIN)/galsCode.v $(DPD_BIN)/galsData.v
	cp $(DPD_BIN)/galsCode.v $(DPD_BIN)/code.v
	cp $(DPD_BIN)/galsData.v $(DPD_BIN)/data.v
	cp $(DPD_BIN)/dpdGALS.elf $(DPD_BIN)/dpd.elf
	g++ -O2 -std=c++11 $(DFLAGS) -I $(INC) -I $(HL) -I $(DPD_INC) -c -o $(DPD_BIN)/test.o $(DPD_SRC)/test.cpp
	g++ -O2 -std=c++11 -o $(DPD_BIN)/test $(HOST_OBJS) $(HL)/*.o $(DPD_BIN)/test.o \
	  -static-libgcc -static-libstdc++ \
          -ljtag_atlantic -ljtag_client -lscotch -L$(QUARTUS_ROOTDIR)/linux64 \
          -Wl,-rpath,$(QUARTUS_ROOTDIR)/linux64 -lmetis -lpthread -lboost_program_options -lboost_filesystem -lboost_system -fopenmp

test-gals-new-verlet-large: DFLAGS=-DTESTING -DGALS -DBETTER_VERLET -DLARGE_TEST
test-gals-new-verlet-large: $(INC)/config.h $(HL)/*.o $(HOST_OBJS) $(DPD_BIN)/galsCode.v $(DPD_BIN)/galsData.v
	cp $(DPD_BIN)/galsCode.v $(DPD_BIN)/code.v
	cp $(DPD_BIN)/galsData.v $(DPD_BIN)/data.v
	cp $(DPD_BIN)/dpdGALS.elf $(DPD_BIN)/dpd.elf
	g++ -O2 -std=c++11 $(DFLAGS) -I $(INC) -I $(HL) -I $(DPD_INC) -c -o $(DPD_BIN)/test.o $(DPD_SRC)/test.cpp
	g++ -O2 -std=c++11 -o $(DPD_BIN)/test $(HOST_OBJS) $(HL)/*.o $(DPD_BIN)/test.o \
	  -static-libgcc -static-libstdc++ \
          -ljtag_atlantic -ljtag_client -lscotch -L$(QUARTUS_ROOTDIR)/linux64 \
          -Wl,-rpath,$(QUARTUS_ROOTDIR)/linux64 -lmetis -lpthread -lboost_program_options -lboost_filesystem -lboost_system -fopenmp

test-gals-obo-new-verlet: DFLAGS=-DTESTING -DGALS -DONE_BY_ONE -DBETTER_VERLET
test-gals-obo-new-verlet: $(INC)/config.h $(HL)/*.o $(HOST_OBJS) $(DPD_BIN)/galsCode.v $(DPD_BIN)/galsData.v
	cp $(DPD_BIN)/galsCode.v $(DPD_BIN)/code.v
	cp $(DPD_BIN)/galsData.v $(DPD_BIN)/data.v
	cp $(DPD_BIN)/dpdGALS.elf $(DPD_BIN)/dpd.elf
	g++ -O2 -std=c++11 $(DFLAGS) -I $(INC) -I $(HL) -I $(DPD_INC) -c -o $(DPD_BIN)/test.o $(DPD_SRC)/test.cpp
	g++ -O2 -std=c++11 -o $(DPD_BIN)/test $(HOST_OBJS) $(HL)/*.o $(DPD_BIN)/test.o \
	  -static-libgcc -static-libstdc++ \
          -ljtag_atlantic -ljtag_client -lscotch -L$(QUARTUS_ROOTDIR)/linux64 \
          -Wl,-rpath,$(QUARTUS_ROOTDIR)/linux64 -lmetis -lpthread -lboost_program_options -lboost_filesystem -lboost_system -fopenmp

test-gals-obo-new-verlet-large: DFLAGS=-DTESTING -DGALS -DONE_BY_ONE -DBETTER_VERLET -DLARGE_TEST
test-gals-obo-new-verlet-large: $(INC)/config.h $(HL)/*.o $(HOST_OBJS) $(DPD_BIN)/galsCode.v $(DPD_BIN)/galsData.v
	cp $(DPD_BIN)/galsCode.v $(DPD_BIN)/code.v
	cp $(DPD_BIN)/galsData.v $(DPD_BIN)/data.v
	cp $(DPD_BIN)/dpdGALS.elf $(DPD_BIN)/dpd.elf
	g++ -O2 -std=c++11 $(DFLAGS) -I $(INC) -I $(HL) -I $(DPD_INC) -c -o $(DPD_BIN)/test.o $(DPD_SRC)/test.cpp
	g++ -O2 -std=c++11 -o $(DPD_BIN)/test $(HOST_OBJS) $(HL)/*.o $(DPD_BIN)/test.o \
	  -static-libgcc -static-libstdc++ \
          -ljtag_atlantic -ljtag_client -lscotch -L$(QUARTUS_ROOTDIR)/linux64 \
          -Wl,-rpath,$(QUARTUS_ROOTDIR)/linux64 -lmetis -lpthread -lboost_program_options -lboost_filesystem -lboost_system -fopenmp

test-improved-gals: DFLAGS=-DTESTING -DGALS -DIMPROVED_GALS
test-improved-gals: $(INC)/config.h $(HL)/*.o $(HOST_OBJS) $(DPD_BIN)/galsCode.v $(DPD_BIN)/galsData.v
	cp $(DPD_BIN)/galsCode.v $(DPD_BIN)/code.v
	cp $(DPD_BIN)/galsData.v $(DPD_BIN)/data.v
	cp $(DPD_BIN)/dpdGALS.elf $(DPD_BIN)/dpd.elf
	g++ -O2 -std=c++11 $(DFLAGS) -I $(INC) -I $(HL) -I $(DPD_INC) -c -o $(DPD_BIN)/test.o $(DPD_SRC)/test.cpp
	g++ -O2 -std=c++11 -o $(DPD_BIN)/test $(HOST_OBJS) $(HL)/*.o $(DPD_BIN)/test.o \
	  -static-libgcc -static-libstdc++ \
          -ljtag_atlantic -ljtag_client -lscotch -L$(QUARTUS_ROOTDIR)/linux64 \
          -Wl,-rpath,$(QUARTUS_ROOTDIR)/linux64 -lmetis -lpthread -lboost_program_options -lboost_filesystem -lboost_system -fopenmp

test-improved-gals-large: DFLAGS=-DTESTING -DGALS -DIMPROVED_GALS -DLARGE_TEST
test-improved-gals-large: $(INC)/config.h $(HL)/*.o $(HOST_OBJS) $(DPD_BIN)/galsCode.v $(DPD_BIN)/galsData.v
	cp $(DPD_BIN)/galsCode.v $(DPD_BIN)/code.v
	cp $(DPD_BIN)/galsData.v $(DPD_BIN)/data.v
	cp $(DPD_BIN)/dpdGALS.elf $(DPD_BIN)/dpd.elf
	g++ -O2 -std=c++11 $(DFLAGS) -I $(INC) -I $(HL) -I $(DPD_INC) -c -o $(DPD_BIN)/test.o $(DPD_SRC)/test.cpp
	g++ -O2 -std=c++11 -o $(DPD_BIN)/test $(HOST_OBJS) $(HL)/*.o $(DPD_BIN)/test.o \
	  -static-libgcc -static-libstdc++ \
          -ljtag_atlantic -ljtag_client -lscotch -L$(QUARTUS_ROOTDIR)/linux64 \
          -Wl,-rpath,$(QUARTUS_ROOTDIR)/linux64 -lmetis -lpthread -lboost_program_options -lboost_filesystem -lboost_system -fopenmp

test-improved-gals-obo: DFLAGS=-DTESTING -DGALS -DIMPROVED_GALS -DONE_BY_ONE
test-improved-gals-obo: $(INC)/config.h $(HL)/*.o $(HOST_OBJS) $(DPD_BIN)/galsCode.v $(DPD_BIN)/galsData.v
	cp $(DPD_BIN)/galsCode.v $(DPD_BIN)/code.v
	cp $(DPD_BIN)/galsData.v $(DPD_BIN)/data.v
	cp $(DPD_BIN)/dpdGALS.elf $(DPD_BIN)/dpd.elf
	g++ -O2 -std=c++11 $(DFLAGS) -I $(INC) -I $(HL) -I $(DPD_INC) -c -o $(DPD_BIN)/test.o $(DPD_SRC)/test.cpp
	g++ -O2 -std=c++11 -o $(DPD_BIN)/test $(HOST_OBJS) $(HL)/*.o $(DPD_BIN)/test.o \
	  -static-libgcc -static-libstdc++ \
          -ljtag_atlantic -ljtag_client -lscotch -L$(QUARTUS_ROOTDIR)/linux64 \
          -Wl,-rpath,$(QUARTUS_ROOTDIR)/linux64 -lmetis -lpthread -lboost_program_options -lboost_filesystem -lboost_system -fopenmp

test-improved-gals-obo-large: DFLAGS=-DTESTING -DGALS -DIMPROVED_GALS -DONE_BY_ONE -DLARGE_TEST
test-improved-gals-obo-large: $(INC)/config.h $(HL)/*.o $(HOST_OBJS) $(DPD_BIN)/galsCode.v $(DPD_BIN)/galsData.v
	cp $(DPD_BIN)/galsCode.v $(DPD_BIN)/code.v
	cp $(DPD_BIN)/galsData.v $(DPD_BIN)/data.v
	cp $(DPD_BIN)/dpdGALS.elf $(DPD_BIN)/dpd.elf
	g++ -O2 -std=c++11 $(DFLAGS) -I $(INC) -I $(HL) -I $(DPD_INC) -c -o $(DPD_BIN)/test.o $(DPD_SRC)/test.cpp
	g++ -O2 -std=c++11 -o $(DPD_BIN)/test $(HOST_OBJS) $(HL)/*.o $(DPD_BIN)/test.o \
	  -static-libgcc -static-libstdc++ \
          -ljtag_atlantic -ljtag_client -lscotch -L$(QUARTUS_ROOTDIR)/linux64 \
          -Wl,-rpath,$(QUARTUS_ROOTDIR)/linux64 -lmetis -lpthread -lboost_program_options -lboost_filesystem -lboost_system -fopenmp

test-improved-gals-new-verlet: DFLAGS=-DTESTING -DGALS -DIMPROVED_GALS -DBETTER_VERLET
test-improved-gals-new-verlet: $(INC)/config.h $(HL)/*.o $(HOST_OBJS) $(DPD_BIN)/galsCode.v $(DPD_BIN)/galsData.v
	cp $(DPD_BIN)/galsCode.v $(DPD_BIN)/code.v
	cp $(DPD_BIN)/galsData.v $(DPD_BIN)/data.v
	cp $(DPD_BIN)/dpdGALS.elf $(DPD_BIN)/dpd.elf
	g++ -O2 -std=c++11 $(DFLAGS) -I $(INC) -I $(HL) -I $(DPD_INC) -c -o $(DPD_BIN)/test.o $(DPD_SRC)/test.cpp
	g++ -O2 -std=c++11 -o $(DPD_BIN)/test $(HOST_OBJS) $(HL)/*.o $(DPD_BIN)/test.o \
	  -static-libgcc -static-libstdc++ \
          -ljtag_atlantic -ljtag_client -lscotch -L$(QUARTUS_ROOTDIR)/linux64 \
          -Wl,-rpath,$(QUARTUS_ROOTDIR)/linux64 -lmetis -lpthread -lboost_program_options -lboost_filesystem -lboost_system -fopenmp

test-improved-gals-new-verlet-large: DFLAGS=-DTESTING -DGALS -DIMPROVED_GALS -DBETTER_VERLET -DLARGE_TEST
test-improved-gals-new-verlet-large: $(INC)/config.h $(HL)/*.o $(HOST_OBJS) $(DPD_BIN)/galsCode.v $(DPD_BIN)/galsData.v
	cp $(DPD_BIN)/galsCode.v $(DPD_BIN)/code.v
	cp $(DPD_BIN)/galsData.v $(DPD_BIN)/data.v
	cp $(DPD_BIN)/dpdGALS.elf $(DPD_BIN)/dpd.elf
	g++ -O2 -std=c++11 $(DFLAGS) -I $(INC) -I $(HL) -I $(DPD_INC) -c -o $(DPD_BIN)/test.o $(DPD_SRC)/test.cpp
	g++ -O2 -std=c++11 -o $(DPD_BIN)/test $(HOST_OBJS) $(HL)/*.o $(DPD_BIN)/test.o \
	  -static-libgcc -static-libstdc++ \
          -ljtag_atlantic -ljtag_client -lscotch -L$(QUARTUS_ROOTDIR)/linux64 \
          -Wl,-rpath,$(QUARTUS_ROOTDIR)/linux64 -lmetis -lpthread -lboost_program_options -lboost_filesystem -lboost_system -fopenmp

test-improved-gals-obo-new-verlet: DFLAGS=-DTESTING -DGALS -DONE_BY_ONE -DIMPROVED_GALS -DBETTER_VERLET
test-improved-gals-obo-new-verlet: $(INC)/config.h $(HL)/*.o $(HOST_OBJS) $(DPD_BIN)/galsCode.v $(DPD_BIN)/galsData.v
	cp $(DPD_BIN)/galsCode.v $(DPD_BIN)/code.v
	cp $(DPD_BIN)/galsData.v $(DPD_BIN)/data.v
	cp $(DPD_BIN)/dpdGALS.elf $(DPD_BIN)/dpd.elf
	g++ -O2 -std=c++11 $(DFLAGS) -I $(INC) -I $(HL) -I $(DPD_INC) -c -o $(DPD_BIN)/test.o $(DPD_SRC)/test.cpp
	g++ -O2 -std=c++11 -o $(DPD_BIN)/test $(HOST_OBJS) $(HL)/*.o $(DPD_BIN)/test.o \
	  -static-libgcc -static-libstdc++ \
          -ljtag_atlantic -ljtag_client -lscotch -L$(QUARTUS_ROOTDIR)/linux64 \
          -Wl,-rpath,$(QUARTUS_ROOTDIR)/linux64 -lmetis -lpthread -lboost_program_options -lboost_filesystem -lboost_system -fopenmp

test-improved-gals-obo-new-verlet-large: DFLAGS=-DTESTING -DGALS -DONE_BY_ONE -DIMPROVED_GALS -DBETTER_VERLET -DLARGE_TEST
test-improved-gals-obo-new-verlet-large: $(INC)/config.h $(HL)/*.o $(HOST_OBJS) $(DPD_BIN)/galsCode.v $(DPD_BIN)/galsData.v
	cp $(DPD_BIN)/galsCode.v $(DPD_BIN)/code.v
	cp $(DPD_BIN)/galsData.v $(DPD_BIN)/data.v
	cp $(DPD_BIN)/dpdGALS.elf $(DPD_BIN)/dpd.elf
	g++ -O2 -std=c++11 $(DFLAGS) -I $(INC) -I $(HL) -I $(DPD_INC) -c -o $(DPD_BIN)/test.o $(DPD_SRC)/test.cpp
	g++ -O2 -std=c++11 -o $(DPD_BIN)/test $(HOST_OBJS) $(HL)/*.o $(DPD_BIN)/test.o \
	  -static-libgcc -static-libstdc++ \
          -ljtag_atlantic -ljtag_client -lscotch -L$(QUARTUS_ROOTDIR)/linux64 \
          -Wl,-rpath,$(QUARTUS_ROOTDIR)/linux64 -lmetis -lpthread -lboost_program_options -lboost_filesystem -lboost_system -fopenmp

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
          -ljtag_atlantic -ljtag_client -lscotch -L$(QUARTUS_ROOTDIR)/linux64 \
          -Wl,-rpath,$(QUARTUS_ROOTDIR)/linux64 -lmetis -lpthread -lboost_program_options -lboost_filesystem -lboost_system -fopenmp

test-large: DFLAGS=-DTESTING -DLARGE_TEST
test-large: $(INC)/config.h $(HL)/*.o $(HOST_OBJS) $(DPD_BIN)/code.v $(DPD_BIN)/data.v
	g++ -O2 -std=c++11 $(DFLAGS) -I $(INC) -I $(HL) -I $(DPD_INC) -c -o $(DPD_BIN)/test.o $(DPD_SRC)/test.cpp
	g++ -O2 -std=c++11 -o $(DPD_BIN)/test $(HOST_OBJS) $(HL)/*.o $(DPD_BIN)/test.o \
	  -static-libgcc -static-libstdc++ \
          -ljtag_atlantic -ljtag_client -lscotch -L$(QUARTUS_ROOTDIR)/linux64 \
          -Wl,-rpath,$(QUARTUS_ROOTDIR)/linux64 -lmetis -lpthread -lboost_program_options -lboost_filesystem -lboost_system -fopenmp

# ------------ BONDS TESTING----------------------------
test-bonds: DFLAGS=-DTESTING -DBOND_TESTING
test-bonds: $(INC)/config.h $(HL)/*.o $(HOST_OBJS) $(DPD_BIN)/code.v $(DPD_BIN)/data.v
	g++ -O2 -std=c++11 $(DFLAGS) -I $(INC) -I $(HL) -I $(DPD_INC) -c -o $(DPD_BIN)/test.o $(DPD_SRC)/test.cpp
	g++ -O2 -std=c++11 -o $(DPD_BIN)/test $(HOST_OBJS) $(HL)/*.o $(DPD_BIN)/test.o \
	  -static-libgcc -static-libstdc++ \
          -ljtag_atlantic -ljtag_client -lscotch -L$(QUARTUS_ROOTDIR)/linux64 \
          -Wl,-rpath,$(QUARTUS_ROOTDIR)/linux64 -lmetis -lpthread -lboost_program_options -lboost_filesystem -lboost_system -fopenmp

# --------------- BEAD COUNTER -------------------------
bead-count: DFLAGS=-DTIMER -DGALS -DONE_BY_ONE -DBEAD_COUNTER
bead-count: $(DPD_BIN) $(DPD_BIN)/galsCode.v $(DPD_BIN)/galsData.v $(DPD_BIN)/run
	cp $(DPD_BIN)/galsCode.v $(DPD_BIN)/code.v
	cp $(DPD_BIN)/galsData.v $(DPD_BIN)/data.v
	cp $(DPD_BIN)/dpdGALS.elf $(DPD_BIN)/dpd.elf

bead-count-dram: DFLAGS=-DTIMER -DGALS -DONE_BY_ONE -DBEAD_COUNTER -DDRAM
bead-count-dram: $(DPD_BIN) $(DPD_BIN)/galsCode.v $(DPD_BIN)/galsData.v $(DPD_BIN)/run
	cp $(DPD_BIN)/galsCode.v $(DPD_BIN)/code.v
	cp $(DPD_BIN)/galsData.v $(DPD_BIN)/data.v
	cp $(DPD_BIN)/dpdGALS.elf $(DPD_BIN)/dpd.elf

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
          -ljtag_atlantic -ljtag_client -lscotch -L$(QUARTUS_ROOTDIR)/linux64 \
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

gals-obo: DFLAGS=-DVISUALISE -DGALS -DONE_BY_ONE
gals-obo: $(DPD_BIN) $(DPD_BIN)/galsCode.v $(DPD_BIN)/galsData.v $(DPD_BIN)/run
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
          -ljtag_atlantic -ljtag_client -lscotch -L$(QUARTUS_ROOTDIR)/linux64 \
          -Wl,-rpath,$(QUARTUS_ROOTDIR)/linux64 -lmetis -lpthread -lboost_program_options -lboost_filesystem -lboost_system -fopenmp

test-gals-large: DFLAGS=-DTESTING -DGALS -DLARGE_TEST
test-gals-large: $(INC)/config.h $(HL)/*.o $(HOST_OBJS) $(DPD_BIN)/galsCode.v $(DPD_BIN)/galsData.v
	cp $(DPD_BIN)/galsCode.v $(DPD_BIN)/code.v
	cp $(DPD_BIN)/galsData.v $(DPD_BIN)/data.v
	cp $(DPD_BIN)/dpdGALS.elf $(DPD_BIN)/dpd.elf
	g++ -O2 -std=c++11 $(DFLAGS) -I $(INC) -I $(HL) -I $(DPD_INC) -c -o $(DPD_BIN)/test.o $(DPD_SRC)/test.cpp
	g++ -O2 -std=c++11 -o $(DPD_BIN)/test $(HOST_OBJS) $(HL)/*.o $(DPD_BIN)/test.o \
	  -static-libgcc -static-libstdc++ \
          -ljtag_atlantic -ljtag_client -lscotch -L$(QUARTUS_ROOTDIR)/linux64 \
          -Wl,-rpath,$(QUARTUS_ROOTDIR)/linux64 -lmetis -lpthread -lboost_program_options -lboost_filesystem -lboost_system -fopenmp

timed-gals: DFLAGS=-DTIMER -DGALS
timed-gals: $(DPD_BIN) $(DPD_BIN)/galsCode.v $(DPD_BIN)/galsData.v $(DPD_BIN)/run
	cp $(DPD_BIN)/galsCode.v $(DPD_BIN)/code.v
	cp $(DPD_BIN)/galsData.v $(DPD_BIN)/data.v
	cp $(DPD_BIN)/dpdGALS.elf $(DPD_BIN)/dpd.elf

timed-gals-obo: DFLAGS=-DTIMER -DGALS -DONE_BY_ONE
timed-gals-obo: $(DPD_BIN) $(DPD_BIN)/galsCode.v $(DPD_BIN)/galsData.v $(DPD_BIN)/run
	cp $(DPD_BIN)/galsCode.v $(DPD_BIN)/code.v
	cp $(DPD_BIN)/galsData.v $(DPD_BIN)/data.v
	cp $(DPD_BIN)/dpdGALS.elf $(DPD_BIN)/dpd.elf

timed-dram-gals: DFLAGS=-DTIMER -DGALS -DDRAM
timed-dram-gals: $(DPD_BIN) $(DPD_BIN)/galsCode.v $(DPD_BIN)/galsData.v $(DPD_BIN)/run
	cp $(DPD_BIN)/galsCode.v $(DPD_BIN)/code.v
	cp $(DPD_BIN)/galsData.v $(DPD_BIN)/data.v
	cp $(DPD_BIN)/dpdGALS.elf $(DPD_BIN)/dpd.elf

timed-gals-obo-dram: DFLAGS=-DTIMER -DGALS -DONE_BY_ONE -DDRAM
timed-gals-obo-dram: $(DPD_BIN) $(DPD_BIN)/galsCode.v $(DPD_BIN)/galsData.v $(DPD_BIN)/run
	cp $(DPD_BIN)/galsCode.v $(DPD_BIN)/code.v
	cp $(DPD_BIN)/galsData.v $(DPD_BIN)/data.v
	cp $(DPD_BIN)/dpdGALS.elf $(DPD_BIN)/dpd.elf

timed-gals-new-verlet: DFLAGS=-DTIMER -DGALS -DBETTER_VERLET
timed-gals-new-verlet: $(DPD_BIN) $(DPD_BIN)/galsCode.v $(DPD_BIN)/galsData.v $(DPD_BIN)/run
	cp $(DPD_BIN)/galsCode.v $(DPD_BIN)/code.v
	cp $(DPD_BIN)/galsData.v $(DPD_BIN)/data.v
	cp $(DPD_BIN)/dpdGALS.elf $(DPD_BIN)/dpd.elf

timed-gals-new-verlet-dram: DFLAGS=-DTIMER -DGALS -DBETTER_VERLET -DDRAM
timed-gals-new-verlet-dram: $(DPD_BIN) $(DPD_BIN)/galsCode.v $(DPD_BIN)/galsData.v $(DPD_BIN)/run
	cp $(DPD_BIN)/galsCode.v $(DPD_BIN)/code.v
	cp $(DPD_BIN)/galsData.v $(DPD_BIN)/data.v
	cp $(DPD_BIN)/dpdGALS.elf $(DPD_BIN)/dpd.elf

timed-gals-obo-new-verlet: DFLAGS=-DTIMER -DGALS -DBETTER_VERLET -DONE_BY_ONE
timed-gals-obo-new-verlet: $(DPD_BIN) $(DPD_BIN)/galsCode.v $(DPD_BIN)/galsData.v $(DPD_BIN)/run
	cp $(DPD_BIN)/galsCode.v $(DPD_BIN)/code.v
	cp $(DPD_BIN)/galsData.v $(DPD_BIN)/data.v
	cp $(DPD_BIN)/dpdGALS.elf $(DPD_BIN)/dpd.elf

timed-gals-obo-new-verlet-dram: DFLAGS=-DTIMER -DGALS -DBETTER_VERLET -DONE_BY_ONE -DDRAM
timed-gals-obo-new-verlet-dram: $(DPD_BIN) $(DPD_BIN)/galsCode.v $(DPD_BIN)/galsData.v $(DPD_BIN)/run
	cp $(DPD_BIN)/galsCode.v $(DPD_BIN)/code.v
	cp $(DPD_BIN)/galsData.v $(DPD_BIN)/data.v
	cp $(DPD_BIN)/dpdGALS.elf $(DPD_BIN)/dpd.elf

timed-improved-gals: DFLAGS=-DTIMER -DGALS -DIMPROVED_GALS
timed-improved-gals: $(DPD_BIN) $(DPD_BIN)/galsCode.v $(DPD_BIN)/galsData.v $(DPD_BIN)/run
	cp $(DPD_BIN)/galsCode.v $(DPD_BIN)/code.v
	cp $(DPD_BIN)/galsData.v $(DPD_BIN)/data.v
	cp $(DPD_BIN)/dpdGALS.elf $(DPD_BIN)/dpd.elf

timed-improved-gals-dram: DFLAGS=-DTIMER -DGALS -DIMPROVED_GALS -DDRAM
timed-improved-gals-dram: $(DPD_BIN) $(DPD_BIN)/galsCode.v $(DPD_BIN)/galsData.v $(DPD_BIN)/run
	cp $(DPD_BIN)/galsCode.v $(DPD_BIN)/code.v
	cp $(DPD_BIN)/galsData.v $(DPD_BIN)/data.v
	cp $(DPD_BIN)/dpdGALS.elf $(DPD_BIN)/dpd.elf

timed-improved-gals-obo: DFLAGS=-DTIMER -DGALS -DIMPROVED_GALS -DONE_BY_ONE
timed-improved-gals-obo: $(DPD_BIN) $(DPD_BIN)/galsCode.v $(DPD_BIN)/galsData.v $(DPD_BIN)/run
	cp $(DPD_BIN)/galsCode.v $(DPD_BIN)/code.v
	cp $(DPD_BIN)/galsData.v $(DPD_BIN)/data.v
	cp $(DPD_BIN)/dpdGALS.elf $(DPD_BIN)/dpd.elf

timed-improved-gals-obo-dram: DFLAGS=-DTIMER -DGALS -DIMPROVED_GALS -DONE_BY_ONE -DDRAM
timed-improved-gals-obo-dram: $(DPD_BIN) $(DPD_BIN)/galsCode.v $(DPD_BIN)/galsData.v $(DPD_BIN)/run
	cp $(DPD_BIN)/galsCode.v $(DPD_BIN)/code.v
	cp $(DPD_BIN)/galsData.v $(DPD_BIN)/data.v
	cp $(DPD_BIN)/dpdGALS.elf $(DPD_BIN)/dpd.elf

timed-improved-gals-new-verlet: DFLAGS=-DTIMER -DGALS -DIMPROVED_GALS -DBETTER_VERLET
timed-improved-gals-new-verlet: $(DPD_BIN) $(DPD_BIN)/galsCode.v $(DPD_BIN)/galsData.v $(DPD_BIN)/run
	cp $(DPD_BIN)/galsCode.v $(DPD_BIN)/code.v
	cp $(DPD_BIN)/galsData.v $(DPD_BIN)/data.v
	cp $(DPD_BIN)/dpdGALS.elf $(DPD_BIN)/dpd.elf

timed-improved-gals-new-verlet-dram: DFLAGS=-DTIMER -DGALS -DIMPROVED_GALS -DBETTER_VERLET -DDRAM
timed-improved-gals-new-verlet-dram: $(DPD_BIN) $(DPD_BIN)/galsCode.v $(DPD_BIN)/galsData.v $(DPD_BIN)/run
	cp $(DPD_BIN)/galsCode.v $(DPD_BIN)/code.v
	cp $(DPD_BIN)/galsData.v $(DPD_BIN)/data.v
	cp $(DPD_BIN)/dpdGALS.elf $(DPD_BIN)/dpd.elf

timed-improved-gals-obo-new-verlet: DFLAGS=-DTIMER -DGALS -DIMPROVED_GALS -DBETTER_VERLET -DONE_BY_ONE
timed-improved-gals-obo-new-verlet: $(DPD_BIN) $(DPD_BIN)/galsCode.v $(DPD_BIN)/galsData.v $(DPD_BIN)/run
	cp $(DPD_BIN)/galsCode.v $(DPD_BIN)/code.v
	cp $(DPD_BIN)/galsData.v $(DPD_BIN)/data.v
	cp $(DPD_BIN)/dpdGALS.elf $(DPD_BIN)/dpd.elf

timed-improved-gals-obo-new-verlet-dram: DFLAGS=-DTIMER -DGALS -DIMPROVED_GALS -DBETTER_VERLET -DONE_BY_ONE -DDRAM
timed-improved-gals-obo-new-verlet-dram: $(DPD_BIN) $(DPD_BIN)/galsCode.v $(DPD_BIN)/galsData.v $(DPD_BIN)/run
	cp $(DPD_BIN)/galsCode.v $(DPD_BIN)/code.v
	cp $(DPD_BIN)/galsData.v $(DPD_BIN)/data.v
	cp $(DPD_BIN)/dpdGALS.elf $(DPD_BIN)/dpd.elf

visual-improved-gals-obo-new-verlet: DFLAGS=-DVISUALISE -DGALS -DIMPROVED_GALS -DBETTER_VERLET -DONE_BY_ONE
visual-improved-gals-obo-new-verlet: $(DPD_BIN) $(DPD_BIN)/galsCode.v $(DPD_BIN)/galsData.v $(DPD_BIN)/run
	cp $(DPD_BIN)/galsCode.v $(DPD_BIN)/code.v
	cp $(DPD_BIN)/galsData.v $(DPD_BIN)/data.v
	cp $(DPD_BIN)/dpdGALS.elf $(DPD_BIN)/dpd.elf

stats-gals: DFLAGS=-DSTATS -DGALS
stats-gals: TINSEL_LIB_INC=$(TINSEL_LIB)/lib.o
stats-gals: clean clean-tinsel $(TINSEL_LIB)/lib.o $(DPD_BIN) $(DPD_BIN)/galsCode.v $(DPD_BIN)/galsData.v $(DPD_BIN)/run
	cp $(DPD_BIN)/galsCode.v $(DPD_BIN)/code.v
	cp $(DPD_BIN)/galsData.v $(DPD_BIN)/data.v
	cp $(DPD_BIN)/dpdGALS.elf $(DPD_BIN)/dpd.elf

# ------------------ Oil and water with bonds ----------------------------
$(DPD_BIN)/OilWaterBonds.o: $(DPD_SRC)/OilWaterBonds.cpp
	g++ -O2 -std=c++11 $(DFLAGS) -I $(INC) -I $(HL) -I $(DPD_INC) -c -o $(DPD_BIN)/OilWaterBonds.o $(DPD_SRC)/OilWaterBonds.cpp

$(DPD_BIN)/bonds_run: $(DPD_INC)/dpdGALS.h $(HL)/*.o $(DPD_BIN) $(HOST_OBJS)
	g++ -O2 -std=c++11 -o $(DPD_BIN)/run $(HOST_OBJS) $(HL)/*.o $(DPD_BIN)/OilWaterBonds.o \
	  -static-libgcc -static-libstdc++ \
          -ljtag_atlantic -ljtag_client -lscotch -L$(QUARTUS_ROOTDIR)/linux64 \
          -Wl,-rpath,$(QUARTUS_ROOTDIR)/linux64 -lmetis -lpthread -lboost_program_options -lboost_filesystem -lboost_system -fopenmp

visual-oil-water-bonds: DFLAGS=-DVISUALISE -DGALS -DIMPROVED_GALS -DBETTER_VERLET -DONE_BY_ONE -DBONDS
visual-oil-water-bonds: $(DPD_BIN) $(DPD_BIN)/galsCode.v $(DPD_BIN)/galsData.v $(DPD_BIN)/OilWaterBonds.o $(DPD_BIN)/bonds_run
	cp $(DPD_BIN)/galsCode.v $(DPD_BIN)/code.v
	cp $(DPD_BIN)/galsData.v $(DPD_BIN)/data.v
	cp $(DPD_BIN)/dpdGALS.elf $(DPD_BIN)/dpd.elf

timed-oil-water-bonds: DFLAGS=-DTIMER -DGALS -DIMPROVED_GALS -DBETTER_VERLET -DONE_BY_ONE -DBONDS
timed-oil-water-bonds: $(DPD_BIN) $(DPD_BIN)/galsCode.v $(DPD_BIN)/galsData.v $(DPD_BIN)/OilWaterBonds.o $(DPD_BIN)/bonds_run
	cp $(DPD_BIN)/galsCode.v $(DPD_BIN)/code.v
	cp $(DPD_BIN)/galsData.v $(DPD_BIN)/data.v
	cp $(DPD_BIN)/dpdGALS.elf $(DPD_BIN)/dpd.elf

# ---------------------------- EXAMPLES --------------------------------
$(DPD_BIN)/bondsOnly.o: $(DPD_EXAMPLES)/bondsOnly.cpp
	g++ -O2 -std=c++11 $(DFLAGS) -I $(INC) -I $(HL) -I $(DPD_INC) -c -o $(DPD_BIN)/bondsOnly.o $(DPD_EXAMPLES)/bondsOnly.cpp

$(DPD_BIN)/bondsOnlyRun: $(DPD_INC)/dpdGALS.h $(HL)/*.o $(DPD_BIN) $(HOST_OBJS)
	g++ -O2 -std=c++11 -o $(DPD_BIN)/run $(HOST_OBJS) $(HL)/*.o $(DPD_BIN)/bondsOnly.o \
	  -static-libgcc -static-libstdc++ \
          -ljtag_atlantic -ljtag_client -lscotch -L$(QUARTUS_ROOTDIR)/linux64 \
          -Wl,-rpath,$(QUARTUS_ROOTDIR)/linux64 -lmetis -lpthread -lboost_program_options -lboost_filesystem -lboost_system -fopenmp

visual-bonds-only: DFLAGS=-DVISUALISE -DGALS -DIMPROVED_GALS -DBETTER_VERLET -DONE_BY_ONE -DBONDS -DDISABLE_CONS_FORCE -DDISABLE_DRAG_FORCE -DDISABLE_RAND_FORCE
visual-bonds-only: $(DPD_BIN) $(DPD_BIN)/galsCode.v $(DPD_BIN)/galsData.v $(DPD_BIN)/bondsOnly.o $(DPD_BIN)/bondsOnlyRun
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
	rm -rf DPD_mapping*.json
