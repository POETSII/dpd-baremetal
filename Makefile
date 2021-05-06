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
DPD_HEADERS = $(DPD_INC)/DPDStructs.hpp $(DPD_INC)/dpd.hpp
DPD_OBJS = $(DPD_BIN)/Vector3D.o $(DPD_BIN)/utils.o
COMMON_OBJS = $(DPD_BIN)/HostMessenger.o $(DPD_BIN)/Cells.o $(DPD_BIN)/Volume.o \
 			  $(DPD_BIN)/SimulationVolume.o $(DPD_BIN)/Simulator.o
POLITE_OBJS = $(COMMON_OBJS) $(HL)/*.o $(DPD_BIN)/POLiteMessenger.o $(DPD_BIN)/POLiteCells.o $(DPD_BIN)/POLiteVolume.o $(DPD_BIN)/POLiteSimulator.o $(DPD_BIN)/ExternalClient.o $(DPD_BIN)/ExternalServer.o
SERIAL_OBJS = $(COMMON_OBJS) $(DPD_BIN)/SerialMessenger.o $(DPD_BIN)/SerialUtils.o $(DPD_BIN)/SerialCells.o $(DPD_BIN)/SerialVolume.o $(DPD_BIN)/SerialSimulator.o

# Script for connecting device as external
SOCAT_SCRIPT = ./socat/socat_script

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

# -------------- Common object files --------------------------
$(DPD_BIN)/ExternalClient.o: $(DPD_SRC)/ExternalClient.cpp $(DPD_INC)/ExternalClient.hpp
	mkdir -p $(DPD_BIN)
	g++ -O2 -std=c++11 $(DFLAGS) $(EXTERNAL_FLAGS) -I $(INC) -I $(QUEUE_INC) -I $(HL) -I $(DPD_INC) -c -o $(DPD_BIN)/ExternalClient.o $(DPD_SRC)/ExternalClient.cpp

$(DPD_BIN)/ExternalServer.o: $(DPD_SRC)/ExternalServer.cpp $(DPD_INC)/ExternalServer.hpp
	mkdir -p $(DPD_BIN)
	g++ -O2 -std=c++11 $(DFLAGS) $(EXTERNAL_FLAGS) -I $(INC) -I $(QUEUE_INC) -I $(HL) -I $(DPD_INC) -c -o $(DPD_BIN)/ExternalServer.o $(DPD_SRC)/ExternalServer.cpp

$(DPD_BIN)/HostMessenger.o: $(DPD_SRC)/HostMessenger.cpp $(DPD_INC)/HostMessenger.hpp
	mkdir -p $(DPD_BIN)
	g++ -O2 -std=c++11 $(DFLAGS) $(EXTERNAL_FLAGS) -I $(INC) -I $(QUEUE_INC) -I $(HL) -I $(DPD_INC) -c -o $(DPD_BIN)/HostMessenger.o $(DPD_SRC)/HostMessenger.cpp

$(DPD_BIN)/Cells.o: $(DPD_SRC)/Cells.cpp $(DPD_INC)/Cells.hpp
	mkdir -p $(DPD_BIN)
	g++ -O2 -std=c++11 $(DFLAGS) $(EXTERNAL_FLAGS) -I $(INC) -I $(QUEUE_INC) -I $(HL) -I $(DPD_INC) -c -o $(DPD_BIN)/Cells.o $(DPD_SRC)/Cells.cpp

$(DPD_BIN)/SimulationCells.o: $(DPD_SRC)/SimulationCells.cpp $(DPD_INC)/SimulationCells.hpp
	mkdir -p $(DPD_BIN)
	g++ -O2 -std=c++11 $(DFLAGS) $(EXTERNAL_FLAGS) -I $(INC) -I $(QUEUE_INC) -I $(HL) -I $(DPD_INC) -c -o $(DPD_BIN)/SimulationCells.o $(DPD_SRC)/SimulationCells.cpp

$(DPD_BIN)/Volume.o: $(DPD_SRC)/Volume.cpp $(DPD_INC)/Volume.hpp
	mkdir -p $(DPD_BIN)
	g++ -O2 -std=c++11 $(DFLAGS) $(EXTERNAL_FLAGS) -I $(INC) -I $(QUEUE_INC) -I $(HL) -I $(DPD_INC) -c -o $(DPD_BIN)/Volume.o $(DPD_SRC)/Volume.cpp

$(DPD_BIN)/SimulationVolume.o: $(DPD_SRC)/SimulationVolume.cpp $(DPD_INC)/SimulationVolume.hpp
	mkdir -p $(DPD_BIN)
	g++ -O2 -std=c++11 $(DFLAGS) $(EXTERNAL_FLAGS) -I $(INC) -I $(QUEUE_INC) -I $(HL) -I $(DPD_INC) -c -o $(DPD_BIN)/SimulationVolume.o $(DPD_SRC)/SimulationVolume.cpp

$(DPD_BIN)/Executor.o: $(DPD_SRC)/Executor.cpp $(DPD_INC)/Executor.hpp
	mkdir -p $(DPD_BIN)
	g++ -O2 -std=c++11 $(DFLAGS) $(EXTERNAL_FLAGS) -I $(INC) -I $(QUEUE_INC) -I $(HL) -I $(DPD_INC) -c -o $(DPD_BIN)/Executor.o $(DPD_SRC)/Executor.cpp

$(DPD_BIN)/Simulator.o: $(DPD_SRC)/Simulator.cpp $(DPD_INC)/Simulator.hpp
	mkdir -p $(DPD_BIN)
	g++ -O2 -std=c++11 $(DFLAGS) $(EXTERNAL_FLAGS) -I $(INC) -I $(QUEUE_INC) -I $(HL) -I $(DPD_INC) -c -o $(DPD_BIN)/Simulator.o $(DPD_SRC)/Simulator.cpp

# -------------- POLite Object files --------------------------
$(DPD_BIN)/POLiteMessenger.o: $(DPD_SRC)/POLiteMessenger.cpp $(DPD_INC)/POLiteMessenger.hpp
	mkdir -p $(DPD_BIN)
	g++ -O2 -std=c++11 $(DFLAGS) $(EXTERNAL_FLAGS) -I $(INC) -I $(QUEUE_INC) -I $(HL) -I $(DPD_INC) -c -o $(DPD_BIN)/POLiteMessenger.o $(DPD_SRC)/POLiteMessenger.cpp

$(DPD_BIN)/POLiteCells.o: $(DPD_SRC)/POLiteCells.cpp $(DPD_INC)/POLiteCells.hpp
	mkdir -p $(DPD_BIN)
	g++ -O2 -std=c++11 $(DFLAGS) $(EXTERNAL_FLAGS) -I $(INC) -I $(QUEUE_INC) -I $(HL) -I $(DPD_INC) -c -o $(DPD_BIN)/POLiteCells.o $(DPD_SRC)/POLiteCells.cpp

$(DPD_BIN)/POLiteVolume.o: $(DPD_SRC)/POLiteVolume.cpp $(DPD_INC)/POLiteVolume.hpp
	mkdir -p $(DPD_BIN)
	g++ -O2 -std=c++11 $(DFLAGS) $(EXTERNAL_FLAGS) -I $(INC) -I $(QUEUE_INC) -I $(HL) -I $(DPD_INC) -c -o $(DPD_BIN)/POLiteVolume.o $(DPD_SRC)/POLiteVolume.cpp

$(DPD_BIN)/POLiteSimulator.o: $(DPD_SRC)/POLiteSimulator.cpp $(DPD_INC)/POLiteSimulator.hpp
	mkdir -p $(DPD_BIN)
	g++ -O2 -std=c++11 $(DFLAGS) $(EXTERNAL_FLAGS) -I $(INC) -I $(QUEUE_INC) -I $(HL) -I $(DPD_INC) -c -o $(DPD_BIN)/POLiteSimulator.o $(DPD_SRC)/POLiteSimulator.cpp

# -------------- Tinsel Object files --------------------------
$(DPD_BIN)/Vector3D.o: $(DPD_SRC)/Vector3D.cpp $(DPD_INC)/Vector3D.hpp
	mkdir -p $(DPD_BIN)
	$(RV_CC) $(CFLAGS) -Wall -c -DTINSEL -I $(DPD_INC) $(LD_FLAGS) $< -o $@

$(DPD_BIN)/utils.o: $(DPD_SRC)/utils.cpp $(DPD_INC)/utils.hpp
	mkdir -p $(DPD_BIN)
	$(RV_CC) $(CFLAGS) -Wall -c -DTINSEL $(DFLAGS) $(EXTERNAL_FLAGS) -I $(DPD_INC) $(LD_FLAGS) $< -o $@

# ------------- Serial simulation object files ---------------------------
$(DPD_BIN)/SerialMessenger.o: $(DPD_SRC)/SerialMessenger.cpp $(DPD_INC)/SerialMessenger.hpp
	mkdir -p $(DPD_BIN)
	g++ -O2 -std=c++11 $(DFLAGS) $(EXTERNAL_FLAGS) -I $(INC) -I $(QUEUE_INC) -I $(DPD_INC) -c -o $(DPD_BIN)/SerialMessenger.o $(DPD_SRC)/SerialMessenger.cpp

$(DPD_BIN)/SerialCells.o: $(DPD_SRC)/SerialCells.cpp $(DPD_INC)/SerialCells.hpp
	mkdir -p $(DPD_BIN)
	g++ -O2 -std=c++11 $(DFLAGS) $(EXTERNAL_FLAGS) -I $(INC) -I $(QUEUE_INC) -I $(DPD_INC) -c -o $(DPD_BIN)/SerialCells.o $(DPD_SRC)/SerialCells.cpp

$(DPD_BIN)/SerialVolume.o: $(DPD_SRC)/SerialVolume.cpp $(DPD_INC)/SerialVolume.hpp
	mkdir -p $(DPD_BIN)
	g++ -O2 -std=c++11 $(DFLAGS) $(EXTERNAL_FLAGS) -I $(INC) -I $(QUEUE_INC) -I $(DPD_INC) -c -o $(DPD_BIN)/SerialVolume.o $(DPD_SRC)/SerialVolume.cpp

$(DPD_BIN)/SerialSimulator.o: $(DPD_SRC)/SerialSimulator.cpp $(DPD_INC)/SerialSimulator.hpp
	mkdir -p $(DPD_BIN)
	g++ -O2 -std=c++11 $(DFLAGS) $(EXTERNAL_FLAGS) -I $(INC) -I $(QUEUE_INC) -I $(DPD_INC) -c -o $(DPD_BIN)/SerialSimulator.o $(DPD_SRC)/SerialSimulator.cpp

$(DPD_BIN)/SerialUtils.o: $(DPD_SRC)/utils.cpp $(DPD_INC)/utils.hpp
	mkdir -p $(DPD_BIN)
	g++ -O2 -std=c++11 $(DFLAGS) $(EXTERNAL_FLAGS) -I $(INC) -I $(QUEUE_INC) -I $(DPD_INC) -c -o $(DPD_BIN)/SerialUtils.o $(DPD_SRC)/utils.cpp

# The external client
$(DPD_BIN)/dpd-bridge: $(DPD_SRC)/dpd-bridge.cpp $(POLITE_OBJS)
	g++ -O2 -std=c++17 -o $(DPD_BIN)/dpd-bridge -I $(INC) -I $(QUEUE_INC) -I $(HL) -I $(DPD_INC) $(POLITE_OBJS) $(DPD_SRC)/dpd-bridge.cpp \
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
$(DPD_BIN)/dpd.elf: DFLAGS+=
$(DPD_BIN)/dpd.elf: $(DPD_SRC)/sync.cpp $(DPD_INC)/sync.h $(DPD_BIN)/link.ld $(INC)/config.h $(INC)/tinsel.h $(DPD_BIN)/entry.o $(DPD_BIN) $(DPD_OBJS) $(DPD_HEADERS)
	$(RV_CC) $(CFLAGS) -Wall -c -DTINSEL $(DFLAGS) $(EXTERNAL_FLAGS) -I $(DPD_INC) -o $(DPD_BIN)/sync.o $<
	$(RV_LD) $(LDFLAGS) -T $(DPD_BIN)/link.ld -o $@ $(DPD_BIN)/entry.o $(DPD_BIN)/sync.o $(TINSEL_LIB_INC) $(DPD_OBJS)

base-sync: DFLAGS+=
base-sync: $(DPD_BIN) $(HL)/*.o $(DPD_BIN)/code.v $(DPD_BIN)/data.v

# ----------------- GALS elf ------------------------------
$(DPD_BIN)/galsCode.v: $(DPD_BIN)/gals.elf $(DPD_BIN)
	$(BIN)/checkelf.sh $(DPD_BIN)/gals.elf
	$(RV_OBJCOPY) -O verilog --only-section=.text $(DPD_BIN)/gals.elf $@

$(DPD_BIN)/galsData.v: $(DPD_BIN)/gals.elf $(DPD_BIN)
	$(RV_OBJCOPY) -O verilog --remove-section=.text \
                --set-section-flags .bss=alloc,load,contents $(DPD_BIN)/gals.elf $@

$(DPD_BIN)/gals.elf: $(DPD_SRC)/gals.cpp $(DPD_INC)/gals.h $(DPD_BIN)/link.ld $(INC)/config.h $(INC)/tinsel.h $(DPD_BIN)/entry.o $(DPD_BIN) $(DPD_OBJS) $(DPD_HEADERS)
	$(RV_CC) $(CFLAGS) -Wall -c -DTINSEL $(DFLAGS) $(EXTERNAL_FLAGS) -I $(DPD_INC) -o $(DPD_BIN)/gals.o $<
	$(RV_LD) $(LDFLAGS) -T $(DPD_BIN)/link.ld -o $@ $(DPD_BIN)/entry.o $(DPD_BIN)/gals.o $(TINSEL_LIB_INC) $(DPD_OBJS)

# Base GALS recipe which is used by all GALS recipes
# Improved gals and one by one make the best version of GALS
base-gals: DFLAGS+=-DGALS
base-gals: $(DPD_BIN) $(HL)/*.o $(DPD_BIN)/galsCode.v $(DPD_BIN)/galsData.v
	mv $(DPD_BIN)/galsCode.v $(DPD_BIN)/code.v
	mv $(DPD_BIN)/galsData.v $(DPD_BIN)/data.v
	mv $(DPD_BIN)/gals.elf $(DPD_BIN)/dpd.elf

improvedgals: DFLAGS+=-DIMPROVED_GALS
improvedgals: base-gals

# ---------------------------- x86 SERIAL SIMULATOR --------------------------------
serial: DFLAGS+=-DSERIAL
serial: $(SERIAL_OBJS)

# ---------------------------- Build the test file ---------------------------------

test: DFLAGS+=-DTESTING
test:
	g++ -O2 -std=c++11 $(DFLAGS) $(EXTERNAL_FLAGS) -I $(INC) -I $(QUEUE_INC) -I $(HL) -I $(DPD_INC) -c -o $(DPD_BIN)/test.o $(DPD_SRC)/test.cpp
	g++ -O2 -std=c++11 -o $(DPD_BIN)/test $(OBJS) $(DPD_BIN)/test.o \
	  -static-libgcc -static-libstdc++ \
      -Wl,-rpath, $(METIS) -lboost_program_options -lboost_filesystem -lboost_system -fopenmp

# ---------------------------- Cleaner --------------------------------
.PHONY: clean
clean:
	rm -rf $(DPD_BIN) *.sock state.json
	rm -rf _frames
	rm -rf _meta.json
	rm -rf _state.json
	rm -rf node_modules
	rm -rf DPD_mapping*.json

# ************** EXAMPLES **************

oilwater: DFLAGS+=
oilwater: $(DPD_EXAMPLES)/oilWater
	g++ -O2 -std=c++11 $(DFLAGS) $(EXTERNAL_FLAGS) \
	-I $(HL) -I $(DPD_INC) -c -o $(DPD_BIN)/oilWater.o $(DPD_EXAMPLES)/oilWater.cpp
	g++ -O2 -std=c++11 -o $(DPD_BIN)/run $(OBJS) $(DPD_BIN)/oilWater.o \
		-static-libgcc -static-libstdc++ 		-Wl,-rpath, $(METIS) -lpthread -lboost_program_options \
		-lboost_filesystem -lboost_system -fopenmp

vesicle: DFLAGS+=-DVESICLE_SELF_ASSEMBLY -DBONDS
vesicle: $(DPD_EXAMPLES)/VesicleSelfAssembly
	g++ -O2 -std=c++11 $(DFLAGS) $(EXTERNAL_FLAGS) \
	-I $(HL) -I $(DPD_INC) -c -o $(DPD_BIN)/VesicleSelfAssembly.o $(DPD_EXAMPLES)/VesicleSelfAssembly.cpp
	g++ -O2 -std=c++11 -o $(DPD_BIN)/run $(OBJS) $(DPD_BIN)/VesicleSelfAssembly.o \
		-static-libgcc -static-libstdc++ 		-Wl,-rpath, $(METIS) -lpthread -lboost_program_options \
		-lboost_filesystem -lboost_system -fopenmp

corners: DFLAGS+=
corners: $(DPD_EXAMPLES)/corner-tests
	g++ -O2 -std=c++11 $(DFLAGS) $(EXTERNAL_FLAGS) \
	-I $(HL) -I $(DPD_INC) -c -o $(DPD_BIN)/corner-tests.o $(DPD_EXAMPLES)/corner-tests.cpp
	g++ -O2 -std=c++11 -o $(DPD_BIN)/run $(OBJS) $(DPD_BIN)/corner-tests.o \
		-static-libgcc -static-libstdc++ 		-Wl,-rpath, $(METIS) -lpthread -lboost_program_options \
		-lboost_filesystem -lboost_system -fopenmp

gravity: DFLAGS+=-DGRAVITY
gravity: $(DPD_EXAMPLES)/BoxOilWaterGravity
	g++ -O2 -std=c++11 $(DFLAGS) $(EXTERNAL_FLAGS) \
	-I $(HL) -I $(DPD_INC) -c -o $(DPD_BIN)/BoxOilWaterGravity.o $(DPD_EXAMPLES)/BoxOilWaterGravity.cpp
	g++ -O2 -std=c++11 -o $(DPD_BIN)/run $(OBJS) $(DPD_BIN)/BoxOilWaterGravity.o \
		-static-libgcc -static-libstdc++ 		-Wl,-rpath, $(METIS) -lpthread -lboost_program_options \
		-lboost_filesystem -lboost_system -fopenmp

# ************** Simulator: sync**************

# **********Operation: visual**********

sync-visual: OBJS+=$(POLITE_OBJS)
sync-visual: METIS=-lmetis
sync-visual: DFLAGS+=-DVISUALISE
sync-visual: $(POLITE_OBJS) base-sync

# ******Example: oilwater******

sync-visual-oilwater: DFLAGS+=
sync-visual-oilwater: sync-visual oilwater

sync-visual-oilwater-fastest: DFLAGS+=-DBETTER_VERLET -DSMALL_DT_EARLY -DONE_BY_ONE -DREDUCED_LOCAL_CALCS 
sync-visual-oilwater-fastest: sync-visual-oilwater

sync-visual-oilwater-smallest: DFLAGS+=-DBETTER_VERLET -DSMALL_DT_EARLY -DSEND_TO_SELF -DSINGLE_FORCE_LOOP 
sync-visual-oilwater-smallest: sync-visual-oilwater


# ******Example: vesicle******

sync-visual-vesicle: DFLAGS+=-DVESICLE_SELF_ASSEMBLY -DBONDS
sync-visual-vesicle: sync-visual vesicle

sync-visual-vesicle-fastest: DFLAGS+=-DBETTER_VERLET -DSMALL_DT_EARLY -DONE_BY_ONE -DREDUCED_LOCAL_CALCS 
sync-visual-vesicle-fastest: sync-visual-vesicle

sync-visual-vesicle-smallest: DFLAGS+=-DBETTER_VERLET -DSMALL_DT_EARLY -DSEND_TO_SELF -DSINGLE_FORCE_LOOP 
sync-visual-vesicle-smallest: sync-visual-vesicle


# ******Example: corners******

sync-visual-corners: DFLAGS+=
sync-visual-corners: sync-visual corners

sync-visual-corners-fastest: DFLAGS+=-DBETTER_VERLET -DSMALL_DT_EARLY -DONE_BY_ONE -DREDUCED_LOCAL_CALCS 
sync-visual-corners-fastest: sync-visual-corners

sync-visual-corners-smallest: DFLAGS+=-DBETTER_VERLET -DSMALL_DT_EARLY -DSEND_TO_SELF -DSINGLE_FORCE_LOOP 
sync-visual-corners-smallest: sync-visual-corners


# ******Example: gravity******

sync-visual-gravity: DFLAGS+=-DGRAVITY
sync-visual-gravity: sync-visual gravity

sync-visual-gravity-fastest: DFLAGS+=-DBETTER_VERLET -DSMALL_DT_EARLY -DONE_BY_ONE -DREDUCED_LOCAL_CALCS 
sync-visual-gravity-fastest: sync-visual-gravity

sync-visual-gravity-smallest: DFLAGS+=-DBETTER_VERLET -DSMALL_DT_EARLY -DSEND_TO_SELF -DSINGLE_FORCE_LOOP 
sync-visual-gravity-smallest: sync-visual-gravity


# **********Operation: timed**********

sync-timed: OBJS+=$(POLITE_OBJS)
sync-timed: METIS=-lmetis
sync-timed: DFLAGS+=-DTIMER
sync-timed: $(POLITE_OBJS) base-sync

# ******Example: oilwater******

sync-timed-oilwater: DFLAGS+=
sync-timed-oilwater: sync-timed oilwater

sync-timed-oilwater-fastest: DFLAGS+=-DBETTER_VERLET -DSMALL_DT_EARLY -DONE_BY_ONE -DREDUCED_LOCAL_CALCS 
sync-timed-oilwater-fastest: sync-timed-oilwater

sync-timed-oilwater-smallest: DFLAGS+=-DBETTER_VERLET -DSMALL_DT_EARLY -DSEND_TO_SELF -DSINGLE_FORCE_LOOP 
sync-timed-oilwater-smallest: sync-timed-oilwater


# ******Example: vesicle******

sync-timed-vesicle: DFLAGS+=-DVESICLE_SELF_ASSEMBLY -DBONDS
sync-timed-vesicle: sync-timed vesicle

sync-timed-vesicle-fastest: DFLAGS+=-DBETTER_VERLET -DSMALL_DT_EARLY -DONE_BY_ONE -DREDUCED_LOCAL_CALCS 
sync-timed-vesicle-fastest: sync-timed-vesicle

sync-timed-vesicle-smallest: DFLAGS+=-DBETTER_VERLET -DSMALL_DT_EARLY -DSEND_TO_SELF -DSINGLE_FORCE_LOOP 
sync-timed-vesicle-smallest: sync-timed-vesicle


# ******Example: corners******

sync-timed-corners: DFLAGS+=
sync-timed-corners: sync-timed corners

sync-timed-corners-fastest: DFLAGS+=-DBETTER_VERLET -DSMALL_DT_EARLY -DONE_BY_ONE -DREDUCED_LOCAL_CALCS 
sync-timed-corners-fastest: sync-timed-corners

sync-timed-corners-smallest: DFLAGS+=-DBETTER_VERLET -DSMALL_DT_EARLY -DSEND_TO_SELF -DSINGLE_FORCE_LOOP 
sync-timed-corners-smallest: sync-timed-corners


# ******Example: gravity******

sync-timed-gravity: DFLAGS+=-DGRAVITY
sync-timed-gravity: sync-timed gravity

sync-timed-gravity-fastest: DFLAGS+=-DBETTER_VERLET -DSMALL_DT_EARLY -DONE_BY_ONE -DREDUCED_LOCAL_CALCS 
sync-timed-gravity-fastest: sync-timed-gravity

sync-timed-gravity-smallest: DFLAGS+=-DBETTER_VERLET -DSMALL_DT_EARLY -DSEND_TO_SELF -DSINGLE_FORCE_LOOP 
sync-timed-gravity-smallest: sync-timed-gravity


# **********Operation: stats**********

sync-stats: OBJS+=$(POLITE_OBJS)
sync-stats: METIS=-lmetis
sync-stats: DFLAGS+=-DSTATS
sync-stats: $(POLITE_OBJS) base-sync

# ******Example: oilwater******

sync-stats-oilwater: DFLAGS+=
sync-stats-oilwater: sync-stats oilwater

sync-stats-oilwater-fastest: DFLAGS+=-DBETTER_VERLET -DSMALL_DT_EARLY -DONE_BY_ONE -DREDUCED_LOCAL_CALCS 
sync-stats-oilwater-fastest: sync-stats-oilwater

sync-stats-oilwater-smallest: DFLAGS+=-DBETTER_VERLET -DSMALL_DT_EARLY -DSEND_TO_SELF -DSINGLE_FORCE_LOOP 
sync-stats-oilwater-smallest: sync-stats-oilwater


# ******Example: vesicle******

sync-stats-vesicle: DFLAGS+=-DVESICLE_SELF_ASSEMBLY -DBONDS
sync-stats-vesicle: sync-stats vesicle

sync-stats-vesicle-fastest: DFLAGS+=-DBETTER_VERLET -DSMALL_DT_EARLY -DONE_BY_ONE -DREDUCED_LOCAL_CALCS 
sync-stats-vesicle-fastest: sync-stats-vesicle

sync-stats-vesicle-smallest: DFLAGS+=-DBETTER_VERLET -DSMALL_DT_EARLY -DSEND_TO_SELF -DSINGLE_FORCE_LOOP 
sync-stats-vesicle-smallest: sync-stats-vesicle


# ******Example: corners******

sync-stats-corners: DFLAGS+=
sync-stats-corners: sync-stats corners

sync-stats-corners-fastest: DFLAGS+=-DBETTER_VERLET -DSMALL_DT_EARLY -DONE_BY_ONE -DREDUCED_LOCAL_CALCS 
sync-stats-corners-fastest: sync-stats-corners

sync-stats-corners-smallest: DFLAGS+=-DBETTER_VERLET -DSMALL_DT_EARLY -DSEND_TO_SELF -DSINGLE_FORCE_LOOP 
sync-stats-corners-smallest: sync-stats-corners


# ******Example: gravity******

sync-stats-gravity: DFLAGS+=-DGRAVITY
sync-stats-gravity: sync-stats gravity

sync-stats-gravity-fastest: DFLAGS+=-DBETTER_VERLET -DSMALL_DT_EARLY -DONE_BY_ONE -DREDUCED_LOCAL_CALCS 
sync-stats-gravity-fastest: sync-stats-gravity

sync-stats-gravity-smallest: DFLAGS+=-DBETTER_VERLET -DSMALL_DT_EARLY -DSEND_TO_SELF -DSINGLE_FORCE_LOOP 
sync-stats-gravity-smallest: sync-stats-gravity


# ************** Simulator: gals**************

# **********Operation: visual**********

gals-visual: OBJS+=$(POLITE_OBJS)
gals-visual: METIS=-lmetis
gals-visual: DFLAGS+=-DVISUALISE
gals-visual: $(POLITE_OBJS) base-gals

# ******Example: oilwater******

gals-visual-oilwater: DFLAGS+=
gals-visual-oilwater: gals-visual oilwater

gals-visual-oilwater-fastest: DFLAGS+=-DBETTER_VERLET -DSMALL_DT_EARLY -DONE_BY_ONE -DREDUCED_LOCAL_CALCS 
gals-visual-oilwater-fastest: gals-visual-oilwater

gals-visual-oilwater-smallest: DFLAGS+=-DBETTER_VERLET -DSMALL_DT_EARLY -DSEND_TO_SELF -DSINGLE_FORCE_LOOP 
gals-visual-oilwater-smallest: gals-visual-oilwater


# ******Example: vesicle******

gals-visual-vesicle: DFLAGS+=-DVESICLE_SELF_ASSEMBLY -DBONDS
gals-visual-vesicle: gals-visual vesicle

gals-visual-vesicle-fastest: DFLAGS+=-DBETTER_VERLET -DSMALL_DT_EARLY -DONE_BY_ONE -DREDUCED_LOCAL_CALCS 
gals-visual-vesicle-fastest: gals-visual-vesicle

gals-visual-vesicle-smallest: DFLAGS+=-DBETTER_VERLET -DSMALL_DT_EARLY -DSEND_TO_SELF -DSINGLE_FORCE_LOOP 
gals-visual-vesicle-smallest: gals-visual-vesicle


# ******Example: corners******

gals-visual-corners: DFLAGS+=
gals-visual-corners: gals-visual corners

gals-visual-corners-fastest: DFLAGS+=-DBETTER_VERLET -DSMALL_DT_EARLY -DONE_BY_ONE -DREDUCED_LOCAL_CALCS 
gals-visual-corners-fastest: gals-visual-corners

gals-visual-corners-smallest: DFLAGS+=-DBETTER_VERLET -DSMALL_DT_EARLY -DSEND_TO_SELF -DSINGLE_FORCE_LOOP 
gals-visual-corners-smallest: gals-visual-corners


# ******Example: gravity******

gals-visual-gravity: DFLAGS+=-DGRAVITY
gals-visual-gravity: gals-visual gravity

gals-visual-gravity-fastest: DFLAGS+=-DBETTER_VERLET -DSMALL_DT_EARLY -DONE_BY_ONE -DREDUCED_LOCAL_CALCS 
gals-visual-gravity-fastest: gals-visual-gravity

gals-visual-gravity-smallest: DFLAGS+=-DBETTER_VERLET -DSMALL_DT_EARLY -DSEND_TO_SELF -DSINGLE_FORCE_LOOP 
gals-visual-gravity-smallest: gals-visual-gravity


# **********Operation: timed**********

gals-timed: OBJS+=$(POLITE_OBJS)
gals-timed: METIS=-lmetis
gals-timed: DFLAGS+=-DTIMER
gals-timed: $(POLITE_OBJS) base-gals

# ******Example: oilwater******

gals-timed-oilwater: DFLAGS+=
gals-timed-oilwater: gals-timed oilwater

gals-timed-oilwater-fastest: DFLAGS+=-DBETTER_VERLET -DSMALL_DT_EARLY -DONE_BY_ONE -DREDUCED_LOCAL_CALCS 
gals-timed-oilwater-fastest: gals-timed-oilwater

gals-timed-oilwater-smallest: DFLAGS+=-DBETTER_VERLET -DSMALL_DT_EARLY -DSEND_TO_SELF -DSINGLE_FORCE_LOOP 
gals-timed-oilwater-smallest: gals-timed-oilwater


# ******Example: vesicle******

gals-timed-vesicle: DFLAGS+=-DVESICLE_SELF_ASSEMBLY -DBONDS
gals-timed-vesicle: gals-timed vesicle

gals-timed-vesicle-fastest: DFLAGS+=-DBETTER_VERLET -DSMALL_DT_EARLY -DONE_BY_ONE -DREDUCED_LOCAL_CALCS 
gals-timed-vesicle-fastest: gals-timed-vesicle

gals-timed-vesicle-smallest: DFLAGS+=-DBETTER_VERLET -DSMALL_DT_EARLY -DSEND_TO_SELF -DSINGLE_FORCE_LOOP 
gals-timed-vesicle-smallest: gals-timed-vesicle


# ******Example: corners******

gals-timed-corners: DFLAGS+=
gals-timed-corners: gals-timed corners

gals-timed-corners-fastest: DFLAGS+=-DBETTER_VERLET -DSMALL_DT_EARLY -DONE_BY_ONE -DREDUCED_LOCAL_CALCS 
gals-timed-corners-fastest: gals-timed-corners

gals-timed-corners-smallest: DFLAGS+=-DBETTER_VERLET -DSMALL_DT_EARLY -DSEND_TO_SELF -DSINGLE_FORCE_LOOP 
gals-timed-corners-smallest: gals-timed-corners


# ******Example: gravity******

gals-timed-gravity: DFLAGS+=-DGRAVITY
gals-timed-gravity: gals-timed gravity

gals-timed-gravity-fastest: DFLAGS+=-DBETTER_VERLET -DSMALL_DT_EARLY -DONE_BY_ONE -DREDUCED_LOCAL_CALCS 
gals-timed-gravity-fastest: gals-timed-gravity

gals-timed-gravity-smallest: DFLAGS+=-DBETTER_VERLET -DSMALL_DT_EARLY -DSEND_TO_SELF -DSINGLE_FORCE_LOOP 
gals-timed-gravity-smallest: gals-timed-gravity


# **********Operation: stats**********

gals-stats: OBJS+=$(POLITE_OBJS)
gals-stats: METIS=-lmetis
gals-stats: DFLAGS+=-DSTATS
gals-stats: $(POLITE_OBJS) base-gals

# ******Example: oilwater******

gals-stats-oilwater: DFLAGS+=
gals-stats-oilwater: gals-stats oilwater

gals-stats-oilwater-fastest: DFLAGS+=-DBETTER_VERLET -DSMALL_DT_EARLY -DONE_BY_ONE -DREDUCED_LOCAL_CALCS 
gals-stats-oilwater-fastest: gals-stats-oilwater

gals-stats-oilwater-smallest: DFLAGS+=-DBETTER_VERLET -DSMALL_DT_EARLY -DSEND_TO_SELF -DSINGLE_FORCE_LOOP 
gals-stats-oilwater-smallest: gals-stats-oilwater


# ******Example: vesicle******

gals-stats-vesicle: DFLAGS+=-DVESICLE_SELF_ASSEMBLY -DBONDS
gals-stats-vesicle: gals-stats vesicle

gals-stats-vesicle-fastest: DFLAGS+=-DBETTER_VERLET -DSMALL_DT_EARLY -DONE_BY_ONE -DREDUCED_LOCAL_CALCS 
gals-stats-vesicle-fastest: gals-stats-vesicle

gals-stats-vesicle-smallest: DFLAGS+=-DBETTER_VERLET -DSMALL_DT_EARLY -DSEND_TO_SELF -DSINGLE_FORCE_LOOP 
gals-stats-vesicle-smallest: gals-stats-vesicle


# ******Example: corners******

gals-stats-corners: DFLAGS+=
gals-stats-corners: gals-stats corners

gals-stats-corners-fastest: DFLAGS+=-DBETTER_VERLET -DSMALL_DT_EARLY -DONE_BY_ONE -DREDUCED_LOCAL_CALCS 
gals-stats-corners-fastest: gals-stats-corners

gals-stats-corners-smallest: DFLAGS+=-DBETTER_VERLET -DSMALL_DT_EARLY -DSEND_TO_SELF -DSINGLE_FORCE_LOOP 
gals-stats-corners-smallest: gals-stats-corners


# ******Example: gravity******

gals-stats-gravity: DFLAGS+=-DGRAVITY
gals-stats-gravity: gals-stats gravity

gals-stats-gravity-fastest: DFLAGS+=-DBETTER_VERLET -DSMALL_DT_EARLY -DONE_BY_ONE -DREDUCED_LOCAL_CALCS 
gals-stats-gravity-fastest: gals-stats-gravity

gals-stats-gravity-smallest: DFLAGS+=-DBETTER_VERLET -DSMALL_DT_EARLY -DSEND_TO_SELF -DSINGLE_FORCE_LOOP 
gals-stats-gravity-smallest: gals-stats-gravity


# ************** Simulator: improvedgals**************

# **********Operation: visual**********

improvedgals-visual: OBJS+=$(POLITE_OBJS)
improvedgals-visual: METIS=-lmetis
improvedgals-visual: DFLAGS+=-DVISUALISE
improvedgals-visual: $(POLITE_OBJS) base-gals

# ******Example: oilwater******

improvedgals-visual-oilwater: DFLAGS+=
improvedgals-visual-oilwater: improvedgals-visual oilwater

improvedgals-visual-oilwater-fastest: DFLAGS+=-DBETTER_VERLET -DSMALL_DT_EARLY -DONE_BY_ONE -DREDUCED_LOCAL_CALCS 
improvedgals-visual-oilwater-fastest: improvedgals-visual-oilwater

improvedgals-visual-oilwater-smallest: DFLAGS+=-DBETTER_VERLET -DSMALL_DT_EARLY -DSEND_TO_SELF -DSINGLE_FORCE_LOOP 
improvedgals-visual-oilwater-smallest: improvedgals-visual-oilwater


# ******Example: vesicle******

improvedgals-visual-vesicle: DFLAGS+=-DVESICLE_SELF_ASSEMBLY -DBONDS
improvedgals-visual-vesicle: improvedgals-visual vesicle

improvedgals-visual-vesicle-fastest: DFLAGS+=-DBETTER_VERLET -DSMALL_DT_EARLY -DONE_BY_ONE -DREDUCED_LOCAL_CALCS 
improvedgals-visual-vesicle-fastest: improvedgals-visual-vesicle

improvedgals-visual-vesicle-smallest: DFLAGS+=-DBETTER_VERLET -DSMALL_DT_EARLY -DSEND_TO_SELF -DSINGLE_FORCE_LOOP 
improvedgals-visual-vesicle-smallest: improvedgals-visual-vesicle


# ******Example: corners******

improvedgals-visual-corners: DFLAGS+=
improvedgals-visual-corners: improvedgals-visual corners

improvedgals-visual-corners-fastest: DFLAGS+=-DBETTER_VERLET -DSMALL_DT_EARLY -DONE_BY_ONE -DREDUCED_LOCAL_CALCS 
improvedgals-visual-corners-fastest: improvedgals-visual-corners

improvedgals-visual-corners-smallest: DFLAGS+=-DBETTER_VERLET -DSMALL_DT_EARLY -DSEND_TO_SELF -DSINGLE_FORCE_LOOP 
improvedgals-visual-corners-smallest: improvedgals-visual-corners


# ******Example: gravity******

improvedgals-visual-gravity: DFLAGS+=-DGRAVITY
improvedgals-visual-gravity: improvedgals-visual gravity

improvedgals-visual-gravity-fastest: DFLAGS+=-DBETTER_VERLET -DSMALL_DT_EARLY -DONE_BY_ONE -DREDUCED_LOCAL_CALCS 
improvedgals-visual-gravity-fastest: improvedgals-visual-gravity

improvedgals-visual-gravity-smallest: DFLAGS+=-DBETTER_VERLET -DSMALL_DT_EARLY -DSEND_TO_SELF -DSINGLE_FORCE_LOOP 
improvedgals-visual-gravity-smallest: improvedgals-visual-gravity


# **********Operation: timed**********

improvedgals-timed: OBJS+=$(POLITE_OBJS)
improvedgals-timed: METIS=-lmetis
improvedgals-timed: DFLAGS+=-DTIMER
improvedgals-timed: $(POLITE_OBJS) base-gals

# ******Example: oilwater******

improvedgals-timed-oilwater: DFLAGS+=
improvedgals-timed-oilwater: improvedgals-timed oilwater

improvedgals-timed-oilwater-fastest: DFLAGS+=-DBETTER_VERLET -DSMALL_DT_EARLY -DONE_BY_ONE -DREDUCED_LOCAL_CALCS 
improvedgals-timed-oilwater-fastest: improvedgals-timed-oilwater

improvedgals-timed-oilwater-smallest: DFLAGS+=-DBETTER_VERLET -DSMALL_DT_EARLY -DSEND_TO_SELF -DSINGLE_FORCE_LOOP 
improvedgals-timed-oilwater-smallest: improvedgals-timed-oilwater


# ******Example: vesicle******

improvedgals-timed-vesicle: DFLAGS+=-DVESICLE_SELF_ASSEMBLY -DBONDS
improvedgals-timed-vesicle: improvedgals-timed vesicle

improvedgals-timed-vesicle-fastest: DFLAGS+=-DBETTER_VERLET -DSMALL_DT_EARLY -DONE_BY_ONE -DREDUCED_LOCAL_CALCS 
improvedgals-timed-vesicle-fastest: improvedgals-timed-vesicle

improvedgals-timed-vesicle-smallest: DFLAGS+=-DBETTER_VERLET -DSMALL_DT_EARLY -DSEND_TO_SELF -DSINGLE_FORCE_LOOP 
improvedgals-timed-vesicle-smallest: improvedgals-timed-vesicle


# ******Example: corners******

improvedgals-timed-corners: DFLAGS+=
improvedgals-timed-corners: improvedgals-timed corners

improvedgals-timed-corners-fastest: DFLAGS+=-DBETTER_VERLET -DSMALL_DT_EARLY -DONE_BY_ONE -DREDUCED_LOCAL_CALCS 
improvedgals-timed-corners-fastest: improvedgals-timed-corners

improvedgals-timed-corners-smallest: DFLAGS+=-DBETTER_VERLET -DSMALL_DT_EARLY -DSEND_TO_SELF -DSINGLE_FORCE_LOOP 
improvedgals-timed-corners-smallest: improvedgals-timed-corners


# ******Example: gravity******

improvedgals-timed-gravity: DFLAGS+=-DGRAVITY
improvedgals-timed-gravity: improvedgals-timed gravity

improvedgals-timed-gravity-fastest: DFLAGS+=-DBETTER_VERLET -DSMALL_DT_EARLY -DONE_BY_ONE -DREDUCED_LOCAL_CALCS 
improvedgals-timed-gravity-fastest: improvedgals-timed-gravity

improvedgals-timed-gravity-smallest: DFLAGS+=-DBETTER_VERLET -DSMALL_DT_EARLY -DSEND_TO_SELF -DSINGLE_FORCE_LOOP 
improvedgals-timed-gravity-smallest: improvedgals-timed-gravity


# **********Operation: stats**********

improvedgals-stats: OBJS+=$(POLITE_OBJS)
improvedgals-stats: METIS=-lmetis
improvedgals-stats: DFLAGS+=-DSTATS
improvedgals-stats: $(POLITE_OBJS) base-gals

# ******Example: oilwater******

improvedgals-stats-oilwater: DFLAGS+=
improvedgals-stats-oilwater: improvedgals-stats oilwater

improvedgals-stats-oilwater-fastest: DFLAGS+=-DBETTER_VERLET -DSMALL_DT_EARLY -DONE_BY_ONE -DREDUCED_LOCAL_CALCS 
improvedgals-stats-oilwater-fastest: improvedgals-stats-oilwater

improvedgals-stats-oilwater-smallest: DFLAGS+=-DBETTER_VERLET -DSMALL_DT_EARLY -DSEND_TO_SELF -DSINGLE_FORCE_LOOP 
improvedgals-stats-oilwater-smallest: improvedgals-stats-oilwater


# ******Example: vesicle******

improvedgals-stats-vesicle: DFLAGS+=-DVESICLE_SELF_ASSEMBLY -DBONDS
improvedgals-stats-vesicle: improvedgals-stats vesicle

improvedgals-stats-vesicle-fastest: DFLAGS+=-DBETTER_VERLET -DSMALL_DT_EARLY -DONE_BY_ONE -DREDUCED_LOCAL_CALCS 
improvedgals-stats-vesicle-fastest: improvedgals-stats-vesicle

improvedgals-stats-vesicle-smallest: DFLAGS+=-DBETTER_VERLET -DSMALL_DT_EARLY -DSEND_TO_SELF -DSINGLE_FORCE_LOOP 
improvedgals-stats-vesicle-smallest: improvedgals-stats-vesicle


# ******Example: corners******

improvedgals-stats-corners: DFLAGS+=
improvedgals-stats-corners: improvedgals-stats corners

improvedgals-stats-corners-fastest: DFLAGS+=-DBETTER_VERLET -DSMALL_DT_EARLY -DONE_BY_ONE -DREDUCED_LOCAL_CALCS 
improvedgals-stats-corners-fastest: improvedgals-stats-corners

improvedgals-stats-corners-smallest: DFLAGS+=-DBETTER_VERLET -DSMALL_DT_EARLY -DSEND_TO_SELF -DSINGLE_FORCE_LOOP 
improvedgals-stats-corners-smallest: improvedgals-stats-corners


# ******Example: gravity******

improvedgals-stats-gravity: DFLAGS+=-DGRAVITY
improvedgals-stats-gravity: improvedgals-stats gravity

improvedgals-stats-gravity-fastest: DFLAGS+=-DBETTER_VERLET -DSMALL_DT_EARLY -DONE_BY_ONE -DREDUCED_LOCAL_CALCS 
improvedgals-stats-gravity-fastest: improvedgals-stats-gravity

improvedgals-stats-gravity-smallest: DFLAGS+=-DBETTER_VERLET -DSMALL_DT_EARLY -DSEND_TO_SELF -DSINGLE_FORCE_LOOP 
improvedgals-stats-gravity-smallest: improvedgals-stats-gravity


# ************** Simulator: serial**************

# **********Operation: visual**********

serial-visual: OBJS+=$(SERIAL_OBJS)
serial-visual: METIS=
serial-visual: DFLAGS+=-DVISUALISE
serial-visual: $(SERIAL_OBJS) serial

# ******Example: oilwater******

serial-visual-oilwater: DFLAGS+=
serial-visual-oilwater: serial-visual oilwater

serial-visual-oilwater-fastest: DFLAGS+=-DBETTER_VERLET -DSMALL_DT_EARLY -DONE_BY_ONE -DREDUCED_LOCAL_CALCS 
serial-visual-oilwater-fastest: serial-visual-oilwater

serial-visual-oilwater-smallest: DFLAGS+=-DBETTER_VERLET -DSMALL_DT_EARLY -DSEND_TO_SELF -DSINGLE_FORCE_LOOP 
serial-visual-oilwater-smallest: serial-visual-oilwater


# ******Example: vesicle******

serial-visual-vesicle: DFLAGS+=-DVESICLE_SELF_ASSEMBLY -DBONDS
serial-visual-vesicle: serial-visual vesicle

serial-visual-vesicle-fastest: DFLAGS+=-DBETTER_VERLET -DSMALL_DT_EARLY -DONE_BY_ONE -DREDUCED_LOCAL_CALCS 
serial-visual-vesicle-fastest: serial-visual-vesicle

serial-visual-vesicle-smallest: DFLAGS+=-DBETTER_VERLET -DSMALL_DT_EARLY -DSEND_TO_SELF -DSINGLE_FORCE_LOOP 
serial-visual-vesicle-smallest: serial-visual-vesicle


# ******Example: corners******

serial-visual-corners: DFLAGS+=
serial-visual-corners: serial-visual corners

serial-visual-corners-fastest: DFLAGS+=-DBETTER_VERLET -DSMALL_DT_EARLY -DONE_BY_ONE -DREDUCED_LOCAL_CALCS 
serial-visual-corners-fastest: serial-visual-corners

serial-visual-corners-smallest: DFLAGS+=-DBETTER_VERLET -DSMALL_DT_EARLY -DSEND_TO_SELF -DSINGLE_FORCE_LOOP 
serial-visual-corners-smallest: serial-visual-corners


# ******Example: gravity******

serial-visual-gravity: DFLAGS+=-DGRAVITY
serial-visual-gravity: serial-visual gravity

serial-visual-gravity-fastest: DFLAGS+=-DBETTER_VERLET -DSMALL_DT_EARLY -DONE_BY_ONE -DREDUCED_LOCAL_CALCS 
serial-visual-gravity-fastest: serial-visual-gravity

serial-visual-gravity-smallest: DFLAGS+=-DBETTER_VERLET -DSMALL_DT_EARLY -DSEND_TO_SELF -DSINGLE_FORCE_LOOP 
serial-visual-gravity-smallest: serial-visual-gravity


# **********Operation: timed**********

serial-timed: OBJS+=$(SERIAL_OBJS)
serial-timed: METIS=
serial-timed: DFLAGS+=-DTIMER
serial-timed: $(SERIAL_OBJS) serial

# ******Example: oilwater******

serial-timed-oilwater: DFLAGS+=
serial-timed-oilwater: serial-timed oilwater

serial-timed-oilwater-fastest: DFLAGS+=-DBETTER_VERLET -DSMALL_DT_EARLY -DONE_BY_ONE -DREDUCED_LOCAL_CALCS 
serial-timed-oilwater-fastest: serial-timed-oilwater

serial-timed-oilwater-smallest: DFLAGS+=-DBETTER_VERLET -DSMALL_DT_EARLY -DSEND_TO_SELF -DSINGLE_FORCE_LOOP 
serial-timed-oilwater-smallest: serial-timed-oilwater


# ******Example: vesicle******

serial-timed-vesicle: DFLAGS+=-DVESICLE_SELF_ASSEMBLY -DBONDS
serial-timed-vesicle: serial-timed vesicle

serial-timed-vesicle-fastest: DFLAGS+=-DBETTER_VERLET -DSMALL_DT_EARLY -DONE_BY_ONE -DREDUCED_LOCAL_CALCS 
serial-timed-vesicle-fastest: serial-timed-vesicle

serial-timed-vesicle-smallest: DFLAGS+=-DBETTER_VERLET -DSMALL_DT_EARLY -DSEND_TO_SELF -DSINGLE_FORCE_LOOP 
serial-timed-vesicle-smallest: serial-timed-vesicle


# ******Example: corners******

serial-timed-corners: DFLAGS+=
serial-timed-corners: serial-timed corners

serial-timed-corners-fastest: DFLAGS+=-DBETTER_VERLET -DSMALL_DT_EARLY -DONE_BY_ONE -DREDUCED_LOCAL_CALCS 
serial-timed-corners-fastest: serial-timed-corners

serial-timed-corners-smallest: DFLAGS+=-DBETTER_VERLET -DSMALL_DT_EARLY -DSEND_TO_SELF -DSINGLE_FORCE_LOOP 
serial-timed-corners-smallest: serial-timed-corners


# ******Example: gravity******

serial-timed-gravity: DFLAGS+=-DGRAVITY
serial-timed-gravity: serial-timed gravity

serial-timed-gravity-fastest: DFLAGS+=-DBETTER_VERLET -DSMALL_DT_EARLY -DONE_BY_ONE -DREDUCED_LOCAL_CALCS 
serial-timed-gravity-fastest: serial-timed-gravity

serial-timed-gravity-smallest: DFLAGS+=-DBETTER_VERLET -DSMALL_DT_EARLY -DSEND_TO_SELF -DSINGLE_FORCE_LOOP 
serial-timed-gravity-smallest: serial-timed-gravity

#****************** REGRESSION TESTING ******************
# ************** TEST Simulator: sync**************

test-sync: OBJS=$(POLITE_OBJS)
test-sync: METIS=-lmetis
test-sync: DFLAGS+=-DTESTING 
test-sync: $(POLITE_OBJS) base-sync test

test-sync-betterverlet: DFLAGS+=-DBETTER_VERLET
test-sync-betterverlet: test-sync

test-sync-dtchange: DFLAGS+=-DSMALL_DT_EARLY
test-sync-dtchange: test-sync

test-sync-doublesqrt: DFLAGS+=-DDOUBLE_SQRT
test-sync-doublesqrt: test-sync

test-sync-msgmgmt: DFLAGS+=-DMESSAGE_MANAGEMENT
test-sync-msgmgmt: test-sync

test-sync-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-sync-reducedlocalcalcs: test-sync

test-sync-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-singleforceloop: test-sync

test-sync-singlereducedforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP -DREDUCED_LOCAL_CALCS
test-sync-singlereducedforceloop: test-sync

# **TEST Local calculation method: onebyone**
test-sync-onebyone: DFLAGS+=-DONE_BY_ONE
test-sync-onebyone: test-sync

test-sync-onebyone-betterverlet: DFLAGS+=-DBETTER_VERLET
test-sync-onebyone-betterverlet: test-sync-onebyone

test-sync-onebyone-dtchange: DFLAGS+=-DSMALL_DT_EARLY
test-sync-onebyone-dtchange: test-sync-onebyone

test-sync-onebyone-doublesqrt: DFLAGS+=-DDOUBLE_SQRT
test-sync-onebyone-doublesqrt: test-sync-onebyone

test-sync-onebyone-msgmgmt: DFLAGS+=-DMESSAGE_MANAGEMENT
test-sync-onebyone-msgmgmt: test-sync-onebyone

test-sync-onebyone-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-sync-onebyone-reducedlocalcalcs: test-sync-onebyone

test-sync-onebyone-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-onebyone-singleforceloop: test-sync-onebyone

test-sync-onebyone-singlereducedforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP -DREDUCED_LOCAL_CALCS
test-sync-onebyone-singlereducedforceloop: test-sync-onebyone

# **TEST Local calculation method: sendtoself**
test-sync-sendtoself: DFLAGS+=-DSEND_TO_SELF
test-sync-sendtoself: test-sync

test-sync-sendtoself-betterverlet: DFLAGS+=-DBETTER_VERLET
test-sync-sendtoself-betterverlet: test-sync-sendtoself

test-sync-sendtoself-dtchange: DFLAGS+=-DSMALL_DT_EARLY
test-sync-sendtoself-dtchange: test-sync-sendtoself

test-sync-sendtoself-doublesqrt: DFLAGS+=-DDOUBLE_SQRT
test-sync-sendtoself-doublesqrt: test-sync-sendtoself

test-sync-sendtoself-msgmgmt: DFLAGS+=-DMESSAGE_MANAGEMENT
test-sync-sendtoself-msgmgmt: test-sync-sendtoself

test-sync-sendtoself-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-sendtoself-singleforceloop: test-sync-sendtoself

test-sync-sendtoself-singlereducedforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP -DREDUCED_LOCAL_CALCS
test-sync-sendtoself-singlereducedforceloop: test-sync-sendtoself

test-sync-large: DFLAGS+=-DLARGE_TEST
test-sync-large: test-sync

test-sync-large-betterverlet: DFLAGS+=-DBETTER_VERLET
test-sync-large-betterverlet: test-sync-large

test-sync-large-dtchange: DFLAGS+=-DSMALL_DT_EARLY
test-sync-large-dtchange: test-sync-large

test-sync-large-doublesqrt: DFLAGS+=-DDOUBLE_SQRT
test-sync-large-doublesqrt: test-sync-large

test-sync-large-msgmgmt: DFLAGS+=-DMESSAGE_MANAGEMENT
test-sync-large-msgmgmt: test-sync-large

test-sync-large-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-sync-large-reducedlocalcalcs: test-sync-large

test-sync-large-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-large-singleforceloop: test-sync-large

test-sync-large-singlereducedforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP -DREDUCED_LOCAL_CALCS
test-sync-large-singlereducedforceloop: test-sync-large

# **TEST Local calculation method: onebyone**
test-sync-large-onebyone: DFLAGS+=-DONE_BY_ONE
test-sync-large-onebyone: test-sync-large

test-sync-large-onebyone-betterverlet: DFLAGS+=-DBETTER_VERLET
test-sync-large-onebyone-betterverlet: test-sync-large-onebyone

test-sync-large-onebyone-dtchange: DFLAGS+=-DSMALL_DT_EARLY
test-sync-large-onebyone-dtchange: test-sync-large-onebyone

test-sync-large-onebyone-doublesqrt: DFLAGS+=-DDOUBLE_SQRT
test-sync-large-onebyone-doublesqrt: test-sync-large-onebyone

test-sync-large-onebyone-msgmgmt: DFLAGS+=-DMESSAGE_MANAGEMENT
test-sync-large-onebyone-msgmgmt: test-sync-large-onebyone

test-sync-large-onebyone-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-sync-large-onebyone-reducedlocalcalcs: test-sync-large-onebyone

test-sync-large-onebyone-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-large-onebyone-singleforceloop: test-sync-large-onebyone

test-sync-large-onebyone-singlereducedforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP -DREDUCED_LOCAL_CALCS
test-sync-large-onebyone-singlereducedforceloop: test-sync-large-onebyone

# **TEST Local calculation method: sendtoself**
test-sync-large-sendtoself: DFLAGS+=-DSEND_TO_SELF
test-sync-large-sendtoself: test-sync-large

test-sync-large-sendtoself-betterverlet: DFLAGS+=-DBETTER_VERLET
test-sync-large-sendtoself-betterverlet: test-sync-large-sendtoself

test-sync-large-sendtoself-dtchange: DFLAGS+=-DSMALL_DT_EARLY
test-sync-large-sendtoself-dtchange: test-sync-large-sendtoself

test-sync-large-sendtoself-doublesqrt: DFLAGS+=-DDOUBLE_SQRT
test-sync-large-sendtoself-doublesqrt: test-sync-large-sendtoself

test-sync-large-sendtoself-msgmgmt: DFLAGS+=-DMESSAGE_MANAGEMENT
test-sync-large-sendtoself-msgmgmt: test-sync-large-sendtoself

test-sync-large-sendtoself-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-large-sendtoself-singleforceloop: test-sync-large-sendtoself

test-sync-large-sendtoself-singlereducedforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP -DREDUCED_LOCAL_CALCS
test-sync-large-sendtoself-singlereducedforceloop: test-sync-large-sendtoself

test-sync-bonds: DFLAGS+=-DBONDS -DVESICLE_SELF_ASSEMBLY
test-sync-bonds: test-sync

test-sync-bonds-betterverlet: DFLAGS+=-DBETTER_VERLET
test-sync-bonds-betterverlet: test-sync-bonds

test-sync-bonds-dtchange: DFLAGS+=-DSMALL_DT_EARLY
test-sync-bonds-dtchange: test-sync-bonds

test-sync-bonds-doublesqrt: DFLAGS+=-DDOUBLE_SQRT
test-sync-bonds-doublesqrt: test-sync-bonds

test-sync-bonds-msgmgmt: DFLAGS+=-DMESSAGE_MANAGEMENT
test-sync-bonds-msgmgmt: test-sync-bonds

test-sync-bonds-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-sync-bonds-reducedlocalcalcs: test-sync-bonds

test-sync-bonds-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-bonds-singleforceloop: test-sync-bonds

test-sync-bonds-singlereducedforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP -DREDUCED_LOCAL_CALCS
test-sync-bonds-singlereducedforceloop: test-sync-bonds

# **TEST Local calculation method: onebyone**
test-sync-bonds-onebyone: DFLAGS+=-DONE_BY_ONE
test-sync-bonds-onebyone: test-sync-bonds

test-sync-bonds-onebyone-betterverlet: DFLAGS+=-DBETTER_VERLET
test-sync-bonds-onebyone-betterverlet: test-sync-bonds-onebyone

test-sync-bonds-onebyone-dtchange: DFLAGS+=-DSMALL_DT_EARLY
test-sync-bonds-onebyone-dtchange: test-sync-bonds-onebyone

test-sync-bonds-onebyone-doublesqrt: DFLAGS+=-DDOUBLE_SQRT
test-sync-bonds-onebyone-doublesqrt: test-sync-bonds-onebyone

test-sync-bonds-onebyone-msgmgmt: DFLAGS+=-DMESSAGE_MANAGEMENT
test-sync-bonds-onebyone-msgmgmt: test-sync-bonds-onebyone

test-sync-bonds-onebyone-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-sync-bonds-onebyone-reducedlocalcalcs: test-sync-bonds-onebyone

test-sync-bonds-onebyone-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-bonds-onebyone-singleforceloop: test-sync-bonds-onebyone

test-sync-bonds-onebyone-singlereducedforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP -DREDUCED_LOCAL_CALCS
test-sync-bonds-onebyone-singlereducedforceloop: test-sync-bonds-onebyone

# **TEST Local calculation method: sendtoself**
test-sync-bonds-sendtoself: DFLAGS+=-DSEND_TO_SELF
test-sync-bonds-sendtoself: test-sync-bonds

test-sync-bonds-sendtoself-betterverlet: DFLAGS+=-DBETTER_VERLET
test-sync-bonds-sendtoself-betterverlet: test-sync-bonds-sendtoself

test-sync-bonds-sendtoself-dtchange: DFLAGS+=-DSMALL_DT_EARLY
test-sync-bonds-sendtoself-dtchange: test-sync-bonds-sendtoself

test-sync-bonds-sendtoself-doublesqrt: DFLAGS+=-DDOUBLE_SQRT
test-sync-bonds-sendtoself-doublesqrt: test-sync-bonds-sendtoself

test-sync-bonds-sendtoself-msgmgmt: DFLAGS+=-DMESSAGE_MANAGEMENT
test-sync-bonds-sendtoself-msgmgmt: test-sync-bonds-sendtoself

test-sync-bonds-sendtoself-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-bonds-sendtoself-singleforceloop: test-sync-bonds-sendtoself

test-sync-bonds-sendtoself-singlereducedforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP -DREDUCED_LOCAL_CALCS
test-sync-bonds-sendtoself-singlereducedforceloop: test-sync-bonds-sendtoself


# ************** TEST Simulator: gals**************

test-gals: OBJS=$(POLITE_OBJS)
test-gals: METIS=-lmetis
test-gals: DFLAGS+=-DTESTING -DGALS
test-gals: $(POLITE_OBJS) base-gals test

test-gals-betterverlet: DFLAGS+=-DBETTER_VERLET
test-gals-betterverlet: test-gals

test-gals-dtchange: DFLAGS+=-DSMALL_DT_EARLY
test-gals-dtchange: test-gals

test-gals-doublesqrt: DFLAGS+=-DDOUBLE_SQRT
test-gals-doublesqrt: test-gals

test-gals-msgmgmt: DFLAGS+=-DMESSAGE_MANAGEMENT
test-gals-msgmgmt: test-gals

test-gals-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-gals-reducedlocalcalcs: test-gals

test-gals-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-singleforceloop: test-gals

test-gals-singlereducedforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP -DREDUCED_LOCAL_CALCS
test-gals-singlereducedforceloop: test-gals

# **TEST Local calculation method: onebyone**
test-gals-onebyone: DFLAGS+=-DONE_BY_ONE
test-gals-onebyone: test-gals

test-gals-onebyone-betterverlet: DFLAGS+=-DBETTER_VERLET
test-gals-onebyone-betterverlet: test-gals-onebyone

test-gals-onebyone-dtchange: DFLAGS+=-DSMALL_DT_EARLY
test-gals-onebyone-dtchange: test-gals-onebyone

test-gals-onebyone-doublesqrt: DFLAGS+=-DDOUBLE_SQRT
test-gals-onebyone-doublesqrt: test-gals-onebyone

test-gals-onebyone-msgmgmt: DFLAGS+=-DMESSAGE_MANAGEMENT
test-gals-onebyone-msgmgmt: test-gals-onebyone

test-gals-onebyone-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-gals-onebyone-reducedlocalcalcs: test-gals-onebyone

test-gals-onebyone-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-onebyone-singleforceloop: test-gals-onebyone

test-gals-onebyone-singlereducedforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP -DREDUCED_LOCAL_CALCS
test-gals-onebyone-singlereducedforceloop: test-gals-onebyone

test-gals-large: DFLAGS+=-DLARGE_TEST
test-gals-large: test-gals

test-gals-large-betterverlet: DFLAGS+=-DBETTER_VERLET
test-gals-large-betterverlet: test-gals-large

test-gals-large-dtchange: DFLAGS+=-DSMALL_DT_EARLY
test-gals-large-dtchange: test-gals-large

test-gals-large-doublesqrt: DFLAGS+=-DDOUBLE_SQRT
test-gals-large-doublesqrt: test-gals-large

test-gals-large-msgmgmt: DFLAGS+=-DMESSAGE_MANAGEMENT
test-gals-large-msgmgmt: test-gals-large

test-gals-large-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-gals-large-reducedlocalcalcs: test-gals-large

test-gals-large-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-large-singleforceloop: test-gals-large

test-gals-large-singlereducedforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP -DREDUCED_LOCAL_CALCS
test-gals-large-singlereducedforceloop: test-gals-large

# **TEST Local calculation method: onebyone**
test-gals-large-onebyone: DFLAGS+=-DONE_BY_ONE
test-gals-large-onebyone: test-gals-large

test-gals-large-onebyone-betterverlet: DFLAGS+=-DBETTER_VERLET
test-gals-large-onebyone-betterverlet: test-gals-large-onebyone

test-gals-large-onebyone-dtchange: DFLAGS+=-DSMALL_DT_EARLY
test-gals-large-onebyone-dtchange: test-gals-large-onebyone

test-gals-large-onebyone-doublesqrt: DFLAGS+=-DDOUBLE_SQRT
test-gals-large-onebyone-doublesqrt: test-gals-large-onebyone

test-gals-large-onebyone-msgmgmt: DFLAGS+=-DMESSAGE_MANAGEMENT
test-gals-large-onebyone-msgmgmt: test-gals-large-onebyone

test-gals-large-onebyone-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-gals-large-onebyone-reducedlocalcalcs: test-gals-large-onebyone

test-gals-large-onebyone-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-large-onebyone-singleforceloop: test-gals-large-onebyone

test-gals-large-onebyone-singlereducedforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP -DREDUCED_LOCAL_CALCS
test-gals-large-onebyone-singlereducedforceloop: test-gals-large-onebyone

test-gals-bonds: DFLAGS+=-DBONDS -DVESICLE_SELF_ASSEMBLY
test-gals-bonds: test-gals

test-gals-bonds-betterverlet: DFLAGS+=-DBETTER_VERLET
test-gals-bonds-betterverlet: test-gals-bonds

test-gals-bonds-dtchange: DFLAGS+=-DSMALL_DT_EARLY
test-gals-bonds-dtchange: test-gals-bonds

test-gals-bonds-doublesqrt: DFLAGS+=-DDOUBLE_SQRT
test-gals-bonds-doublesqrt: test-gals-bonds

test-gals-bonds-msgmgmt: DFLAGS+=-DMESSAGE_MANAGEMENT
test-gals-bonds-msgmgmt: test-gals-bonds

test-gals-bonds-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-gals-bonds-reducedlocalcalcs: test-gals-bonds

test-gals-bonds-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-bonds-singleforceloop: test-gals-bonds

test-gals-bonds-singlereducedforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP -DREDUCED_LOCAL_CALCS
test-gals-bonds-singlereducedforceloop: test-gals-bonds

# **TEST Local calculation method: onebyone**
test-gals-bonds-onebyone: DFLAGS+=-DONE_BY_ONE
test-gals-bonds-onebyone: test-gals-bonds

test-gals-bonds-onebyone-betterverlet: DFLAGS+=-DBETTER_VERLET
test-gals-bonds-onebyone-betterverlet: test-gals-bonds-onebyone

test-gals-bonds-onebyone-dtchange: DFLAGS+=-DSMALL_DT_EARLY
test-gals-bonds-onebyone-dtchange: test-gals-bonds-onebyone

test-gals-bonds-onebyone-doublesqrt: DFLAGS+=-DDOUBLE_SQRT
test-gals-bonds-onebyone-doublesqrt: test-gals-bonds-onebyone

test-gals-bonds-onebyone-msgmgmt: DFLAGS+=-DMESSAGE_MANAGEMENT
test-gals-bonds-onebyone-msgmgmt: test-gals-bonds-onebyone

test-gals-bonds-onebyone-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-gals-bonds-onebyone-reducedlocalcalcs: test-gals-bonds-onebyone

test-gals-bonds-onebyone-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-bonds-onebyone-singleforceloop: test-gals-bonds-onebyone

test-gals-bonds-onebyone-singlereducedforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP -DREDUCED_LOCAL_CALCS
test-gals-bonds-onebyone-singlereducedforceloop: test-gals-bonds-onebyone


# ************** TEST Simulator: improvedgals**************

test-improvedgals: OBJS=$(POLITE_OBJS)
test-improvedgals: METIS=-lmetis
test-improvedgals: DFLAGS+=-DTESTING -DGALS -DIMPROVED_GALS
test-improvedgals: $(POLITE_OBJS) base-gals test

test-improvedgals-betterverlet: DFLAGS+=-DBETTER_VERLET
test-improvedgals-betterverlet: test-improvedgals

test-improvedgals-dtchange: DFLAGS+=-DSMALL_DT_EARLY
test-improvedgals-dtchange: test-improvedgals

test-improvedgals-doublesqrt: DFLAGS+=-DDOUBLE_SQRT
test-improvedgals-doublesqrt: test-improvedgals

test-improvedgals-msgmgmt: DFLAGS+=-DMESSAGE_MANAGEMENT
test-improvedgals-msgmgmt: test-improvedgals

test-improvedgals-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-improvedgals-reducedlocalcalcs: test-improvedgals

test-improvedgals-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-singleforceloop: test-improvedgals

test-improvedgals-singlereducedforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP -DREDUCED_LOCAL_CALCS
test-improvedgals-singlereducedforceloop: test-improvedgals

# **TEST Local calculation method: onebyone**
test-improvedgals-onebyone: DFLAGS+=-DONE_BY_ONE
test-improvedgals-onebyone: test-improvedgals

test-improvedgals-onebyone-betterverlet: DFLAGS+=-DBETTER_VERLET
test-improvedgals-onebyone-betterverlet: test-improvedgals-onebyone

test-improvedgals-onebyone-dtchange: DFLAGS+=-DSMALL_DT_EARLY
test-improvedgals-onebyone-dtchange: test-improvedgals-onebyone

test-improvedgals-onebyone-doublesqrt: DFLAGS+=-DDOUBLE_SQRT
test-improvedgals-onebyone-doublesqrt: test-improvedgals-onebyone

test-improvedgals-onebyone-msgmgmt: DFLAGS+=-DMESSAGE_MANAGEMENT
test-improvedgals-onebyone-msgmgmt: test-improvedgals-onebyone

test-improvedgals-onebyone-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-improvedgals-onebyone-reducedlocalcalcs: test-improvedgals-onebyone

test-improvedgals-onebyone-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-onebyone-singleforceloop: test-improvedgals-onebyone

test-improvedgals-onebyone-singlereducedforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP -DREDUCED_LOCAL_CALCS
test-improvedgals-onebyone-singlereducedforceloop: test-improvedgals-onebyone

test-improvedgals-large: DFLAGS+=-DLARGE_TEST
test-improvedgals-large: test-improvedgals

test-improvedgals-large-betterverlet: DFLAGS+=-DBETTER_VERLET
test-improvedgals-large-betterverlet: test-improvedgals-large

test-improvedgals-large-dtchange: DFLAGS+=-DSMALL_DT_EARLY
test-improvedgals-large-dtchange: test-improvedgals-large

test-improvedgals-large-doublesqrt: DFLAGS+=-DDOUBLE_SQRT
test-improvedgals-large-doublesqrt: test-improvedgals-large

test-improvedgals-large-msgmgmt: DFLAGS+=-DMESSAGE_MANAGEMENT
test-improvedgals-large-msgmgmt: test-improvedgals-large

test-improvedgals-large-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-improvedgals-large-reducedlocalcalcs: test-improvedgals-large

test-improvedgals-large-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-large-singleforceloop: test-improvedgals-large

test-improvedgals-large-singlereducedforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP -DREDUCED_LOCAL_CALCS
test-improvedgals-large-singlereducedforceloop: test-improvedgals-large

# **TEST Local calculation method: onebyone**
test-improvedgals-large-onebyone: DFLAGS+=-DONE_BY_ONE
test-improvedgals-large-onebyone: test-improvedgals-large

test-improvedgals-large-onebyone-betterverlet: DFLAGS+=-DBETTER_VERLET
test-improvedgals-large-onebyone-betterverlet: test-improvedgals-large-onebyone

test-improvedgals-large-onebyone-dtchange: DFLAGS+=-DSMALL_DT_EARLY
test-improvedgals-large-onebyone-dtchange: test-improvedgals-large-onebyone

test-improvedgals-large-onebyone-doublesqrt: DFLAGS+=-DDOUBLE_SQRT
test-improvedgals-large-onebyone-doublesqrt: test-improvedgals-large-onebyone

test-improvedgals-large-onebyone-msgmgmt: DFLAGS+=-DMESSAGE_MANAGEMENT
test-improvedgals-large-onebyone-msgmgmt: test-improvedgals-large-onebyone

test-improvedgals-large-onebyone-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-improvedgals-large-onebyone-reducedlocalcalcs: test-improvedgals-large-onebyone

test-improvedgals-large-onebyone-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-large-onebyone-singleforceloop: test-improvedgals-large-onebyone

test-improvedgals-large-onebyone-singlereducedforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP -DREDUCED_LOCAL_CALCS
test-improvedgals-large-onebyone-singlereducedforceloop: test-improvedgals-large-onebyone

test-improvedgals-bonds: DFLAGS+=-DBONDS -DVESICLE_SELF_ASSEMBLY
test-improvedgals-bonds: test-improvedgals

test-improvedgals-bonds-betterverlet: DFLAGS+=-DBETTER_VERLET
test-improvedgals-bonds-betterverlet: test-improvedgals-bonds

test-improvedgals-bonds-dtchange: DFLAGS+=-DSMALL_DT_EARLY
test-improvedgals-bonds-dtchange: test-improvedgals-bonds

test-improvedgals-bonds-doublesqrt: DFLAGS+=-DDOUBLE_SQRT
test-improvedgals-bonds-doublesqrt: test-improvedgals-bonds

test-improvedgals-bonds-msgmgmt: DFLAGS+=-DMESSAGE_MANAGEMENT
test-improvedgals-bonds-msgmgmt: test-improvedgals-bonds

test-improvedgals-bonds-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-improvedgals-bonds-reducedlocalcalcs: test-improvedgals-bonds

test-improvedgals-bonds-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-bonds-singleforceloop: test-improvedgals-bonds

test-improvedgals-bonds-singlereducedforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP -DREDUCED_LOCAL_CALCS
test-improvedgals-bonds-singlereducedforceloop: test-improvedgals-bonds

# **TEST Local calculation method: onebyone**
test-improvedgals-bonds-onebyone: DFLAGS+=-DONE_BY_ONE
test-improvedgals-bonds-onebyone: test-improvedgals-bonds

test-improvedgals-bonds-onebyone-betterverlet: DFLAGS+=-DBETTER_VERLET
test-improvedgals-bonds-onebyone-betterverlet: test-improvedgals-bonds-onebyone

test-improvedgals-bonds-onebyone-dtchange: DFLAGS+=-DSMALL_DT_EARLY
test-improvedgals-bonds-onebyone-dtchange: test-improvedgals-bonds-onebyone

test-improvedgals-bonds-onebyone-doublesqrt: DFLAGS+=-DDOUBLE_SQRT
test-improvedgals-bonds-onebyone-doublesqrt: test-improvedgals-bonds-onebyone

test-improvedgals-bonds-onebyone-msgmgmt: DFLAGS+=-DMESSAGE_MANAGEMENT
test-improvedgals-bonds-onebyone-msgmgmt: test-improvedgals-bonds-onebyone

test-improvedgals-bonds-onebyone-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-improvedgals-bonds-onebyone-reducedlocalcalcs: test-improvedgals-bonds-onebyone

test-improvedgals-bonds-onebyone-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-bonds-onebyone-singleforceloop: test-improvedgals-bonds-onebyone

test-improvedgals-bonds-onebyone-singlereducedforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP -DREDUCED_LOCAL_CALCS
test-improvedgals-bonds-onebyone-singlereducedforceloop: test-improvedgals-bonds-onebyone


# ************** TEST Simulator: serial**************

test-serial: OBJS=$(SERIAL_OBJS)
test-serial: METIS=
test-serial: DFLAGS+=-DTESTING -DSERIAL
test-serial: $(SERIAL_OBJS) serial test

test-serial-betterverlet: DFLAGS+=-DBETTER_VERLET
test-serial-betterverlet: test-serial

test-serial-dtchange: DFLAGS+=-DSMALL_DT_EARLY
test-serial-dtchange: test-serial

test-serial-doublesqrt: DFLAGS+=-DDOUBLE_SQRT
test-serial-doublesqrt: test-serial

test-serial-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-serial-reducedlocalcalcs: test-serial

test-serial-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-serial-singleforceloop: test-serial

test-serial-singlereducedforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP -DREDUCED_LOCAL_CALCS
test-serial-singlereducedforceloop: test-serial

test-serial-large: DFLAGS+=-DLARGE_TEST
test-serial-large: test-serial

test-serial-large-betterverlet: DFLAGS+=-DBETTER_VERLET
test-serial-large-betterverlet: test-serial-large

test-serial-large-dtchange: DFLAGS+=-DSMALL_DT_EARLY
test-serial-large-dtchange: test-serial-large

test-serial-large-doublesqrt: DFLAGS+=-DDOUBLE_SQRT
test-serial-large-doublesqrt: test-serial-large

test-serial-large-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-serial-large-reducedlocalcalcs: test-serial-large

test-serial-large-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-serial-large-singleforceloop: test-serial-large

test-serial-large-singlereducedforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP -DREDUCED_LOCAL_CALCS
test-serial-large-singlereducedforceloop: test-serial-large

test-serial-bonds: DFLAGS+=-DBONDS -DVESICLE_SELF_ASSEMBLY
test-serial-bonds: test-serial

test-serial-bonds-betterverlet: DFLAGS+=-DBETTER_VERLET
test-serial-bonds-betterverlet: test-serial-bonds

test-serial-bonds-dtchange: DFLAGS+=-DSMALL_DT_EARLY
test-serial-bonds-dtchange: test-serial-bonds

test-serial-bonds-doublesqrt: DFLAGS+=-DDOUBLE_SQRT
test-serial-bonds-doublesqrt: test-serial-bonds

test-serial-bonds-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-serial-bonds-reducedlocalcalcs: test-serial-bonds

test-serial-bonds-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-serial-bonds-singleforceloop: test-serial-bonds

test-serial-bonds-singlereducedforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP -DREDUCED_LOCAL_CALCS
test-serial-bonds-singlereducedforceloop: test-serial-bonds

