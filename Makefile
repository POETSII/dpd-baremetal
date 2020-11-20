# Tinsel root
TINSEL_ROOT=submodules/tinsel

# Tinsel lib
TINSEL_LIB=$(TINSEL_ROOT)/lib

# Directories
DPD_BIN=./bin
DPD_SRC=./src
DPD_INC=./inc
DPD_UTILS=./utils
DPD_EXAMPLES=./examples
QUEUE_INC=./inc/concurrentqueue

# Tinsel includes
include $(TINSEL_ROOT)/globals.mk

# Local compiler flags
CFLAGS = $(RV_CFLAGS) -O2 -I $(INC) -I $(QUEUE_INC) -std=c++11
LDFLAGS = -melf32lriscv -G 0
DPD_OBJS = $(DPD_BIN)/Vector3D.o $(DPD_BIN)/utils.o
HOST_OBJS = $(DPD_BIN)/universe.o $(DPD_BIN)/ExternalClient.o $(DPD_BIN)/ExternalServer.o

# Script for connecting device as external
SOCAT_SCRIPT = ./scripts/socat_script

# This should be kept up to date as the "best" POETS DPD version
run: timed-improved-gals-new-verlet

visual: DFLAGS=-DVISUALISE -DGALS -DIMPROVED_GALS -DONE_BY_ONE
visual: base-gals oil-water

# This is used on a client PC to receive bead data for visualisation
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
	g++ -O2 -std=c++11 $(DFLAGS) $(EXTERNAL_FLAGS) -I $(INC) -I $(QUEUE_INC) -I $(HL) -I $(DPD_INC) -c -o $(DPD_BIN)/ExternalClient.o $(DPD_SRC)/ExternalClient.cpp

$(DPD_BIN)/ExternalServer.o: $(DPD_SRC)/ExternalServer.cpp $(DPD_INC)/ExternalServer.hpp
	mkdir -p $(DPD_BIN)
	g++ -O2 -std=c++11 $(DFLAGS) $(EXTERNAL_FLAGS) -I $(INC) -I $(QUEUE_INC) -I $(HL) -I $(DPD_INC) -c -o $(DPD_BIN)/ExternalServer.o $(DPD_SRC)/ExternalServer.cpp

$(DPD_BIN)/universe.o: $(DPD_SRC)/universe.cpp $(DPD_INC)/universe.hpp
	mkdir -p $(DPD_BIN)
	g++ -O2 -std=c++11 $(DFLAGS) $(EXTERNAL_FLAGS) -I $(INC) -I $(QUEUE_INC) -I $(HL) -I $(DPD_INC) -c -o $(DPD_BIN)/universe.o $(DPD_SRC)/universe.cpp

# -------------- Tinsel Object files --------------------------
$(DPD_BIN)/Vector3D.o: $(DPD_SRC)/Vector3D.cpp $(DPD_INC)/Vector3D.hpp
	mkdir -p $(DPD_BIN)
	$(RV_CC) $(CFLAGS) -Wall -c -DTINSEL -I $(DPD_INC) $(LD_FLAGS) $< -o $@

$(DPD_BIN)/utils.o: $(DPD_SRC)/utils.cpp $(DPD_INC)/utils.hpp
	mkdir -p $(DPD_BIN)
	$(RV_CC) $(CFLAGS) -Wall -c -DTINSEL $(DFLAGS) $(EXTERNAL_FLAGS) -I $(DPD_INC) $(LD_FLAGS) $< -o $@

# The external client
$(DPD_BIN)/dpd-bridge: $(DPD_SRC)/dpd-bridge.cpp $(HOST_OBJS)
	g++ -O2 -std=c++17 -o $(DPD_BIN)/dpd-bridge -I $(INC) -I $(QUEUE_INC) -I $(HL) -I $(DPD_INC) $(HOST_OBJS) $(DPD_SRC)/dpd-bridge.cpp \
		-lboost_program_options -lboost_filesystem -lboost_system -lpthread -lstdc++fs

# Compilation necessary for any form of POETS DPD
$(DPD_BIN)/entry.o: $(DPD_BIN)
	$(RV_CC) $(CFLAGS) -Wall -c -o $(DPD_BIN)/entry.o $(DPD_UTILS)/entry.S

$(DPD_BIN)/link.ld: $(DPD_UTILS)/genld.sh $(DPD_BIN)
	$(DPD_UTILS)/genld.sh > $(DPD_BIN)/link.ld

$(INC)/config.h: $(TINSEL_ROOT)/config.py
	make -C $(INC)

$(HL)/%.o:
	make -C $(HL)

# -------------- Synchronous elf --------------------------
$(DPD_BIN)/code.v: $(DPD_BIN)/dpd.elf $(DPD_BIN)
	$(BIN)/checkelf.sh $(DPD_BIN)/dpd.elf
	$(RV_OBJCOPY) -O verilog --only-section=.text $(DPD_BIN)/dpd.elf $@

$(DPD_BIN)/data.v: $(DPD_BIN)/dpd.elf $(DPD_BIN)
	$(RV_OBJCOPY) -O verilog --remove-section=.text \
                --set-section-flags .bss=alloc,load,contents $(DPD_BIN)/dpd.elf $@

# One by one is the best form
$(DPD_BIN)/dpd.elf: DFLAGS+=-DONE_BY_ONE
$(DPD_BIN)/dpd.elf: $(DPD_SRC)/sync.cpp $(DPD_INC)/sync.h $(DPD_BIN)/link.ld $(INC)/config.h $(INC)/tinsel.h $(DPD_BIN)/entry.o $(DPD_BIN) $(DPD_OBJS)
	$(RV_CC) $(CFLAGS) -Wall -c -DTINSEL $(DFLAGS) $(EXTERNAL_FLAGS) -I $(DPD_INC) -o $(DPD_BIN)/sync.o $<
	$(RV_LD) $(LDFLAGS) -T $(DPD_BIN)/link.ld -o $@ $(DPD_BIN)/entry.o $(DPD_BIN)/sync.o $(TINSEL_LIB_INC) $(DPD_OBJS)

# ----------------- GALS elf ------------------------------
$(DPD_BIN)/galsCode.v: $(DPD_BIN)/gals.elf $(DPD_BIN)
	$(BIN)/checkelf.sh $(DPD_BIN)/gals.elf
	$(RV_OBJCOPY) -O verilog --only-section=.text $(DPD_BIN)/gals.elf $@

$(DPD_BIN)/galsData.v: $(DPD_BIN)/gals.elf $(DPD_BIN)
	$(RV_OBJCOPY) -O verilog --remove-section=.text \
                --set-section-flags .bss=alloc,load,contents $(DPD_BIN)/gals.elf $@

$(DPD_BIN)/gals.elf: $(DPD_SRC)/gals.cpp $(DPD_INC)/gals.h $(DPD_BIN)/link.ld $(INC)/config.h $(INC)/tinsel.h $(DPD_BIN)/entry.o $(DPD_BIN) $(DPD_OBJS)
	$(RV_CC) $(CFLAGS) -Wall -c -DTINSEL $(DFLAGS) $(EXTERNAL_FLAGS) -I $(DPD_INC) -o $(DPD_BIN)/gals.o $<
	$(RV_LD) $(LDFLAGS) -T $(DPD_BIN)/link.ld -o $@ $(DPD_BIN)/entry.o $(DPD_BIN)/gals.o $(TINSEL_LIB_INC) $(DPD_OBJS)

# ----------------- Serial simulator ------------------------------
$(DPD_BIN)/serial.o: DFLAGS+=-DSERIAL
$(DPD_BIN)/serial.o: $(DPD_SRC)/serial.cpp $(DPD_INC)/serial.hpp
	mkdir -p $(DPD_BIN)
	g++ -O2 -std=c++11 $(DFLAGS) $(EXTERNAL_FLAGS) -I $(INC) -I $(QUEUE_INC) -I $(HL) -I $(DPD_INC) -I $(QUEUE_INC) -c -o $(DPD_BIN)/serial.o $(DPD_SRC)/serial.cpp

# Base GALS recipe which is used by all GALS recipes
# Improved gals and one by one make the best version of GALS
base-gals: DFLAGS+=-DGALS -DIMPROVED_GALS -DONE_BY_ONE
base-gals: $(DPD_BIN) $(DPD_BIN)/galsCode.v $(DPD_BIN)/galsData.v
	mv $(DPD_BIN)/galsCode.v $(DPD_BIN)/code.v
	mv $(DPD_BIN)/galsData.v $(DPD_BIN)/data.v
	mv $(DPD_BIN)/gals.elf $(DPD_BIN)/dpd.elf

# -------------- POETS DPD Applications --------------------------
# Simply to compile the volume initialisation for each example and cannot be
# used alone. A backend must be compiled also and these included

# Oil and water
oil-water: $(DPD_SRC)/run.cpp $(DPD_INC)/sync.h $(DPD_INC)/gals.h $(HL)/*.o $(DPD_BIN) $(HOST_OBJS)
	g++ -O2 -std=c++11 $(DFLAGS) $(EXTERNAL_FLAGS) -I $(INC) -I $(QUEUE_INC) -I $(HL) -I $(DPD_INC) -c -o $(DPD_BIN)/run.o $(DPD_SRC)/run.cpp
	g++ -O2 -std=c++11 -o $(DPD_BIN)/run $(HOST_OBJS) $(HL)/*.o $(DPD_BIN)/run.o \
	  -static-libgcc -static-libstdc++ \
      -ljtag_atlantic -ljtag_client -lscotch -L$(QUARTUS_ROOTDIR)/linux64 \
	  -L$(QUARTUS_ROOTDIR)/linux64 \
      -Wl,-rpath,$(QUARTUS_ROOTDIR)/linux64 -lmetis -lpthread -lboost_program_options -lboost_filesystem -lboost_system -fopenmp

# Oil and water with chains of bonded oil and water beads
oil-water-bonds: DFLAGS+=-DBONDS
oil-water-bonds: $(DPD_SRC)/OilWaterBonds.cpp $(DPD_INC)/sync.h $(DPD_INC)/gals.h $(HL)/*.o $(DPD_BIN) $(HOST_OBJS)
	g++ -O2 -std=c++11 $(DFLAGS) $(EXTERNAL_FLAGS) -I $(INC) -I $(QUEUE_INC) -I $(HL) -I $(DPD_INC) -c -o $(DPD_BIN)/OilWaterBonds.o $(DPD_SRC)/OilWaterBonds.cpp
	g++ -O2 -std=c++11 -o $(DPD_BIN)/run $(HOST_OBJS) $(HL)/*.o $(DPD_BIN)/OilWaterBonds.o \
	  -static-libgcc -static-libstdc++ \
          -ljtag_atlantic -ljtag_client -lscotch -L$(QUARTUS_ROOTDIR)/linux64 \
          -Wl,-rpath,$(QUARTUS_ROOTDIR)/linux64 -lmetis -lpthread -lboost_program_options -lboost_filesystem -lboost_system -fopenmp

# Only water beads
water-only: $(DPD_SRC)/WaterOnly.cpp $(DPD_INC)/sync.h $(DPD_INC)/gals.h $(HL)/*.o $(DPD_BIN) $(HOST_OBJS)
	g++ -O2 -std=c++11 $(DFLAGS) $(EXTERNAL_FLAGS) -I $(INC) -I $(QUEUE_INC) -I $(HL) -I $(DPD_INC) -c -o $(DPD_BIN)/WaterOnly.o $(DPD_SRC)/WaterOnly.cpp
	g++ -O2 -std=c++11 -o $(DPD_BIN)/run $(HOST_OBJS) $(HL)/*.o $(DPD_BIN)/WaterOnly.o \
	  -static-libgcc -static-libstdc++ \
          -ljtag_atlantic -ljtag_client -lscotch -L$(QUARTUS_ROOTDIR)/linux64 \
          -Wl,-rpath,$(QUARTUS_ROOTDIR)/linux64 -lmetis -lpthread -lboost_program_options -lboost_filesystem -lboost_system -fopenmp

# Simple example containing 3 pairs of bonded beads to test the bond force
bonds-only: $(DPD_EXAMPLES)/bondsOnly.cpp $(DPD_INC)/sync.h $(DPD_INC)/gals.h $(HL)/*.o $(DPD_BIN) $(HOST_OBJS)
	g++ -O2 -std=c++11 $(DFLAGS) $(EXTERNAL_FLAGS) -I $(INC) -I $(QUEUE_INC) -I $(HL) -I $(DPD_INC) -c -o $(DPD_BIN)/bondsOnly.o $(DPD_EXAMPLES)/bondsOnly.cpp
	g++ -O2 -std=c++11 -o $(DPD_BIN)/run $(HOST_OBJS) $(HL)/*.o $(DPD_BIN)/bondsOnly.o \
	  -static-libgcc -static-libstdc++ \
          -ljtag_atlantic -ljtag_client -lscotch -L$(QUARTUS_ROOTDIR)/linux64 \
          -Wl,-rpath,$(QUARTUS_ROOTDIR)/linux64 -lmetis -lpthread -lboost_program_options -lboost_filesystem -lboost_system -fopenmp

# RESTART SIMULATION - Used to restart a simulation from a saved state
restart: $(DPD_SRC)/restart.cpp $(DPD_INC)/sync.h $(DPD_INC)/gals.h $(HL)/*.o $(DPD_BIN) $(HOST_OBJS)
	g++ -O2 -std=c++11 $(DFLAGS) $(EXTERNAL_FLAGS) -I $(INC) -I $(QUEUE_INC) -I $(HL) -I $(DPD_INC) -c -o $(DPD_BIN)/restart.o $(DPD_SRC)/restart.cpp
	g++ -O2 -std=c++11 -o $(DPD_BIN)/restart $(HOST_OBJS) $(HL)/*.o $(DPD_BIN)/restart.o \
	  -static-libgcc -static-libstdc++ \
          -ljtag_atlantic -ljtag_client -lscotch -L$(QUARTUS_ROOTDIR)/linux64 \
          -Wl,-rpath,$(QUARTUS_ROOTDIR)/linux64 -lmetis -lpthread -lboost_program_options -lboost_filesystem -lboost_system -fopenmp

# --------------------------- SYNCHRONOUS TESTING ---------------------------
# Base for testing synchronous application
.PHONY: test
test: DFLAGS+=-DTESTING -DONE_BY_ONE
test: $(INC)/config.h $(HL)/*.o $(HOST_OBJS) $(DPD_BIN)/code.v $(DPD_BIN)/data.v
	g++ -O2 -std=c++11 $(DFLAGS) $(EXTERNAL_FLAGS) -I $(INC) -I $(QUEUE_INC) -I $(HL) -I $(DPD_INC) -c -o $(DPD_BIN)/test.o $(DPD_SRC)/test.cpp
	g++ -O2 -std=c++11 -o $(DPD_BIN)/test $(HOST_OBJS) $(HL)/*.o $(DPD_BIN)/test.o \
	  -static-libgcc -static-libstdc++ \
          -ljtag_atlantic -ljtag_client -lscotch -L$(QUARTUS_ROOTDIR)/linux64 \
          -Wl,-rpath,$(QUARTUS_ROOTDIR)/linux64 -lmetis -lpthread -lboost_program_options -lboost_filesystem -lboost_system -fopenmp

# Larger test
test-large: DFLAGS+=-DLARGE_TEST
test-large: test

# Test the improved verlet
test-new-verlet: DFLAGS=-DTESTING -DBETTER_VERLET
test-new-verlet: test

# Test the improved verlet with a larger test
test-new-verlet-large: DFLAGS=-DTESTING -DBETTER_VERLET -DLARGE_TEST
test-new-verlet-large: test-large

# Test with bonds
test-bonds: DFLAGS=-DBONDS
test-bonds: test

test-bonds-new-verlet: DFLAGS=-DBONDS -DBETTER_VERLET
test-bonds-new-verlet: test

# DT change refers to having a smaller dt for the first 1000 timesteps
test-dt-change: DFLAGS=-DSMALL_DT_EARLY
test-dt-change: test

test-large-dt-change: DFLAGS+=-DLARGE_TEST -DSMALL_DT_EARLY
test-large-dt-change: test

test-new-verlet-dt-change: DFLAGS=-DTESTING -DBETTER_VERLET -DSMALL_DT_EARLY
test-new-verlet-dt-change: test

test-new-verlet-large-dt-change: DFLAGS=-DTESTING -DBETTER_VERLET -DLARGE_TEST -DSMALL_DT_EARLY
test-new-verlet-large-dt-change: test-large

test-bonds-dt-change: DFLAGS=-DBONDS -DSMALL_DT_EARLY
test-bonds-dt-change: test

test-bonds-new-verlet-dt-change: DFLAGS=-DBONDS -DBETTER_VERLET -DSMALL_DT_EARLY
test-bonds-new-verlet-dt-change: test

# --------------------------- GALS TESTING ---------------------------
# Base for testing GALS application
# Improved gals and one by one make for the best GALS version
test-gals: DFLAGS+=-DTESTING -DGALS -DIMPROVED_GALS -DONE_BY_ONE
test-gals: $(INC)/config.h $(HL)/*.o $(HOST_OBJS) base-gals
	g++ -O2 -std=c++11 $(DFLAGS) $(EXTERNAL_FLAGS) -I $(INC) -I $(QUEUE_INC) -I $(HL) -I $(DPD_INC) -c -o $(DPD_BIN)/test.o $(DPD_SRC)/test.cpp
	g++ -O2 -std=c++11 -o $(DPD_BIN)/test $(HOST_OBJS) $(HL)/*.o $(DPD_BIN)/test.o \
	  -static-libgcc -static-libstdc++ \
          -ljtag_atlantic -ljtag_client -lscotch -L$(QUARTUS_ROOTDIR)/linux64 \
          -Wl,-rpath,$(QUARTUS_ROOTDIR)/linux64 -lmetis -lpthread -lboost_program_options -lboost_filesystem -lboost_system -fopenmp

# Larger test
test-gals-large: DFLAGS+=-DLARGE_TEST
test-gals-large: test-gals

# Test the improved verlet
test-gals-new-verlet: DFLAGS=-DBETTER_VERLET
test-gals-new-verlet: test-gals

# Test the improved verlet with a larger test
test-gals-new-verlet-large: DFLAGS=-DBETTER_VERLET
test-gals-new-verlet-large: test-gals-large

# Test with bonds
test-gals-bonds: DFLAGS=-DBONDS
test-gals-bonds: test-gals

# Test with bonds and new verlet
test-gals-bonds-new-verlet: DFLAGS=-DBONDS -DBETTER_VERLET
test-gals-bonds-new-verlet: test-gals

test-gals-dt-change: DFLAGS+=-DTESTING -DGALS -DIMPROVED_GALS -DONE_BY_ONE -DSMALL_DT_EARLY
test-gals-dt-change: test-gals

# Larger test
test-gals-large-dt-change: DFLAGS+=-DLARGE_TEST -DSMALL_DT_EARLY
test-gals-large-dt-change: test-gals

# Test the improved verlet
test-gals-new-verlet-dt-change: DFLAGS=-DBETTER_VERLET -DSMALL_DT_EARLY
test-gals-new-verlet-dt-change: test-gals

# Test the improved verlet with a larger test
test-gals-new-verlet-large-dt-change: DFLAGS=-DBETTER_VERLET -DSMALL_DT_EARLY
test-gals-new-verlet-large-dt-change: test-gals-large

# Test with bonds
test-gals-bonds-dt-change: DFLAGS=-DBONDS -DSMALL_DT_EARLY
test-gals-bonds-dt-change: test-gals

# Test with bonds and new verlet
test-gals-bonds-new-verlet-dt-change: DFLAGS=-DBONDS -DBETTER_VERLET -DSMALL_DT_EARLY
test-gals-bonds-new-verlet-dt-change: test-gals

# --------------------------- TIMED RUNS ---------------------------
timed-run: DFLAGS=-DTIMER
timed-run: run

timed-dram-run: DFLAGS=-DTIMER -DDRAM
timed-dram-run: run

timed-run-new-verlet: DFLAGS=-DTIMER -DBETTER_VERLET
timed-run-new-verlet: run

timed-dram-run-new-verlet: DFLAGS=-DTIMER -DDRAM -DBETTER_VERLET
timed-dram-run-new-verlet: run

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

onebyone-new-verlet: DFLAGS=-DTIMER -DONE_BY_ONE -DBETTER_VERLET
onebyone-new-verlet: run

onebyone-new-verlet-dram: DFLAGS=-DTIMER -DONE_BY_ONE -DBETTER_VERLET -DDRAM
onebyone-new-verlet-dram: run

onebyone-double-sqrt-new-verlet: DFLAGS=-DTIMER -DONE_BY_ONE -DDOUBLE_SQRT -DBETTER_VERLET
onebyone-double-sqrt-new-verlet: run

test-onebyone: DFLAGS=-DONE_BY_ONE
test-onebyone: test

test-onebyone-large: DFLAGS=-DONE_BY_ONE
test-onebyone-large: test-large

test-onebyone-new-verlet: DFLAGS=-DTESTING -DONE_BY_ONE -DBETTER_VERLET
test-onebyone-new-verlet: test

test-onebyone-new-verlet-large: DFLAGS=-DTESTING -DONE_BY_ONE -DBETTER_VERLET -DLARGE_TEST
test-onebyone-new-verlet-large: test

# ------------ Send beads to self to reduce local calculation memory space -------------------
sendtoself: DFLAGS=-DTIMER -DSEND_TO_SELF
sendtoself: run

sendtoself-dram: DFLAGS=-DTIMER -DSEND_TO_SELF -DDRAM
sendtoself-dram: run

sendtoself-new-verlet: DFLAGS=-DTIMER -DSEND_TO_SELF -DBETTER_VERLET
sendtoself-new-verlet: run

sendtoself-dram-new-verlet: DFLAGS=-DTIMER -DSEND_TO_SELF -DDRAM -DBETTER_VERLET
sendtoself-dram-new-verlet: run

test-sendtoself: DFLAGS=-DTESTING -DSEND_TO_SELF
test-sendtoself: test

test-sendtoself-large: DFLAGS=-DSEND_TO_SELF -DLARGE_TEST
test-sendtoself-large: test

test-sendtoself-new-verlet: DFLAGS=-DSEND_TO_SELF -DBETTER_VERLET
test-sendtoself-new-verlet: test

# TODO: 46x46x46 is too large for send to self. Make smaller test for this, or smaller for all
test-sendtoself-new-verlet-large: DFLAGS=-DSEND_TO_SELF -DBETTER_VERLET -DLARGE_TEST
test-sendtoself-new-verlet-large: test

# ------------ Use message manager to reduce NoC strain -------------------
# With multicast as part of tinsel-0.7 > this is now obsolete

# Standard timed with message management
timed-msg-mgmt: DFLAGS=-DTIMER -DMESSAGE_MANAGEMENT
timed-msg-mgmt: run

# Standard timed with message management and vertices mapped to DRAM
timed-msg-mgmt-dram: DFLAGS=-DTIMER -DMESSAGE_MANAGEMENT -DDRAM
timed-msg-mgmt-dram: run

timed-msg-mgmt-new-verlet: DFLAGS=-DTIMER -DMESSAGE_MANAGEMENT -DBETTER_VERLET
timed-msg-mgmt-new-verlet: run

timed-msg-mgmt-new-verlet-dram: DFLAGS=-DTIMER -DMESSAGE_MANAGEMENT -DDRAM -DBETTER_VERLET
timed-msg-mgmt-new-verlet-dram: run

# Test standard with message management
test-msg-mgmt: DFLAGS=-DMESSAGE_MANAGEMENT
test-msg-mgmt: test

# Large test of standard with message management
test-msg-mgmt-large: DFLAGS=-DMESSAGE_MANAGEMENT -DLARGE_TEST
test-msg-mgmt-large: test

test-msg-mgmt-new-verlet: DFLAGS=-DMESSAGE_MANAGEMENT -DBETTER_VERLET
test-msg-mgmt-new-verlet: test

test-msg-mgmt-new-verlet-large: DFLAGS=-DMESSAGE_MANAGEMENT -DBETTER_VERLET -DLARGE_TEST
test-msg-mgmt-new-verlet-large: test

# One by one timed with message management
timed-obo-msg-mgmt: DFLAGS=-DTIMER -DONE_BY_ONE -DMESSAGE_MANAGEMENT
timed-obo-msg-mgmt: run

# One by one with message management
obo-msg-mgmt: DFLAGS=-DONE_BY_ONE -DMESSAGE_MANAGEMENT -DVISUALISE
obo-msg-mgmt: run

# One by one timed with message management and vertices mapped to DRAM
timed-obo-msg-mgmt-dram: DFLAGS=-DTIMER -DONE_BY_ONE -DMESSAGE_MANAGEMENT -DDRAM
timed-obo-msg-mgmt-dram: run

timed-obo-msg-mgmt-new-verlet: DFLAGS=-DTIMER -DONE_BY_ONE -DMESSAGE_MANAGEMENT -DBETTER_VERLET
timed-obo-msg-mgmt-new-verlet: run

obo-msg-mgmt-new-verlet: DFLAGS=-DONE_BY_ONE -DMESSAGE_MANAGEMENT -DVISUALISE -DBETTER_VERLET
obo-msg-mgmt-new-verlet: run

timed-obo-msg-mgmt-new-verlet-dram: DFLAGS=-DTIMER -DONE_BY_ONE -DMESSAGE_MANAGEMENT -DDRAM -DBETTER_VERLET
timed-obo-msg-mgmt-new-verlet-dram: run

# Test one by one with message management
test-obo-msg-mgmt: DFLAGS=-DONE_BY_ONE -DMESSAGE_MANAGEMENT
test-obo-msg-mgmt: test

# Large test of one by one with message management
test-obo-msg-mgmt-large: DFLAGS=-DONE_BY_ONE -DMESSAGE_MANAGEMENT -DLARGE_TEST
test-obo-msg-mgmt-large: test

test-obo-msg-mgmt-new-verlet: DFLAGS=-DONE_BY_ONE -DMESSAGE_MANAGEMENT -DBETTER_VERLET
test-obo-msg-mgmt-new-verlet: test

test-obo-msg-mgmt-new-verlet-large: DFLAGS=-DONE_BY_ONE -DMESSAGE_MANAGEMENT -DBETTER_VERLET -DLARGE_TEST
test-obo-msg-mgmt-new-verlet-large: test

# Send to self timed with message management
timed-sts-msg-mgmt: DFLAGS=-DTIMER -DSEND_TO_SELF -DMESSAGE_MANAGEMENT
timed-sts-msg-mgmt: run

timed-sts-msg-mgmt-new-verlet: DFLAGS=-DTIMER -DSEND_TO_SELF -DMESSAGE_MANAGEMENT -DBETTER_VERLET
timed-sts-msg-mgmt-new-verlet: run

# Send to self with message management
sts-msg-mgmt: DFLAGS=-DSEND_TO_SELF -DMESSAGE_MANAGEMENT -DVISUALISE
sts-msg-mgmt: run

sts-msg-mgmt-new-verlet: DFLAGS=-DSEND_TO_SELF -DMESSAGE_MANAGEMENT -DVISUALISE -DBETTER_VERLET
sts-msg-mgmt-new-verlet: run

# Send to self timed with message management and vertices mapped to DRAM
timed-sts-msg-mgmt-dram: DFLAGS=-DTIMER -DSEND_TO_SELF -DMESSAGE_MANAGEMENT -DDRAM
timed-sts-msg-mgmt-dram: run

timed-sts-msg-mgmt-new-verlet-dram: DFLAGS=-DTIMER -DSEND_TO_SELF -DMESSAGE_MANAGEMENT -DDRAM -DBETTER_VERLET
timed-sts-msg-mgmt-new-verlet-dram: run

# Test send to self with message management
test-sts-msg-mgmt: DFLAGS=-DSEND_TO_SELF -DMESSAGE_MANAGEMENT
test-sts-msg-mgmt: test

# Large send to self with message management
test-sts-msg-mgmt-large: DFLAGS=-DSEND_TO_SELF -DMESSAGE_MANAGEMENT -DLARGE_TEST
test-sts-msg-mgmt-large: test

test-sts-msg-mgmt-new-verlet: DFLAGS=-DSEND_TO_SELF -DMESSAGE_MANAGEMENT -DBETTER_VERLET
test-sts-msg-mgmt-new-verlet: test

test-sts-msg-mgmt-new-verlet-large: DFLAGS=-DSEND_TO_SELF -DMESSAGE_MANAGEMENT -DBETTER_VERLET -DLARGE_TEST
test-sts-msg-mgmt-new-verlet-large: test

gals-msg-mgmt: DFLAGS=-DVISUALISE -DMESSAGE_MANAGEMENT
gals-msg-mgmt: base-gals

timed-gals-msg-mgmt: DFLAGS=-DTIMER -DGALS -DMESSAGE_MANAGEMENT
timed-gals-msg-mgmt: base-gals

timed-dram-gals-msg-mgmt: DFLAGS=-DTIMER -DDRAM -DGALS -DMESSAGE_MANAGEMENT
timed-dram-gals-msg-mgmt: base-gals

timed-gals-double-sqrt: DFLAGS=-DTIMER -DGALS -DDOUBLE_SQRT
timed-gals-double-sqrt: base-gals

test-gals-msg-mgmt: DFLAGS=-DGALS -DMESSAGE_MANAGEMENT -DTESTING
test-gals-msg-mgmt: $(INC)/config.h $(HL)/*.o $(HOST_OBJS) base-gals test-gals

test-gals-msg-mgmt-large: DFLAGS=-DGALS -DMESSAGE_MANAGEMENT -DLARGE_TEST
test-gals-msg-mgmt-large: $(INC)/config.h $(HL)/*.o base-gals test-gals

test-gals-double-sqrt: DFLAGS=-DGALS -DDOUBLE_SQRT
test-gals-double-sqrt: $(INC)/config.h $(HL)/*.o $(HOST_OBJS) base-gals test-gals

test-gals-obo: DFLAGS=-DGALS -DONE_BY_ONE
test-gals-obo: $(INC)/config.h $(HL)/*.o $(HOST_OBJS) base-gals test-gals

test-gals-obo-large: DFLAGS=-DGALS -DONE_BY_ONE -DLARGE_TEST
test-gals-obo-large: $(INC)/config.h $(HL)/*.o $(HOST_OBJS) base-gals test-gals

test-gals-new-verlet: DFLAGS=-DGALS -DBETTER_VERLET
test-gals-new-verlet: $(INC)/config.h $(HL)/*.o $(HOST_OBJS) base-gals test-gals

test-gals-new-verlet-large: DFLAGS=-DGALS -DBETTER_VERLET -DLARGE_TEST
test-gals-new-verlet-large: $(INC)/config.h $(HL)/*.o $(HOST_OBJS) base-gals test-gals

test-gals-obo-new-verlet: DFLAGS=-DGALS -DONE_BY_ONE -DBETTER_VERLET
test-gals-obo-new-verlet: $(INC)/config.h $(HL)/*.o $(HOST_OBJS) base-gals test-gals

test-gals-obo-new-verlet-large: DFLAGS=-DGALS -DONE_BY_ONE -DBETTER_VERLET -DLARGE_TEST
test-gals-obo-new-verlet-large: $(INC)/config.h $(HL)/*.o $(HOST_OBJS) base-gals test-gals

test-improved-gals: DFLAGS=-DGALS -DIMPROVED_GALS
test-improved-gals: $(INC)/config.h $(HL)/*.o $(HOST_OBJS) base-gals test-gals

test-improved-gals-large: DFLAGS=-DGALS -DIMPROVED_GALS -DLARGE_TEST
test-improved-gals-large: $(INC)/config.h $(HL)/*.o $(HOST_OBJS) base-gals test-gals

test-improved-gals-obo: DFLAGS=-DTESTING -DGALS -DIMPROVED_GALS -DONE_BY_ONE
test-improved-gals-obo: $(INC)/config.h $(HL)/*.o $(HOST_OBJS) base-gals test-gals

test-improved-gals-obo-large: DFLAGS=-DGALS -DIMPROVED_GALS -DONE_BY_ONE -DLARGE_TEST
test-improved-gals-obo-large: $(INC)/config.h $(HL)/*.o $(HOST_OBJS) base-gals test-gals

test-improved-gals-new-verlet: DFLAGS=-DGALS -DIMPROVED_GALS -DBETTER_VERLET
test-improved-gals-new-verlet: $(INC)/config.h $(HL)/*.o $(HOST_OBJS) base-gals test-gals

test-improved-gals-new-verlet-large: DFLAGS=-DGALS -DIMPROVED_GALS -DBETTER_VERLET -DLARGE_TEST
test-improved-gals-new-verlet-large: $(INC)/config.h $(HL)/*.o $(HOST_OBJS) base-gals test-gals

test-improved-gals-obo-new-verlet: DFLAGS=-DGALS -DONE_BY_ONE -DIMPROVED_GALS -DBETTER_VERLET
test-improved-gals-obo-new-verlet: $(INC)/config.h $(HL)/*.o $(HOST_OBJS) base-gals test-gals

test-improved-gals-obo-new-verlet-large: DFLAGS=-DGALS -DONE_BY_ONE -DIMPROVED_GALS -DBETTER_VERLET -DLARGE_TEST
test-improved-gals-obo-new-verlet-large: $(INC)/config.h $(HL)/*.o $(HOST_OBJS) base-gals test-gals

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

# --------------- BEAD COUNTER -------------------------
bead-count: DFLAGS=-DTIMER -DGALS -DONE_BY_ONE -DBEAD_COUNTER
bead-count: base-gals

bead-count-dram: DFLAGS=-DTIMER -DGALS -DONE_BY_ONE -DBEAD_COUNTER -DDRAM
bead-count-dram: base-gals

# ------------- FILE PARSING AND RUNNING ---------------
$(DPD_BIN)/parsedCode.v: $(DPD_BIN)/parsedDPD.elf $(DPD_BIN)
	$(BIN)/checkelf.sh $(DPD_BIN)/parsedDPD.elf
	$(RV_OBJCOPY) -O verilog --only-section=.text $(DPD_BIN)/parsedDPD.elf $@

$(DPD_BIN)/parsedData.v: $(DPD_BIN)/parsedDPD.elf $(DPD_BIN)
	$(RV_OBJCOPY) -O verilog --remove-section=.text \
                --set-section-flags .bss=alloc,load,contents $(DPD_BIN)/parsedDPD.elf $@

$(DPD_BIN)/parsedDPD.elf: $(DPD_SRC)/parsedsync.cpp $(DPD_INC)/parsedsync.h $(DPD_BIN)/link.ld $(INC)/config.h $(INC)/tinsel.h $(DPD_BIN)/entry.o $(DPD_BIN) $(DPD_OBJS)
	$(RV_CC) $(CFLAGS) -Wall -c -DTINSEL $(DFLAGS) $(EXTERNAL_FLAGS) -I $(DPD_INC) -o $(DPD_BIN)/parsedsync.o $<
	$(RV_LD) $(LDFLAGS) -T $(DPD_BIN)/link.ld -o $@ $(DPD_BIN)/entry.o $(DPD_BIN)/parsedsync.o $(TINSEL_LIB_INC) $(DPD_OBJS)

PARSE_OBJS = $(DPD_BIN)/ExternalClient.o $(DPD_BIN)/ExternalServer.o

$(DPD_BIN)/parserRun: $(DPD_SRC)/parserRun.cpp $(PARSE_OBJS) $(DPD_SRC)/parseUniverse.cpp $(DPD_INC)/parseUniverse.hpp $(DPD_SRC)/parser.cpp $(DPD_INC)/parser.hpp $(DPD_INC)/parsedsync.h $(HL)/*.o $(DPD_BIN) $(PARSE_OBJS)
	g++ -O2 -std=c++11 $(DFLAGS) $(EXTERNAL_FLAGS) -I $(INC) -I $(QUEUE_INC) -I $(HL) -I $(DPD_INC) -c -o $(DPD_BIN)/parserRun.o $(DPD_SRC)/parserRun.cpp
	g++ -O2 -std=c++11 -o $(DPD_BIN)/parserRun $(PARSE_OBJS) $(HL)/*.o $(DPD_BIN)/parserRun.o \
	  -static-libgcc -static-libstdc++ \
          -ljtag_atlantic -ljtag_client -lscotch -L$(QUARTUS_ROOTDIR)/linux64 \
          -Wl,-rpath,$(QUARTUS_ROOTDIR)/linux64 -lmetis -lpthread -lboost_program_options -lboost_filesystem -lboost_system -fopenmp

parse: DFLAGS=-DVISUALISE
parse: $(DPD_BIN) $(DPD_BIN)/parsedCode.v $(DPD_BIN)/parsedData.v $(DPD_BIN)/parserRun

ptime: DFLAGS=-DTIMER
ptime: $(DPD_BIN) $(DPD_SRC)/timer.cpp $(DPD_INC)/timer.h $(DPD_BIN)/parsedCode.v $(DPD_BIN)/parsedData.v $(DPD_BIN)/parserRun

pobo: DFLAGS=-DTIMER -DONE_BY_ONE
pobo: $(DPD_BIN) $(DPD_SRC)/timer.cpp $(DPD_INC)/timer.h $(DPD_BIN)/parsedCode.v $(DPD_BIN)/parsedData.v $(DPD_BIN)/parserRun

pobov: DFLAGS=-DVISUALISE -DONE_BY_ONE
pobov: $(DPD_BIN) $(DPD_BIN)/parsedCode.v $(DPD_BIN)/parsedData.v $(DPD_BIN)/parserRun

visual-gals: DFLAGS=-DVISUALISE
visual-gals: base-gals oil-water

gals-obo: DFLAGS=-DVISUALISE -DGALS -DONE_BY_ONE
gals-obo: base-gals oil-water

timed-gals: DFLAGS=-DTIMER -DGALS
timed-gals: base-gals oil-water

timed-gals-obo: DFLAGS=-DTIMER -DGALS -DONE_BY_ONE
timed-gals-obo: base-gals oil-water

timed-dram-gals: DFLAGS=-DTIMER -DDRAM -DDGALS
timed-dram-gals: base-gals oil-water

timed-gals-obo-dram: DFLAGS=-DTIMER -DGALS -DONE_BY_ONE -DDRAM
timed-gals-obo-dram: base-gals oil-water

timed-gals-new-verlet: DFLAGS=-DTIMER -DGALS -DBETTER_VERLET
timed-gals-new-verlet: base-gals oil-water

timed-gals-new-verlet-dram: DFLAGS=-DTIMER -DGALS -DBETTER_VERLET -DDRAM
timed-gals-new-verlet-dram: base-gals oil-water

timed-gals-obo-new-verlet: DFLAGS=-DTIMER -DGALS -DBETTER_VERLET -DONE_BY_ONE
timed-gals-obo-new-verlet: base-gals oil-water

timed-gals-obo-new-verlet-dram: DFLAGS=-DTIMER -DGALS -DBETTER_VERLET -DONE_BY_ONE -DDRAM
timed-gals-obo-new-verlet-dram: base-gals oil-water

timed-improved-gals: DFLAGS=-DTIMER -DGALS -DIMPROVED_GALS
timed-improved-gals: base-gals oil-water

timed-improved-gals-dram: DFLAGS=-DTIMER -DGALS -DIMPROVED_GALS -DDRAM
timed-improved-gals-dram: base-gals oil-water

timed-improved-gals-obo: DFLAGS=-DTIMER -DGALS -DIMPROVED_GALS -DONE_BY_ONE
timed-improved-gals-obo: base-gals oil-water

timed-improved-gals-obo-dram: DFLAGS=-DTIMER -DGALS -DIMPROVED_GALS -DONE_BY_ONE -DDRAM
timed-improved-gals-obo-dram: base-gals oil-water

timed-improved-gals-new-verlet: DFLAGS=-DTIMER -DGALS -DIMPROVED_GALS -DBETTER_VERLET
timed-improved-gals-new-verlet: base-gals oil-water

timed-improved-gals-new-verlet-dram: DFLAGS=-DTIMER -DGALS -DIMPROVED_GALS -DBETTER_VERLET -DDRAM
timed-improved-gals-new-verlet-dram: base-gals oil-water

timed-improved-gals-obo-new-verlet: DFLAGS=-DTIMER -DGALS -DIMPROVED_GALS -DBETTER_VERLET -DONE_BY_ONE
timed-improved-gals-obo-new-verlet: base-gals oil-water

timed-improved-gals-obo-new-verlet-dram: DFLAGS=-DTIMER -DGALS -DIMPROVED_GALS -DBETTER_VERLET -DONE_BY_ONE -DDRAM
timed-improved-gals-obo-new-verlet-dram: base-gals oil-water

visual-improved-gals-obo-new-verlet: DFLAGS=-DVISUALISE -DGALS -DIMPROVED_GALS -DBETTER_VERLET -DONE_BY_ONE
visual-improved-gals-obo-new-verlet: base-gals oil-water

stats-gals: DFLAGS=-DSTATS
stats-gals: TINSEL_LIB_INC=$(TINSEL_LIB)/lib.o
stats-gals: clean clean-tinsel $(TINSEL_LIB)/lib.o base-gals oil-water

# ------------------ Oil and water with bonds ----------------------------
$(DPD_BIN)/OilWaterBonds.o: $(DPD_SRC)/OilWaterBonds.cpp
	g++ -O2 -std=c++11 $(DFLAGS) $(EXTERNAL_FLAGS) -I $(INC) -I $(QUEUE_INC) -I $(HL) -I $(DPD_INC) -c -o $(DPD_BIN)/OilWaterBonds.o $(DPD_SRC)/OilWaterBonds.cpp

$(DPD_BIN)/bonds_run: $(DPD_BIN)/OilWaterBonds.o $(HL)/*.o $(DPD_BIN) $(HOST_OBJS)
	g++ -O2 -std=c++11 -o $(DPD_BIN)/run $(HOST_OBJS) $(HL)/*.o $(DPD_BIN)/OilWaterBonds.o \
	  -static-libgcc -static-libstdc++ \
          -ljtag_atlantic -ljtag_client -lscotch -L$(QUARTUS_ROOTDIR)/linux64 \
          -Wl,-rpath,$(QUARTUS_ROOTDIR)/linux64 -lmetis -lpthread -lboost_program_options -lboost_filesystem -lboost_system -fopenmp

visual-oil-water-bonds: DFLAGS=-DVISUALISE -DGALS -DIMPROVED_GALS -DBETTER_VERLET -DONE_BY_ONE -DBONDS -DSMALL_DT_EARLY
visual-oil-water-bonds: $(DPD_BIN) base-gals $(DPD_SRC)/OilWaterBonds.cpp oil-water-bonds

visual-sync-oil-water-bonds: DFLAGS=-DVISUALISE -DBETTER_VERLET -DONE_BY_ONE -DSMALL_DT_EARLY -DBONDS
visual-sync-oil-water-bonds: $(DPD_BIN) $(DPD_BIN)/code.v $(DPD_BIN)/data.v $(DPD_SRC)/OilWaterBonds.cpp oil-water-bonds

visual-sync-oil-water-bonds-dram: DFLAGS=-DVISUALISE -DBETTER_VERLET -DONE_BY_ONE -DSMALL_DT_EARLY -DBONDS -DDRAM
visual-sync-oil-water-bonds-dram: $(DPD_BIN) $(DPD_BIN)/code.v $(DPD_BIN)/data.v $(DPD_SRC)/OilWaterBonds.cpp oil-water-bonds

visual-serial-oil-water-bonds: DFLAGS=-DSERIAL -DBONDS -DVISUALISE -DBETTER_VERLET -DSMALL_DT_EARLY
visual-serial-oil-water-bonds: HOST_OBJS+=$(DPD_BIN)/serial.o $(DPD_BIN)/Vector3D.o $(DPD_BIN)/utils.o
visual-serial-oil-water-bonds: serial-objs $(DPD_BIN)/serial.o $(HOST_OBJS) oil-water-bonds

timed-oil-water-bonds: DFLAGS=-DTIMER -DGALS -DIMPROVED_GALS -DBETTER_VERLET -DONE_BY_ONE -DBONDS -DSMALL_DT_EARLY
timed-oil-water-bonds: $(DPD_BIN) base-gals $(DPD_SRC)/OilWaterBonds.cpp oil-water-bonds

# -------------- WATER ONLY SIMULATION ------------------------------------------------------
timed-water-only: DFLAGS=-DTIMER -DGALS -DIMPROVED_GALS -DBETTER_VERLET -DONE_BY_ONE
timed-water-only: $(DPD_BIN) base-gals water-only

visual-water-only: DFLAGS=-DVISUALISE -DGALS -DIMPROVED_GALS -DBETTER_VERLET -DONE_BY_ONE
visual-water-only: $(DPD_BIN) base-gals water-only

# ---------------------------- EXAMPLES --------------------------------
visual-bonds-only: DFLAGS=-DVISUALISE -DGALS -DIMPROVED_GALS -DBETTER_VERLET -DONE_BY_ONE -DBONDS
visual-bonds-only: $(DPD_BIN) base-gals bonds-only

visual-gals-restart: DFLAGS=-DVISUALISE -DGALS -DIMPROVED_GALS -DBETTER_VERLET -DONE_BY_ONE -DBONDS -DSMALL_DT_EARLY
visual-gals-restart: $(DPD_BIN) base-gals restart

# ---------------------------- x86 SERIAL SIMULATOR --------------------------------
serial-objs: $(DPD_INC)/Vector3D.hpp $(DPD_SRC)/Vector3D.cpp $(DPD_INC)/utils.hpp $(DPD_SRC)/utils.cpp
serial-objs:
	mkdir -p $(DPD_BIN)
	g++ -O2 -std=c++11 $(DFLAGS) $(EXTERNAL_FLAGS) -I $(INC) -I $(QUEUE_INC) -I $(HL) -I $(DPD_INC) -c -o $(DPD_BIN)/Vector3D.o $(DPD_SRC)/Vector3D.cpp
	g++ -O2 -std=c++11 $(DFLAGS) $(EXTERNAL_FLAGS) -I $(INC) -I $(QUEUE_INC) -I $(HL) -I $(DPD_INC) -c -o $(DPD_BIN)/utils.o $(DPD_SRC)/utils.cpp

timed-serial-oil-water: DFLAGS=-DSERIAL -DTIMER
timed-serial-oil-water: HOST_OBJS+=$(DPD_BIN)/serial.o $(DPD_BIN)/Vector3D.o $(DPD_BIN)/utils.o
timed-serial-oil-water: serial-objs $(DPD_BIN)/serial.o $(HOST_OBJS) oil-water

visual-serial-oil-water: DFLAGS=-DSERIAL -DVISUALISE
visual-serial-oil-water: HOST_OBJS+=$(DPD_BIN)/serial.o $(DPD_BIN)/Vector3D.o $(DPD_BIN)/utils.o
visual-serial-oil-water: serial-objs $(DPD_BIN)/serial.o $(HOST_OBJS) oil-water

test-serial: DFLAGS+=-DTESTING -DSERIAL
test-serial: HOST_OBJS+=$(DPD_BIN)/serial.o $(DPD_BIN)/Vector3D.o $(DPD_BIN)/utils.o
test-serial: serial-objs $(DPD_BIN)/serial.o $(HOST_OBJS)
	g++ -O2 -std=c++11 $(DFLAGS) $(EXTERNAL_FLAGS) -I $(INC) -I $(QUEUE_INC) -I $(HL) -I $(DPD_INC) -c -o $(DPD_BIN)/test.o $(DPD_SRC)/test.cpp
	g++ -O2 -std=c++11 -o $(DPD_BIN)/test $(HOST_OBJS) $(HL)/*.o $(DPD_BIN)/test.o \
	  -static-libgcc -static-libstdc++ \
          -ljtag_atlantic -ljtag_client -lscotch -L$(QUARTUS_ROOTDIR)/linux64 \
          -Wl,-rpath,$(QUARTUS_ROOTDIR)/linux64 -lmetis -lpthread -lboost_program_options -lboost_filesystem -lboost_system -fopenmp

test-serial-large: DFLAGS+=-DLARGE_TEST
test-serial-large: test-serial

.PHONY: clean
clean:
	rm -rf $(DPD_BIN) *.sock state.json
	rm -rf _frames
	rm -rf _meta.json
	rm -rf _state.json
	rm -rf node_modules
	rm -rf DPD_mapping*.json
