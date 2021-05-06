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
$(DPD_BIN)/dpd.elf: DFLAGS+=-DONE_BY_ONE
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
test: $(INC)/config.h $(HL)/*.o $(POLITE_OBJS) base-sync
	g++ -O2 -std=c++11 $(DFLAGS) $(EXTERNAL_FLAGS) -I $(INC) -I $(QUEUE_INC) -I $(HL) -I $(DPD_INC) -c -o $(DPD_BIN)/test.o $(DPD_SRC)/test.cpp
	g++ -O2 -std=c++11 -o $(DPD_BIN)/test $(POLITE_OBJS) $(DPD_BIN)/test.o \
	  -static-libgcc -static-libstdc++ \
      -L$(QUARTUS_ROOTDIR)/linux64 \
      -Wl,-rpath,$(QUARTUS_ROOTDIR)/linux64 $(METIS)-lmetis -lboost_program_options -lboost_filesystem -lboost_system -fopenmp

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
		-static-libgcc -static-libstdc++ -L$(QUARTUS_ROOTDIR)/linux64 \
		-Wl,-rpath,$(QUARTUS_ROOTDIR)/linux64 -lmetis -lpthread -lboost_program_options \
		-lboost_filesystem -lboost_system -fopenmp

vesicle: DFLAGS+=-DVESICLE_SELF_ASSEMBLY -DBONDS
vesicle: $(DPD_EXAMPLES)/VesicleSelfAssembly
	g++ -O2 -std=c++11 $(DFLAGS) $(EXTERNAL_FLAGS) \
	-I $(HL) -I $(DPD_INC) -c -o $(DPD_BIN)/VesicleSelfAssembly.o $(DPD_EXAMPLES)/VesicleSelfAssembly.cpp
	g++ -O2 -std=c++11 -o $(DPD_BIN)/run $(OBJS) $(DPD_BIN)/VesicleSelfAssembly.o \
		-static-libgcc -static-libstdc++ -L$(QUARTUS_ROOTDIR)/linux64 \
		-Wl,-rpath,$(QUARTUS_ROOTDIR)/linux64 -lmetis -lpthread -lboost_program_options \
		-lboost_filesystem -lboost_system -fopenmp

corners: DFLAGS+=
corners: $(DPD_EXAMPLES)/corner-tests
	g++ -O2 -std=c++11 $(DFLAGS) $(EXTERNAL_FLAGS) \
	-I $(HL) -I $(DPD_INC) -c -o $(DPD_BIN)/corner-tests.o $(DPD_EXAMPLES)/corner-tests.cpp
	g++ -O2 -std=c++11 -o $(DPD_BIN)/run $(OBJS) $(DPD_BIN)/corner-tests.o \
		-static-libgcc -static-libstdc++ -L$(QUARTUS_ROOTDIR)/linux64 \
		-Wl,-rpath,$(QUARTUS_ROOTDIR)/linux64 -lmetis -lpthread -lboost_program_options \
		-lboost_filesystem -lboost_system -fopenmp

gravity: DFLAGS+=-DGRAVITY
gravity: $(DPD_EXAMPLES)/BoxOilWaterGravity
	g++ -O2 -std=c++11 $(DFLAGS) $(EXTERNAL_FLAGS) \
	-I $(HL) -I $(DPD_INC) -c -o $(DPD_BIN)/BoxOilWaterGravity.o $(DPD_EXAMPLES)/BoxOilWaterGravity.cpp
	g++ -O2 -std=c++11 -o $(DPD_BIN)/run $(OBJS) $(DPD_BIN)/BoxOilWaterGravity.o \
		-static-libgcc -static-libstdc++ -L$(QUARTUS_ROOTDIR)/linux64 \
		-Wl,-rpath,$(QUARTUS_ROOTDIR)/linux64 -lmetis -lpthread -lboost_program_options \
		-lboost_filesystem -lboost_system -fopenmp

# ************** Simulator: sync**************

# **********Operation: visual**********

sync-visual: OBJS+=$(POLITE_OBJS)
sync-visual: DFLAGS+=-DVISUALISE
sync-visual: base-sync

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
sync-timed: DFLAGS+=-DTIMER
sync-timed: base-sync

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
sync-stats: DFLAGS+=-DSTATS
sync-stats: base-sync

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
gals-visual: DFLAGS+=-DVISUALISE
gals-visual: base-gals

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
gals-timed: DFLAGS+=-DTIMER
gals-timed: base-gals

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
gals-stats: DFLAGS+=-DSTATS
gals-stats: base-gals

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

improvedgals-visual: OBJS+=$(-lmetis)
improvedgals-visual: DFLAGS+=-DVISUALISE
improvedgals-visual: base-gals

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

improvedgals-timed: OBJS+=$(-lmetis)
improvedgals-timed: DFLAGS+=-DTIMER
improvedgals-timed: base-gals

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

improvedgals-stats: OBJS+=$(-lmetis)
improvedgals-stats: DFLAGS+=-DSTATS
improvedgals-stats: base-gals

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
serial-visual: DFLAGS+=-DVISUALISE
serial-visual: serial

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
serial-timed: DFLAGS+=-DTIMER
serial-timed: serial

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
test-sync: DFLAGS+=-DTESTING 
test-sync: base-sync test

test-sync-betterverlet: DFLAGS+=-DBETTER_VERLET
test-sync-betterverlet: test-sync

test-sync-betterverlet-dtchange: DFLAGS+=-DSMALL_DT_EARLY
test-sync-betterverlet-dtchange: test-sync-betterverlet

test-sync-betterverlet-dtchange-doublesqrt: DFLAGS+=-DOUBLE_SQRT
test-sync-betterverlet-dtchange-doublesqrt: test-sync-betterverlet-dtchange

test-sync-betterverlet-dtchange-doublesqrt-msgmgmt: DFLAGS+=-DMESSAGE_MANAGEMENT
test-sync-betterverlet-dtchange-doublesqrt-msgmgmt: test-sync-betterverlet-dtchange-doublesqrt

test-sync-betterverlet-dtchange-doublesqrt-msgmgmt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-sync-betterverlet-dtchange-doublesqrt-msgmgmt-floatonly: test-sync-betterverlet-dtchange-doublesqrt-msgmgmt

test-sync-betterverlet-dtchange-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-sync-betterverlet-dtchange-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs: test-sync-betterverlet-dtchange-doublesqrt-msgmgmt-floatonly

test-sync-betterverlet-dtchange-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-betterverlet-dtchange-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs-singleforceloop: test-sync-betterverlet-dtchange-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs

test-sync-betterverlet-dtchange-doublesqrt-msgmgmt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-betterverlet-dtchange-doublesqrt-msgmgmt-floatonly-singleforceloop: test-sync-betterverlet-dtchange-doublesqrt-msgmgmt-floatonly

test-sync-betterverlet-dtchange-doublesqrt-msgmgmt-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-sync-betterverlet-dtchange-doublesqrt-msgmgmt-reducedlocalcalcs: test-sync-betterverlet-dtchange-doublesqrt-msgmgmt

test-sync-betterverlet-dtchange-doublesqrt-msgmgmt-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-betterverlet-dtchange-doublesqrt-msgmgmt-reducedlocalcalcs-singleforceloop: test-sync-betterverlet-dtchange-doublesqrt-msgmgmt-reducedlocalcalcs

test-sync-betterverlet-dtchange-doublesqrt-msgmgmt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-betterverlet-dtchange-doublesqrt-msgmgmt-singleforceloop: test-sync-betterverlet-dtchange-doublesqrt-msgmgmt

test-sync-betterverlet-dtchange-doublesqrt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-sync-betterverlet-dtchange-doublesqrt-floatonly: test-sync-betterverlet-dtchange-doublesqrt

test-sync-betterverlet-dtchange-doublesqrt-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-sync-betterverlet-dtchange-doublesqrt-floatonly-reducedlocalcalcs: test-sync-betterverlet-dtchange-doublesqrt-floatonly

test-sync-betterverlet-dtchange-doublesqrt-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-betterverlet-dtchange-doublesqrt-floatonly-reducedlocalcalcs-singleforceloop: test-sync-betterverlet-dtchange-doublesqrt-floatonly-reducedlocalcalcs

test-sync-betterverlet-dtchange-doublesqrt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-betterverlet-dtchange-doublesqrt-floatonly-singleforceloop: test-sync-betterverlet-dtchange-doublesqrt-floatonly

test-sync-betterverlet-dtchange-doublesqrt-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-sync-betterverlet-dtchange-doublesqrt-reducedlocalcalcs: test-sync-betterverlet-dtchange-doublesqrt

test-sync-betterverlet-dtchange-doublesqrt-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-betterverlet-dtchange-doublesqrt-reducedlocalcalcs-singleforceloop: test-sync-betterverlet-dtchange-doublesqrt-reducedlocalcalcs

test-sync-betterverlet-dtchange-doublesqrt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-betterverlet-dtchange-doublesqrt-singleforceloop: test-sync-betterverlet-dtchange-doublesqrt

test-sync-betterverlet-dtchange-msgmgmt: DFLAGS+=-DMESSAGE_MANAGEMENT
test-sync-betterverlet-dtchange-msgmgmt: test-sync-betterverlet-dtchange

test-sync-betterverlet-dtchange-msgmgmt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-sync-betterverlet-dtchange-msgmgmt-floatonly: test-sync-betterverlet-dtchange-msgmgmt

test-sync-betterverlet-dtchange-msgmgmt-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-sync-betterverlet-dtchange-msgmgmt-floatonly-reducedlocalcalcs: test-sync-betterverlet-dtchange-msgmgmt-floatonly

test-sync-betterverlet-dtchange-msgmgmt-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-betterverlet-dtchange-msgmgmt-floatonly-reducedlocalcalcs-singleforceloop: test-sync-betterverlet-dtchange-msgmgmt-floatonly-reducedlocalcalcs

test-sync-betterverlet-dtchange-msgmgmt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-betterverlet-dtchange-msgmgmt-floatonly-singleforceloop: test-sync-betterverlet-dtchange-msgmgmt-floatonly

test-sync-betterverlet-dtchange-msgmgmt-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-sync-betterverlet-dtchange-msgmgmt-reducedlocalcalcs: test-sync-betterverlet-dtchange-msgmgmt

test-sync-betterverlet-dtchange-msgmgmt-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-betterverlet-dtchange-msgmgmt-reducedlocalcalcs-singleforceloop: test-sync-betterverlet-dtchange-msgmgmt-reducedlocalcalcs

test-sync-betterverlet-dtchange-msgmgmt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-betterverlet-dtchange-msgmgmt-singleforceloop: test-sync-betterverlet-dtchange-msgmgmt

test-sync-betterverlet-dtchange-floatonly: DFLAGS+=-DFLOAT_ONLY
test-sync-betterverlet-dtchange-floatonly: test-sync-betterverlet-dtchange

test-sync-betterverlet-dtchange-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-sync-betterverlet-dtchange-floatonly-reducedlocalcalcs: test-sync-betterverlet-dtchange-floatonly

test-sync-betterverlet-dtchange-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-betterverlet-dtchange-floatonly-reducedlocalcalcs-singleforceloop: test-sync-betterverlet-dtchange-floatonly-reducedlocalcalcs

test-sync-betterverlet-dtchange-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-betterverlet-dtchange-floatonly-singleforceloop: test-sync-betterverlet-dtchange-floatonly

test-sync-betterverlet-dtchange-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-sync-betterverlet-dtchange-reducedlocalcalcs: test-sync-betterverlet-dtchange

test-sync-betterverlet-dtchange-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-betterverlet-dtchange-reducedlocalcalcs-singleforceloop: test-sync-betterverlet-dtchange-reducedlocalcalcs

test-sync-betterverlet-dtchange-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-betterverlet-dtchange-singleforceloop: test-sync-betterverlet-dtchange

test-sync-betterverlet-doublesqrt: DFLAGS+=-DOUBLE_SQRT
test-sync-betterverlet-doublesqrt: test-sync-betterverlet

test-sync-betterverlet-doublesqrt-msgmgmt: DFLAGS+=-DMESSAGE_MANAGEMENT
test-sync-betterverlet-doublesqrt-msgmgmt: test-sync-betterverlet-doublesqrt

test-sync-betterverlet-doublesqrt-msgmgmt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-sync-betterverlet-doublesqrt-msgmgmt-floatonly: test-sync-betterverlet-doublesqrt-msgmgmt

test-sync-betterverlet-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-sync-betterverlet-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs: test-sync-betterverlet-doublesqrt-msgmgmt-floatonly

test-sync-betterverlet-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-betterverlet-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs-singleforceloop: test-sync-betterverlet-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs

test-sync-betterverlet-doublesqrt-msgmgmt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-betterverlet-doublesqrt-msgmgmt-floatonly-singleforceloop: test-sync-betterverlet-doublesqrt-msgmgmt-floatonly

test-sync-betterverlet-doublesqrt-msgmgmt-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-sync-betterverlet-doublesqrt-msgmgmt-reducedlocalcalcs: test-sync-betterverlet-doublesqrt-msgmgmt

test-sync-betterverlet-doublesqrt-msgmgmt-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-betterverlet-doublesqrt-msgmgmt-reducedlocalcalcs-singleforceloop: test-sync-betterverlet-doublesqrt-msgmgmt-reducedlocalcalcs

test-sync-betterverlet-doublesqrt-msgmgmt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-betterverlet-doublesqrt-msgmgmt-singleforceloop: test-sync-betterverlet-doublesqrt-msgmgmt

test-sync-betterverlet-doublesqrt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-sync-betterverlet-doublesqrt-floatonly: test-sync-betterverlet-doublesqrt

test-sync-betterverlet-doublesqrt-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-sync-betterverlet-doublesqrt-floatonly-reducedlocalcalcs: test-sync-betterverlet-doublesqrt-floatonly

test-sync-betterverlet-doublesqrt-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-betterverlet-doublesqrt-floatonly-reducedlocalcalcs-singleforceloop: test-sync-betterverlet-doublesqrt-floatonly-reducedlocalcalcs

test-sync-betterverlet-doublesqrt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-betterverlet-doublesqrt-floatonly-singleforceloop: test-sync-betterverlet-doublesqrt-floatonly

test-sync-betterverlet-doublesqrt-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-sync-betterverlet-doublesqrt-reducedlocalcalcs: test-sync-betterverlet-doublesqrt

test-sync-betterverlet-doublesqrt-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-betterverlet-doublesqrt-reducedlocalcalcs-singleforceloop: test-sync-betterverlet-doublesqrt-reducedlocalcalcs

test-sync-betterverlet-doublesqrt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-betterverlet-doublesqrt-singleforceloop: test-sync-betterverlet-doublesqrt

test-sync-betterverlet-msgmgmt: DFLAGS+=-DMESSAGE_MANAGEMENT
test-sync-betterverlet-msgmgmt: test-sync-betterverlet

test-sync-betterverlet-msgmgmt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-sync-betterverlet-msgmgmt-floatonly: test-sync-betterverlet-msgmgmt

test-sync-betterverlet-msgmgmt-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-sync-betterverlet-msgmgmt-floatonly-reducedlocalcalcs: test-sync-betterverlet-msgmgmt-floatonly

test-sync-betterverlet-msgmgmt-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-betterverlet-msgmgmt-floatonly-reducedlocalcalcs-singleforceloop: test-sync-betterverlet-msgmgmt-floatonly-reducedlocalcalcs

test-sync-betterverlet-msgmgmt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-betterverlet-msgmgmt-floatonly-singleforceloop: test-sync-betterverlet-msgmgmt-floatonly

test-sync-betterverlet-msgmgmt-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-sync-betterverlet-msgmgmt-reducedlocalcalcs: test-sync-betterverlet-msgmgmt

test-sync-betterverlet-msgmgmt-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-betterverlet-msgmgmt-reducedlocalcalcs-singleforceloop: test-sync-betterverlet-msgmgmt-reducedlocalcalcs

test-sync-betterverlet-msgmgmt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-betterverlet-msgmgmt-singleforceloop: test-sync-betterverlet-msgmgmt

test-sync-betterverlet-floatonly: DFLAGS+=-DFLOAT_ONLY
test-sync-betterverlet-floatonly: test-sync-betterverlet

test-sync-betterverlet-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-sync-betterverlet-floatonly-reducedlocalcalcs: test-sync-betterverlet-floatonly

test-sync-betterverlet-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-betterverlet-floatonly-reducedlocalcalcs-singleforceloop: test-sync-betterverlet-floatonly-reducedlocalcalcs

test-sync-betterverlet-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-betterverlet-floatonly-singleforceloop: test-sync-betterverlet-floatonly

test-sync-betterverlet-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-sync-betterverlet-reducedlocalcalcs: test-sync-betterverlet

test-sync-betterverlet-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-betterverlet-reducedlocalcalcs-singleforceloop: test-sync-betterverlet-reducedlocalcalcs

test-sync-betterverlet-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-betterverlet-singleforceloop: test-sync-betterverlet

test-sync-dtchange: DFLAGS+=-DSMALL_DT_EARLY
test-sync-dtchange: test-sync

test-sync-dtchange-doublesqrt: DFLAGS+=-DOUBLE_SQRT
test-sync-dtchange-doublesqrt: test-sync-dtchange

test-sync-dtchange-doublesqrt-msgmgmt: DFLAGS+=-DMESSAGE_MANAGEMENT
test-sync-dtchange-doublesqrt-msgmgmt: test-sync-dtchange-doublesqrt

test-sync-dtchange-doublesqrt-msgmgmt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-sync-dtchange-doublesqrt-msgmgmt-floatonly: test-sync-dtchange-doublesqrt-msgmgmt

test-sync-dtchange-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-sync-dtchange-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs: test-sync-dtchange-doublesqrt-msgmgmt-floatonly

test-sync-dtchange-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-dtchange-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs-singleforceloop: test-sync-dtchange-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs

test-sync-dtchange-doublesqrt-msgmgmt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-dtchange-doublesqrt-msgmgmt-floatonly-singleforceloop: test-sync-dtchange-doublesqrt-msgmgmt-floatonly

test-sync-dtchange-doublesqrt-msgmgmt-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-sync-dtchange-doublesqrt-msgmgmt-reducedlocalcalcs: test-sync-dtchange-doublesqrt-msgmgmt

test-sync-dtchange-doublesqrt-msgmgmt-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-dtchange-doublesqrt-msgmgmt-reducedlocalcalcs-singleforceloop: test-sync-dtchange-doublesqrt-msgmgmt-reducedlocalcalcs

test-sync-dtchange-doublesqrt-msgmgmt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-dtchange-doublesqrt-msgmgmt-singleforceloop: test-sync-dtchange-doublesqrt-msgmgmt

test-sync-dtchange-doublesqrt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-sync-dtchange-doublesqrt-floatonly: test-sync-dtchange-doublesqrt

test-sync-dtchange-doublesqrt-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-sync-dtchange-doublesqrt-floatonly-reducedlocalcalcs: test-sync-dtchange-doublesqrt-floatonly

test-sync-dtchange-doublesqrt-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-dtchange-doublesqrt-floatonly-reducedlocalcalcs-singleforceloop: test-sync-dtchange-doublesqrt-floatonly-reducedlocalcalcs

test-sync-dtchange-doublesqrt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-dtchange-doublesqrt-floatonly-singleforceloop: test-sync-dtchange-doublesqrt-floatonly

test-sync-dtchange-doublesqrt-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-sync-dtchange-doublesqrt-reducedlocalcalcs: test-sync-dtchange-doublesqrt

test-sync-dtchange-doublesqrt-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-dtchange-doublesqrt-reducedlocalcalcs-singleforceloop: test-sync-dtchange-doublesqrt-reducedlocalcalcs

test-sync-dtchange-doublesqrt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-dtchange-doublesqrt-singleforceloop: test-sync-dtchange-doublesqrt

test-sync-dtchange-msgmgmt: DFLAGS+=-DMESSAGE_MANAGEMENT
test-sync-dtchange-msgmgmt: test-sync-dtchange

test-sync-dtchange-msgmgmt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-sync-dtchange-msgmgmt-floatonly: test-sync-dtchange-msgmgmt

test-sync-dtchange-msgmgmt-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-sync-dtchange-msgmgmt-floatonly-reducedlocalcalcs: test-sync-dtchange-msgmgmt-floatonly

test-sync-dtchange-msgmgmt-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-dtchange-msgmgmt-floatonly-reducedlocalcalcs-singleforceloop: test-sync-dtchange-msgmgmt-floatonly-reducedlocalcalcs

test-sync-dtchange-msgmgmt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-dtchange-msgmgmt-floatonly-singleforceloop: test-sync-dtchange-msgmgmt-floatonly

test-sync-dtchange-msgmgmt-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-sync-dtchange-msgmgmt-reducedlocalcalcs: test-sync-dtchange-msgmgmt

test-sync-dtchange-msgmgmt-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-dtchange-msgmgmt-reducedlocalcalcs-singleforceloop: test-sync-dtchange-msgmgmt-reducedlocalcalcs

test-sync-dtchange-msgmgmt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-dtchange-msgmgmt-singleforceloop: test-sync-dtchange-msgmgmt

test-sync-dtchange-floatonly: DFLAGS+=-DFLOAT_ONLY
test-sync-dtchange-floatonly: test-sync-dtchange

test-sync-dtchange-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-sync-dtchange-floatonly-reducedlocalcalcs: test-sync-dtchange-floatonly

test-sync-dtchange-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-dtchange-floatonly-reducedlocalcalcs-singleforceloop: test-sync-dtchange-floatonly-reducedlocalcalcs

test-sync-dtchange-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-dtchange-floatonly-singleforceloop: test-sync-dtchange-floatonly

test-sync-dtchange-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-sync-dtchange-reducedlocalcalcs: test-sync-dtchange

test-sync-dtchange-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-dtchange-reducedlocalcalcs-singleforceloop: test-sync-dtchange-reducedlocalcalcs

test-sync-dtchange-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-dtchange-singleforceloop: test-sync-dtchange

test-sync-doublesqrt: DFLAGS+=-DOUBLE_SQRT
test-sync-doublesqrt: test-sync

test-sync-doublesqrt-msgmgmt: DFLAGS+=-DMESSAGE_MANAGEMENT
test-sync-doublesqrt-msgmgmt: test-sync-doublesqrt

test-sync-doublesqrt-msgmgmt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-sync-doublesqrt-msgmgmt-floatonly: test-sync-doublesqrt-msgmgmt

test-sync-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-sync-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs: test-sync-doublesqrt-msgmgmt-floatonly

test-sync-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs-singleforceloop: test-sync-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs

test-sync-doublesqrt-msgmgmt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-doublesqrt-msgmgmt-floatonly-singleforceloop: test-sync-doublesqrt-msgmgmt-floatonly

test-sync-doublesqrt-msgmgmt-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-sync-doublesqrt-msgmgmt-reducedlocalcalcs: test-sync-doublesqrt-msgmgmt

test-sync-doublesqrt-msgmgmt-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-doublesqrt-msgmgmt-reducedlocalcalcs-singleforceloop: test-sync-doublesqrt-msgmgmt-reducedlocalcalcs

test-sync-doublesqrt-msgmgmt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-doublesqrt-msgmgmt-singleforceloop: test-sync-doublesqrt-msgmgmt

test-sync-doublesqrt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-sync-doublesqrt-floatonly: test-sync-doublesqrt

test-sync-doublesqrt-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-sync-doublesqrt-floatonly-reducedlocalcalcs: test-sync-doublesqrt-floatonly

test-sync-doublesqrt-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-doublesqrt-floatonly-reducedlocalcalcs-singleforceloop: test-sync-doublesqrt-floatonly-reducedlocalcalcs

test-sync-doublesqrt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-doublesqrt-floatonly-singleforceloop: test-sync-doublesqrt-floatonly

test-sync-doublesqrt-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-sync-doublesqrt-reducedlocalcalcs: test-sync-doublesqrt

test-sync-doublesqrt-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-doublesqrt-reducedlocalcalcs-singleforceloop: test-sync-doublesqrt-reducedlocalcalcs

test-sync-doublesqrt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-doublesqrt-singleforceloop: test-sync-doublesqrt

test-sync-msgmgmt: DFLAGS+=-DMESSAGE_MANAGEMENT
test-sync-msgmgmt: test-sync

test-sync-msgmgmt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-sync-msgmgmt-floatonly: test-sync-msgmgmt

test-sync-msgmgmt-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-sync-msgmgmt-floatonly-reducedlocalcalcs: test-sync-msgmgmt-floatonly

test-sync-msgmgmt-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-msgmgmt-floatonly-reducedlocalcalcs-singleforceloop: test-sync-msgmgmt-floatonly-reducedlocalcalcs

test-sync-msgmgmt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-msgmgmt-floatonly-singleforceloop: test-sync-msgmgmt-floatonly

test-sync-msgmgmt-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-sync-msgmgmt-reducedlocalcalcs: test-sync-msgmgmt

test-sync-msgmgmt-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-msgmgmt-reducedlocalcalcs-singleforceloop: test-sync-msgmgmt-reducedlocalcalcs

test-sync-msgmgmt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-msgmgmt-singleforceloop: test-sync-msgmgmt

test-sync-floatonly: DFLAGS+=-DFLOAT_ONLY
test-sync-floatonly: test-sync

test-sync-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-sync-floatonly-reducedlocalcalcs: test-sync-floatonly

test-sync-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-floatonly-reducedlocalcalcs-singleforceloop: test-sync-floatonly-reducedlocalcalcs

test-sync-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-floatonly-singleforceloop: test-sync-floatonly

test-sync-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-sync-reducedlocalcalcs: test-sync

test-sync-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-reducedlocalcalcs-singleforceloop: test-sync-reducedlocalcalcs

test-sync-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-singleforceloop: test-sync

# **TEST Local calculation method: onebyone**
test-sync-onebyone: DFLAGS+=-DONE_BY_ONE
test-sync-onebyone: test-sync

test-sync-onebyone-betterverlet: DFLAGS+=-DBETTER_VERLET
test-sync-onebyone-betterverlet: test-sync-onebyone

test-sync-onebyone-betterverlet-dtchange: DFLAGS+=-DSMALL_DT_EARLY
test-sync-onebyone-betterverlet-dtchange: test-sync-onebyone-betterverlet

test-sync-onebyone-betterverlet-dtchange-doublesqrt: DFLAGS+=-DOUBLE_SQRT
test-sync-onebyone-betterverlet-dtchange-doublesqrt: test-sync-onebyone-betterverlet-dtchange

test-sync-onebyone-betterverlet-dtchange-doublesqrt-msgmgmt: DFLAGS+=-DMESSAGE_MANAGEMENT
test-sync-onebyone-betterverlet-dtchange-doublesqrt-msgmgmt: test-sync-onebyone-betterverlet-dtchange-doublesqrt

test-sync-onebyone-betterverlet-dtchange-doublesqrt-msgmgmt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-sync-onebyone-betterverlet-dtchange-doublesqrt-msgmgmt-floatonly: test-sync-onebyone-betterverlet-dtchange-doublesqrt-msgmgmt

test-sync-onebyone-betterverlet-dtchange-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-sync-onebyone-betterverlet-dtchange-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs: test-sync-onebyone-betterverlet-dtchange-doublesqrt-msgmgmt-floatonly

test-sync-onebyone-betterverlet-dtchange-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-onebyone-betterverlet-dtchange-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs-singleforceloop: test-sync-onebyone-betterverlet-dtchange-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs

test-sync-onebyone-betterverlet-dtchange-doublesqrt-msgmgmt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-onebyone-betterverlet-dtchange-doublesqrt-msgmgmt-floatonly-singleforceloop: test-sync-onebyone-betterverlet-dtchange-doublesqrt-msgmgmt-floatonly

test-sync-onebyone-betterverlet-dtchange-doublesqrt-msgmgmt-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-sync-onebyone-betterverlet-dtchange-doublesqrt-msgmgmt-reducedlocalcalcs: test-sync-onebyone-betterverlet-dtchange-doublesqrt-msgmgmt

test-sync-onebyone-betterverlet-dtchange-doublesqrt-msgmgmt-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-onebyone-betterverlet-dtchange-doublesqrt-msgmgmt-reducedlocalcalcs-singleforceloop: test-sync-onebyone-betterverlet-dtchange-doublesqrt-msgmgmt-reducedlocalcalcs

test-sync-onebyone-betterverlet-dtchange-doublesqrt-msgmgmt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-onebyone-betterverlet-dtchange-doublesqrt-msgmgmt-singleforceloop: test-sync-onebyone-betterverlet-dtchange-doublesqrt-msgmgmt

test-sync-onebyone-betterverlet-dtchange-doublesqrt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-sync-onebyone-betterverlet-dtchange-doublesqrt-floatonly: test-sync-onebyone-betterverlet-dtchange-doublesqrt

test-sync-onebyone-betterverlet-dtchange-doublesqrt-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-sync-onebyone-betterverlet-dtchange-doublesqrt-floatonly-reducedlocalcalcs: test-sync-onebyone-betterverlet-dtchange-doublesqrt-floatonly

test-sync-onebyone-betterverlet-dtchange-doublesqrt-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-onebyone-betterverlet-dtchange-doublesqrt-floatonly-reducedlocalcalcs-singleforceloop: test-sync-onebyone-betterverlet-dtchange-doublesqrt-floatonly-reducedlocalcalcs

test-sync-onebyone-betterverlet-dtchange-doublesqrt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-onebyone-betterverlet-dtchange-doublesqrt-floatonly-singleforceloop: test-sync-onebyone-betterverlet-dtchange-doublesqrt-floatonly

test-sync-onebyone-betterverlet-dtchange-doublesqrt-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-sync-onebyone-betterverlet-dtchange-doublesqrt-reducedlocalcalcs: test-sync-onebyone-betterverlet-dtchange-doublesqrt

test-sync-onebyone-betterverlet-dtchange-doublesqrt-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-onebyone-betterverlet-dtchange-doublesqrt-reducedlocalcalcs-singleforceloop: test-sync-onebyone-betterverlet-dtchange-doublesqrt-reducedlocalcalcs

test-sync-onebyone-betterverlet-dtchange-doublesqrt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-onebyone-betterverlet-dtchange-doublesqrt-singleforceloop: test-sync-onebyone-betterverlet-dtchange-doublesqrt

test-sync-onebyone-betterverlet-dtchange-msgmgmt: DFLAGS+=-DMESSAGE_MANAGEMENT
test-sync-onebyone-betterverlet-dtchange-msgmgmt: test-sync-onebyone-betterverlet-dtchange

test-sync-onebyone-betterverlet-dtchange-msgmgmt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-sync-onebyone-betterverlet-dtchange-msgmgmt-floatonly: test-sync-onebyone-betterverlet-dtchange-msgmgmt

test-sync-onebyone-betterverlet-dtchange-msgmgmt-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-sync-onebyone-betterverlet-dtchange-msgmgmt-floatonly-reducedlocalcalcs: test-sync-onebyone-betterverlet-dtchange-msgmgmt-floatonly

test-sync-onebyone-betterverlet-dtchange-msgmgmt-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-onebyone-betterverlet-dtchange-msgmgmt-floatonly-reducedlocalcalcs-singleforceloop: test-sync-onebyone-betterverlet-dtchange-msgmgmt-floatonly-reducedlocalcalcs

test-sync-onebyone-betterverlet-dtchange-msgmgmt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-onebyone-betterverlet-dtchange-msgmgmt-floatonly-singleforceloop: test-sync-onebyone-betterverlet-dtchange-msgmgmt-floatonly

test-sync-onebyone-betterverlet-dtchange-msgmgmt-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-sync-onebyone-betterverlet-dtchange-msgmgmt-reducedlocalcalcs: test-sync-onebyone-betterverlet-dtchange-msgmgmt

test-sync-onebyone-betterverlet-dtchange-msgmgmt-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-onebyone-betterverlet-dtchange-msgmgmt-reducedlocalcalcs-singleforceloop: test-sync-onebyone-betterverlet-dtchange-msgmgmt-reducedlocalcalcs

test-sync-onebyone-betterverlet-dtchange-msgmgmt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-onebyone-betterverlet-dtchange-msgmgmt-singleforceloop: test-sync-onebyone-betterverlet-dtchange-msgmgmt

test-sync-onebyone-betterverlet-dtchange-floatonly: DFLAGS+=-DFLOAT_ONLY
test-sync-onebyone-betterverlet-dtchange-floatonly: test-sync-onebyone-betterverlet-dtchange

test-sync-onebyone-betterverlet-dtchange-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-sync-onebyone-betterverlet-dtchange-floatonly-reducedlocalcalcs: test-sync-onebyone-betterverlet-dtchange-floatonly

test-sync-onebyone-betterverlet-dtchange-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-onebyone-betterverlet-dtchange-floatonly-reducedlocalcalcs-singleforceloop: test-sync-onebyone-betterverlet-dtchange-floatonly-reducedlocalcalcs

test-sync-onebyone-betterverlet-dtchange-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-onebyone-betterverlet-dtchange-floatonly-singleforceloop: test-sync-onebyone-betterverlet-dtchange-floatonly

test-sync-onebyone-betterverlet-dtchange-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-sync-onebyone-betterverlet-dtchange-reducedlocalcalcs: test-sync-onebyone-betterverlet-dtchange

test-sync-onebyone-betterverlet-dtchange-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-onebyone-betterverlet-dtchange-reducedlocalcalcs-singleforceloop: test-sync-onebyone-betterverlet-dtchange-reducedlocalcalcs

test-sync-onebyone-betterverlet-dtchange-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-onebyone-betterverlet-dtchange-singleforceloop: test-sync-onebyone-betterverlet-dtchange

test-sync-onebyone-betterverlet-doublesqrt: DFLAGS+=-DOUBLE_SQRT
test-sync-onebyone-betterverlet-doublesqrt: test-sync-onebyone-betterverlet

test-sync-onebyone-betterverlet-doublesqrt-msgmgmt: DFLAGS+=-DMESSAGE_MANAGEMENT
test-sync-onebyone-betterverlet-doublesqrt-msgmgmt: test-sync-onebyone-betterverlet-doublesqrt

test-sync-onebyone-betterverlet-doublesqrt-msgmgmt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-sync-onebyone-betterverlet-doublesqrt-msgmgmt-floatonly: test-sync-onebyone-betterverlet-doublesqrt-msgmgmt

test-sync-onebyone-betterverlet-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-sync-onebyone-betterverlet-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs: test-sync-onebyone-betterverlet-doublesqrt-msgmgmt-floatonly

test-sync-onebyone-betterverlet-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-onebyone-betterverlet-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs-singleforceloop: test-sync-onebyone-betterverlet-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs

test-sync-onebyone-betterverlet-doublesqrt-msgmgmt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-onebyone-betterverlet-doublesqrt-msgmgmt-floatonly-singleforceloop: test-sync-onebyone-betterverlet-doublesqrt-msgmgmt-floatonly

test-sync-onebyone-betterverlet-doublesqrt-msgmgmt-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-sync-onebyone-betterverlet-doublesqrt-msgmgmt-reducedlocalcalcs: test-sync-onebyone-betterverlet-doublesqrt-msgmgmt

test-sync-onebyone-betterverlet-doublesqrt-msgmgmt-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-onebyone-betterverlet-doublesqrt-msgmgmt-reducedlocalcalcs-singleforceloop: test-sync-onebyone-betterverlet-doublesqrt-msgmgmt-reducedlocalcalcs

test-sync-onebyone-betterverlet-doublesqrt-msgmgmt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-onebyone-betterverlet-doublesqrt-msgmgmt-singleforceloop: test-sync-onebyone-betterverlet-doublesqrt-msgmgmt

test-sync-onebyone-betterverlet-doublesqrt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-sync-onebyone-betterverlet-doublesqrt-floatonly: test-sync-onebyone-betterverlet-doublesqrt

test-sync-onebyone-betterverlet-doublesqrt-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-sync-onebyone-betterverlet-doublesqrt-floatonly-reducedlocalcalcs: test-sync-onebyone-betterverlet-doublesqrt-floatonly

test-sync-onebyone-betterverlet-doublesqrt-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-onebyone-betterverlet-doublesqrt-floatonly-reducedlocalcalcs-singleforceloop: test-sync-onebyone-betterverlet-doublesqrt-floatonly-reducedlocalcalcs

test-sync-onebyone-betterverlet-doublesqrt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-onebyone-betterverlet-doublesqrt-floatonly-singleforceloop: test-sync-onebyone-betterverlet-doublesqrt-floatonly

test-sync-onebyone-betterverlet-doublesqrt-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-sync-onebyone-betterverlet-doublesqrt-reducedlocalcalcs: test-sync-onebyone-betterverlet-doublesqrt

test-sync-onebyone-betterverlet-doublesqrt-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-onebyone-betterverlet-doublesqrt-reducedlocalcalcs-singleforceloop: test-sync-onebyone-betterverlet-doublesqrt-reducedlocalcalcs

test-sync-onebyone-betterverlet-doublesqrt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-onebyone-betterverlet-doublesqrt-singleforceloop: test-sync-onebyone-betterverlet-doublesqrt

test-sync-onebyone-betterverlet-msgmgmt: DFLAGS+=-DMESSAGE_MANAGEMENT
test-sync-onebyone-betterverlet-msgmgmt: test-sync-onebyone-betterverlet

test-sync-onebyone-betterverlet-msgmgmt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-sync-onebyone-betterverlet-msgmgmt-floatonly: test-sync-onebyone-betterverlet-msgmgmt

test-sync-onebyone-betterverlet-msgmgmt-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-sync-onebyone-betterverlet-msgmgmt-floatonly-reducedlocalcalcs: test-sync-onebyone-betterverlet-msgmgmt-floatonly

test-sync-onebyone-betterverlet-msgmgmt-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-onebyone-betterverlet-msgmgmt-floatonly-reducedlocalcalcs-singleforceloop: test-sync-onebyone-betterverlet-msgmgmt-floatonly-reducedlocalcalcs

test-sync-onebyone-betterverlet-msgmgmt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-onebyone-betterverlet-msgmgmt-floatonly-singleforceloop: test-sync-onebyone-betterverlet-msgmgmt-floatonly

test-sync-onebyone-betterverlet-msgmgmt-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-sync-onebyone-betterverlet-msgmgmt-reducedlocalcalcs: test-sync-onebyone-betterverlet-msgmgmt

test-sync-onebyone-betterverlet-msgmgmt-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-onebyone-betterverlet-msgmgmt-reducedlocalcalcs-singleforceloop: test-sync-onebyone-betterverlet-msgmgmt-reducedlocalcalcs

test-sync-onebyone-betterverlet-msgmgmt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-onebyone-betterverlet-msgmgmt-singleforceloop: test-sync-onebyone-betterverlet-msgmgmt

test-sync-onebyone-betterverlet-floatonly: DFLAGS+=-DFLOAT_ONLY
test-sync-onebyone-betterverlet-floatonly: test-sync-onebyone-betterverlet

test-sync-onebyone-betterverlet-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-sync-onebyone-betterverlet-floatonly-reducedlocalcalcs: test-sync-onebyone-betterverlet-floatonly

test-sync-onebyone-betterverlet-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-onebyone-betterverlet-floatonly-reducedlocalcalcs-singleforceloop: test-sync-onebyone-betterverlet-floatonly-reducedlocalcalcs

test-sync-onebyone-betterverlet-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-onebyone-betterverlet-floatonly-singleforceloop: test-sync-onebyone-betterverlet-floatonly

test-sync-onebyone-betterverlet-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-sync-onebyone-betterverlet-reducedlocalcalcs: test-sync-onebyone-betterverlet

test-sync-onebyone-betterverlet-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-onebyone-betterverlet-reducedlocalcalcs-singleforceloop: test-sync-onebyone-betterverlet-reducedlocalcalcs

test-sync-onebyone-betterverlet-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-onebyone-betterverlet-singleforceloop: test-sync-onebyone-betterverlet

test-sync-onebyone-dtchange: DFLAGS+=-DSMALL_DT_EARLY
test-sync-onebyone-dtchange: test-sync-onebyone

test-sync-onebyone-dtchange-doublesqrt: DFLAGS+=-DOUBLE_SQRT
test-sync-onebyone-dtchange-doublesqrt: test-sync-onebyone-dtchange

test-sync-onebyone-dtchange-doublesqrt-msgmgmt: DFLAGS+=-DMESSAGE_MANAGEMENT
test-sync-onebyone-dtchange-doublesqrt-msgmgmt: test-sync-onebyone-dtchange-doublesqrt

test-sync-onebyone-dtchange-doublesqrt-msgmgmt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-sync-onebyone-dtchange-doublesqrt-msgmgmt-floatonly: test-sync-onebyone-dtchange-doublesqrt-msgmgmt

test-sync-onebyone-dtchange-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-sync-onebyone-dtchange-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs: test-sync-onebyone-dtchange-doublesqrt-msgmgmt-floatonly

test-sync-onebyone-dtchange-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-onebyone-dtchange-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs-singleforceloop: test-sync-onebyone-dtchange-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs

test-sync-onebyone-dtchange-doublesqrt-msgmgmt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-onebyone-dtchange-doublesqrt-msgmgmt-floatonly-singleforceloop: test-sync-onebyone-dtchange-doublesqrt-msgmgmt-floatonly

test-sync-onebyone-dtchange-doublesqrt-msgmgmt-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-sync-onebyone-dtchange-doublesqrt-msgmgmt-reducedlocalcalcs: test-sync-onebyone-dtchange-doublesqrt-msgmgmt

test-sync-onebyone-dtchange-doublesqrt-msgmgmt-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-onebyone-dtchange-doublesqrt-msgmgmt-reducedlocalcalcs-singleforceloop: test-sync-onebyone-dtchange-doublesqrt-msgmgmt-reducedlocalcalcs

test-sync-onebyone-dtchange-doublesqrt-msgmgmt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-onebyone-dtchange-doublesqrt-msgmgmt-singleforceloop: test-sync-onebyone-dtchange-doublesqrt-msgmgmt

test-sync-onebyone-dtchange-doublesqrt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-sync-onebyone-dtchange-doublesqrt-floatonly: test-sync-onebyone-dtchange-doublesqrt

test-sync-onebyone-dtchange-doublesqrt-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-sync-onebyone-dtchange-doublesqrt-floatonly-reducedlocalcalcs: test-sync-onebyone-dtchange-doublesqrt-floatonly

test-sync-onebyone-dtchange-doublesqrt-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-onebyone-dtchange-doublesqrt-floatonly-reducedlocalcalcs-singleforceloop: test-sync-onebyone-dtchange-doublesqrt-floatonly-reducedlocalcalcs

test-sync-onebyone-dtchange-doublesqrt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-onebyone-dtchange-doublesqrt-floatonly-singleforceloop: test-sync-onebyone-dtchange-doublesqrt-floatonly

test-sync-onebyone-dtchange-doublesqrt-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-sync-onebyone-dtchange-doublesqrt-reducedlocalcalcs: test-sync-onebyone-dtchange-doublesqrt

test-sync-onebyone-dtchange-doublesqrt-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-onebyone-dtchange-doublesqrt-reducedlocalcalcs-singleforceloop: test-sync-onebyone-dtchange-doublesqrt-reducedlocalcalcs

test-sync-onebyone-dtchange-doublesqrt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-onebyone-dtchange-doublesqrt-singleforceloop: test-sync-onebyone-dtchange-doublesqrt

test-sync-onebyone-dtchange-msgmgmt: DFLAGS+=-DMESSAGE_MANAGEMENT
test-sync-onebyone-dtchange-msgmgmt: test-sync-onebyone-dtchange

test-sync-onebyone-dtchange-msgmgmt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-sync-onebyone-dtchange-msgmgmt-floatonly: test-sync-onebyone-dtchange-msgmgmt

test-sync-onebyone-dtchange-msgmgmt-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-sync-onebyone-dtchange-msgmgmt-floatonly-reducedlocalcalcs: test-sync-onebyone-dtchange-msgmgmt-floatonly

test-sync-onebyone-dtchange-msgmgmt-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-onebyone-dtchange-msgmgmt-floatonly-reducedlocalcalcs-singleforceloop: test-sync-onebyone-dtchange-msgmgmt-floatonly-reducedlocalcalcs

test-sync-onebyone-dtchange-msgmgmt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-onebyone-dtchange-msgmgmt-floatonly-singleforceloop: test-sync-onebyone-dtchange-msgmgmt-floatonly

test-sync-onebyone-dtchange-msgmgmt-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-sync-onebyone-dtchange-msgmgmt-reducedlocalcalcs: test-sync-onebyone-dtchange-msgmgmt

test-sync-onebyone-dtchange-msgmgmt-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-onebyone-dtchange-msgmgmt-reducedlocalcalcs-singleforceloop: test-sync-onebyone-dtchange-msgmgmt-reducedlocalcalcs

test-sync-onebyone-dtchange-msgmgmt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-onebyone-dtchange-msgmgmt-singleforceloop: test-sync-onebyone-dtchange-msgmgmt

test-sync-onebyone-dtchange-floatonly: DFLAGS+=-DFLOAT_ONLY
test-sync-onebyone-dtchange-floatonly: test-sync-onebyone-dtchange

test-sync-onebyone-dtchange-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-sync-onebyone-dtchange-floatonly-reducedlocalcalcs: test-sync-onebyone-dtchange-floatonly

test-sync-onebyone-dtchange-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-onebyone-dtchange-floatonly-reducedlocalcalcs-singleforceloop: test-sync-onebyone-dtchange-floatonly-reducedlocalcalcs

test-sync-onebyone-dtchange-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-onebyone-dtchange-floatonly-singleforceloop: test-sync-onebyone-dtchange-floatonly

test-sync-onebyone-dtchange-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-sync-onebyone-dtchange-reducedlocalcalcs: test-sync-onebyone-dtchange

test-sync-onebyone-dtchange-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-onebyone-dtchange-reducedlocalcalcs-singleforceloop: test-sync-onebyone-dtchange-reducedlocalcalcs

test-sync-onebyone-dtchange-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-onebyone-dtchange-singleforceloop: test-sync-onebyone-dtchange

test-sync-onebyone-doublesqrt: DFLAGS+=-DOUBLE_SQRT
test-sync-onebyone-doublesqrt: test-sync-onebyone

test-sync-onebyone-doublesqrt-msgmgmt: DFLAGS+=-DMESSAGE_MANAGEMENT
test-sync-onebyone-doublesqrt-msgmgmt: test-sync-onebyone-doublesqrt

test-sync-onebyone-doublesqrt-msgmgmt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-sync-onebyone-doublesqrt-msgmgmt-floatonly: test-sync-onebyone-doublesqrt-msgmgmt

test-sync-onebyone-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-sync-onebyone-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs: test-sync-onebyone-doublesqrt-msgmgmt-floatonly

test-sync-onebyone-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-onebyone-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs-singleforceloop: test-sync-onebyone-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs

test-sync-onebyone-doublesqrt-msgmgmt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-onebyone-doublesqrt-msgmgmt-floatonly-singleforceloop: test-sync-onebyone-doublesqrt-msgmgmt-floatonly

test-sync-onebyone-doublesqrt-msgmgmt-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-sync-onebyone-doublesqrt-msgmgmt-reducedlocalcalcs: test-sync-onebyone-doublesqrt-msgmgmt

test-sync-onebyone-doublesqrt-msgmgmt-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-onebyone-doublesqrt-msgmgmt-reducedlocalcalcs-singleforceloop: test-sync-onebyone-doublesqrt-msgmgmt-reducedlocalcalcs

test-sync-onebyone-doublesqrt-msgmgmt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-onebyone-doublesqrt-msgmgmt-singleforceloop: test-sync-onebyone-doublesqrt-msgmgmt

test-sync-onebyone-doublesqrt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-sync-onebyone-doublesqrt-floatonly: test-sync-onebyone-doublesqrt

test-sync-onebyone-doublesqrt-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-sync-onebyone-doublesqrt-floatonly-reducedlocalcalcs: test-sync-onebyone-doublesqrt-floatonly

test-sync-onebyone-doublesqrt-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-onebyone-doublesqrt-floatonly-reducedlocalcalcs-singleforceloop: test-sync-onebyone-doublesqrt-floatonly-reducedlocalcalcs

test-sync-onebyone-doublesqrt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-onebyone-doublesqrt-floatonly-singleforceloop: test-sync-onebyone-doublesqrt-floatonly

test-sync-onebyone-doublesqrt-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-sync-onebyone-doublesqrt-reducedlocalcalcs: test-sync-onebyone-doublesqrt

test-sync-onebyone-doublesqrt-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-onebyone-doublesqrt-reducedlocalcalcs-singleforceloop: test-sync-onebyone-doublesqrt-reducedlocalcalcs

test-sync-onebyone-doublesqrt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-onebyone-doublesqrt-singleforceloop: test-sync-onebyone-doublesqrt

test-sync-onebyone-msgmgmt: DFLAGS+=-DMESSAGE_MANAGEMENT
test-sync-onebyone-msgmgmt: test-sync-onebyone

test-sync-onebyone-msgmgmt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-sync-onebyone-msgmgmt-floatonly: test-sync-onebyone-msgmgmt

test-sync-onebyone-msgmgmt-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-sync-onebyone-msgmgmt-floatonly-reducedlocalcalcs: test-sync-onebyone-msgmgmt-floatonly

test-sync-onebyone-msgmgmt-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-onebyone-msgmgmt-floatonly-reducedlocalcalcs-singleforceloop: test-sync-onebyone-msgmgmt-floatonly-reducedlocalcalcs

test-sync-onebyone-msgmgmt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-onebyone-msgmgmt-floatonly-singleforceloop: test-sync-onebyone-msgmgmt-floatonly

test-sync-onebyone-msgmgmt-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-sync-onebyone-msgmgmt-reducedlocalcalcs: test-sync-onebyone-msgmgmt

test-sync-onebyone-msgmgmt-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-onebyone-msgmgmt-reducedlocalcalcs-singleforceloop: test-sync-onebyone-msgmgmt-reducedlocalcalcs

test-sync-onebyone-msgmgmt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-onebyone-msgmgmt-singleforceloop: test-sync-onebyone-msgmgmt

test-sync-onebyone-floatonly: DFLAGS+=-DFLOAT_ONLY
test-sync-onebyone-floatonly: test-sync-onebyone

test-sync-onebyone-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-sync-onebyone-floatonly-reducedlocalcalcs: test-sync-onebyone-floatonly

test-sync-onebyone-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-onebyone-floatonly-reducedlocalcalcs-singleforceloop: test-sync-onebyone-floatonly-reducedlocalcalcs

test-sync-onebyone-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-onebyone-floatonly-singleforceloop: test-sync-onebyone-floatonly

test-sync-onebyone-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-sync-onebyone-reducedlocalcalcs: test-sync-onebyone

test-sync-onebyone-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-onebyone-reducedlocalcalcs-singleforceloop: test-sync-onebyone-reducedlocalcalcs

test-sync-onebyone-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-onebyone-singleforceloop: test-sync-onebyone

# **TEST Local calculation method: sendtoself**
test-sync-sendtoself: DFLAGS+=-DSEND_TO_SELF
test-sync-sendtoself: test-sync

test-sync-sendtoself-betterverlet: DFLAGS+=-DBETTER_VERLET
test-sync-sendtoself-betterverlet: test-sync-sendtoself

test-sync-sendtoself-betterverlet-dtchange: DFLAGS+=-DSMALL_DT_EARLY
test-sync-sendtoself-betterverlet-dtchange: test-sync-sendtoself-betterverlet

test-sync-sendtoself-betterverlet-dtchange-doublesqrt: DFLAGS+=-DOUBLE_SQRT
test-sync-sendtoself-betterverlet-dtchange-doublesqrt: test-sync-sendtoself-betterverlet-dtchange

test-sync-sendtoself-betterverlet-dtchange-doublesqrt-msgmgmt: DFLAGS+=-DMESSAGE_MANAGEMENT
test-sync-sendtoself-betterverlet-dtchange-doublesqrt-msgmgmt: test-sync-sendtoself-betterverlet-dtchange-doublesqrt

test-sync-sendtoself-betterverlet-dtchange-doublesqrt-msgmgmt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-sync-sendtoself-betterverlet-dtchange-doublesqrt-msgmgmt-floatonly: test-sync-sendtoself-betterverlet-dtchange-doublesqrt-msgmgmt

test-sync-sendtoself-betterverlet-dtchange-doublesqrt-msgmgmt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-sendtoself-betterverlet-dtchange-doublesqrt-msgmgmt-floatonly-singleforceloop: test-sync-sendtoself-betterverlet-dtchange-doublesqrt-msgmgmt-floatonly

test-sync-sendtoself-betterverlet-dtchange-doublesqrt-msgmgmt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-sendtoself-betterverlet-dtchange-doublesqrt-msgmgmt-singleforceloop: test-sync-sendtoself-betterverlet-dtchange-doublesqrt-msgmgmt

test-sync-sendtoself-betterverlet-dtchange-doublesqrt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-sync-sendtoself-betterverlet-dtchange-doublesqrt-floatonly: test-sync-sendtoself-betterverlet-dtchange-doublesqrt

test-sync-sendtoself-betterverlet-dtchange-doublesqrt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-sendtoself-betterverlet-dtchange-doublesqrt-floatonly-singleforceloop: test-sync-sendtoself-betterverlet-dtchange-doublesqrt-floatonly

test-sync-sendtoself-betterverlet-dtchange-doublesqrt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-sendtoself-betterverlet-dtchange-doublesqrt-singleforceloop: test-sync-sendtoself-betterverlet-dtchange-doublesqrt

test-sync-sendtoself-betterverlet-dtchange-msgmgmt: DFLAGS+=-DMESSAGE_MANAGEMENT
test-sync-sendtoself-betterverlet-dtchange-msgmgmt: test-sync-sendtoself-betterverlet-dtchange

test-sync-sendtoself-betterverlet-dtchange-msgmgmt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-sync-sendtoself-betterverlet-dtchange-msgmgmt-floatonly: test-sync-sendtoself-betterverlet-dtchange-msgmgmt

test-sync-sendtoself-betterverlet-dtchange-msgmgmt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-sendtoself-betterverlet-dtchange-msgmgmt-floatonly-singleforceloop: test-sync-sendtoself-betterverlet-dtchange-msgmgmt-floatonly

test-sync-sendtoself-betterverlet-dtchange-msgmgmt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-sendtoself-betterverlet-dtchange-msgmgmt-singleforceloop: test-sync-sendtoself-betterverlet-dtchange-msgmgmt

test-sync-sendtoself-betterverlet-dtchange-floatonly: DFLAGS+=-DFLOAT_ONLY
test-sync-sendtoself-betterverlet-dtchange-floatonly: test-sync-sendtoself-betterverlet-dtchange

test-sync-sendtoself-betterverlet-dtchange-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-sendtoself-betterverlet-dtchange-floatonly-singleforceloop: test-sync-sendtoself-betterverlet-dtchange-floatonly

test-sync-sendtoself-betterverlet-dtchange-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-sendtoself-betterverlet-dtchange-singleforceloop: test-sync-sendtoself-betterverlet-dtchange

test-sync-sendtoself-betterverlet-doublesqrt: DFLAGS+=-DOUBLE_SQRT
test-sync-sendtoself-betterverlet-doublesqrt: test-sync-sendtoself-betterverlet

test-sync-sendtoself-betterverlet-doublesqrt-msgmgmt: DFLAGS+=-DMESSAGE_MANAGEMENT
test-sync-sendtoself-betterverlet-doublesqrt-msgmgmt: test-sync-sendtoself-betterverlet-doublesqrt

test-sync-sendtoself-betterverlet-doublesqrt-msgmgmt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-sync-sendtoself-betterverlet-doublesqrt-msgmgmt-floatonly: test-sync-sendtoself-betterverlet-doublesqrt-msgmgmt

test-sync-sendtoself-betterverlet-doublesqrt-msgmgmt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-sendtoself-betterverlet-doublesqrt-msgmgmt-floatonly-singleforceloop: test-sync-sendtoself-betterverlet-doublesqrt-msgmgmt-floatonly

test-sync-sendtoself-betterverlet-doublesqrt-msgmgmt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-sendtoself-betterverlet-doublesqrt-msgmgmt-singleforceloop: test-sync-sendtoself-betterverlet-doublesqrt-msgmgmt

test-sync-sendtoself-betterverlet-doublesqrt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-sync-sendtoself-betterverlet-doublesqrt-floatonly: test-sync-sendtoself-betterverlet-doublesqrt

test-sync-sendtoself-betterverlet-doublesqrt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-sendtoself-betterverlet-doublesqrt-floatonly-singleforceloop: test-sync-sendtoself-betterverlet-doublesqrt-floatonly

test-sync-sendtoself-betterverlet-doublesqrt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-sendtoself-betterverlet-doublesqrt-singleforceloop: test-sync-sendtoself-betterverlet-doublesqrt

test-sync-sendtoself-betterverlet-msgmgmt: DFLAGS+=-DMESSAGE_MANAGEMENT
test-sync-sendtoself-betterverlet-msgmgmt: test-sync-sendtoself-betterverlet

test-sync-sendtoself-betterverlet-msgmgmt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-sync-sendtoself-betterverlet-msgmgmt-floatonly: test-sync-sendtoself-betterverlet-msgmgmt

test-sync-sendtoself-betterverlet-msgmgmt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-sendtoself-betterverlet-msgmgmt-floatonly-singleforceloop: test-sync-sendtoself-betterverlet-msgmgmt-floatonly

test-sync-sendtoself-betterverlet-msgmgmt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-sendtoself-betterverlet-msgmgmt-singleforceloop: test-sync-sendtoself-betterverlet-msgmgmt

test-sync-sendtoself-betterverlet-floatonly: DFLAGS+=-DFLOAT_ONLY
test-sync-sendtoself-betterverlet-floatonly: test-sync-sendtoself-betterverlet

test-sync-sendtoself-betterverlet-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-sendtoself-betterverlet-floatonly-singleforceloop: test-sync-sendtoself-betterverlet-floatonly

test-sync-sendtoself-betterverlet-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-sendtoself-betterverlet-singleforceloop: test-sync-sendtoself-betterverlet

test-sync-sendtoself-dtchange: DFLAGS+=-DSMALL_DT_EARLY
test-sync-sendtoself-dtchange: test-sync-sendtoself

test-sync-sendtoself-dtchange-doublesqrt: DFLAGS+=-DOUBLE_SQRT
test-sync-sendtoself-dtchange-doublesqrt: test-sync-sendtoself-dtchange

test-sync-sendtoself-dtchange-doublesqrt-msgmgmt: DFLAGS+=-DMESSAGE_MANAGEMENT
test-sync-sendtoself-dtchange-doublesqrt-msgmgmt: test-sync-sendtoself-dtchange-doublesqrt

test-sync-sendtoself-dtchange-doublesqrt-msgmgmt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-sync-sendtoself-dtchange-doublesqrt-msgmgmt-floatonly: test-sync-sendtoself-dtchange-doublesqrt-msgmgmt

test-sync-sendtoself-dtchange-doublesqrt-msgmgmt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-sendtoself-dtchange-doublesqrt-msgmgmt-floatonly-singleforceloop: test-sync-sendtoself-dtchange-doublesqrt-msgmgmt-floatonly

test-sync-sendtoself-dtchange-doublesqrt-msgmgmt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-sendtoself-dtchange-doublesqrt-msgmgmt-singleforceloop: test-sync-sendtoself-dtchange-doublesqrt-msgmgmt

test-sync-sendtoself-dtchange-doublesqrt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-sync-sendtoself-dtchange-doublesqrt-floatonly: test-sync-sendtoself-dtchange-doublesqrt

test-sync-sendtoself-dtchange-doublesqrt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-sendtoself-dtchange-doublesqrt-floatonly-singleforceloop: test-sync-sendtoself-dtchange-doublesqrt-floatonly

test-sync-sendtoself-dtchange-doublesqrt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-sendtoself-dtchange-doublesqrt-singleforceloop: test-sync-sendtoself-dtchange-doublesqrt

test-sync-sendtoself-dtchange-msgmgmt: DFLAGS+=-DMESSAGE_MANAGEMENT
test-sync-sendtoself-dtchange-msgmgmt: test-sync-sendtoself-dtchange

test-sync-sendtoself-dtchange-msgmgmt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-sync-sendtoself-dtchange-msgmgmt-floatonly: test-sync-sendtoself-dtchange-msgmgmt

test-sync-sendtoself-dtchange-msgmgmt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-sendtoself-dtchange-msgmgmt-floatonly-singleforceloop: test-sync-sendtoself-dtchange-msgmgmt-floatonly

test-sync-sendtoself-dtchange-msgmgmt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-sendtoself-dtchange-msgmgmt-singleforceloop: test-sync-sendtoself-dtchange-msgmgmt

test-sync-sendtoself-dtchange-floatonly: DFLAGS+=-DFLOAT_ONLY
test-sync-sendtoself-dtchange-floatonly: test-sync-sendtoself-dtchange

test-sync-sendtoself-dtchange-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-sendtoself-dtchange-floatonly-singleforceloop: test-sync-sendtoself-dtchange-floatonly

test-sync-sendtoself-dtchange-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-sendtoself-dtchange-singleforceloop: test-sync-sendtoself-dtchange

test-sync-sendtoself-doublesqrt: DFLAGS+=-DOUBLE_SQRT
test-sync-sendtoself-doublesqrt: test-sync-sendtoself

test-sync-sendtoself-doublesqrt-msgmgmt: DFLAGS+=-DMESSAGE_MANAGEMENT
test-sync-sendtoself-doublesqrt-msgmgmt: test-sync-sendtoself-doublesqrt

test-sync-sendtoself-doublesqrt-msgmgmt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-sync-sendtoself-doublesqrt-msgmgmt-floatonly: test-sync-sendtoself-doublesqrt-msgmgmt

test-sync-sendtoself-doublesqrt-msgmgmt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-sendtoself-doublesqrt-msgmgmt-floatonly-singleforceloop: test-sync-sendtoself-doublesqrt-msgmgmt-floatonly

test-sync-sendtoself-doublesqrt-msgmgmt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-sendtoself-doublesqrt-msgmgmt-singleforceloop: test-sync-sendtoself-doublesqrt-msgmgmt

test-sync-sendtoself-doublesqrt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-sync-sendtoself-doublesqrt-floatonly: test-sync-sendtoself-doublesqrt

test-sync-sendtoself-doublesqrt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-sendtoself-doublesqrt-floatonly-singleforceloop: test-sync-sendtoself-doublesqrt-floatonly

test-sync-sendtoself-doublesqrt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-sendtoself-doublesqrt-singleforceloop: test-sync-sendtoself-doublesqrt

test-sync-sendtoself-msgmgmt: DFLAGS+=-DMESSAGE_MANAGEMENT
test-sync-sendtoself-msgmgmt: test-sync-sendtoself

test-sync-sendtoself-msgmgmt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-sync-sendtoself-msgmgmt-floatonly: test-sync-sendtoself-msgmgmt

test-sync-sendtoself-msgmgmt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-sendtoself-msgmgmt-floatonly-singleforceloop: test-sync-sendtoself-msgmgmt-floatonly

test-sync-sendtoself-msgmgmt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-sendtoself-msgmgmt-singleforceloop: test-sync-sendtoself-msgmgmt

test-sync-sendtoself-floatonly: DFLAGS+=-DFLOAT_ONLY
test-sync-sendtoself-floatonly: test-sync-sendtoself

test-sync-sendtoself-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-sendtoself-floatonly-singleforceloop: test-sync-sendtoself-floatonly

test-sync-sendtoself-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-sendtoself-singleforceloop: test-sync-sendtoself

test-sync-large: DFLAGS+=-DLARGE_TEST
test-sync-large: test-sync

test-sync-large-betterverlet: DFLAGS+=-DBETTER_VERLET
test-sync-large-betterverlet: test-sync-large

test-sync-large-betterverlet-dtchange: DFLAGS+=-DSMALL_DT_EARLY
test-sync-large-betterverlet-dtchange: test-sync-large-betterverlet

test-sync-large-betterverlet-dtchange-doublesqrt: DFLAGS+=-DOUBLE_SQRT
test-sync-large-betterverlet-dtchange-doublesqrt: test-sync-large-betterverlet-dtchange

test-sync-large-betterverlet-dtchange-doublesqrt-msgmgmt: DFLAGS+=-DMESSAGE_MANAGEMENT
test-sync-large-betterverlet-dtchange-doublesqrt-msgmgmt: test-sync-large-betterverlet-dtchange-doublesqrt

test-sync-large-betterverlet-dtchange-doublesqrt-msgmgmt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-sync-large-betterverlet-dtchange-doublesqrt-msgmgmt-floatonly: test-sync-large-betterverlet-dtchange-doublesqrt-msgmgmt

test-sync-large-betterverlet-dtchange-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-sync-large-betterverlet-dtchange-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs: test-sync-large-betterverlet-dtchange-doublesqrt-msgmgmt-floatonly

test-sync-large-betterverlet-dtchange-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-large-betterverlet-dtchange-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs-singleforceloop: test-sync-large-betterverlet-dtchange-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs

test-sync-large-betterverlet-dtchange-doublesqrt-msgmgmt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-large-betterverlet-dtchange-doublesqrt-msgmgmt-floatonly-singleforceloop: test-sync-large-betterverlet-dtchange-doublesqrt-msgmgmt-floatonly

test-sync-large-betterverlet-dtchange-doublesqrt-msgmgmt-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-sync-large-betterverlet-dtchange-doublesqrt-msgmgmt-reducedlocalcalcs: test-sync-large-betterverlet-dtchange-doublesqrt-msgmgmt

test-sync-large-betterverlet-dtchange-doublesqrt-msgmgmt-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-large-betterverlet-dtchange-doublesqrt-msgmgmt-reducedlocalcalcs-singleforceloop: test-sync-large-betterverlet-dtchange-doublesqrt-msgmgmt-reducedlocalcalcs

test-sync-large-betterverlet-dtchange-doublesqrt-msgmgmt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-large-betterverlet-dtchange-doublesqrt-msgmgmt-singleforceloop: test-sync-large-betterverlet-dtchange-doublesqrt-msgmgmt

test-sync-large-betterverlet-dtchange-doublesqrt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-sync-large-betterverlet-dtchange-doublesqrt-floatonly: test-sync-large-betterverlet-dtchange-doublesqrt

test-sync-large-betterverlet-dtchange-doublesqrt-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-sync-large-betterverlet-dtchange-doublesqrt-floatonly-reducedlocalcalcs: test-sync-large-betterverlet-dtchange-doublesqrt-floatonly

test-sync-large-betterverlet-dtchange-doublesqrt-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-large-betterverlet-dtchange-doublesqrt-floatonly-reducedlocalcalcs-singleforceloop: test-sync-large-betterverlet-dtchange-doublesqrt-floatonly-reducedlocalcalcs

test-sync-large-betterverlet-dtchange-doublesqrt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-large-betterverlet-dtchange-doublesqrt-floatonly-singleforceloop: test-sync-large-betterverlet-dtchange-doublesqrt-floatonly

test-sync-large-betterverlet-dtchange-doublesqrt-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-sync-large-betterverlet-dtchange-doublesqrt-reducedlocalcalcs: test-sync-large-betterverlet-dtchange-doublesqrt

test-sync-large-betterverlet-dtchange-doublesqrt-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-large-betterverlet-dtchange-doublesqrt-reducedlocalcalcs-singleforceloop: test-sync-large-betterverlet-dtchange-doublesqrt-reducedlocalcalcs

test-sync-large-betterverlet-dtchange-doublesqrt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-large-betterverlet-dtchange-doublesqrt-singleforceloop: test-sync-large-betterverlet-dtchange-doublesqrt

test-sync-large-betterverlet-dtchange-msgmgmt: DFLAGS+=-DMESSAGE_MANAGEMENT
test-sync-large-betterverlet-dtchange-msgmgmt: test-sync-large-betterverlet-dtchange

test-sync-large-betterverlet-dtchange-msgmgmt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-sync-large-betterverlet-dtchange-msgmgmt-floatonly: test-sync-large-betterverlet-dtchange-msgmgmt

test-sync-large-betterverlet-dtchange-msgmgmt-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-sync-large-betterverlet-dtchange-msgmgmt-floatonly-reducedlocalcalcs: test-sync-large-betterverlet-dtchange-msgmgmt-floatonly

test-sync-large-betterverlet-dtchange-msgmgmt-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-large-betterverlet-dtchange-msgmgmt-floatonly-reducedlocalcalcs-singleforceloop: test-sync-large-betterverlet-dtchange-msgmgmt-floatonly-reducedlocalcalcs

test-sync-large-betterverlet-dtchange-msgmgmt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-large-betterverlet-dtchange-msgmgmt-floatonly-singleforceloop: test-sync-large-betterverlet-dtchange-msgmgmt-floatonly

test-sync-large-betterverlet-dtchange-msgmgmt-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-sync-large-betterverlet-dtchange-msgmgmt-reducedlocalcalcs: test-sync-large-betterverlet-dtchange-msgmgmt

test-sync-large-betterverlet-dtchange-msgmgmt-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-large-betterverlet-dtchange-msgmgmt-reducedlocalcalcs-singleforceloop: test-sync-large-betterverlet-dtchange-msgmgmt-reducedlocalcalcs

test-sync-large-betterverlet-dtchange-msgmgmt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-large-betterverlet-dtchange-msgmgmt-singleforceloop: test-sync-large-betterverlet-dtchange-msgmgmt

test-sync-large-betterverlet-dtchange-floatonly: DFLAGS+=-DFLOAT_ONLY
test-sync-large-betterverlet-dtchange-floatonly: test-sync-large-betterverlet-dtchange

test-sync-large-betterverlet-dtchange-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-sync-large-betterverlet-dtchange-floatonly-reducedlocalcalcs: test-sync-large-betterverlet-dtchange-floatonly

test-sync-large-betterverlet-dtchange-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-large-betterverlet-dtchange-floatonly-reducedlocalcalcs-singleforceloop: test-sync-large-betterverlet-dtchange-floatonly-reducedlocalcalcs

test-sync-large-betterverlet-dtchange-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-large-betterverlet-dtchange-floatonly-singleforceloop: test-sync-large-betterverlet-dtchange-floatonly

test-sync-large-betterverlet-dtchange-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-sync-large-betterverlet-dtchange-reducedlocalcalcs: test-sync-large-betterverlet-dtchange

test-sync-large-betterverlet-dtchange-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-large-betterverlet-dtchange-reducedlocalcalcs-singleforceloop: test-sync-large-betterverlet-dtchange-reducedlocalcalcs

test-sync-large-betterverlet-dtchange-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-large-betterverlet-dtchange-singleforceloop: test-sync-large-betterverlet-dtchange

test-sync-large-betterverlet-doublesqrt: DFLAGS+=-DOUBLE_SQRT
test-sync-large-betterverlet-doublesqrt: test-sync-large-betterverlet

test-sync-large-betterverlet-doublesqrt-msgmgmt: DFLAGS+=-DMESSAGE_MANAGEMENT
test-sync-large-betterverlet-doublesqrt-msgmgmt: test-sync-large-betterverlet-doublesqrt

test-sync-large-betterverlet-doublesqrt-msgmgmt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-sync-large-betterverlet-doublesqrt-msgmgmt-floatonly: test-sync-large-betterverlet-doublesqrt-msgmgmt

test-sync-large-betterverlet-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-sync-large-betterverlet-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs: test-sync-large-betterverlet-doublesqrt-msgmgmt-floatonly

test-sync-large-betterverlet-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-large-betterverlet-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs-singleforceloop: test-sync-large-betterverlet-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs

test-sync-large-betterverlet-doublesqrt-msgmgmt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-large-betterverlet-doublesqrt-msgmgmt-floatonly-singleforceloop: test-sync-large-betterverlet-doublesqrt-msgmgmt-floatonly

test-sync-large-betterverlet-doublesqrt-msgmgmt-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-sync-large-betterverlet-doublesqrt-msgmgmt-reducedlocalcalcs: test-sync-large-betterverlet-doublesqrt-msgmgmt

test-sync-large-betterverlet-doublesqrt-msgmgmt-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-large-betterverlet-doublesqrt-msgmgmt-reducedlocalcalcs-singleforceloop: test-sync-large-betterverlet-doublesqrt-msgmgmt-reducedlocalcalcs

test-sync-large-betterverlet-doublesqrt-msgmgmt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-large-betterverlet-doublesqrt-msgmgmt-singleforceloop: test-sync-large-betterverlet-doublesqrt-msgmgmt

test-sync-large-betterverlet-doublesqrt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-sync-large-betterverlet-doublesqrt-floatonly: test-sync-large-betterverlet-doublesqrt

test-sync-large-betterverlet-doublesqrt-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-sync-large-betterverlet-doublesqrt-floatonly-reducedlocalcalcs: test-sync-large-betterverlet-doublesqrt-floatonly

test-sync-large-betterverlet-doublesqrt-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-large-betterverlet-doublesqrt-floatonly-reducedlocalcalcs-singleforceloop: test-sync-large-betterverlet-doublesqrt-floatonly-reducedlocalcalcs

test-sync-large-betterverlet-doublesqrt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-large-betterverlet-doublesqrt-floatonly-singleforceloop: test-sync-large-betterverlet-doublesqrt-floatonly

test-sync-large-betterverlet-doublesqrt-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-sync-large-betterverlet-doublesqrt-reducedlocalcalcs: test-sync-large-betterverlet-doublesqrt

test-sync-large-betterverlet-doublesqrt-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-large-betterverlet-doublesqrt-reducedlocalcalcs-singleforceloop: test-sync-large-betterverlet-doublesqrt-reducedlocalcalcs

test-sync-large-betterverlet-doublesqrt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-large-betterverlet-doublesqrt-singleforceloop: test-sync-large-betterverlet-doublesqrt

test-sync-large-betterverlet-msgmgmt: DFLAGS+=-DMESSAGE_MANAGEMENT
test-sync-large-betterverlet-msgmgmt: test-sync-large-betterverlet

test-sync-large-betterverlet-msgmgmt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-sync-large-betterverlet-msgmgmt-floatonly: test-sync-large-betterverlet-msgmgmt

test-sync-large-betterverlet-msgmgmt-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-sync-large-betterverlet-msgmgmt-floatonly-reducedlocalcalcs: test-sync-large-betterverlet-msgmgmt-floatonly

test-sync-large-betterverlet-msgmgmt-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-large-betterverlet-msgmgmt-floatonly-reducedlocalcalcs-singleforceloop: test-sync-large-betterverlet-msgmgmt-floatonly-reducedlocalcalcs

test-sync-large-betterverlet-msgmgmt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-large-betterverlet-msgmgmt-floatonly-singleforceloop: test-sync-large-betterverlet-msgmgmt-floatonly

test-sync-large-betterverlet-msgmgmt-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-sync-large-betterverlet-msgmgmt-reducedlocalcalcs: test-sync-large-betterverlet-msgmgmt

test-sync-large-betterverlet-msgmgmt-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-large-betterverlet-msgmgmt-reducedlocalcalcs-singleforceloop: test-sync-large-betterverlet-msgmgmt-reducedlocalcalcs

test-sync-large-betterverlet-msgmgmt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-large-betterverlet-msgmgmt-singleforceloop: test-sync-large-betterverlet-msgmgmt

test-sync-large-betterverlet-floatonly: DFLAGS+=-DFLOAT_ONLY
test-sync-large-betterverlet-floatonly: test-sync-large-betterverlet

test-sync-large-betterverlet-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-sync-large-betterverlet-floatonly-reducedlocalcalcs: test-sync-large-betterverlet-floatonly

test-sync-large-betterverlet-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-large-betterverlet-floatonly-reducedlocalcalcs-singleforceloop: test-sync-large-betterverlet-floatonly-reducedlocalcalcs

test-sync-large-betterverlet-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-large-betterverlet-floatonly-singleforceloop: test-sync-large-betterverlet-floatonly

test-sync-large-betterverlet-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-sync-large-betterverlet-reducedlocalcalcs: test-sync-large-betterverlet

test-sync-large-betterverlet-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-large-betterverlet-reducedlocalcalcs-singleforceloop: test-sync-large-betterverlet-reducedlocalcalcs

test-sync-large-betterverlet-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-large-betterverlet-singleforceloop: test-sync-large-betterverlet

test-sync-large-dtchange: DFLAGS+=-DSMALL_DT_EARLY
test-sync-large-dtchange: test-sync-large

test-sync-large-dtchange-doublesqrt: DFLAGS+=-DOUBLE_SQRT
test-sync-large-dtchange-doublesqrt: test-sync-large-dtchange

test-sync-large-dtchange-doublesqrt-msgmgmt: DFLAGS+=-DMESSAGE_MANAGEMENT
test-sync-large-dtchange-doublesqrt-msgmgmt: test-sync-large-dtchange-doublesqrt

test-sync-large-dtchange-doublesqrt-msgmgmt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-sync-large-dtchange-doublesqrt-msgmgmt-floatonly: test-sync-large-dtchange-doublesqrt-msgmgmt

test-sync-large-dtchange-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-sync-large-dtchange-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs: test-sync-large-dtchange-doublesqrt-msgmgmt-floatonly

test-sync-large-dtchange-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-large-dtchange-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs-singleforceloop: test-sync-large-dtchange-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs

test-sync-large-dtchange-doublesqrt-msgmgmt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-large-dtchange-doublesqrt-msgmgmt-floatonly-singleforceloop: test-sync-large-dtchange-doublesqrt-msgmgmt-floatonly

test-sync-large-dtchange-doublesqrt-msgmgmt-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-sync-large-dtchange-doublesqrt-msgmgmt-reducedlocalcalcs: test-sync-large-dtchange-doublesqrt-msgmgmt

test-sync-large-dtchange-doublesqrt-msgmgmt-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-large-dtchange-doublesqrt-msgmgmt-reducedlocalcalcs-singleforceloop: test-sync-large-dtchange-doublesqrt-msgmgmt-reducedlocalcalcs

test-sync-large-dtchange-doublesqrt-msgmgmt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-large-dtchange-doublesqrt-msgmgmt-singleforceloop: test-sync-large-dtchange-doublesqrt-msgmgmt

test-sync-large-dtchange-doublesqrt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-sync-large-dtchange-doublesqrt-floatonly: test-sync-large-dtchange-doublesqrt

test-sync-large-dtchange-doublesqrt-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-sync-large-dtchange-doublesqrt-floatonly-reducedlocalcalcs: test-sync-large-dtchange-doublesqrt-floatonly

test-sync-large-dtchange-doublesqrt-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-large-dtchange-doublesqrt-floatonly-reducedlocalcalcs-singleforceloop: test-sync-large-dtchange-doublesqrt-floatonly-reducedlocalcalcs

test-sync-large-dtchange-doublesqrt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-large-dtchange-doublesqrt-floatonly-singleforceloop: test-sync-large-dtchange-doublesqrt-floatonly

test-sync-large-dtchange-doublesqrt-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-sync-large-dtchange-doublesqrt-reducedlocalcalcs: test-sync-large-dtchange-doublesqrt

test-sync-large-dtchange-doublesqrt-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-large-dtchange-doublesqrt-reducedlocalcalcs-singleforceloop: test-sync-large-dtchange-doublesqrt-reducedlocalcalcs

test-sync-large-dtchange-doublesqrt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-large-dtchange-doublesqrt-singleforceloop: test-sync-large-dtchange-doublesqrt

test-sync-large-dtchange-msgmgmt: DFLAGS+=-DMESSAGE_MANAGEMENT
test-sync-large-dtchange-msgmgmt: test-sync-large-dtchange

test-sync-large-dtchange-msgmgmt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-sync-large-dtchange-msgmgmt-floatonly: test-sync-large-dtchange-msgmgmt

test-sync-large-dtchange-msgmgmt-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-sync-large-dtchange-msgmgmt-floatonly-reducedlocalcalcs: test-sync-large-dtchange-msgmgmt-floatonly

test-sync-large-dtchange-msgmgmt-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-large-dtchange-msgmgmt-floatonly-reducedlocalcalcs-singleforceloop: test-sync-large-dtchange-msgmgmt-floatonly-reducedlocalcalcs

test-sync-large-dtchange-msgmgmt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-large-dtchange-msgmgmt-floatonly-singleforceloop: test-sync-large-dtchange-msgmgmt-floatonly

test-sync-large-dtchange-msgmgmt-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-sync-large-dtchange-msgmgmt-reducedlocalcalcs: test-sync-large-dtchange-msgmgmt

test-sync-large-dtchange-msgmgmt-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-large-dtchange-msgmgmt-reducedlocalcalcs-singleforceloop: test-sync-large-dtchange-msgmgmt-reducedlocalcalcs

test-sync-large-dtchange-msgmgmt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-large-dtchange-msgmgmt-singleforceloop: test-sync-large-dtchange-msgmgmt

test-sync-large-dtchange-floatonly: DFLAGS+=-DFLOAT_ONLY
test-sync-large-dtchange-floatonly: test-sync-large-dtchange

test-sync-large-dtchange-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-sync-large-dtchange-floatonly-reducedlocalcalcs: test-sync-large-dtchange-floatonly

test-sync-large-dtchange-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-large-dtchange-floatonly-reducedlocalcalcs-singleforceloop: test-sync-large-dtchange-floatonly-reducedlocalcalcs

test-sync-large-dtchange-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-large-dtchange-floatonly-singleforceloop: test-sync-large-dtchange-floatonly

test-sync-large-dtchange-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-sync-large-dtchange-reducedlocalcalcs: test-sync-large-dtchange

test-sync-large-dtchange-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-large-dtchange-reducedlocalcalcs-singleforceloop: test-sync-large-dtchange-reducedlocalcalcs

test-sync-large-dtchange-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-large-dtchange-singleforceloop: test-sync-large-dtchange

test-sync-large-doublesqrt: DFLAGS+=-DOUBLE_SQRT
test-sync-large-doublesqrt: test-sync-large

test-sync-large-doublesqrt-msgmgmt: DFLAGS+=-DMESSAGE_MANAGEMENT
test-sync-large-doublesqrt-msgmgmt: test-sync-large-doublesqrt

test-sync-large-doublesqrt-msgmgmt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-sync-large-doublesqrt-msgmgmt-floatonly: test-sync-large-doublesqrt-msgmgmt

test-sync-large-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-sync-large-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs: test-sync-large-doublesqrt-msgmgmt-floatonly

test-sync-large-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-large-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs-singleforceloop: test-sync-large-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs

test-sync-large-doublesqrt-msgmgmt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-large-doublesqrt-msgmgmt-floatonly-singleforceloop: test-sync-large-doublesqrt-msgmgmt-floatonly

test-sync-large-doublesqrt-msgmgmt-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-sync-large-doublesqrt-msgmgmt-reducedlocalcalcs: test-sync-large-doublesqrt-msgmgmt

test-sync-large-doublesqrt-msgmgmt-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-large-doublesqrt-msgmgmt-reducedlocalcalcs-singleforceloop: test-sync-large-doublesqrt-msgmgmt-reducedlocalcalcs

test-sync-large-doublesqrt-msgmgmt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-large-doublesqrt-msgmgmt-singleforceloop: test-sync-large-doublesqrt-msgmgmt

test-sync-large-doublesqrt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-sync-large-doublesqrt-floatonly: test-sync-large-doublesqrt

test-sync-large-doublesqrt-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-sync-large-doublesqrt-floatonly-reducedlocalcalcs: test-sync-large-doublesqrt-floatonly

test-sync-large-doublesqrt-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-large-doublesqrt-floatonly-reducedlocalcalcs-singleforceloop: test-sync-large-doublesqrt-floatonly-reducedlocalcalcs

test-sync-large-doublesqrt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-large-doublesqrt-floatonly-singleforceloop: test-sync-large-doublesqrt-floatonly

test-sync-large-doublesqrt-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-sync-large-doublesqrt-reducedlocalcalcs: test-sync-large-doublesqrt

test-sync-large-doublesqrt-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-large-doublesqrt-reducedlocalcalcs-singleforceloop: test-sync-large-doublesqrt-reducedlocalcalcs

test-sync-large-doublesqrt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-large-doublesqrt-singleforceloop: test-sync-large-doublesqrt

test-sync-large-msgmgmt: DFLAGS+=-DMESSAGE_MANAGEMENT
test-sync-large-msgmgmt: test-sync-large

test-sync-large-msgmgmt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-sync-large-msgmgmt-floatonly: test-sync-large-msgmgmt

test-sync-large-msgmgmt-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-sync-large-msgmgmt-floatonly-reducedlocalcalcs: test-sync-large-msgmgmt-floatonly

test-sync-large-msgmgmt-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-large-msgmgmt-floatonly-reducedlocalcalcs-singleforceloop: test-sync-large-msgmgmt-floatonly-reducedlocalcalcs

test-sync-large-msgmgmt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-large-msgmgmt-floatonly-singleforceloop: test-sync-large-msgmgmt-floatonly

test-sync-large-msgmgmt-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-sync-large-msgmgmt-reducedlocalcalcs: test-sync-large-msgmgmt

test-sync-large-msgmgmt-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-large-msgmgmt-reducedlocalcalcs-singleforceloop: test-sync-large-msgmgmt-reducedlocalcalcs

test-sync-large-msgmgmt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-large-msgmgmt-singleforceloop: test-sync-large-msgmgmt

test-sync-large-floatonly: DFLAGS+=-DFLOAT_ONLY
test-sync-large-floatonly: test-sync-large

test-sync-large-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-sync-large-floatonly-reducedlocalcalcs: test-sync-large-floatonly

test-sync-large-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-large-floatonly-reducedlocalcalcs-singleforceloop: test-sync-large-floatonly-reducedlocalcalcs

test-sync-large-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-large-floatonly-singleforceloop: test-sync-large-floatonly

test-sync-large-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-sync-large-reducedlocalcalcs: test-sync-large

test-sync-large-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-large-reducedlocalcalcs-singleforceloop: test-sync-large-reducedlocalcalcs

test-sync-large-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-large-singleforceloop: test-sync-large

# **TEST Local calculation method: onebyone**
test-sync-large-onebyone: DFLAGS+=-DONE_BY_ONE
test-sync-large-onebyone: test-sync-large

test-sync-large-onebyone-betterverlet: DFLAGS+=-DBETTER_VERLET
test-sync-large-onebyone-betterverlet: test-sync-large-onebyone

test-sync-large-onebyone-betterverlet-dtchange: DFLAGS+=-DSMALL_DT_EARLY
test-sync-large-onebyone-betterverlet-dtchange: test-sync-large-onebyone-betterverlet

test-sync-large-onebyone-betterverlet-dtchange-doublesqrt: DFLAGS+=-DOUBLE_SQRT
test-sync-large-onebyone-betterverlet-dtchange-doublesqrt: test-sync-large-onebyone-betterverlet-dtchange

test-sync-large-onebyone-betterverlet-dtchange-doublesqrt-msgmgmt: DFLAGS+=-DMESSAGE_MANAGEMENT
test-sync-large-onebyone-betterverlet-dtchange-doublesqrt-msgmgmt: test-sync-large-onebyone-betterverlet-dtchange-doublesqrt

test-sync-large-onebyone-betterverlet-dtchange-doublesqrt-msgmgmt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-sync-large-onebyone-betterverlet-dtchange-doublesqrt-msgmgmt-floatonly: test-sync-large-onebyone-betterverlet-dtchange-doublesqrt-msgmgmt

test-sync-large-onebyone-betterverlet-dtchange-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-sync-large-onebyone-betterverlet-dtchange-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs: test-sync-large-onebyone-betterverlet-dtchange-doublesqrt-msgmgmt-floatonly

test-sync-large-onebyone-betterverlet-dtchange-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-large-onebyone-betterverlet-dtchange-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs-singleforceloop: test-sync-large-onebyone-betterverlet-dtchange-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs

test-sync-large-onebyone-betterverlet-dtchange-doublesqrt-msgmgmt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-large-onebyone-betterverlet-dtchange-doublesqrt-msgmgmt-floatonly-singleforceloop: test-sync-large-onebyone-betterverlet-dtchange-doublesqrt-msgmgmt-floatonly

test-sync-large-onebyone-betterverlet-dtchange-doublesqrt-msgmgmt-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-sync-large-onebyone-betterverlet-dtchange-doublesqrt-msgmgmt-reducedlocalcalcs: test-sync-large-onebyone-betterverlet-dtchange-doublesqrt-msgmgmt

test-sync-large-onebyone-betterverlet-dtchange-doublesqrt-msgmgmt-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-large-onebyone-betterverlet-dtchange-doublesqrt-msgmgmt-reducedlocalcalcs-singleforceloop: test-sync-large-onebyone-betterverlet-dtchange-doublesqrt-msgmgmt-reducedlocalcalcs

test-sync-large-onebyone-betterverlet-dtchange-doublesqrt-msgmgmt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-large-onebyone-betterverlet-dtchange-doublesqrt-msgmgmt-singleforceloop: test-sync-large-onebyone-betterverlet-dtchange-doublesqrt-msgmgmt

test-sync-large-onebyone-betterverlet-dtchange-doublesqrt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-sync-large-onebyone-betterverlet-dtchange-doublesqrt-floatonly: test-sync-large-onebyone-betterverlet-dtchange-doublesqrt

test-sync-large-onebyone-betterverlet-dtchange-doublesqrt-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-sync-large-onebyone-betterverlet-dtchange-doublesqrt-floatonly-reducedlocalcalcs: test-sync-large-onebyone-betterverlet-dtchange-doublesqrt-floatonly

test-sync-large-onebyone-betterverlet-dtchange-doublesqrt-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-large-onebyone-betterverlet-dtchange-doublesqrt-floatonly-reducedlocalcalcs-singleforceloop: test-sync-large-onebyone-betterverlet-dtchange-doublesqrt-floatonly-reducedlocalcalcs

test-sync-large-onebyone-betterverlet-dtchange-doublesqrt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-large-onebyone-betterverlet-dtchange-doublesqrt-floatonly-singleforceloop: test-sync-large-onebyone-betterverlet-dtchange-doublesqrt-floatonly

test-sync-large-onebyone-betterverlet-dtchange-doublesqrt-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-sync-large-onebyone-betterverlet-dtchange-doublesqrt-reducedlocalcalcs: test-sync-large-onebyone-betterverlet-dtchange-doublesqrt

test-sync-large-onebyone-betterverlet-dtchange-doublesqrt-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-large-onebyone-betterverlet-dtchange-doublesqrt-reducedlocalcalcs-singleforceloop: test-sync-large-onebyone-betterverlet-dtchange-doublesqrt-reducedlocalcalcs

test-sync-large-onebyone-betterverlet-dtchange-doublesqrt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-large-onebyone-betterverlet-dtchange-doublesqrt-singleforceloop: test-sync-large-onebyone-betterverlet-dtchange-doublesqrt

test-sync-large-onebyone-betterverlet-dtchange-msgmgmt: DFLAGS+=-DMESSAGE_MANAGEMENT
test-sync-large-onebyone-betterverlet-dtchange-msgmgmt: test-sync-large-onebyone-betterverlet-dtchange

test-sync-large-onebyone-betterverlet-dtchange-msgmgmt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-sync-large-onebyone-betterverlet-dtchange-msgmgmt-floatonly: test-sync-large-onebyone-betterverlet-dtchange-msgmgmt

test-sync-large-onebyone-betterverlet-dtchange-msgmgmt-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-sync-large-onebyone-betterverlet-dtchange-msgmgmt-floatonly-reducedlocalcalcs: test-sync-large-onebyone-betterverlet-dtchange-msgmgmt-floatonly

test-sync-large-onebyone-betterverlet-dtchange-msgmgmt-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-large-onebyone-betterverlet-dtchange-msgmgmt-floatonly-reducedlocalcalcs-singleforceloop: test-sync-large-onebyone-betterverlet-dtchange-msgmgmt-floatonly-reducedlocalcalcs

test-sync-large-onebyone-betterverlet-dtchange-msgmgmt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-large-onebyone-betterverlet-dtchange-msgmgmt-floatonly-singleforceloop: test-sync-large-onebyone-betterverlet-dtchange-msgmgmt-floatonly

test-sync-large-onebyone-betterverlet-dtchange-msgmgmt-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-sync-large-onebyone-betterverlet-dtchange-msgmgmt-reducedlocalcalcs: test-sync-large-onebyone-betterverlet-dtchange-msgmgmt

test-sync-large-onebyone-betterverlet-dtchange-msgmgmt-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-large-onebyone-betterverlet-dtchange-msgmgmt-reducedlocalcalcs-singleforceloop: test-sync-large-onebyone-betterverlet-dtchange-msgmgmt-reducedlocalcalcs

test-sync-large-onebyone-betterverlet-dtchange-msgmgmt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-large-onebyone-betterverlet-dtchange-msgmgmt-singleforceloop: test-sync-large-onebyone-betterverlet-dtchange-msgmgmt

test-sync-large-onebyone-betterverlet-dtchange-floatonly: DFLAGS+=-DFLOAT_ONLY
test-sync-large-onebyone-betterverlet-dtchange-floatonly: test-sync-large-onebyone-betterverlet-dtchange

test-sync-large-onebyone-betterverlet-dtchange-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-sync-large-onebyone-betterverlet-dtchange-floatonly-reducedlocalcalcs: test-sync-large-onebyone-betterverlet-dtchange-floatonly

test-sync-large-onebyone-betterverlet-dtchange-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-large-onebyone-betterverlet-dtchange-floatonly-reducedlocalcalcs-singleforceloop: test-sync-large-onebyone-betterverlet-dtchange-floatonly-reducedlocalcalcs

test-sync-large-onebyone-betterverlet-dtchange-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-large-onebyone-betterverlet-dtchange-floatonly-singleforceloop: test-sync-large-onebyone-betterverlet-dtchange-floatonly

test-sync-large-onebyone-betterverlet-dtchange-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-sync-large-onebyone-betterverlet-dtchange-reducedlocalcalcs: test-sync-large-onebyone-betterverlet-dtchange

test-sync-large-onebyone-betterverlet-dtchange-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-large-onebyone-betterverlet-dtchange-reducedlocalcalcs-singleforceloop: test-sync-large-onebyone-betterverlet-dtchange-reducedlocalcalcs

test-sync-large-onebyone-betterverlet-dtchange-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-large-onebyone-betterverlet-dtchange-singleforceloop: test-sync-large-onebyone-betterverlet-dtchange

test-sync-large-onebyone-betterverlet-doublesqrt: DFLAGS+=-DOUBLE_SQRT
test-sync-large-onebyone-betterverlet-doublesqrt: test-sync-large-onebyone-betterverlet

test-sync-large-onebyone-betterverlet-doublesqrt-msgmgmt: DFLAGS+=-DMESSAGE_MANAGEMENT
test-sync-large-onebyone-betterverlet-doublesqrt-msgmgmt: test-sync-large-onebyone-betterverlet-doublesqrt

test-sync-large-onebyone-betterverlet-doublesqrt-msgmgmt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-sync-large-onebyone-betterverlet-doublesqrt-msgmgmt-floatonly: test-sync-large-onebyone-betterverlet-doublesqrt-msgmgmt

test-sync-large-onebyone-betterverlet-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-sync-large-onebyone-betterverlet-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs: test-sync-large-onebyone-betterverlet-doublesqrt-msgmgmt-floatonly

test-sync-large-onebyone-betterverlet-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-large-onebyone-betterverlet-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs-singleforceloop: test-sync-large-onebyone-betterverlet-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs

test-sync-large-onebyone-betterverlet-doublesqrt-msgmgmt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-large-onebyone-betterverlet-doublesqrt-msgmgmt-floatonly-singleforceloop: test-sync-large-onebyone-betterverlet-doublesqrt-msgmgmt-floatonly

test-sync-large-onebyone-betterverlet-doublesqrt-msgmgmt-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-sync-large-onebyone-betterverlet-doublesqrt-msgmgmt-reducedlocalcalcs: test-sync-large-onebyone-betterverlet-doublesqrt-msgmgmt

test-sync-large-onebyone-betterverlet-doublesqrt-msgmgmt-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-large-onebyone-betterverlet-doublesqrt-msgmgmt-reducedlocalcalcs-singleforceloop: test-sync-large-onebyone-betterverlet-doublesqrt-msgmgmt-reducedlocalcalcs

test-sync-large-onebyone-betterverlet-doublesqrt-msgmgmt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-large-onebyone-betterverlet-doublesqrt-msgmgmt-singleforceloop: test-sync-large-onebyone-betterverlet-doublesqrt-msgmgmt

test-sync-large-onebyone-betterverlet-doublesqrt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-sync-large-onebyone-betterverlet-doublesqrt-floatonly: test-sync-large-onebyone-betterverlet-doublesqrt

test-sync-large-onebyone-betterverlet-doublesqrt-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-sync-large-onebyone-betterverlet-doublesqrt-floatonly-reducedlocalcalcs: test-sync-large-onebyone-betterverlet-doublesqrt-floatonly

test-sync-large-onebyone-betterverlet-doublesqrt-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-large-onebyone-betterverlet-doublesqrt-floatonly-reducedlocalcalcs-singleforceloop: test-sync-large-onebyone-betterverlet-doublesqrt-floatonly-reducedlocalcalcs

test-sync-large-onebyone-betterverlet-doublesqrt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-large-onebyone-betterverlet-doublesqrt-floatonly-singleforceloop: test-sync-large-onebyone-betterverlet-doublesqrt-floatonly

test-sync-large-onebyone-betterverlet-doublesqrt-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-sync-large-onebyone-betterverlet-doublesqrt-reducedlocalcalcs: test-sync-large-onebyone-betterverlet-doublesqrt

test-sync-large-onebyone-betterverlet-doublesqrt-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-large-onebyone-betterverlet-doublesqrt-reducedlocalcalcs-singleforceloop: test-sync-large-onebyone-betterverlet-doublesqrt-reducedlocalcalcs

test-sync-large-onebyone-betterverlet-doublesqrt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-large-onebyone-betterverlet-doublesqrt-singleforceloop: test-sync-large-onebyone-betterverlet-doublesqrt

test-sync-large-onebyone-betterverlet-msgmgmt: DFLAGS+=-DMESSAGE_MANAGEMENT
test-sync-large-onebyone-betterverlet-msgmgmt: test-sync-large-onebyone-betterverlet

test-sync-large-onebyone-betterverlet-msgmgmt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-sync-large-onebyone-betterverlet-msgmgmt-floatonly: test-sync-large-onebyone-betterverlet-msgmgmt

test-sync-large-onebyone-betterverlet-msgmgmt-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-sync-large-onebyone-betterverlet-msgmgmt-floatonly-reducedlocalcalcs: test-sync-large-onebyone-betterverlet-msgmgmt-floatonly

test-sync-large-onebyone-betterverlet-msgmgmt-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-large-onebyone-betterverlet-msgmgmt-floatonly-reducedlocalcalcs-singleforceloop: test-sync-large-onebyone-betterverlet-msgmgmt-floatonly-reducedlocalcalcs

test-sync-large-onebyone-betterverlet-msgmgmt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-large-onebyone-betterverlet-msgmgmt-floatonly-singleforceloop: test-sync-large-onebyone-betterverlet-msgmgmt-floatonly

test-sync-large-onebyone-betterverlet-msgmgmt-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-sync-large-onebyone-betterverlet-msgmgmt-reducedlocalcalcs: test-sync-large-onebyone-betterverlet-msgmgmt

test-sync-large-onebyone-betterverlet-msgmgmt-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-large-onebyone-betterverlet-msgmgmt-reducedlocalcalcs-singleforceloop: test-sync-large-onebyone-betterverlet-msgmgmt-reducedlocalcalcs

test-sync-large-onebyone-betterverlet-msgmgmt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-large-onebyone-betterverlet-msgmgmt-singleforceloop: test-sync-large-onebyone-betterverlet-msgmgmt

test-sync-large-onebyone-betterverlet-floatonly: DFLAGS+=-DFLOAT_ONLY
test-sync-large-onebyone-betterverlet-floatonly: test-sync-large-onebyone-betterverlet

test-sync-large-onebyone-betterverlet-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-sync-large-onebyone-betterverlet-floatonly-reducedlocalcalcs: test-sync-large-onebyone-betterverlet-floatonly

test-sync-large-onebyone-betterverlet-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-large-onebyone-betterverlet-floatonly-reducedlocalcalcs-singleforceloop: test-sync-large-onebyone-betterverlet-floatonly-reducedlocalcalcs

test-sync-large-onebyone-betterverlet-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-large-onebyone-betterverlet-floatonly-singleforceloop: test-sync-large-onebyone-betterverlet-floatonly

test-sync-large-onebyone-betterverlet-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-sync-large-onebyone-betterverlet-reducedlocalcalcs: test-sync-large-onebyone-betterverlet

test-sync-large-onebyone-betterverlet-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-large-onebyone-betterverlet-reducedlocalcalcs-singleforceloop: test-sync-large-onebyone-betterverlet-reducedlocalcalcs

test-sync-large-onebyone-betterverlet-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-large-onebyone-betterverlet-singleforceloop: test-sync-large-onebyone-betterverlet

test-sync-large-onebyone-dtchange: DFLAGS+=-DSMALL_DT_EARLY
test-sync-large-onebyone-dtchange: test-sync-large-onebyone

test-sync-large-onebyone-dtchange-doublesqrt: DFLAGS+=-DOUBLE_SQRT
test-sync-large-onebyone-dtchange-doublesqrt: test-sync-large-onebyone-dtchange

test-sync-large-onebyone-dtchange-doublesqrt-msgmgmt: DFLAGS+=-DMESSAGE_MANAGEMENT
test-sync-large-onebyone-dtchange-doublesqrt-msgmgmt: test-sync-large-onebyone-dtchange-doublesqrt

test-sync-large-onebyone-dtchange-doublesqrt-msgmgmt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-sync-large-onebyone-dtchange-doublesqrt-msgmgmt-floatonly: test-sync-large-onebyone-dtchange-doublesqrt-msgmgmt

test-sync-large-onebyone-dtchange-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-sync-large-onebyone-dtchange-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs: test-sync-large-onebyone-dtchange-doublesqrt-msgmgmt-floatonly

test-sync-large-onebyone-dtchange-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-large-onebyone-dtchange-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs-singleforceloop: test-sync-large-onebyone-dtchange-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs

test-sync-large-onebyone-dtchange-doublesqrt-msgmgmt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-large-onebyone-dtchange-doublesqrt-msgmgmt-floatonly-singleforceloop: test-sync-large-onebyone-dtchange-doublesqrt-msgmgmt-floatonly

test-sync-large-onebyone-dtchange-doublesqrt-msgmgmt-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-sync-large-onebyone-dtchange-doublesqrt-msgmgmt-reducedlocalcalcs: test-sync-large-onebyone-dtchange-doublesqrt-msgmgmt

test-sync-large-onebyone-dtchange-doublesqrt-msgmgmt-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-large-onebyone-dtchange-doublesqrt-msgmgmt-reducedlocalcalcs-singleforceloop: test-sync-large-onebyone-dtchange-doublesqrt-msgmgmt-reducedlocalcalcs

test-sync-large-onebyone-dtchange-doublesqrt-msgmgmt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-large-onebyone-dtchange-doublesqrt-msgmgmt-singleforceloop: test-sync-large-onebyone-dtchange-doublesqrt-msgmgmt

test-sync-large-onebyone-dtchange-doublesqrt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-sync-large-onebyone-dtchange-doublesqrt-floatonly: test-sync-large-onebyone-dtchange-doublesqrt

test-sync-large-onebyone-dtchange-doublesqrt-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-sync-large-onebyone-dtchange-doublesqrt-floatonly-reducedlocalcalcs: test-sync-large-onebyone-dtchange-doublesqrt-floatonly

test-sync-large-onebyone-dtchange-doublesqrt-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-large-onebyone-dtchange-doublesqrt-floatonly-reducedlocalcalcs-singleforceloop: test-sync-large-onebyone-dtchange-doublesqrt-floatonly-reducedlocalcalcs

test-sync-large-onebyone-dtchange-doublesqrt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-large-onebyone-dtchange-doublesqrt-floatonly-singleforceloop: test-sync-large-onebyone-dtchange-doublesqrt-floatonly

test-sync-large-onebyone-dtchange-doublesqrt-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-sync-large-onebyone-dtchange-doublesqrt-reducedlocalcalcs: test-sync-large-onebyone-dtchange-doublesqrt

test-sync-large-onebyone-dtchange-doublesqrt-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-large-onebyone-dtchange-doublesqrt-reducedlocalcalcs-singleforceloop: test-sync-large-onebyone-dtchange-doublesqrt-reducedlocalcalcs

test-sync-large-onebyone-dtchange-doublesqrt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-large-onebyone-dtchange-doublesqrt-singleforceloop: test-sync-large-onebyone-dtchange-doublesqrt

test-sync-large-onebyone-dtchange-msgmgmt: DFLAGS+=-DMESSAGE_MANAGEMENT
test-sync-large-onebyone-dtchange-msgmgmt: test-sync-large-onebyone-dtchange

test-sync-large-onebyone-dtchange-msgmgmt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-sync-large-onebyone-dtchange-msgmgmt-floatonly: test-sync-large-onebyone-dtchange-msgmgmt

test-sync-large-onebyone-dtchange-msgmgmt-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-sync-large-onebyone-dtchange-msgmgmt-floatonly-reducedlocalcalcs: test-sync-large-onebyone-dtchange-msgmgmt-floatonly

test-sync-large-onebyone-dtchange-msgmgmt-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-large-onebyone-dtchange-msgmgmt-floatonly-reducedlocalcalcs-singleforceloop: test-sync-large-onebyone-dtchange-msgmgmt-floatonly-reducedlocalcalcs

test-sync-large-onebyone-dtchange-msgmgmt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-large-onebyone-dtchange-msgmgmt-floatonly-singleforceloop: test-sync-large-onebyone-dtchange-msgmgmt-floatonly

test-sync-large-onebyone-dtchange-msgmgmt-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-sync-large-onebyone-dtchange-msgmgmt-reducedlocalcalcs: test-sync-large-onebyone-dtchange-msgmgmt

test-sync-large-onebyone-dtchange-msgmgmt-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-large-onebyone-dtchange-msgmgmt-reducedlocalcalcs-singleforceloop: test-sync-large-onebyone-dtchange-msgmgmt-reducedlocalcalcs

test-sync-large-onebyone-dtchange-msgmgmt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-large-onebyone-dtchange-msgmgmt-singleforceloop: test-sync-large-onebyone-dtchange-msgmgmt

test-sync-large-onebyone-dtchange-floatonly: DFLAGS+=-DFLOAT_ONLY
test-sync-large-onebyone-dtchange-floatonly: test-sync-large-onebyone-dtchange

test-sync-large-onebyone-dtchange-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-sync-large-onebyone-dtchange-floatonly-reducedlocalcalcs: test-sync-large-onebyone-dtchange-floatonly

test-sync-large-onebyone-dtchange-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-large-onebyone-dtchange-floatonly-reducedlocalcalcs-singleforceloop: test-sync-large-onebyone-dtchange-floatonly-reducedlocalcalcs

test-sync-large-onebyone-dtchange-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-large-onebyone-dtchange-floatonly-singleforceloop: test-sync-large-onebyone-dtchange-floatonly

test-sync-large-onebyone-dtchange-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-sync-large-onebyone-dtchange-reducedlocalcalcs: test-sync-large-onebyone-dtchange

test-sync-large-onebyone-dtchange-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-large-onebyone-dtchange-reducedlocalcalcs-singleforceloop: test-sync-large-onebyone-dtchange-reducedlocalcalcs

test-sync-large-onebyone-dtchange-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-large-onebyone-dtchange-singleforceloop: test-sync-large-onebyone-dtchange

test-sync-large-onebyone-doublesqrt: DFLAGS+=-DOUBLE_SQRT
test-sync-large-onebyone-doublesqrt: test-sync-large-onebyone

test-sync-large-onebyone-doublesqrt-msgmgmt: DFLAGS+=-DMESSAGE_MANAGEMENT
test-sync-large-onebyone-doublesqrt-msgmgmt: test-sync-large-onebyone-doublesqrt

test-sync-large-onebyone-doublesqrt-msgmgmt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-sync-large-onebyone-doublesqrt-msgmgmt-floatonly: test-sync-large-onebyone-doublesqrt-msgmgmt

test-sync-large-onebyone-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-sync-large-onebyone-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs: test-sync-large-onebyone-doublesqrt-msgmgmt-floatonly

test-sync-large-onebyone-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-large-onebyone-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs-singleforceloop: test-sync-large-onebyone-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs

test-sync-large-onebyone-doublesqrt-msgmgmt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-large-onebyone-doublesqrt-msgmgmt-floatonly-singleforceloop: test-sync-large-onebyone-doublesqrt-msgmgmt-floatonly

test-sync-large-onebyone-doublesqrt-msgmgmt-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-sync-large-onebyone-doublesqrt-msgmgmt-reducedlocalcalcs: test-sync-large-onebyone-doublesqrt-msgmgmt

test-sync-large-onebyone-doublesqrt-msgmgmt-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-large-onebyone-doublesqrt-msgmgmt-reducedlocalcalcs-singleforceloop: test-sync-large-onebyone-doublesqrt-msgmgmt-reducedlocalcalcs

test-sync-large-onebyone-doublesqrt-msgmgmt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-large-onebyone-doublesqrt-msgmgmt-singleforceloop: test-sync-large-onebyone-doublesqrt-msgmgmt

test-sync-large-onebyone-doublesqrt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-sync-large-onebyone-doublesqrt-floatonly: test-sync-large-onebyone-doublesqrt

test-sync-large-onebyone-doublesqrt-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-sync-large-onebyone-doublesqrt-floatonly-reducedlocalcalcs: test-sync-large-onebyone-doublesqrt-floatonly

test-sync-large-onebyone-doublesqrt-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-large-onebyone-doublesqrt-floatonly-reducedlocalcalcs-singleforceloop: test-sync-large-onebyone-doublesqrt-floatonly-reducedlocalcalcs

test-sync-large-onebyone-doublesqrt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-large-onebyone-doublesqrt-floatonly-singleforceloop: test-sync-large-onebyone-doublesqrt-floatonly

test-sync-large-onebyone-doublesqrt-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-sync-large-onebyone-doublesqrt-reducedlocalcalcs: test-sync-large-onebyone-doublesqrt

test-sync-large-onebyone-doublesqrt-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-large-onebyone-doublesqrt-reducedlocalcalcs-singleforceloop: test-sync-large-onebyone-doublesqrt-reducedlocalcalcs

test-sync-large-onebyone-doublesqrt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-large-onebyone-doublesqrt-singleforceloop: test-sync-large-onebyone-doublesqrt

test-sync-large-onebyone-msgmgmt: DFLAGS+=-DMESSAGE_MANAGEMENT
test-sync-large-onebyone-msgmgmt: test-sync-large-onebyone

test-sync-large-onebyone-msgmgmt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-sync-large-onebyone-msgmgmt-floatonly: test-sync-large-onebyone-msgmgmt

test-sync-large-onebyone-msgmgmt-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-sync-large-onebyone-msgmgmt-floatonly-reducedlocalcalcs: test-sync-large-onebyone-msgmgmt-floatonly

test-sync-large-onebyone-msgmgmt-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-large-onebyone-msgmgmt-floatonly-reducedlocalcalcs-singleforceloop: test-sync-large-onebyone-msgmgmt-floatonly-reducedlocalcalcs

test-sync-large-onebyone-msgmgmt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-large-onebyone-msgmgmt-floatonly-singleforceloop: test-sync-large-onebyone-msgmgmt-floatonly

test-sync-large-onebyone-msgmgmt-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-sync-large-onebyone-msgmgmt-reducedlocalcalcs: test-sync-large-onebyone-msgmgmt

test-sync-large-onebyone-msgmgmt-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-large-onebyone-msgmgmt-reducedlocalcalcs-singleforceloop: test-sync-large-onebyone-msgmgmt-reducedlocalcalcs

test-sync-large-onebyone-msgmgmt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-large-onebyone-msgmgmt-singleforceloop: test-sync-large-onebyone-msgmgmt

test-sync-large-onebyone-floatonly: DFLAGS+=-DFLOAT_ONLY
test-sync-large-onebyone-floatonly: test-sync-large-onebyone

test-sync-large-onebyone-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-sync-large-onebyone-floatonly-reducedlocalcalcs: test-sync-large-onebyone-floatonly

test-sync-large-onebyone-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-large-onebyone-floatonly-reducedlocalcalcs-singleforceloop: test-sync-large-onebyone-floatonly-reducedlocalcalcs

test-sync-large-onebyone-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-large-onebyone-floatonly-singleforceloop: test-sync-large-onebyone-floatonly

test-sync-large-onebyone-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-sync-large-onebyone-reducedlocalcalcs: test-sync-large-onebyone

test-sync-large-onebyone-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-large-onebyone-reducedlocalcalcs-singleforceloop: test-sync-large-onebyone-reducedlocalcalcs

test-sync-large-onebyone-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-large-onebyone-singleforceloop: test-sync-large-onebyone

# **TEST Local calculation method: sendtoself**
test-sync-large-sendtoself: DFLAGS+=-DSEND_TO_SELF
test-sync-large-sendtoself: test-sync-large

test-sync-large-sendtoself-betterverlet: DFLAGS+=-DBETTER_VERLET
test-sync-large-sendtoself-betterverlet: test-sync-large-sendtoself

test-sync-large-sendtoself-betterverlet-dtchange: DFLAGS+=-DSMALL_DT_EARLY
test-sync-large-sendtoself-betterverlet-dtchange: test-sync-large-sendtoself-betterverlet

test-sync-large-sendtoself-betterverlet-dtchange-doublesqrt: DFLAGS+=-DOUBLE_SQRT
test-sync-large-sendtoself-betterverlet-dtchange-doublesqrt: test-sync-large-sendtoself-betterverlet-dtchange

test-sync-large-sendtoself-betterverlet-dtchange-doublesqrt-msgmgmt: DFLAGS+=-DMESSAGE_MANAGEMENT
test-sync-large-sendtoself-betterverlet-dtchange-doublesqrt-msgmgmt: test-sync-large-sendtoself-betterverlet-dtchange-doublesqrt

test-sync-large-sendtoself-betterverlet-dtchange-doublesqrt-msgmgmt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-sync-large-sendtoself-betterverlet-dtchange-doublesqrt-msgmgmt-floatonly: test-sync-large-sendtoself-betterverlet-dtchange-doublesqrt-msgmgmt

test-sync-large-sendtoself-betterverlet-dtchange-doublesqrt-msgmgmt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-large-sendtoself-betterverlet-dtchange-doublesqrt-msgmgmt-floatonly-singleforceloop: test-sync-large-sendtoself-betterverlet-dtchange-doublesqrt-msgmgmt-floatonly

test-sync-large-sendtoself-betterverlet-dtchange-doublesqrt-msgmgmt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-large-sendtoself-betterverlet-dtchange-doublesqrt-msgmgmt-singleforceloop: test-sync-large-sendtoself-betterverlet-dtchange-doublesqrt-msgmgmt

test-sync-large-sendtoself-betterverlet-dtchange-doublesqrt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-sync-large-sendtoself-betterverlet-dtchange-doublesqrt-floatonly: test-sync-large-sendtoself-betterverlet-dtchange-doublesqrt

test-sync-large-sendtoself-betterverlet-dtchange-doublesqrt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-large-sendtoself-betterverlet-dtchange-doublesqrt-floatonly-singleforceloop: test-sync-large-sendtoself-betterverlet-dtchange-doublesqrt-floatonly

test-sync-large-sendtoself-betterverlet-dtchange-doublesqrt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-large-sendtoself-betterverlet-dtchange-doublesqrt-singleforceloop: test-sync-large-sendtoself-betterverlet-dtchange-doublesqrt

test-sync-large-sendtoself-betterverlet-dtchange-msgmgmt: DFLAGS+=-DMESSAGE_MANAGEMENT
test-sync-large-sendtoself-betterverlet-dtchange-msgmgmt: test-sync-large-sendtoself-betterverlet-dtchange

test-sync-large-sendtoself-betterverlet-dtchange-msgmgmt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-sync-large-sendtoself-betterverlet-dtchange-msgmgmt-floatonly: test-sync-large-sendtoself-betterverlet-dtchange-msgmgmt

test-sync-large-sendtoself-betterverlet-dtchange-msgmgmt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-large-sendtoself-betterverlet-dtchange-msgmgmt-floatonly-singleforceloop: test-sync-large-sendtoself-betterverlet-dtchange-msgmgmt-floatonly

test-sync-large-sendtoself-betterverlet-dtchange-msgmgmt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-large-sendtoself-betterverlet-dtchange-msgmgmt-singleforceloop: test-sync-large-sendtoself-betterverlet-dtchange-msgmgmt

test-sync-large-sendtoself-betterverlet-dtchange-floatonly: DFLAGS+=-DFLOAT_ONLY
test-sync-large-sendtoself-betterverlet-dtchange-floatonly: test-sync-large-sendtoself-betterverlet-dtchange

test-sync-large-sendtoself-betterverlet-dtchange-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-large-sendtoself-betterverlet-dtchange-floatonly-singleforceloop: test-sync-large-sendtoself-betterverlet-dtchange-floatonly

test-sync-large-sendtoself-betterverlet-dtchange-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-large-sendtoself-betterverlet-dtchange-singleforceloop: test-sync-large-sendtoself-betterverlet-dtchange

test-sync-large-sendtoself-betterverlet-doublesqrt: DFLAGS+=-DOUBLE_SQRT
test-sync-large-sendtoself-betterverlet-doublesqrt: test-sync-large-sendtoself-betterverlet

test-sync-large-sendtoself-betterverlet-doublesqrt-msgmgmt: DFLAGS+=-DMESSAGE_MANAGEMENT
test-sync-large-sendtoself-betterverlet-doublesqrt-msgmgmt: test-sync-large-sendtoself-betterverlet-doublesqrt

test-sync-large-sendtoself-betterverlet-doublesqrt-msgmgmt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-sync-large-sendtoself-betterverlet-doublesqrt-msgmgmt-floatonly: test-sync-large-sendtoself-betterverlet-doublesqrt-msgmgmt

test-sync-large-sendtoself-betterverlet-doublesqrt-msgmgmt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-large-sendtoself-betterverlet-doublesqrt-msgmgmt-floatonly-singleforceloop: test-sync-large-sendtoself-betterverlet-doublesqrt-msgmgmt-floatonly

test-sync-large-sendtoself-betterverlet-doublesqrt-msgmgmt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-large-sendtoself-betterverlet-doublesqrt-msgmgmt-singleforceloop: test-sync-large-sendtoself-betterverlet-doublesqrt-msgmgmt

test-sync-large-sendtoself-betterverlet-doublesqrt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-sync-large-sendtoself-betterverlet-doublesqrt-floatonly: test-sync-large-sendtoself-betterverlet-doublesqrt

test-sync-large-sendtoself-betterverlet-doublesqrt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-large-sendtoself-betterverlet-doublesqrt-floatonly-singleforceloop: test-sync-large-sendtoself-betterverlet-doublesqrt-floatonly

test-sync-large-sendtoself-betterverlet-doublesqrt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-large-sendtoself-betterverlet-doublesqrt-singleforceloop: test-sync-large-sendtoself-betterverlet-doublesqrt

test-sync-large-sendtoself-betterverlet-msgmgmt: DFLAGS+=-DMESSAGE_MANAGEMENT
test-sync-large-sendtoself-betterverlet-msgmgmt: test-sync-large-sendtoself-betterverlet

test-sync-large-sendtoself-betterverlet-msgmgmt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-sync-large-sendtoself-betterverlet-msgmgmt-floatonly: test-sync-large-sendtoself-betterverlet-msgmgmt

test-sync-large-sendtoself-betterverlet-msgmgmt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-large-sendtoself-betterverlet-msgmgmt-floatonly-singleforceloop: test-sync-large-sendtoself-betterverlet-msgmgmt-floatonly

test-sync-large-sendtoself-betterverlet-msgmgmt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-large-sendtoself-betterverlet-msgmgmt-singleforceloop: test-sync-large-sendtoself-betterverlet-msgmgmt

test-sync-large-sendtoself-betterverlet-floatonly: DFLAGS+=-DFLOAT_ONLY
test-sync-large-sendtoself-betterverlet-floatonly: test-sync-large-sendtoself-betterverlet

test-sync-large-sendtoself-betterverlet-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-large-sendtoself-betterverlet-floatonly-singleforceloop: test-sync-large-sendtoself-betterverlet-floatonly

test-sync-large-sendtoself-betterverlet-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-large-sendtoself-betterverlet-singleforceloop: test-sync-large-sendtoself-betterverlet

test-sync-large-sendtoself-dtchange: DFLAGS+=-DSMALL_DT_EARLY
test-sync-large-sendtoself-dtchange: test-sync-large-sendtoself

test-sync-large-sendtoself-dtchange-doublesqrt: DFLAGS+=-DOUBLE_SQRT
test-sync-large-sendtoself-dtchange-doublesqrt: test-sync-large-sendtoself-dtchange

test-sync-large-sendtoself-dtchange-doublesqrt-msgmgmt: DFLAGS+=-DMESSAGE_MANAGEMENT
test-sync-large-sendtoself-dtchange-doublesqrt-msgmgmt: test-sync-large-sendtoself-dtchange-doublesqrt

test-sync-large-sendtoself-dtchange-doublesqrt-msgmgmt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-sync-large-sendtoself-dtchange-doublesqrt-msgmgmt-floatonly: test-sync-large-sendtoself-dtchange-doublesqrt-msgmgmt

test-sync-large-sendtoself-dtchange-doublesqrt-msgmgmt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-large-sendtoself-dtchange-doublesqrt-msgmgmt-floatonly-singleforceloop: test-sync-large-sendtoself-dtchange-doublesqrt-msgmgmt-floatonly

test-sync-large-sendtoself-dtchange-doublesqrt-msgmgmt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-large-sendtoself-dtchange-doublesqrt-msgmgmt-singleforceloop: test-sync-large-sendtoself-dtchange-doublesqrt-msgmgmt

test-sync-large-sendtoself-dtchange-doublesqrt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-sync-large-sendtoself-dtchange-doublesqrt-floatonly: test-sync-large-sendtoself-dtchange-doublesqrt

test-sync-large-sendtoself-dtchange-doublesqrt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-large-sendtoself-dtchange-doublesqrt-floatonly-singleforceloop: test-sync-large-sendtoself-dtchange-doublesqrt-floatonly

test-sync-large-sendtoself-dtchange-doublesqrt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-large-sendtoself-dtchange-doublesqrt-singleforceloop: test-sync-large-sendtoself-dtchange-doublesqrt

test-sync-large-sendtoself-dtchange-msgmgmt: DFLAGS+=-DMESSAGE_MANAGEMENT
test-sync-large-sendtoself-dtchange-msgmgmt: test-sync-large-sendtoself-dtchange

test-sync-large-sendtoself-dtchange-msgmgmt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-sync-large-sendtoself-dtchange-msgmgmt-floatonly: test-sync-large-sendtoself-dtchange-msgmgmt

test-sync-large-sendtoself-dtchange-msgmgmt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-large-sendtoself-dtchange-msgmgmt-floatonly-singleforceloop: test-sync-large-sendtoself-dtchange-msgmgmt-floatonly

test-sync-large-sendtoself-dtchange-msgmgmt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-large-sendtoself-dtchange-msgmgmt-singleforceloop: test-sync-large-sendtoself-dtchange-msgmgmt

test-sync-large-sendtoself-dtchange-floatonly: DFLAGS+=-DFLOAT_ONLY
test-sync-large-sendtoself-dtchange-floatonly: test-sync-large-sendtoself-dtchange

test-sync-large-sendtoself-dtchange-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-large-sendtoself-dtchange-floatonly-singleforceloop: test-sync-large-sendtoself-dtchange-floatonly

test-sync-large-sendtoself-dtchange-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-large-sendtoself-dtchange-singleforceloop: test-sync-large-sendtoself-dtchange

test-sync-large-sendtoself-doublesqrt: DFLAGS+=-DOUBLE_SQRT
test-sync-large-sendtoself-doublesqrt: test-sync-large-sendtoself

test-sync-large-sendtoself-doublesqrt-msgmgmt: DFLAGS+=-DMESSAGE_MANAGEMENT
test-sync-large-sendtoself-doublesqrt-msgmgmt: test-sync-large-sendtoself-doublesqrt

test-sync-large-sendtoself-doublesqrt-msgmgmt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-sync-large-sendtoself-doublesqrt-msgmgmt-floatonly: test-sync-large-sendtoself-doublesqrt-msgmgmt

test-sync-large-sendtoself-doublesqrt-msgmgmt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-large-sendtoself-doublesqrt-msgmgmt-floatonly-singleforceloop: test-sync-large-sendtoself-doublesqrt-msgmgmt-floatonly

test-sync-large-sendtoself-doublesqrt-msgmgmt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-large-sendtoself-doublesqrt-msgmgmt-singleforceloop: test-sync-large-sendtoself-doublesqrt-msgmgmt

test-sync-large-sendtoself-doublesqrt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-sync-large-sendtoself-doublesqrt-floatonly: test-sync-large-sendtoself-doublesqrt

test-sync-large-sendtoself-doublesqrt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-large-sendtoself-doublesqrt-floatonly-singleforceloop: test-sync-large-sendtoself-doublesqrt-floatonly

test-sync-large-sendtoself-doublesqrt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-large-sendtoself-doublesqrt-singleforceloop: test-sync-large-sendtoself-doublesqrt

test-sync-large-sendtoself-msgmgmt: DFLAGS+=-DMESSAGE_MANAGEMENT
test-sync-large-sendtoself-msgmgmt: test-sync-large-sendtoself

test-sync-large-sendtoself-msgmgmt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-sync-large-sendtoself-msgmgmt-floatonly: test-sync-large-sendtoself-msgmgmt

test-sync-large-sendtoself-msgmgmt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-large-sendtoself-msgmgmt-floatonly-singleforceloop: test-sync-large-sendtoself-msgmgmt-floatonly

test-sync-large-sendtoself-msgmgmt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-large-sendtoself-msgmgmt-singleforceloop: test-sync-large-sendtoself-msgmgmt

test-sync-large-sendtoself-floatonly: DFLAGS+=-DFLOAT_ONLY
test-sync-large-sendtoself-floatonly: test-sync-large-sendtoself

test-sync-large-sendtoself-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-large-sendtoself-floatonly-singleforceloop: test-sync-large-sendtoself-floatonly

test-sync-large-sendtoself-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-large-sendtoself-singleforceloop: test-sync-large-sendtoself

test-sync-bonds: DFLAGS+=-DBONDS
test-sync-bonds: test-sync

test-sync-bonds-betterverlet: DFLAGS+=-DBETTER_VERLET
test-sync-bonds-betterverlet: test-sync-bonds

test-sync-bonds-betterverlet-dtchange: DFLAGS+=-DSMALL_DT_EARLY
test-sync-bonds-betterverlet-dtchange: test-sync-bonds-betterverlet

test-sync-bonds-betterverlet-dtchange-doublesqrt: DFLAGS+=-DOUBLE_SQRT
test-sync-bonds-betterverlet-dtchange-doublesqrt: test-sync-bonds-betterverlet-dtchange

test-sync-bonds-betterverlet-dtchange-doublesqrt-msgmgmt: DFLAGS+=-DMESSAGE_MANAGEMENT
test-sync-bonds-betterverlet-dtchange-doublesqrt-msgmgmt: test-sync-bonds-betterverlet-dtchange-doublesqrt

test-sync-bonds-betterverlet-dtchange-doublesqrt-msgmgmt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-sync-bonds-betterverlet-dtchange-doublesqrt-msgmgmt-floatonly: test-sync-bonds-betterverlet-dtchange-doublesqrt-msgmgmt

test-sync-bonds-betterverlet-dtchange-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-sync-bonds-betterverlet-dtchange-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs: test-sync-bonds-betterverlet-dtchange-doublesqrt-msgmgmt-floatonly

test-sync-bonds-betterverlet-dtchange-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-bonds-betterverlet-dtchange-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs-singleforceloop: test-sync-bonds-betterverlet-dtchange-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs

test-sync-bonds-betterverlet-dtchange-doublesqrt-msgmgmt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-bonds-betterverlet-dtchange-doublesqrt-msgmgmt-floatonly-singleforceloop: test-sync-bonds-betterverlet-dtchange-doublesqrt-msgmgmt-floatonly

test-sync-bonds-betterverlet-dtchange-doublesqrt-msgmgmt-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-sync-bonds-betterverlet-dtchange-doublesqrt-msgmgmt-reducedlocalcalcs: test-sync-bonds-betterverlet-dtchange-doublesqrt-msgmgmt

test-sync-bonds-betterverlet-dtchange-doublesqrt-msgmgmt-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-bonds-betterverlet-dtchange-doublesqrt-msgmgmt-reducedlocalcalcs-singleforceloop: test-sync-bonds-betterverlet-dtchange-doublesqrt-msgmgmt-reducedlocalcalcs

test-sync-bonds-betterverlet-dtchange-doublesqrt-msgmgmt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-bonds-betterverlet-dtchange-doublesqrt-msgmgmt-singleforceloop: test-sync-bonds-betterverlet-dtchange-doublesqrt-msgmgmt

test-sync-bonds-betterverlet-dtchange-doublesqrt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-sync-bonds-betterverlet-dtchange-doublesqrt-floatonly: test-sync-bonds-betterverlet-dtchange-doublesqrt

test-sync-bonds-betterverlet-dtchange-doublesqrt-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-sync-bonds-betterverlet-dtchange-doublesqrt-floatonly-reducedlocalcalcs: test-sync-bonds-betterverlet-dtchange-doublesqrt-floatonly

test-sync-bonds-betterverlet-dtchange-doublesqrt-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-bonds-betterverlet-dtchange-doublesqrt-floatonly-reducedlocalcalcs-singleforceloop: test-sync-bonds-betterverlet-dtchange-doublesqrt-floatonly-reducedlocalcalcs

test-sync-bonds-betterverlet-dtchange-doublesqrt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-bonds-betterverlet-dtchange-doublesqrt-floatonly-singleforceloop: test-sync-bonds-betterverlet-dtchange-doublesqrt-floatonly

test-sync-bonds-betterverlet-dtchange-doublesqrt-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-sync-bonds-betterverlet-dtchange-doublesqrt-reducedlocalcalcs: test-sync-bonds-betterverlet-dtchange-doublesqrt

test-sync-bonds-betterverlet-dtchange-doublesqrt-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-bonds-betterverlet-dtchange-doublesqrt-reducedlocalcalcs-singleforceloop: test-sync-bonds-betterverlet-dtchange-doublesqrt-reducedlocalcalcs

test-sync-bonds-betterverlet-dtchange-doublesqrt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-bonds-betterverlet-dtchange-doublesqrt-singleforceloop: test-sync-bonds-betterverlet-dtchange-doublesqrt

test-sync-bonds-betterverlet-dtchange-msgmgmt: DFLAGS+=-DMESSAGE_MANAGEMENT
test-sync-bonds-betterverlet-dtchange-msgmgmt: test-sync-bonds-betterverlet-dtchange

test-sync-bonds-betterverlet-dtchange-msgmgmt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-sync-bonds-betterverlet-dtchange-msgmgmt-floatonly: test-sync-bonds-betterverlet-dtchange-msgmgmt

test-sync-bonds-betterverlet-dtchange-msgmgmt-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-sync-bonds-betterverlet-dtchange-msgmgmt-floatonly-reducedlocalcalcs: test-sync-bonds-betterverlet-dtchange-msgmgmt-floatonly

test-sync-bonds-betterverlet-dtchange-msgmgmt-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-bonds-betterverlet-dtchange-msgmgmt-floatonly-reducedlocalcalcs-singleforceloop: test-sync-bonds-betterverlet-dtchange-msgmgmt-floatonly-reducedlocalcalcs

test-sync-bonds-betterverlet-dtchange-msgmgmt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-bonds-betterverlet-dtchange-msgmgmt-floatonly-singleforceloop: test-sync-bonds-betterverlet-dtchange-msgmgmt-floatonly

test-sync-bonds-betterverlet-dtchange-msgmgmt-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-sync-bonds-betterverlet-dtchange-msgmgmt-reducedlocalcalcs: test-sync-bonds-betterverlet-dtchange-msgmgmt

test-sync-bonds-betterverlet-dtchange-msgmgmt-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-bonds-betterverlet-dtchange-msgmgmt-reducedlocalcalcs-singleforceloop: test-sync-bonds-betterverlet-dtchange-msgmgmt-reducedlocalcalcs

test-sync-bonds-betterverlet-dtchange-msgmgmt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-bonds-betterverlet-dtchange-msgmgmt-singleforceloop: test-sync-bonds-betterverlet-dtchange-msgmgmt

test-sync-bonds-betterverlet-dtchange-floatonly: DFLAGS+=-DFLOAT_ONLY
test-sync-bonds-betterverlet-dtchange-floatonly: test-sync-bonds-betterverlet-dtchange

test-sync-bonds-betterverlet-dtchange-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-sync-bonds-betterverlet-dtchange-floatonly-reducedlocalcalcs: test-sync-bonds-betterverlet-dtchange-floatonly

test-sync-bonds-betterverlet-dtchange-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-bonds-betterverlet-dtchange-floatonly-reducedlocalcalcs-singleforceloop: test-sync-bonds-betterverlet-dtchange-floatonly-reducedlocalcalcs

test-sync-bonds-betterverlet-dtchange-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-bonds-betterverlet-dtchange-floatonly-singleforceloop: test-sync-bonds-betterverlet-dtchange-floatonly

test-sync-bonds-betterverlet-dtchange-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-sync-bonds-betterverlet-dtchange-reducedlocalcalcs: test-sync-bonds-betterverlet-dtchange

test-sync-bonds-betterverlet-dtchange-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-bonds-betterverlet-dtchange-reducedlocalcalcs-singleforceloop: test-sync-bonds-betterverlet-dtchange-reducedlocalcalcs

test-sync-bonds-betterverlet-dtchange-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-bonds-betterverlet-dtchange-singleforceloop: test-sync-bonds-betterverlet-dtchange

test-sync-bonds-betterverlet-doublesqrt: DFLAGS+=-DOUBLE_SQRT
test-sync-bonds-betterverlet-doublesqrt: test-sync-bonds-betterverlet

test-sync-bonds-betterverlet-doublesqrt-msgmgmt: DFLAGS+=-DMESSAGE_MANAGEMENT
test-sync-bonds-betterverlet-doublesqrt-msgmgmt: test-sync-bonds-betterverlet-doublesqrt

test-sync-bonds-betterverlet-doublesqrt-msgmgmt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-sync-bonds-betterverlet-doublesqrt-msgmgmt-floatonly: test-sync-bonds-betterverlet-doublesqrt-msgmgmt

test-sync-bonds-betterverlet-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-sync-bonds-betterverlet-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs: test-sync-bonds-betterverlet-doublesqrt-msgmgmt-floatonly

test-sync-bonds-betterverlet-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-bonds-betterverlet-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs-singleforceloop: test-sync-bonds-betterverlet-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs

test-sync-bonds-betterverlet-doublesqrt-msgmgmt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-bonds-betterverlet-doublesqrt-msgmgmt-floatonly-singleforceloop: test-sync-bonds-betterverlet-doublesqrt-msgmgmt-floatonly

test-sync-bonds-betterverlet-doublesqrt-msgmgmt-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-sync-bonds-betterverlet-doublesqrt-msgmgmt-reducedlocalcalcs: test-sync-bonds-betterverlet-doublesqrt-msgmgmt

test-sync-bonds-betterverlet-doublesqrt-msgmgmt-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-bonds-betterverlet-doublesqrt-msgmgmt-reducedlocalcalcs-singleforceloop: test-sync-bonds-betterverlet-doublesqrt-msgmgmt-reducedlocalcalcs

test-sync-bonds-betterverlet-doublesqrt-msgmgmt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-bonds-betterverlet-doublesqrt-msgmgmt-singleforceloop: test-sync-bonds-betterverlet-doublesqrt-msgmgmt

test-sync-bonds-betterverlet-doublesqrt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-sync-bonds-betterverlet-doublesqrt-floatonly: test-sync-bonds-betterverlet-doublesqrt

test-sync-bonds-betterverlet-doublesqrt-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-sync-bonds-betterverlet-doublesqrt-floatonly-reducedlocalcalcs: test-sync-bonds-betterverlet-doublesqrt-floatonly

test-sync-bonds-betterverlet-doublesqrt-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-bonds-betterverlet-doublesqrt-floatonly-reducedlocalcalcs-singleforceloop: test-sync-bonds-betterverlet-doublesqrt-floatonly-reducedlocalcalcs

test-sync-bonds-betterverlet-doublesqrt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-bonds-betterverlet-doublesqrt-floatonly-singleforceloop: test-sync-bonds-betterverlet-doublesqrt-floatonly

test-sync-bonds-betterverlet-doublesqrt-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-sync-bonds-betterverlet-doublesqrt-reducedlocalcalcs: test-sync-bonds-betterverlet-doublesqrt

test-sync-bonds-betterverlet-doublesqrt-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-bonds-betterverlet-doublesqrt-reducedlocalcalcs-singleforceloop: test-sync-bonds-betterverlet-doublesqrt-reducedlocalcalcs

test-sync-bonds-betterverlet-doublesqrt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-bonds-betterverlet-doublesqrt-singleforceloop: test-sync-bonds-betterverlet-doublesqrt

test-sync-bonds-betterverlet-msgmgmt: DFLAGS+=-DMESSAGE_MANAGEMENT
test-sync-bonds-betterverlet-msgmgmt: test-sync-bonds-betterverlet

test-sync-bonds-betterverlet-msgmgmt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-sync-bonds-betterverlet-msgmgmt-floatonly: test-sync-bonds-betterverlet-msgmgmt

test-sync-bonds-betterverlet-msgmgmt-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-sync-bonds-betterverlet-msgmgmt-floatonly-reducedlocalcalcs: test-sync-bonds-betterverlet-msgmgmt-floatonly

test-sync-bonds-betterverlet-msgmgmt-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-bonds-betterverlet-msgmgmt-floatonly-reducedlocalcalcs-singleforceloop: test-sync-bonds-betterverlet-msgmgmt-floatonly-reducedlocalcalcs

test-sync-bonds-betterverlet-msgmgmt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-bonds-betterverlet-msgmgmt-floatonly-singleforceloop: test-sync-bonds-betterverlet-msgmgmt-floatonly

test-sync-bonds-betterverlet-msgmgmt-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-sync-bonds-betterverlet-msgmgmt-reducedlocalcalcs: test-sync-bonds-betterverlet-msgmgmt

test-sync-bonds-betterverlet-msgmgmt-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-bonds-betterverlet-msgmgmt-reducedlocalcalcs-singleforceloop: test-sync-bonds-betterverlet-msgmgmt-reducedlocalcalcs

test-sync-bonds-betterverlet-msgmgmt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-bonds-betterverlet-msgmgmt-singleforceloop: test-sync-bonds-betterverlet-msgmgmt

test-sync-bonds-betterverlet-floatonly: DFLAGS+=-DFLOAT_ONLY
test-sync-bonds-betterverlet-floatonly: test-sync-bonds-betterverlet

test-sync-bonds-betterverlet-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-sync-bonds-betterverlet-floatonly-reducedlocalcalcs: test-sync-bonds-betterverlet-floatonly

test-sync-bonds-betterverlet-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-bonds-betterverlet-floatonly-reducedlocalcalcs-singleforceloop: test-sync-bonds-betterverlet-floatonly-reducedlocalcalcs

test-sync-bonds-betterverlet-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-bonds-betterverlet-floatonly-singleforceloop: test-sync-bonds-betterverlet-floatonly

test-sync-bonds-betterverlet-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-sync-bonds-betterverlet-reducedlocalcalcs: test-sync-bonds-betterverlet

test-sync-bonds-betterverlet-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-bonds-betterverlet-reducedlocalcalcs-singleforceloop: test-sync-bonds-betterverlet-reducedlocalcalcs

test-sync-bonds-betterverlet-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-bonds-betterverlet-singleforceloop: test-sync-bonds-betterverlet

test-sync-bonds-dtchange: DFLAGS+=-DSMALL_DT_EARLY
test-sync-bonds-dtchange: test-sync-bonds

test-sync-bonds-dtchange-doublesqrt: DFLAGS+=-DOUBLE_SQRT
test-sync-bonds-dtchange-doublesqrt: test-sync-bonds-dtchange

test-sync-bonds-dtchange-doublesqrt-msgmgmt: DFLAGS+=-DMESSAGE_MANAGEMENT
test-sync-bonds-dtchange-doublesqrt-msgmgmt: test-sync-bonds-dtchange-doublesqrt

test-sync-bonds-dtchange-doublesqrt-msgmgmt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-sync-bonds-dtchange-doublesqrt-msgmgmt-floatonly: test-sync-bonds-dtchange-doublesqrt-msgmgmt

test-sync-bonds-dtchange-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-sync-bonds-dtchange-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs: test-sync-bonds-dtchange-doublesqrt-msgmgmt-floatonly

test-sync-bonds-dtchange-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-bonds-dtchange-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs-singleforceloop: test-sync-bonds-dtchange-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs

test-sync-bonds-dtchange-doublesqrt-msgmgmt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-bonds-dtchange-doublesqrt-msgmgmt-floatonly-singleforceloop: test-sync-bonds-dtchange-doublesqrt-msgmgmt-floatonly

test-sync-bonds-dtchange-doublesqrt-msgmgmt-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-sync-bonds-dtchange-doublesqrt-msgmgmt-reducedlocalcalcs: test-sync-bonds-dtchange-doublesqrt-msgmgmt

test-sync-bonds-dtchange-doublesqrt-msgmgmt-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-bonds-dtchange-doublesqrt-msgmgmt-reducedlocalcalcs-singleforceloop: test-sync-bonds-dtchange-doublesqrt-msgmgmt-reducedlocalcalcs

test-sync-bonds-dtchange-doublesqrt-msgmgmt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-bonds-dtchange-doublesqrt-msgmgmt-singleforceloop: test-sync-bonds-dtchange-doublesqrt-msgmgmt

test-sync-bonds-dtchange-doublesqrt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-sync-bonds-dtchange-doublesqrt-floatonly: test-sync-bonds-dtchange-doublesqrt

test-sync-bonds-dtchange-doublesqrt-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-sync-bonds-dtchange-doublesqrt-floatonly-reducedlocalcalcs: test-sync-bonds-dtchange-doublesqrt-floatonly

test-sync-bonds-dtchange-doublesqrt-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-bonds-dtchange-doublesqrt-floatonly-reducedlocalcalcs-singleforceloop: test-sync-bonds-dtchange-doublesqrt-floatonly-reducedlocalcalcs

test-sync-bonds-dtchange-doublesqrt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-bonds-dtchange-doublesqrt-floatonly-singleforceloop: test-sync-bonds-dtchange-doublesqrt-floatonly

test-sync-bonds-dtchange-doublesqrt-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-sync-bonds-dtchange-doublesqrt-reducedlocalcalcs: test-sync-bonds-dtchange-doublesqrt

test-sync-bonds-dtchange-doublesqrt-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-bonds-dtchange-doublesqrt-reducedlocalcalcs-singleforceloop: test-sync-bonds-dtchange-doublesqrt-reducedlocalcalcs

test-sync-bonds-dtchange-doublesqrt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-bonds-dtchange-doublesqrt-singleforceloop: test-sync-bonds-dtchange-doublesqrt

test-sync-bonds-dtchange-msgmgmt: DFLAGS+=-DMESSAGE_MANAGEMENT
test-sync-bonds-dtchange-msgmgmt: test-sync-bonds-dtchange

test-sync-bonds-dtchange-msgmgmt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-sync-bonds-dtchange-msgmgmt-floatonly: test-sync-bonds-dtchange-msgmgmt

test-sync-bonds-dtchange-msgmgmt-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-sync-bonds-dtchange-msgmgmt-floatonly-reducedlocalcalcs: test-sync-bonds-dtchange-msgmgmt-floatonly

test-sync-bonds-dtchange-msgmgmt-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-bonds-dtchange-msgmgmt-floatonly-reducedlocalcalcs-singleforceloop: test-sync-bonds-dtchange-msgmgmt-floatonly-reducedlocalcalcs

test-sync-bonds-dtchange-msgmgmt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-bonds-dtchange-msgmgmt-floatonly-singleforceloop: test-sync-bonds-dtchange-msgmgmt-floatonly

test-sync-bonds-dtchange-msgmgmt-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-sync-bonds-dtchange-msgmgmt-reducedlocalcalcs: test-sync-bonds-dtchange-msgmgmt

test-sync-bonds-dtchange-msgmgmt-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-bonds-dtchange-msgmgmt-reducedlocalcalcs-singleforceloop: test-sync-bonds-dtchange-msgmgmt-reducedlocalcalcs

test-sync-bonds-dtchange-msgmgmt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-bonds-dtchange-msgmgmt-singleforceloop: test-sync-bonds-dtchange-msgmgmt

test-sync-bonds-dtchange-floatonly: DFLAGS+=-DFLOAT_ONLY
test-sync-bonds-dtchange-floatonly: test-sync-bonds-dtchange

test-sync-bonds-dtchange-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-sync-bonds-dtchange-floatonly-reducedlocalcalcs: test-sync-bonds-dtchange-floatonly

test-sync-bonds-dtchange-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-bonds-dtchange-floatonly-reducedlocalcalcs-singleforceloop: test-sync-bonds-dtchange-floatonly-reducedlocalcalcs

test-sync-bonds-dtchange-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-bonds-dtchange-floatonly-singleforceloop: test-sync-bonds-dtchange-floatonly

test-sync-bonds-dtchange-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-sync-bonds-dtchange-reducedlocalcalcs: test-sync-bonds-dtchange

test-sync-bonds-dtchange-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-bonds-dtchange-reducedlocalcalcs-singleforceloop: test-sync-bonds-dtchange-reducedlocalcalcs

test-sync-bonds-dtchange-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-bonds-dtchange-singleforceloop: test-sync-bonds-dtchange

test-sync-bonds-doublesqrt: DFLAGS+=-DOUBLE_SQRT
test-sync-bonds-doublesqrt: test-sync-bonds

test-sync-bonds-doublesqrt-msgmgmt: DFLAGS+=-DMESSAGE_MANAGEMENT
test-sync-bonds-doublesqrt-msgmgmt: test-sync-bonds-doublesqrt

test-sync-bonds-doublesqrt-msgmgmt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-sync-bonds-doublesqrt-msgmgmt-floatonly: test-sync-bonds-doublesqrt-msgmgmt

test-sync-bonds-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-sync-bonds-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs: test-sync-bonds-doublesqrt-msgmgmt-floatonly

test-sync-bonds-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-bonds-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs-singleforceloop: test-sync-bonds-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs

test-sync-bonds-doublesqrt-msgmgmt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-bonds-doublesqrt-msgmgmt-floatonly-singleforceloop: test-sync-bonds-doublesqrt-msgmgmt-floatonly

test-sync-bonds-doublesqrt-msgmgmt-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-sync-bonds-doublesqrt-msgmgmt-reducedlocalcalcs: test-sync-bonds-doublesqrt-msgmgmt

test-sync-bonds-doublesqrt-msgmgmt-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-bonds-doublesqrt-msgmgmt-reducedlocalcalcs-singleforceloop: test-sync-bonds-doublesqrt-msgmgmt-reducedlocalcalcs

test-sync-bonds-doublesqrt-msgmgmt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-bonds-doublesqrt-msgmgmt-singleforceloop: test-sync-bonds-doublesqrt-msgmgmt

test-sync-bonds-doublesqrt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-sync-bonds-doublesqrt-floatonly: test-sync-bonds-doublesqrt

test-sync-bonds-doublesqrt-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-sync-bonds-doublesqrt-floatonly-reducedlocalcalcs: test-sync-bonds-doublesqrt-floatonly

test-sync-bonds-doublesqrt-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-bonds-doublesqrt-floatonly-reducedlocalcalcs-singleforceloop: test-sync-bonds-doublesqrt-floatonly-reducedlocalcalcs

test-sync-bonds-doublesqrt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-bonds-doublesqrt-floatonly-singleforceloop: test-sync-bonds-doublesqrt-floatonly

test-sync-bonds-doublesqrt-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-sync-bonds-doublesqrt-reducedlocalcalcs: test-sync-bonds-doublesqrt

test-sync-bonds-doublesqrt-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-bonds-doublesqrt-reducedlocalcalcs-singleforceloop: test-sync-bonds-doublesqrt-reducedlocalcalcs

test-sync-bonds-doublesqrt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-bonds-doublesqrt-singleforceloop: test-sync-bonds-doublesqrt

test-sync-bonds-msgmgmt: DFLAGS+=-DMESSAGE_MANAGEMENT
test-sync-bonds-msgmgmt: test-sync-bonds

test-sync-bonds-msgmgmt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-sync-bonds-msgmgmt-floatonly: test-sync-bonds-msgmgmt

test-sync-bonds-msgmgmt-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-sync-bonds-msgmgmt-floatonly-reducedlocalcalcs: test-sync-bonds-msgmgmt-floatonly

test-sync-bonds-msgmgmt-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-bonds-msgmgmt-floatonly-reducedlocalcalcs-singleforceloop: test-sync-bonds-msgmgmt-floatonly-reducedlocalcalcs

test-sync-bonds-msgmgmt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-bonds-msgmgmt-floatonly-singleforceloop: test-sync-bonds-msgmgmt-floatonly

test-sync-bonds-msgmgmt-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-sync-bonds-msgmgmt-reducedlocalcalcs: test-sync-bonds-msgmgmt

test-sync-bonds-msgmgmt-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-bonds-msgmgmt-reducedlocalcalcs-singleforceloop: test-sync-bonds-msgmgmt-reducedlocalcalcs

test-sync-bonds-msgmgmt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-bonds-msgmgmt-singleforceloop: test-sync-bonds-msgmgmt

test-sync-bonds-floatonly: DFLAGS+=-DFLOAT_ONLY
test-sync-bonds-floatonly: test-sync-bonds

test-sync-bonds-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-sync-bonds-floatonly-reducedlocalcalcs: test-sync-bonds-floatonly

test-sync-bonds-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-bonds-floatonly-reducedlocalcalcs-singleforceloop: test-sync-bonds-floatonly-reducedlocalcalcs

test-sync-bonds-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-bonds-floatonly-singleforceloop: test-sync-bonds-floatonly

test-sync-bonds-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-sync-bonds-reducedlocalcalcs: test-sync-bonds

test-sync-bonds-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-bonds-reducedlocalcalcs-singleforceloop: test-sync-bonds-reducedlocalcalcs

test-sync-bonds-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-bonds-singleforceloop: test-sync-bonds

# **TEST Local calculation method: onebyone**
test-sync-bonds-onebyone: DFLAGS+=-DONE_BY_ONE
test-sync-bonds-onebyone: test-sync-bonds

test-sync-bonds-onebyone-betterverlet: DFLAGS+=-DBETTER_VERLET
test-sync-bonds-onebyone-betterverlet: test-sync-bonds-onebyone

test-sync-bonds-onebyone-betterverlet-dtchange: DFLAGS+=-DSMALL_DT_EARLY
test-sync-bonds-onebyone-betterverlet-dtchange: test-sync-bonds-onebyone-betterverlet

test-sync-bonds-onebyone-betterverlet-dtchange-doublesqrt: DFLAGS+=-DOUBLE_SQRT
test-sync-bonds-onebyone-betterverlet-dtchange-doublesqrt: test-sync-bonds-onebyone-betterverlet-dtchange

test-sync-bonds-onebyone-betterverlet-dtchange-doublesqrt-msgmgmt: DFLAGS+=-DMESSAGE_MANAGEMENT
test-sync-bonds-onebyone-betterverlet-dtchange-doublesqrt-msgmgmt: test-sync-bonds-onebyone-betterverlet-dtchange-doublesqrt

test-sync-bonds-onebyone-betterverlet-dtchange-doublesqrt-msgmgmt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-sync-bonds-onebyone-betterverlet-dtchange-doublesqrt-msgmgmt-floatonly: test-sync-bonds-onebyone-betterverlet-dtchange-doublesqrt-msgmgmt

test-sync-bonds-onebyone-betterverlet-dtchange-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-sync-bonds-onebyone-betterverlet-dtchange-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs: test-sync-bonds-onebyone-betterverlet-dtchange-doublesqrt-msgmgmt-floatonly

test-sync-bonds-onebyone-betterverlet-dtchange-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-bonds-onebyone-betterverlet-dtchange-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs-singleforceloop: test-sync-bonds-onebyone-betterverlet-dtchange-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs

test-sync-bonds-onebyone-betterverlet-dtchange-doublesqrt-msgmgmt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-bonds-onebyone-betterverlet-dtchange-doublesqrt-msgmgmt-floatonly-singleforceloop: test-sync-bonds-onebyone-betterverlet-dtchange-doublesqrt-msgmgmt-floatonly

test-sync-bonds-onebyone-betterverlet-dtchange-doublesqrt-msgmgmt-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-sync-bonds-onebyone-betterverlet-dtchange-doublesqrt-msgmgmt-reducedlocalcalcs: test-sync-bonds-onebyone-betterverlet-dtchange-doublesqrt-msgmgmt

test-sync-bonds-onebyone-betterverlet-dtchange-doublesqrt-msgmgmt-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-bonds-onebyone-betterverlet-dtchange-doublesqrt-msgmgmt-reducedlocalcalcs-singleforceloop: test-sync-bonds-onebyone-betterverlet-dtchange-doublesqrt-msgmgmt-reducedlocalcalcs

test-sync-bonds-onebyone-betterverlet-dtchange-doublesqrt-msgmgmt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-bonds-onebyone-betterverlet-dtchange-doublesqrt-msgmgmt-singleforceloop: test-sync-bonds-onebyone-betterverlet-dtchange-doublesqrt-msgmgmt

test-sync-bonds-onebyone-betterverlet-dtchange-doublesqrt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-sync-bonds-onebyone-betterverlet-dtchange-doublesqrt-floatonly: test-sync-bonds-onebyone-betterverlet-dtchange-doublesqrt

test-sync-bonds-onebyone-betterverlet-dtchange-doublesqrt-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-sync-bonds-onebyone-betterverlet-dtchange-doublesqrt-floatonly-reducedlocalcalcs: test-sync-bonds-onebyone-betterverlet-dtchange-doublesqrt-floatonly

test-sync-bonds-onebyone-betterverlet-dtchange-doublesqrt-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-bonds-onebyone-betterverlet-dtchange-doublesqrt-floatonly-reducedlocalcalcs-singleforceloop: test-sync-bonds-onebyone-betterverlet-dtchange-doublesqrt-floatonly-reducedlocalcalcs

test-sync-bonds-onebyone-betterverlet-dtchange-doublesqrt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-bonds-onebyone-betterverlet-dtchange-doublesqrt-floatonly-singleforceloop: test-sync-bonds-onebyone-betterverlet-dtchange-doublesqrt-floatonly

test-sync-bonds-onebyone-betterverlet-dtchange-doublesqrt-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-sync-bonds-onebyone-betterverlet-dtchange-doublesqrt-reducedlocalcalcs: test-sync-bonds-onebyone-betterverlet-dtchange-doublesqrt

test-sync-bonds-onebyone-betterverlet-dtchange-doublesqrt-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-bonds-onebyone-betterverlet-dtchange-doublesqrt-reducedlocalcalcs-singleforceloop: test-sync-bonds-onebyone-betterverlet-dtchange-doublesqrt-reducedlocalcalcs

test-sync-bonds-onebyone-betterverlet-dtchange-doublesqrt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-bonds-onebyone-betterverlet-dtchange-doublesqrt-singleforceloop: test-sync-bonds-onebyone-betterverlet-dtchange-doublesqrt

test-sync-bonds-onebyone-betterverlet-dtchange-msgmgmt: DFLAGS+=-DMESSAGE_MANAGEMENT
test-sync-bonds-onebyone-betterverlet-dtchange-msgmgmt: test-sync-bonds-onebyone-betterverlet-dtchange

test-sync-bonds-onebyone-betterverlet-dtchange-msgmgmt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-sync-bonds-onebyone-betterverlet-dtchange-msgmgmt-floatonly: test-sync-bonds-onebyone-betterverlet-dtchange-msgmgmt

test-sync-bonds-onebyone-betterverlet-dtchange-msgmgmt-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-sync-bonds-onebyone-betterverlet-dtchange-msgmgmt-floatonly-reducedlocalcalcs: test-sync-bonds-onebyone-betterverlet-dtchange-msgmgmt-floatonly

test-sync-bonds-onebyone-betterverlet-dtchange-msgmgmt-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-bonds-onebyone-betterverlet-dtchange-msgmgmt-floatonly-reducedlocalcalcs-singleforceloop: test-sync-bonds-onebyone-betterverlet-dtchange-msgmgmt-floatonly-reducedlocalcalcs

test-sync-bonds-onebyone-betterverlet-dtchange-msgmgmt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-bonds-onebyone-betterverlet-dtchange-msgmgmt-floatonly-singleforceloop: test-sync-bonds-onebyone-betterverlet-dtchange-msgmgmt-floatonly

test-sync-bonds-onebyone-betterverlet-dtchange-msgmgmt-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-sync-bonds-onebyone-betterverlet-dtchange-msgmgmt-reducedlocalcalcs: test-sync-bonds-onebyone-betterverlet-dtchange-msgmgmt

test-sync-bonds-onebyone-betterverlet-dtchange-msgmgmt-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-bonds-onebyone-betterverlet-dtchange-msgmgmt-reducedlocalcalcs-singleforceloop: test-sync-bonds-onebyone-betterverlet-dtchange-msgmgmt-reducedlocalcalcs

test-sync-bonds-onebyone-betterverlet-dtchange-msgmgmt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-bonds-onebyone-betterverlet-dtchange-msgmgmt-singleforceloop: test-sync-bonds-onebyone-betterverlet-dtchange-msgmgmt

test-sync-bonds-onebyone-betterverlet-dtchange-floatonly: DFLAGS+=-DFLOAT_ONLY
test-sync-bonds-onebyone-betterverlet-dtchange-floatonly: test-sync-bonds-onebyone-betterverlet-dtchange

test-sync-bonds-onebyone-betterverlet-dtchange-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-sync-bonds-onebyone-betterverlet-dtchange-floatonly-reducedlocalcalcs: test-sync-bonds-onebyone-betterverlet-dtchange-floatonly

test-sync-bonds-onebyone-betterverlet-dtchange-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-bonds-onebyone-betterverlet-dtchange-floatonly-reducedlocalcalcs-singleforceloop: test-sync-bonds-onebyone-betterverlet-dtchange-floatonly-reducedlocalcalcs

test-sync-bonds-onebyone-betterverlet-dtchange-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-bonds-onebyone-betterverlet-dtchange-floatonly-singleforceloop: test-sync-bonds-onebyone-betterverlet-dtchange-floatonly

test-sync-bonds-onebyone-betterverlet-dtchange-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-sync-bonds-onebyone-betterverlet-dtchange-reducedlocalcalcs: test-sync-bonds-onebyone-betterverlet-dtchange

test-sync-bonds-onebyone-betterverlet-dtchange-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-bonds-onebyone-betterverlet-dtchange-reducedlocalcalcs-singleforceloop: test-sync-bonds-onebyone-betterverlet-dtchange-reducedlocalcalcs

test-sync-bonds-onebyone-betterverlet-dtchange-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-bonds-onebyone-betterverlet-dtchange-singleforceloop: test-sync-bonds-onebyone-betterverlet-dtchange

test-sync-bonds-onebyone-betterverlet-doublesqrt: DFLAGS+=-DOUBLE_SQRT
test-sync-bonds-onebyone-betterverlet-doublesqrt: test-sync-bonds-onebyone-betterverlet

test-sync-bonds-onebyone-betterverlet-doublesqrt-msgmgmt: DFLAGS+=-DMESSAGE_MANAGEMENT
test-sync-bonds-onebyone-betterverlet-doublesqrt-msgmgmt: test-sync-bonds-onebyone-betterverlet-doublesqrt

test-sync-bonds-onebyone-betterverlet-doublesqrt-msgmgmt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-sync-bonds-onebyone-betterverlet-doublesqrt-msgmgmt-floatonly: test-sync-bonds-onebyone-betterverlet-doublesqrt-msgmgmt

test-sync-bonds-onebyone-betterverlet-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-sync-bonds-onebyone-betterverlet-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs: test-sync-bonds-onebyone-betterverlet-doublesqrt-msgmgmt-floatonly

test-sync-bonds-onebyone-betterverlet-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-bonds-onebyone-betterverlet-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs-singleforceloop: test-sync-bonds-onebyone-betterverlet-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs

test-sync-bonds-onebyone-betterverlet-doublesqrt-msgmgmt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-bonds-onebyone-betterverlet-doublesqrt-msgmgmt-floatonly-singleforceloop: test-sync-bonds-onebyone-betterverlet-doublesqrt-msgmgmt-floatonly

test-sync-bonds-onebyone-betterverlet-doublesqrt-msgmgmt-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-sync-bonds-onebyone-betterverlet-doublesqrt-msgmgmt-reducedlocalcalcs: test-sync-bonds-onebyone-betterverlet-doublesqrt-msgmgmt

test-sync-bonds-onebyone-betterverlet-doublesqrt-msgmgmt-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-bonds-onebyone-betterverlet-doublesqrt-msgmgmt-reducedlocalcalcs-singleforceloop: test-sync-bonds-onebyone-betterverlet-doublesqrt-msgmgmt-reducedlocalcalcs

test-sync-bonds-onebyone-betterverlet-doublesqrt-msgmgmt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-bonds-onebyone-betterverlet-doublesqrt-msgmgmt-singleforceloop: test-sync-bonds-onebyone-betterverlet-doublesqrt-msgmgmt

test-sync-bonds-onebyone-betterverlet-doublesqrt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-sync-bonds-onebyone-betterverlet-doublesqrt-floatonly: test-sync-bonds-onebyone-betterverlet-doublesqrt

test-sync-bonds-onebyone-betterverlet-doublesqrt-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-sync-bonds-onebyone-betterverlet-doublesqrt-floatonly-reducedlocalcalcs: test-sync-bonds-onebyone-betterverlet-doublesqrt-floatonly

test-sync-bonds-onebyone-betterverlet-doublesqrt-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-bonds-onebyone-betterverlet-doublesqrt-floatonly-reducedlocalcalcs-singleforceloop: test-sync-bonds-onebyone-betterverlet-doublesqrt-floatonly-reducedlocalcalcs

test-sync-bonds-onebyone-betterverlet-doublesqrt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-bonds-onebyone-betterverlet-doublesqrt-floatonly-singleforceloop: test-sync-bonds-onebyone-betterverlet-doublesqrt-floatonly

test-sync-bonds-onebyone-betterverlet-doublesqrt-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-sync-bonds-onebyone-betterverlet-doublesqrt-reducedlocalcalcs: test-sync-bonds-onebyone-betterverlet-doublesqrt

test-sync-bonds-onebyone-betterverlet-doublesqrt-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-bonds-onebyone-betterverlet-doublesqrt-reducedlocalcalcs-singleforceloop: test-sync-bonds-onebyone-betterverlet-doublesqrt-reducedlocalcalcs

test-sync-bonds-onebyone-betterverlet-doublesqrt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-bonds-onebyone-betterverlet-doublesqrt-singleforceloop: test-sync-bonds-onebyone-betterverlet-doublesqrt

test-sync-bonds-onebyone-betterverlet-msgmgmt: DFLAGS+=-DMESSAGE_MANAGEMENT
test-sync-bonds-onebyone-betterverlet-msgmgmt: test-sync-bonds-onebyone-betterverlet

test-sync-bonds-onebyone-betterverlet-msgmgmt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-sync-bonds-onebyone-betterverlet-msgmgmt-floatonly: test-sync-bonds-onebyone-betterverlet-msgmgmt

test-sync-bonds-onebyone-betterverlet-msgmgmt-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-sync-bonds-onebyone-betterverlet-msgmgmt-floatonly-reducedlocalcalcs: test-sync-bonds-onebyone-betterverlet-msgmgmt-floatonly

test-sync-bonds-onebyone-betterverlet-msgmgmt-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-bonds-onebyone-betterverlet-msgmgmt-floatonly-reducedlocalcalcs-singleforceloop: test-sync-bonds-onebyone-betterverlet-msgmgmt-floatonly-reducedlocalcalcs

test-sync-bonds-onebyone-betterverlet-msgmgmt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-bonds-onebyone-betterverlet-msgmgmt-floatonly-singleforceloop: test-sync-bonds-onebyone-betterverlet-msgmgmt-floatonly

test-sync-bonds-onebyone-betterverlet-msgmgmt-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-sync-bonds-onebyone-betterverlet-msgmgmt-reducedlocalcalcs: test-sync-bonds-onebyone-betterverlet-msgmgmt

test-sync-bonds-onebyone-betterverlet-msgmgmt-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-bonds-onebyone-betterverlet-msgmgmt-reducedlocalcalcs-singleforceloop: test-sync-bonds-onebyone-betterverlet-msgmgmt-reducedlocalcalcs

test-sync-bonds-onebyone-betterverlet-msgmgmt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-bonds-onebyone-betterverlet-msgmgmt-singleforceloop: test-sync-bonds-onebyone-betterverlet-msgmgmt

test-sync-bonds-onebyone-betterverlet-floatonly: DFLAGS+=-DFLOAT_ONLY
test-sync-bonds-onebyone-betterverlet-floatonly: test-sync-bonds-onebyone-betterverlet

test-sync-bonds-onebyone-betterverlet-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-sync-bonds-onebyone-betterverlet-floatonly-reducedlocalcalcs: test-sync-bonds-onebyone-betterverlet-floatonly

test-sync-bonds-onebyone-betterverlet-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-bonds-onebyone-betterverlet-floatonly-reducedlocalcalcs-singleforceloop: test-sync-bonds-onebyone-betterverlet-floatonly-reducedlocalcalcs

test-sync-bonds-onebyone-betterverlet-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-bonds-onebyone-betterverlet-floatonly-singleforceloop: test-sync-bonds-onebyone-betterverlet-floatonly

test-sync-bonds-onebyone-betterverlet-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-sync-bonds-onebyone-betterverlet-reducedlocalcalcs: test-sync-bonds-onebyone-betterverlet

test-sync-bonds-onebyone-betterverlet-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-bonds-onebyone-betterverlet-reducedlocalcalcs-singleforceloop: test-sync-bonds-onebyone-betterverlet-reducedlocalcalcs

test-sync-bonds-onebyone-betterverlet-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-bonds-onebyone-betterverlet-singleforceloop: test-sync-bonds-onebyone-betterverlet

test-sync-bonds-onebyone-dtchange: DFLAGS+=-DSMALL_DT_EARLY
test-sync-bonds-onebyone-dtchange: test-sync-bonds-onebyone

test-sync-bonds-onebyone-dtchange-doublesqrt: DFLAGS+=-DOUBLE_SQRT
test-sync-bonds-onebyone-dtchange-doublesqrt: test-sync-bonds-onebyone-dtchange

test-sync-bonds-onebyone-dtchange-doublesqrt-msgmgmt: DFLAGS+=-DMESSAGE_MANAGEMENT
test-sync-bonds-onebyone-dtchange-doublesqrt-msgmgmt: test-sync-bonds-onebyone-dtchange-doublesqrt

test-sync-bonds-onebyone-dtchange-doublesqrt-msgmgmt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-sync-bonds-onebyone-dtchange-doublesqrt-msgmgmt-floatonly: test-sync-bonds-onebyone-dtchange-doublesqrt-msgmgmt

test-sync-bonds-onebyone-dtchange-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-sync-bonds-onebyone-dtchange-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs: test-sync-bonds-onebyone-dtchange-doublesqrt-msgmgmt-floatonly

test-sync-bonds-onebyone-dtchange-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-bonds-onebyone-dtchange-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs-singleforceloop: test-sync-bonds-onebyone-dtchange-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs

test-sync-bonds-onebyone-dtchange-doublesqrt-msgmgmt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-bonds-onebyone-dtchange-doublesqrt-msgmgmt-floatonly-singleforceloop: test-sync-bonds-onebyone-dtchange-doublesqrt-msgmgmt-floatonly

test-sync-bonds-onebyone-dtchange-doublesqrt-msgmgmt-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-sync-bonds-onebyone-dtchange-doublesqrt-msgmgmt-reducedlocalcalcs: test-sync-bonds-onebyone-dtchange-doublesqrt-msgmgmt

test-sync-bonds-onebyone-dtchange-doublesqrt-msgmgmt-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-bonds-onebyone-dtchange-doublesqrt-msgmgmt-reducedlocalcalcs-singleforceloop: test-sync-bonds-onebyone-dtchange-doublesqrt-msgmgmt-reducedlocalcalcs

test-sync-bonds-onebyone-dtchange-doublesqrt-msgmgmt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-bonds-onebyone-dtchange-doublesqrt-msgmgmt-singleforceloop: test-sync-bonds-onebyone-dtchange-doublesqrt-msgmgmt

test-sync-bonds-onebyone-dtchange-doublesqrt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-sync-bonds-onebyone-dtchange-doublesqrt-floatonly: test-sync-bonds-onebyone-dtchange-doublesqrt

test-sync-bonds-onebyone-dtchange-doublesqrt-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-sync-bonds-onebyone-dtchange-doublesqrt-floatonly-reducedlocalcalcs: test-sync-bonds-onebyone-dtchange-doublesqrt-floatonly

test-sync-bonds-onebyone-dtchange-doublesqrt-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-bonds-onebyone-dtchange-doublesqrt-floatonly-reducedlocalcalcs-singleforceloop: test-sync-bonds-onebyone-dtchange-doublesqrt-floatonly-reducedlocalcalcs

test-sync-bonds-onebyone-dtchange-doublesqrt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-bonds-onebyone-dtchange-doublesqrt-floatonly-singleforceloop: test-sync-bonds-onebyone-dtchange-doublesqrt-floatonly

test-sync-bonds-onebyone-dtchange-doublesqrt-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-sync-bonds-onebyone-dtchange-doublesqrt-reducedlocalcalcs: test-sync-bonds-onebyone-dtchange-doublesqrt

test-sync-bonds-onebyone-dtchange-doublesqrt-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-bonds-onebyone-dtchange-doublesqrt-reducedlocalcalcs-singleforceloop: test-sync-bonds-onebyone-dtchange-doublesqrt-reducedlocalcalcs

test-sync-bonds-onebyone-dtchange-doublesqrt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-bonds-onebyone-dtchange-doublesqrt-singleforceloop: test-sync-bonds-onebyone-dtchange-doublesqrt

test-sync-bonds-onebyone-dtchange-msgmgmt: DFLAGS+=-DMESSAGE_MANAGEMENT
test-sync-bonds-onebyone-dtchange-msgmgmt: test-sync-bonds-onebyone-dtchange

test-sync-bonds-onebyone-dtchange-msgmgmt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-sync-bonds-onebyone-dtchange-msgmgmt-floatonly: test-sync-bonds-onebyone-dtchange-msgmgmt

test-sync-bonds-onebyone-dtchange-msgmgmt-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-sync-bonds-onebyone-dtchange-msgmgmt-floatonly-reducedlocalcalcs: test-sync-bonds-onebyone-dtchange-msgmgmt-floatonly

test-sync-bonds-onebyone-dtchange-msgmgmt-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-bonds-onebyone-dtchange-msgmgmt-floatonly-reducedlocalcalcs-singleforceloop: test-sync-bonds-onebyone-dtchange-msgmgmt-floatonly-reducedlocalcalcs

test-sync-bonds-onebyone-dtchange-msgmgmt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-bonds-onebyone-dtchange-msgmgmt-floatonly-singleforceloop: test-sync-bonds-onebyone-dtchange-msgmgmt-floatonly

test-sync-bonds-onebyone-dtchange-msgmgmt-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-sync-bonds-onebyone-dtchange-msgmgmt-reducedlocalcalcs: test-sync-bonds-onebyone-dtchange-msgmgmt

test-sync-bonds-onebyone-dtchange-msgmgmt-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-bonds-onebyone-dtchange-msgmgmt-reducedlocalcalcs-singleforceloop: test-sync-bonds-onebyone-dtchange-msgmgmt-reducedlocalcalcs

test-sync-bonds-onebyone-dtchange-msgmgmt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-bonds-onebyone-dtchange-msgmgmt-singleforceloop: test-sync-bonds-onebyone-dtchange-msgmgmt

test-sync-bonds-onebyone-dtchange-floatonly: DFLAGS+=-DFLOAT_ONLY
test-sync-bonds-onebyone-dtchange-floatonly: test-sync-bonds-onebyone-dtchange

test-sync-bonds-onebyone-dtchange-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-sync-bonds-onebyone-dtchange-floatonly-reducedlocalcalcs: test-sync-bonds-onebyone-dtchange-floatonly

test-sync-bonds-onebyone-dtchange-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-bonds-onebyone-dtchange-floatonly-reducedlocalcalcs-singleforceloop: test-sync-bonds-onebyone-dtchange-floatonly-reducedlocalcalcs

test-sync-bonds-onebyone-dtchange-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-bonds-onebyone-dtchange-floatonly-singleforceloop: test-sync-bonds-onebyone-dtchange-floatonly

test-sync-bonds-onebyone-dtchange-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-sync-bonds-onebyone-dtchange-reducedlocalcalcs: test-sync-bonds-onebyone-dtchange

test-sync-bonds-onebyone-dtchange-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-bonds-onebyone-dtchange-reducedlocalcalcs-singleforceloop: test-sync-bonds-onebyone-dtchange-reducedlocalcalcs

test-sync-bonds-onebyone-dtchange-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-bonds-onebyone-dtchange-singleforceloop: test-sync-bonds-onebyone-dtchange

test-sync-bonds-onebyone-doublesqrt: DFLAGS+=-DOUBLE_SQRT
test-sync-bonds-onebyone-doublesqrt: test-sync-bonds-onebyone

test-sync-bonds-onebyone-doublesqrt-msgmgmt: DFLAGS+=-DMESSAGE_MANAGEMENT
test-sync-bonds-onebyone-doublesqrt-msgmgmt: test-sync-bonds-onebyone-doublesqrt

test-sync-bonds-onebyone-doublesqrt-msgmgmt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-sync-bonds-onebyone-doublesqrt-msgmgmt-floatonly: test-sync-bonds-onebyone-doublesqrt-msgmgmt

test-sync-bonds-onebyone-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-sync-bonds-onebyone-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs: test-sync-bonds-onebyone-doublesqrt-msgmgmt-floatonly

test-sync-bonds-onebyone-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-bonds-onebyone-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs-singleforceloop: test-sync-bonds-onebyone-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs

test-sync-bonds-onebyone-doublesqrt-msgmgmt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-bonds-onebyone-doublesqrt-msgmgmt-floatonly-singleforceloop: test-sync-bonds-onebyone-doublesqrt-msgmgmt-floatonly

test-sync-bonds-onebyone-doublesqrt-msgmgmt-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-sync-bonds-onebyone-doublesqrt-msgmgmt-reducedlocalcalcs: test-sync-bonds-onebyone-doublesqrt-msgmgmt

test-sync-bonds-onebyone-doublesqrt-msgmgmt-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-bonds-onebyone-doublesqrt-msgmgmt-reducedlocalcalcs-singleforceloop: test-sync-bonds-onebyone-doublesqrt-msgmgmt-reducedlocalcalcs

test-sync-bonds-onebyone-doublesqrt-msgmgmt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-bonds-onebyone-doublesqrt-msgmgmt-singleforceloop: test-sync-bonds-onebyone-doublesqrt-msgmgmt

test-sync-bonds-onebyone-doublesqrt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-sync-bonds-onebyone-doublesqrt-floatonly: test-sync-bonds-onebyone-doublesqrt

test-sync-bonds-onebyone-doublesqrt-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-sync-bonds-onebyone-doublesqrt-floatonly-reducedlocalcalcs: test-sync-bonds-onebyone-doublesqrt-floatonly

test-sync-bonds-onebyone-doublesqrt-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-bonds-onebyone-doublesqrt-floatonly-reducedlocalcalcs-singleforceloop: test-sync-bonds-onebyone-doublesqrt-floatonly-reducedlocalcalcs

test-sync-bonds-onebyone-doublesqrt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-bonds-onebyone-doublesqrt-floatonly-singleforceloop: test-sync-bonds-onebyone-doublesqrt-floatonly

test-sync-bonds-onebyone-doublesqrt-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-sync-bonds-onebyone-doublesqrt-reducedlocalcalcs: test-sync-bonds-onebyone-doublesqrt

test-sync-bonds-onebyone-doublesqrt-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-bonds-onebyone-doublesqrt-reducedlocalcalcs-singleforceloop: test-sync-bonds-onebyone-doublesqrt-reducedlocalcalcs

test-sync-bonds-onebyone-doublesqrt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-bonds-onebyone-doublesqrt-singleforceloop: test-sync-bonds-onebyone-doublesqrt

test-sync-bonds-onebyone-msgmgmt: DFLAGS+=-DMESSAGE_MANAGEMENT
test-sync-bonds-onebyone-msgmgmt: test-sync-bonds-onebyone

test-sync-bonds-onebyone-msgmgmt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-sync-bonds-onebyone-msgmgmt-floatonly: test-sync-bonds-onebyone-msgmgmt

test-sync-bonds-onebyone-msgmgmt-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-sync-bonds-onebyone-msgmgmt-floatonly-reducedlocalcalcs: test-sync-bonds-onebyone-msgmgmt-floatonly

test-sync-bonds-onebyone-msgmgmt-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-bonds-onebyone-msgmgmt-floatonly-reducedlocalcalcs-singleforceloop: test-sync-bonds-onebyone-msgmgmt-floatonly-reducedlocalcalcs

test-sync-bonds-onebyone-msgmgmt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-bonds-onebyone-msgmgmt-floatonly-singleforceloop: test-sync-bonds-onebyone-msgmgmt-floatonly

test-sync-bonds-onebyone-msgmgmt-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-sync-bonds-onebyone-msgmgmt-reducedlocalcalcs: test-sync-bonds-onebyone-msgmgmt

test-sync-bonds-onebyone-msgmgmt-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-bonds-onebyone-msgmgmt-reducedlocalcalcs-singleforceloop: test-sync-bonds-onebyone-msgmgmt-reducedlocalcalcs

test-sync-bonds-onebyone-msgmgmt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-bonds-onebyone-msgmgmt-singleforceloop: test-sync-bonds-onebyone-msgmgmt

test-sync-bonds-onebyone-floatonly: DFLAGS+=-DFLOAT_ONLY
test-sync-bonds-onebyone-floatonly: test-sync-bonds-onebyone

test-sync-bonds-onebyone-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-sync-bonds-onebyone-floatonly-reducedlocalcalcs: test-sync-bonds-onebyone-floatonly

test-sync-bonds-onebyone-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-bonds-onebyone-floatonly-reducedlocalcalcs-singleforceloop: test-sync-bonds-onebyone-floatonly-reducedlocalcalcs

test-sync-bonds-onebyone-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-bonds-onebyone-floatonly-singleforceloop: test-sync-bonds-onebyone-floatonly

test-sync-bonds-onebyone-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-sync-bonds-onebyone-reducedlocalcalcs: test-sync-bonds-onebyone

test-sync-bonds-onebyone-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-bonds-onebyone-reducedlocalcalcs-singleforceloop: test-sync-bonds-onebyone-reducedlocalcalcs

test-sync-bonds-onebyone-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-bonds-onebyone-singleforceloop: test-sync-bonds-onebyone

# **TEST Local calculation method: sendtoself**
test-sync-bonds-sendtoself: DFLAGS+=-DSEND_TO_SELF
test-sync-bonds-sendtoself: test-sync-bonds

test-sync-bonds-sendtoself-betterverlet: DFLAGS+=-DBETTER_VERLET
test-sync-bonds-sendtoself-betterverlet: test-sync-bonds-sendtoself

test-sync-bonds-sendtoself-betterverlet-dtchange: DFLAGS+=-DSMALL_DT_EARLY
test-sync-bonds-sendtoself-betterverlet-dtchange: test-sync-bonds-sendtoself-betterverlet

test-sync-bonds-sendtoself-betterverlet-dtchange-doublesqrt: DFLAGS+=-DOUBLE_SQRT
test-sync-bonds-sendtoself-betterverlet-dtchange-doublesqrt: test-sync-bonds-sendtoself-betterverlet-dtchange

test-sync-bonds-sendtoself-betterverlet-dtchange-doublesqrt-msgmgmt: DFLAGS+=-DMESSAGE_MANAGEMENT
test-sync-bonds-sendtoself-betterverlet-dtchange-doublesqrt-msgmgmt: test-sync-bonds-sendtoself-betterverlet-dtchange-doublesqrt

test-sync-bonds-sendtoself-betterverlet-dtchange-doublesqrt-msgmgmt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-sync-bonds-sendtoself-betterverlet-dtchange-doublesqrt-msgmgmt-floatonly: test-sync-bonds-sendtoself-betterverlet-dtchange-doublesqrt-msgmgmt

test-sync-bonds-sendtoself-betterverlet-dtchange-doublesqrt-msgmgmt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-bonds-sendtoself-betterverlet-dtchange-doublesqrt-msgmgmt-floatonly-singleforceloop: test-sync-bonds-sendtoself-betterverlet-dtchange-doublesqrt-msgmgmt-floatonly

test-sync-bonds-sendtoself-betterverlet-dtchange-doublesqrt-msgmgmt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-bonds-sendtoself-betterverlet-dtchange-doublesqrt-msgmgmt-singleforceloop: test-sync-bonds-sendtoself-betterverlet-dtchange-doublesqrt-msgmgmt

test-sync-bonds-sendtoself-betterverlet-dtchange-doublesqrt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-sync-bonds-sendtoself-betterverlet-dtchange-doublesqrt-floatonly: test-sync-bonds-sendtoself-betterverlet-dtchange-doublesqrt

test-sync-bonds-sendtoself-betterverlet-dtchange-doublesqrt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-bonds-sendtoself-betterverlet-dtchange-doublesqrt-floatonly-singleforceloop: test-sync-bonds-sendtoself-betterverlet-dtchange-doublesqrt-floatonly

test-sync-bonds-sendtoself-betterverlet-dtchange-doublesqrt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-bonds-sendtoself-betterverlet-dtchange-doublesqrt-singleforceloop: test-sync-bonds-sendtoself-betterverlet-dtchange-doublesqrt

test-sync-bonds-sendtoself-betterverlet-dtchange-msgmgmt: DFLAGS+=-DMESSAGE_MANAGEMENT
test-sync-bonds-sendtoself-betterverlet-dtchange-msgmgmt: test-sync-bonds-sendtoself-betterverlet-dtchange

test-sync-bonds-sendtoself-betterverlet-dtchange-msgmgmt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-sync-bonds-sendtoself-betterverlet-dtchange-msgmgmt-floatonly: test-sync-bonds-sendtoself-betterverlet-dtchange-msgmgmt

test-sync-bonds-sendtoself-betterverlet-dtchange-msgmgmt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-bonds-sendtoself-betterverlet-dtchange-msgmgmt-floatonly-singleforceloop: test-sync-bonds-sendtoself-betterverlet-dtchange-msgmgmt-floatonly

test-sync-bonds-sendtoself-betterverlet-dtchange-msgmgmt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-bonds-sendtoself-betterverlet-dtchange-msgmgmt-singleforceloop: test-sync-bonds-sendtoself-betterverlet-dtchange-msgmgmt

test-sync-bonds-sendtoself-betterverlet-dtchange-floatonly: DFLAGS+=-DFLOAT_ONLY
test-sync-bonds-sendtoself-betterverlet-dtchange-floatonly: test-sync-bonds-sendtoself-betterverlet-dtchange

test-sync-bonds-sendtoself-betterverlet-dtchange-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-bonds-sendtoself-betterverlet-dtchange-floatonly-singleforceloop: test-sync-bonds-sendtoself-betterverlet-dtchange-floatonly

test-sync-bonds-sendtoself-betterverlet-dtchange-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-bonds-sendtoself-betterverlet-dtchange-singleforceloop: test-sync-bonds-sendtoself-betterverlet-dtchange

test-sync-bonds-sendtoself-betterverlet-doublesqrt: DFLAGS+=-DOUBLE_SQRT
test-sync-bonds-sendtoself-betterverlet-doublesqrt: test-sync-bonds-sendtoself-betterverlet

test-sync-bonds-sendtoself-betterverlet-doublesqrt-msgmgmt: DFLAGS+=-DMESSAGE_MANAGEMENT
test-sync-bonds-sendtoself-betterverlet-doublesqrt-msgmgmt: test-sync-bonds-sendtoself-betterverlet-doublesqrt

test-sync-bonds-sendtoself-betterverlet-doublesqrt-msgmgmt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-sync-bonds-sendtoself-betterverlet-doublesqrt-msgmgmt-floatonly: test-sync-bonds-sendtoself-betterverlet-doublesqrt-msgmgmt

test-sync-bonds-sendtoself-betterverlet-doublesqrt-msgmgmt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-bonds-sendtoself-betterverlet-doublesqrt-msgmgmt-floatonly-singleforceloop: test-sync-bonds-sendtoself-betterverlet-doublesqrt-msgmgmt-floatonly

test-sync-bonds-sendtoself-betterverlet-doublesqrt-msgmgmt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-bonds-sendtoself-betterverlet-doublesqrt-msgmgmt-singleforceloop: test-sync-bonds-sendtoself-betterverlet-doublesqrt-msgmgmt

test-sync-bonds-sendtoself-betterverlet-doublesqrt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-sync-bonds-sendtoself-betterverlet-doublesqrt-floatonly: test-sync-bonds-sendtoself-betterverlet-doublesqrt

test-sync-bonds-sendtoself-betterverlet-doublesqrt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-bonds-sendtoself-betterverlet-doublesqrt-floatonly-singleforceloop: test-sync-bonds-sendtoself-betterverlet-doublesqrt-floatonly

test-sync-bonds-sendtoself-betterverlet-doublesqrt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-bonds-sendtoself-betterverlet-doublesqrt-singleforceloop: test-sync-bonds-sendtoself-betterverlet-doublesqrt

test-sync-bonds-sendtoself-betterverlet-msgmgmt: DFLAGS+=-DMESSAGE_MANAGEMENT
test-sync-bonds-sendtoself-betterverlet-msgmgmt: test-sync-bonds-sendtoself-betterverlet

test-sync-bonds-sendtoself-betterverlet-msgmgmt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-sync-bonds-sendtoself-betterverlet-msgmgmt-floatonly: test-sync-bonds-sendtoself-betterverlet-msgmgmt

test-sync-bonds-sendtoself-betterverlet-msgmgmt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-bonds-sendtoself-betterverlet-msgmgmt-floatonly-singleforceloop: test-sync-bonds-sendtoself-betterverlet-msgmgmt-floatonly

test-sync-bonds-sendtoself-betterverlet-msgmgmt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-bonds-sendtoself-betterverlet-msgmgmt-singleforceloop: test-sync-bonds-sendtoself-betterverlet-msgmgmt

test-sync-bonds-sendtoself-betterverlet-floatonly: DFLAGS+=-DFLOAT_ONLY
test-sync-bonds-sendtoself-betterverlet-floatonly: test-sync-bonds-sendtoself-betterverlet

test-sync-bonds-sendtoself-betterverlet-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-bonds-sendtoself-betterverlet-floatonly-singleforceloop: test-sync-bonds-sendtoself-betterverlet-floatonly

test-sync-bonds-sendtoself-betterverlet-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-bonds-sendtoself-betterverlet-singleforceloop: test-sync-bonds-sendtoself-betterverlet

test-sync-bonds-sendtoself-dtchange: DFLAGS+=-DSMALL_DT_EARLY
test-sync-bonds-sendtoself-dtchange: test-sync-bonds-sendtoself

test-sync-bonds-sendtoself-dtchange-doublesqrt: DFLAGS+=-DOUBLE_SQRT
test-sync-bonds-sendtoself-dtchange-doublesqrt: test-sync-bonds-sendtoself-dtchange

test-sync-bonds-sendtoself-dtchange-doublesqrt-msgmgmt: DFLAGS+=-DMESSAGE_MANAGEMENT
test-sync-bonds-sendtoself-dtchange-doublesqrt-msgmgmt: test-sync-bonds-sendtoself-dtchange-doublesqrt

test-sync-bonds-sendtoself-dtchange-doublesqrt-msgmgmt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-sync-bonds-sendtoself-dtchange-doublesqrt-msgmgmt-floatonly: test-sync-bonds-sendtoself-dtchange-doublesqrt-msgmgmt

test-sync-bonds-sendtoself-dtchange-doublesqrt-msgmgmt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-bonds-sendtoself-dtchange-doublesqrt-msgmgmt-floatonly-singleforceloop: test-sync-bonds-sendtoself-dtchange-doublesqrt-msgmgmt-floatonly

test-sync-bonds-sendtoself-dtchange-doublesqrt-msgmgmt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-bonds-sendtoself-dtchange-doublesqrt-msgmgmt-singleforceloop: test-sync-bonds-sendtoself-dtchange-doublesqrt-msgmgmt

test-sync-bonds-sendtoself-dtchange-doublesqrt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-sync-bonds-sendtoself-dtchange-doublesqrt-floatonly: test-sync-bonds-sendtoself-dtchange-doublesqrt

test-sync-bonds-sendtoself-dtchange-doublesqrt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-bonds-sendtoself-dtchange-doublesqrt-floatonly-singleforceloop: test-sync-bonds-sendtoself-dtchange-doublesqrt-floatonly

test-sync-bonds-sendtoself-dtchange-doublesqrt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-bonds-sendtoself-dtchange-doublesqrt-singleforceloop: test-sync-bonds-sendtoself-dtchange-doublesqrt

test-sync-bonds-sendtoself-dtchange-msgmgmt: DFLAGS+=-DMESSAGE_MANAGEMENT
test-sync-bonds-sendtoself-dtchange-msgmgmt: test-sync-bonds-sendtoself-dtchange

test-sync-bonds-sendtoself-dtchange-msgmgmt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-sync-bonds-sendtoself-dtchange-msgmgmt-floatonly: test-sync-bonds-sendtoself-dtchange-msgmgmt

test-sync-bonds-sendtoself-dtchange-msgmgmt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-bonds-sendtoself-dtchange-msgmgmt-floatonly-singleforceloop: test-sync-bonds-sendtoself-dtchange-msgmgmt-floatonly

test-sync-bonds-sendtoself-dtchange-msgmgmt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-bonds-sendtoself-dtchange-msgmgmt-singleforceloop: test-sync-bonds-sendtoself-dtchange-msgmgmt

test-sync-bonds-sendtoself-dtchange-floatonly: DFLAGS+=-DFLOAT_ONLY
test-sync-bonds-sendtoself-dtchange-floatonly: test-sync-bonds-sendtoself-dtchange

test-sync-bonds-sendtoself-dtchange-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-bonds-sendtoself-dtchange-floatonly-singleforceloop: test-sync-bonds-sendtoself-dtchange-floatonly

test-sync-bonds-sendtoself-dtchange-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-bonds-sendtoself-dtchange-singleforceloop: test-sync-bonds-sendtoself-dtchange

test-sync-bonds-sendtoself-doublesqrt: DFLAGS+=-DOUBLE_SQRT
test-sync-bonds-sendtoself-doublesqrt: test-sync-bonds-sendtoself

test-sync-bonds-sendtoself-doublesqrt-msgmgmt: DFLAGS+=-DMESSAGE_MANAGEMENT
test-sync-bonds-sendtoself-doublesqrt-msgmgmt: test-sync-bonds-sendtoself-doublesqrt

test-sync-bonds-sendtoself-doublesqrt-msgmgmt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-sync-bonds-sendtoself-doublesqrt-msgmgmt-floatonly: test-sync-bonds-sendtoself-doublesqrt-msgmgmt

test-sync-bonds-sendtoself-doublesqrt-msgmgmt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-bonds-sendtoself-doublesqrt-msgmgmt-floatonly-singleforceloop: test-sync-bonds-sendtoself-doublesqrt-msgmgmt-floatonly

test-sync-bonds-sendtoself-doublesqrt-msgmgmt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-bonds-sendtoself-doublesqrt-msgmgmt-singleforceloop: test-sync-bonds-sendtoself-doublesqrt-msgmgmt

test-sync-bonds-sendtoself-doublesqrt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-sync-bonds-sendtoself-doublesqrt-floatonly: test-sync-bonds-sendtoself-doublesqrt

test-sync-bonds-sendtoself-doublesqrt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-bonds-sendtoself-doublesqrt-floatonly-singleforceloop: test-sync-bonds-sendtoself-doublesqrt-floatonly

test-sync-bonds-sendtoself-doublesqrt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-bonds-sendtoself-doublesqrt-singleforceloop: test-sync-bonds-sendtoself-doublesqrt

test-sync-bonds-sendtoself-msgmgmt: DFLAGS+=-DMESSAGE_MANAGEMENT
test-sync-bonds-sendtoself-msgmgmt: test-sync-bonds-sendtoself

test-sync-bonds-sendtoself-msgmgmt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-sync-bonds-sendtoself-msgmgmt-floatonly: test-sync-bonds-sendtoself-msgmgmt

test-sync-bonds-sendtoself-msgmgmt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-bonds-sendtoself-msgmgmt-floatonly-singleforceloop: test-sync-bonds-sendtoself-msgmgmt-floatonly

test-sync-bonds-sendtoself-msgmgmt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-bonds-sendtoself-msgmgmt-singleforceloop: test-sync-bonds-sendtoself-msgmgmt

test-sync-bonds-sendtoself-floatonly: DFLAGS+=-DFLOAT_ONLY
test-sync-bonds-sendtoself-floatonly: test-sync-bonds-sendtoself

test-sync-bonds-sendtoself-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-bonds-sendtoself-floatonly-singleforceloop: test-sync-bonds-sendtoself-floatonly

test-sync-bonds-sendtoself-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-sync-bonds-sendtoself-singleforceloop: test-sync-bonds-sendtoself


# ************** TEST Simulator: gals**************

test-gals: OBJS=$(POLITE_OBJS)
test-gals: DFLAGS+=-DTESTING -DGALS
test-gals: base-gals test

test-gals-betterverlet: DFLAGS+=-DBETTER_VERLET
test-gals-betterverlet: test-gals

test-gals-betterverlet-dtchange: DFLAGS+=-DSMALL_DT_EARLY
test-gals-betterverlet-dtchange: test-gals-betterverlet

test-gals-betterverlet-dtchange-doublesqrt: DFLAGS+=-DOUBLE_SQRT
test-gals-betterverlet-dtchange-doublesqrt: test-gals-betterverlet-dtchange

test-gals-betterverlet-dtchange-doublesqrt-msgmgmt: DFLAGS+=-DMESSAGE_MANAGEMENT
test-gals-betterverlet-dtchange-doublesqrt-msgmgmt: test-gals-betterverlet-dtchange-doublesqrt

test-gals-betterverlet-dtchange-doublesqrt-msgmgmt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-gals-betterverlet-dtchange-doublesqrt-msgmgmt-floatonly: test-gals-betterverlet-dtchange-doublesqrt-msgmgmt

test-gals-betterverlet-dtchange-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-gals-betterverlet-dtchange-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs: test-gals-betterverlet-dtchange-doublesqrt-msgmgmt-floatonly

test-gals-betterverlet-dtchange-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-betterverlet-dtchange-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs-singleforceloop: test-gals-betterverlet-dtchange-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs

test-gals-betterverlet-dtchange-doublesqrt-msgmgmt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-betterverlet-dtchange-doublesqrt-msgmgmt-floatonly-singleforceloop: test-gals-betterverlet-dtchange-doublesqrt-msgmgmt-floatonly

test-gals-betterverlet-dtchange-doublesqrt-msgmgmt-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-gals-betterverlet-dtchange-doublesqrt-msgmgmt-reducedlocalcalcs: test-gals-betterverlet-dtchange-doublesqrt-msgmgmt

test-gals-betterverlet-dtchange-doublesqrt-msgmgmt-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-betterverlet-dtchange-doublesqrt-msgmgmt-reducedlocalcalcs-singleforceloop: test-gals-betterverlet-dtchange-doublesqrt-msgmgmt-reducedlocalcalcs

test-gals-betterverlet-dtchange-doublesqrt-msgmgmt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-betterverlet-dtchange-doublesqrt-msgmgmt-singleforceloop: test-gals-betterverlet-dtchange-doublesqrt-msgmgmt

test-gals-betterverlet-dtchange-doublesqrt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-gals-betterverlet-dtchange-doublesqrt-floatonly: test-gals-betterverlet-dtchange-doublesqrt

test-gals-betterverlet-dtchange-doublesqrt-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-gals-betterverlet-dtchange-doublesqrt-floatonly-reducedlocalcalcs: test-gals-betterverlet-dtchange-doublesqrt-floatonly

test-gals-betterverlet-dtchange-doublesqrt-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-betterverlet-dtchange-doublesqrt-floatonly-reducedlocalcalcs-singleforceloop: test-gals-betterverlet-dtchange-doublesqrt-floatonly-reducedlocalcalcs

test-gals-betterverlet-dtchange-doublesqrt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-betterverlet-dtchange-doublesqrt-floatonly-singleforceloop: test-gals-betterverlet-dtchange-doublesqrt-floatonly

test-gals-betterverlet-dtchange-doublesqrt-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-gals-betterverlet-dtchange-doublesqrt-reducedlocalcalcs: test-gals-betterverlet-dtchange-doublesqrt

test-gals-betterverlet-dtchange-doublesqrt-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-betterverlet-dtchange-doublesqrt-reducedlocalcalcs-singleforceloop: test-gals-betterverlet-dtchange-doublesqrt-reducedlocalcalcs

test-gals-betterverlet-dtchange-doublesqrt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-betterverlet-dtchange-doublesqrt-singleforceloop: test-gals-betterverlet-dtchange-doublesqrt

test-gals-betterverlet-dtchange-msgmgmt: DFLAGS+=-DMESSAGE_MANAGEMENT
test-gals-betterverlet-dtchange-msgmgmt: test-gals-betterverlet-dtchange

test-gals-betterverlet-dtchange-msgmgmt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-gals-betterverlet-dtchange-msgmgmt-floatonly: test-gals-betterverlet-dtchange-msgmgmt

test-gals-betterverlet-dtchange-msgmgmt-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-gals-betterverlet-dtchange-msgmgmt-floatonly-reducedlocalcalcs: test-gals-betterverlet-dtchange-msgmgmt-floatonly

test-gals-betterverlet-dtchange-msgmgmt-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-betterverlet-dtchange-msgmgmt-floatonly-reducedlocalcalcs-singleforceloop: test-gals-betterverlet-dtchange-msgmgmt-floatonly-reducedlocalcalcs

test-gals-betterverlet-dtchange-msgmgmt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-betterverlet-dtchange-msgmgmt-floatonly-singleforceloop: test-gals-betterverlet-dtchange-msgmgmt-floatonly

test-gals-betterverlet-dtchange-msgmgmt-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-gals-betterverlet-dtchange-msgmgmt-reducedlocalcalcs: test-gals-betterverlet-dtchange-msgmgmt

test-gals-betterverlet-dtchange-msgmgmt-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-betterverlet-dtchange-msgmgmt-reducedlocalcalcs-singleforceloop: test-gals-betterverlet-dtchange-msgmgmt-reducedlocalcalcs

test-gals-betterverlet-dtchange-msgmgmt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-betterverlet-dtchange-msgmgmt-singleforceloop: test-gals-betterverlet-dtchange-msgmgmt

test-gals-betterverlet-dtchange-floatonly: DFLAGS+=-DFLOAT_ONLY
test-gals-betterverlet-dtchange-floatonly: test-gals-betterverlet-dtchange

test-gals-betterverlet-dtchange-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-gals-betterverlet-dtchange-floatonly-reducedlocalcalcs: test-gals-betterverlet-dtchange-floatonly

test-gals-betterverlet-dtchange-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-betterverlet-dtchange-floatonly-reducedlocalcalcs-singleforceloop: test-gals-betterverlet-dtchange-floatonly-reducedlocalcalcs

test-gals-betterverlet-dtchange-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-betterverlet-dtchange-floatonly-singleforceloop: test-gals-betterverlet-dtchange-floatonly

test-gals-betterverlet-dtchange-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-gals-betterverlet-dtchange-reducedlocalcalcs: test-gals-betterverlet-dtchange

test-gals-betterverlet-dtchange-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-betterverlet-dtchange-reducedlocalcalcs-singleforceloop: test-gals-betterverlet-dtchange-reducedlocalcalcs

test-gals-betterverlet-dtchange-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-betterverlet-dtchange-singleforceloop: test-gals-betterverlet-dtchange

test-gals-betterverlet-doublesqrt: DFLAGS+=-DOUBLE_SQRT
test-gals-betterverlet-doublesqrt: test-gals-betterverlet

test-gals-betterverlet-doublesqrt-msgmgmt: DFLAGS+=-DMESSAGE_MANAGEMENT
test-gals-betterverlet-doublesqrt-msgmgmt: test-gals-betterverlet-doublesqrt

test-gals-betterverlet-doublesqrt-msgmgmt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-gals-betterverlet-doublesqrt-msgmgmt-floatonly: test-gals-betterverlet-doublesqrt-msgmgmt

test-gals-betterverlet-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-gals-betterverlet-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs: test-gals-betterverlet-doublesqrt-msgmgmt-floatonly

test-gals-betterverlet-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-betterverlet-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs-singleforceloop: test-gals-betterverlet-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs

test-gals-betterverlet-doublesqrt-msgmgmt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-betterverlet-doublesqrt-msgmgmt-floatonly-singleforceloop: test-gals-betterverlet-doublesqrt-msgmgmt-floatonly

test-gals-betterverlet-doublesqrt-msgmgmt-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-gals-betterverlet-doublesqrt-msgmgmt-reducedlocalcalcs: test-gals-betterverlet-doublesqrt-msgmgmt

test-gals-betterverlet-doublesqrt-msgmgmt-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-betterverlet-doublesqrt-msgmgmt-reducedlocalcalcs-singleforceloop: test-gals-betterverlet-doublesqrt-msgmgmt-reducedlocalcalcs

test-gals-betterverlet-doublesqrt-msgmgmt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-betterverlet-doublesqrt-msgmgmt-singleforceloop: test-gals-betterverlet-doublesqrt-msgmgmt

test-gals-betterverlet-doublesqrt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-gals-betterverlet-doublesqrt-floatonly: test-gals-betterverlet-doublesqrt

test-gals-betterverlet-doublesqrt-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-gals-betterverlet-doublesqrt-floatonly-reducedlocalcalcs: test-gals-betterverlet-doublesqrt-floatonly

test-gals-betterverlet-doublesqrt-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-betterverlet-doublesqrt-floatonly-reducedlocalcalcs-singleforceloop: test-gals-betterverlet-doublesqrt-floatonly-reducedlocalcalcs

test-gals-betterverlet-doublesqrt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-betterverlet-doublesqrt-floatonly-singleforceloop: test-gals-betterverlet-doublesqrt-floatonly

test-gals-betterverlet-doublesqrt-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-gals-betterverlet-doublesqrt-reducedlocalcalcs: test-gals-betterverlet-doublesqrt

test-gals-betterverlet-doublesqrt-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-betterverlet-doublesqrt-reducedlocalcalcs-singleforceloop: test-gals-betterverlet-doublesqrt-reducedlocalcalcs

test-gals-betterverlet-doublesqrt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-betterverlet-doublesqrt-singleforceloop: test-gals-betterverlet-doublesqrt

test-gals-betterverlet-msgmgmt: DFLAGS+=-DMESSAGE_MANAGEMENT
test-gals-betterverlet-msgmgmt: test-gals-betterverlet

test-gals-betterverlet-msgmgmt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-gals-betterverlet-msgmgmt-floatonly: test-gals-betterverlet-msgmgmt

test-gals-betterverlet-msgmgmt-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-gals-betterverlet-msgmgmt-floatonly-reducedlocalcalcs: test-gals-betterverlet-msgmgmt-floatonly

test-gals-betterverlet-msgmgmt-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-betterverlet-msgmgmt-floatonly-reducedlocalcalcs-singleforceloop: test-gals-betterverlet-msgmgmt-floatonly-reducedlocalcalcs

test-gals-betterverlet-msgmgmt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-betterverlet-msgmgmt-floatonly-singleforceloop: test-gals-betterverlet-msgmgmt-floatonly

test-gals-betterverlet-msgmgmt-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-gals-betterverlet-msgmgmt-reducedlocalcalcs: test-gals-betterverlet-msgmgmt

test-gals-betterverlet-msgmgmt-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-betterverlet-msgmgmt-reducedlocalcalcs-singleforceloop: test-gals-betterverlet-msgmgmt-reducedlocalcalcs

test-gals-betterverlet-msgmgmt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-betterverlet-msgmgmt-singleforceloop: test-gals-betterverlet-msgmgmt

test-gals-betterverlet-floatonly: DFLAGS+=-DFLOAT_ONLY
test-gals-betterverlet-floatonly: test-gals-betterverlet

test-gals-betterverlet-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-gals-betterverlet-floatonly-reducedlocalcalcs: test-gals-betterverlet-floatonly

test-gals-betterverlet-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-betterverlet-floatonly-reducedlocalcalcs-singleforceloop: test-gals-betterverlet-floatonly-reducedlocalcalcs

test-gals-betterverlet-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-betterverlet-floatonly-singleforceloop: test-gals-betterverlet-floatonly

test-gals-betterverlet-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-gals-betterverlet-reducedlocalcalcs: test-gals-betterverlet

test-gals-betterverlet-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-betterverlet-reducedlocalcalcs-singleforceloop: test-gals-betterverlet-reducedlocalcalcs

test-gals-betterverlet-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-betterverlet-singleforceloop: test-gals-betterverlet

test-gals-dtchange: DFLAGS+=-DSMALL_DT_EARLY
test-gals-dtchange: test-gals

test-gals-dtchange-doublesqrt: DFLAGS+=-DOUBLE_SQRT
test-gals-dtchange-doublesqrt: test-gals-dtchange

test-gals-dtchange-doublesqrt-msgmgmt: DFLAGS+=-DMESSAGE_MANAGEMENT
test-gals-dtchange-doublesqrt-msgmgmt: test-gals-dtchange-doublesqrt

test-gals-dtchange-doublesqrt-msgmgmt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-gals-dtchange-doublesqrt-msgmgmt-floatonly: test-gals-dtchange-doublesqrt-msgmgmt

test-gals-dtchange-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-gals-dtchange-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs: test-gals-dtchange-doublesqrt-msgmgmt-floatonly

test-gals-dtchange-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-dtchange-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs-singleforceloop: test-gals-dtchange-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs

test-gals-dtchange-doublesqrt-msgmgmt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-dtchange-doublesqrt-msgmgmt-floatonly-singleforceloop: test-gals-dtchange-doublesqrt-msgmgmt-floatonly

test-gals-dtchange-doublesqrt-msgmgmt-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-gals-dtchange-doublesqrt-msgmgmt-reducedlocalcalcs: test-gals-dtchange-doublesqrt-msgmgmt

test-gals-dtchange-doublesqrt-msgmgmt-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-dtchange-doublesqrt-msgmgmt-reducedlocalcalcs-singleforceloop: test-gals-dtchange-doublesqrt-msgmgmt-reducedlocalcalcs

test-gals-dtchange-doublesqrt-msgmgmt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-dtchange-doublesqrt-msgmgmt-singleforceloop: test-gals-dtchange-doublesqrt-msgmgmt

test-gals-dtchange-doublesqrt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-gals-dtchange-doublesqrt-floatonly: test-gals-dtchange-doublesqrt

test-gals-dtchange-doublesqrt-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-gals-dtchange-doublesqrt-floatonly-reducedlocalcalcs: test-gals-dtchange-doublesqrt-floatonly

test-gals-dtchange-doublesqrt-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-dtchange-doublesqrt-floatonly-reducedlocalcalcs-singleforceloop: test-gals-dtchange-doublesqrt-floatonly-reducedlocalcalcs

test-gals-dtchange-doublesqrt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-dtchange-doublesqrt-floatonly-singleforceloop: test-gals-dtchange-doublesqrt-floatonly

test-gals-dtchange-doublesqrt-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-gals-dtchange-doublesqrt-reducedlocalcalcs: test-gals-dtchange-doublesqrt

test-gals-dtchange-doublesqrt-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-dtchange-doublesqrt-reducedlocalcalcs-singleforceloop: test-gals-dtchange-doublesqrt-reducedlocalcalcs

test-gals-dtchange-doublesqrt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-dtchange-doublesqrt-singleforceloop: test-gals-dtchange-doublesqrt

test-gals-dtchange-msgmgmt: DFLAGS+=-DMESSAGE_MANAGEMENT
test-gals-dtchange-msgmgmt: test-gals-dtchange

test-gals-dtchange-msgmgmt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-gals-dtchange-msgmgmt-floatonly: test-gals-dtchange-msgmgmt

test-gals-dtchange-msgmgmt-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-gals-dtchange-msgmgmt-floatonly-reducedlocalcalcs: test-gals-dtchange-msgmgmt-floatonly

test-gals-dtchange-msgmgmt-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-dtchange-msgmgmt-floatonly-reducedlocalcalcs-singleforceloop: test-gals-dtchange-msgmgmt-floatonly-reducedlocalcalcs

test-gals-dtchange-msgmgmt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-dtchange-msgmgmt-floatonly-singleforceloop: test-gals-dtchange-msgmgmt-floatonly

test-gals-dtchange-msgmgmt-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-gals-dtchange-msgmgmt-reducedlocalcalcs: test-gals-dtchange-msgmgmt

test-gals-dtchange-msgmgmt-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-dtchange-msgmgmt-reducedlocalcalcs-singleforceloop: test-gals-dtchange-msgmgmt-reducedlocalcalcs

test-gals-dtchange-msgmgmt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-dtchange-msgmgmt-singleforceloop: test-gals-dtchange-msgmgmt

test-gals-dtchange-floatonly: DFLAGS+=-DFLOAT_ONLY
test-gals-dtchange-floatonly: test-gals-dtchange

test-gals-dtchange-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-gals-dtchange-floatonly-reducedlocalcalcs: test-gals-dtchange-floatonly

test-gals-dtchange-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-dtchange-floatonly-reducedlocalcalcs-singleforceloop: test-gals-dtchange-floatonly-reducedlocalcalcs

test-gals-dtchange-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-dtchange-floatonly-singleforceloop: test-gals-dtchange-floatonly

test-gals-dtchange-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-gals-dtchange-reducedlocalcalcs: test-gals-dtchange

test-gals-dtchange-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-dtchange-reducedlocalcalcs-singleforceloop: test-gals-dtchange-reducedlocalcalcs

test-gals-dtchange-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-dtchange-singleforceloop: test-gals-dtchange

test-gals-doublesqrt: DFLAGS+=-DOUBLE_SQRT
test-gals-doublesqrt: test-gals

test-gals-doublesqrt-msgmgmt: DFLAGS+=-DMESSAGE_MANAGEMENT
test-gals-doublesqrt-msgmgmt: test-gals-doublesqrt

test-gals-doublesqrt-msgmgmt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-gals-doublesqrt-msgmgmt-floatonly: test-gals-doublesqrt-msgmgmt

test-gals-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-gals-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs: test-gals-doublesqrt-msgmgmt-floatonly

test-gals-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs-singleforceloop: test-gals-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs

test-gals-doublesqrt-msgmgmt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-doublesqrt-msgmgmt-floatonly-singleforceloop: test-gals-doublesqrt-msgmgmt-floatonly

test-gals-doublesqrt-msgmgmt-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-gals-doublesqrt-msgmgmt-reducedlocalcalcs: test-gals-doublesqrt-msgmgmt

test-gals-doublesqrt-msgmgmt-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-doublesqrt-msgmgmt-reducedlocalcalcs-singleforceloop: test-gals-doublesqrt-msgmgmt-reducedlocalcalcs

test-gals-doublesqrt-msgmgmt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-doublesqrt-msgmgmt-singleforceloop: test-gals-doublesqrt-msgmgmt

test-gals-doublesqrt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-gals-doublesqrt-floatonly: test-gals-doublesqrt

test-gals-doublesqrt-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-gals-doublesqrt-floatonly-reducedlocalcalcs: test-gals-doublesqrt-floatonly

test-gals-doublesqrt-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-doublesqrt-floatonly-reducedlocalcalcs-singleforceloop: test-gals-doublesqrt-floatonly-reducedlocalcalcs

test-gals-doublesqrt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-doublesqrt-floatonly-singleforceloop: test-gals-doublesqrt-floatonly

test-gals-doublesqrt-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-gals-doublesqrt-reducedlocalcalcs: test-gals-doublesqrt

test-gals-doublesqrt-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-doublesqrt-reducedlocalcalcs-singleforceloop: test-gals-doublesqrt-reducedlocalcalcs

test-gals-doublesqrt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-doublesqrt-singleforceloop: test-gals-doublesqrt

test-gals-msgmgmt: DFLAGS+=-DMESSAGE_MANAGEMENT
test-gals-msgmgmt: test-gals

test-gals-msgmgmt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-gals-msgmgmt-floatonly: test-gals-msgmgmt

test-gals-msgmgmt-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-gals-msgmgmt-floatonly-reducedlocalcalcs: test-gals-msgmgmt-floatonly

test-gals-msgmgmt-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-msgmgmt-floatonly-reducedlocalcalcs-singleforceloop: test-gals-msgmgmt-floatonly-reducedlocalcalcs

test-gals-msgmgmt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-msgmgmt-floatonly-singleforceloop: test-gals-msgmgmt-floatonly

test-gals-msgmgmt-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-gals-msgmgmt-reducedlocalcalcs: test-gals-msgmgmt

test-gals-msgmgmt-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-msgmgmt-reducedlocalcalcs-singleforceloop: test-gals-msgmgmt-reducedlocalcalcs

test-gals-msgmgmt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-msgmgmt-singleforceloop: test-gals-msgmgmt

test-gals-floatonly: DFLAGS+=-DFLOAT_ONLY
test-gals-floatonly: test-gals

test-gals-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-gals-floatonly-reducedlocalcalcs: test-gals-floatonly

test-gals-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-floatonly-reducedlocalcalcs-singleforceloop: test-gals-floatonly-reducedlocalcalcs

test-gals-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-floatonly-singleforceloop: test-gals-floatonly

test-gals-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-gals-reducedlocalcalcs: test-gals

test-gals-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-reducedlocalcalcs-singleforceloop: test-gals-reducedlocalcalcs

test-gals-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-singleforceloop: test-gals

# **TEST Local calculation method: onebyone**
test-gals-onebyone: DFLAGS+=-DONE_BY_ONE
test-gals-onebyone: test-gals

test-gals-onebyone-betterverlet: DFLAGS+=-DBETTER_VERLET
test-gals-onebyone-betterverlet: test-gals-onebyone

test-gals-onebyone-betterverlet-dtchange: DFLAGS+=-DSMALL_DT_EARLY
test-gals-onebyone-betterverlet-dtchange: test-gals-onebyone-betterverlet

test-gals-onebyone-betterverlet-dtchange-doublesqrt: DFLAGS+=-DOUBLE_SQRT
test-gals-onebyone-betterverlet-dtchange-doublesqrt: test-gals-onebyone-betterverlet-dtchange

test-gals-onebyone-betterverlet-dtchange-doublesqrt-msgmgmt: DFLAGS+=-DMESSAGE_MANAGEMENT
test-gals-onebyone-betterverlet-dtchange-doublesqrt-msgmgmt: test-gals-onebyone-betterverlet-dtchange-doublesqrt

test-gals-onebyone-betterverlet-dtchange-doublesqrt-msgmgmt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-gals-onebyone-betterverlet-dtchange-doublesqrt-msgmgmt-floatonly: test-gals-onebyone-betterverlet-dtchange-doublesqrt-msgmgmt

test-gals-onebyone-betterverlet-dtchange-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-gals-onebyone-betterverlet-dtchange-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs: test-gals-onebyone-betterverlet-dtchange-doublesqrt-msgmgmt-floatonly

test-gals-onebyone-betterverlet-dtchange-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-onebyone-betterverlet-dtchange-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs-singleforceloop: test-gals-onebyone-betterverlet-dtchange-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs

test-gals-onebyone-betterverlet-dtchange-doublesqrt-msgmgmt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-onebyone-betterverlet-dtchange-doublesqrt-msgmgmt-floatonly-singleforceloop: test-gals-onebyone-betterverlet-dtchange-doublesqrt-msgmgmt-floatonly

test-gals-onebyone-betterverlet-dtchange-doublesqrt-msgmgmt-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-gals-onebyone-betterverlet-dtchange-doublesqrt-msgmgmt-reducedlocalcalcs: test-gals-onebyone-betterverlet-dtchange-doublesqrt-msgmgmt

test-gals-onebyone-betterverlet-dtchange-doublesqrt-msgmgmt-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-onebyone-betterverlet-dtchange-doublesqrt-msgmgmt-reducedlocalcalcs-singleforceloop: test-gals-onebyone-betterverlet-dtchange-doublesqrt-msgmgmt-reducedlocalcalcs

test-gals-onebyone-betterverlet-dtchange-doublesqrt-msgmgmt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-onebyone-betterverlet-dtchange-doublesqrt-msgmgmt-singleforceloop: test-gals-onebyone-betterverlet-dtchange-doublesqrt-msgmgmt

test-gals-onebyone-betterverlet-dtchange-doublesqrt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-gals-onebyone-betterverlet-dtchange-doublesqrt-floatonly: test-gals-onebyone-betterverlet-dtchange-doublesqrt

test-gals-onebyone-betterverlet-dtchange-doublesqrt-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-gals-onebyone-betterverlet-dtchange-doublesqrt-floatonly-reducedlocalcalcs: test-gals-onebyone-betterverlet-dtchange-doublesqrt-floatonly

test-gals-onebyone-betterverlet-dtchange-doublesqrt-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-onebyone-betterverlet-dtchange-doublesqrt-floatonly-reducedlocalcalcs-singleforceloop: test-gals-onebyone-betterverlet-dtchange-doublesqrt-floatonly-reducedlocalcalcs

test-gals-onebyone-betterverlet-dtchange-doublesqrt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-onebyone-betterverlet-dtchange-doublesqrt-floatonly-singleforceloop: test-gals-onebyone-betterverlet-dtchange-doublesqrt-floatonly

test-gals-onebyone-betterverlet-dtchange-doublesqrt-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-gals-onebyone-betterverlet-dtchange-doublesqrt-reducedlocalcalcs: test-gals-onebyone-betterverlet-dtchange-doublesqrt

test-gals-onebyone-betterverlet-dtchange-doublesqrt-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-onebyone-betterverlet-dtchange-doublesqrt-reducedlocalcalcs-singleforceloop: test-gals-onebyone-betterverlet-dtchange-doublesqrt-reducedlocalcalcs

test-gals-onebyone-betterverlet-dtchange-doublesqrt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-onebyone-betterverlet-dtchange-doublesqrt-singleforceloop: test-gals-onebyone-betterverlet-dtchange-doublesqrt

test-gals-onebyone-betterverlet-dtchange-msgmgmt: DFLAGS+=-DMESSAGE_MANAGEMENT
test-gals-onebyone-betterverlet-dtchange-msgmgmt: test-gals-onebyone-betterverlet-dtchange

test-gals-onebyone-betterverlet-dtchange-msgmgmt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-gals-onebyone-betterverlet-dtchange-msgmgmt-floatonly: test-gals-onebyone-betterverlet-dtchange-msgmgmt

test-gals-onebyone-betterverlet-dtchange-msgmgmt-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-gals-onebyone-betterverlet-dtchange-msgmgmt-floatonly-reducedlocalcalcs: test-gals-onebyone-betterverlet-dtchange-msgmgmt-floatonly

test-gals-onebyone-betterverlet-dtchange-msgmgmt-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-onebyone-betterverlet-dtchange-msgmgmt-floatonly-reducedlocalcalcs-singleforceloop: test-gals-onebyone-betterverlet-dtchange-msgmgmt-floatonly-reducedlocalcalcs

test-gals-onebyone-betterverlet-dtchange-msgmgmt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-onebyone-betterverlet-dtchange-msgmgmt-floatonly-singleforceloop: test-gals-onebyone-betterverlet-dtchange-msgmgmt-floatonly

test-gals-onebyone-betterverlet-dtchange-msgmgmt-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-gals-onebyone-betterverlet-dtchange-msgmgmt-reducedlocalcalcs: test-gals-onebyone-betterverlet-dtchange-msgmgmt

test-gals-onebyone-betterverlet-dtchange-msgmgmt-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-onebyone-betterverlet-dtchange-msgmgmt-reducedlocalcalcs-singleforceloop: test-gals-onebyone-betterverlet-dtchange-msgmgmt-reducedlocalcalcs

test-gals-onebyone-betterverlet-dtchange-msgmgmt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-onebyone-betterverlet-dtchange-msgmgmt-singleforceloop: test-gals-onebyone-betterverlet-dtchange-msgmgmt

test-gals-onebyone-betterverlet-dtchange-floatonly: DFLAGS+=-DFLOAT_ONLY
test-gals-onebyone-betterverlet-dtchange-floatonly: test-gals-onebyone-betterverlet-dtchange

test-gals-onebyone-betterverlet-dtchange-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-gals-onebyone-betterverlet-dtchange-floatonly-reducedlocalcalcs: test-gals-onebyone-betterverlet-dtchange-floatonly

test-gals-onebyone-betterverlet-dtchange-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-onebyone-betterverlet-dtchange-floatonly-reducedlocalcalcs-singleforceloop: test-gals-onebyone-betterverlet-dtchange-floatonly-reducedlocalcalcs

test-gals-onebyone-betterverlet-dtchange-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-onebyone-betterverlet-dtchange-floatonly-singleforceloop: test-gals-onebyone-betterverlet-dtchange-floatonly

test-gals-onebyone-betterverlet-dtchange-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-gals-onebyone-betterverlet-dtchange-reducedlocalcalcs: test-gals-onebyone-betterverlet-dtchange

test-gals-onebyone-betterverlet-dtchange-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-onebyone-betterverlet-dtchange-reducedlocalcalcs-singleforceloop: test-gals-onebyone-betterverlet-dtchange-reducedlocalcalcs

test-gals-onebyone-betterverlet-dtchange-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-onebyone-betterverlet-dtchange-singleforceloop: test-gals-onebyone-betterverlet-dtchange

test-gals-onebyone-betterverlet-doublesqrt: DFLAGS+=-DOUBLE_SQRT
test-gals-onebyone-betterverlet-doublesqrt: test-gals-onebyone-betterverlet

test-gals-onebyone-betterverlet-doublesqrt-msgmgmt: DFLAGS+=-DMESSAGE_MANAGEMENT
test-gals-onebyone-betterverlet-doublesqrt-msgmgmt: test-gals-onebyone-betterverlet-doublesqrt

test-gals-onebyone-betterverlet-doublesqrt-msgmgmt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-gals-onebyone-betterverlet-doublesqrt-msgmgmt-floatonly: test-gals-onebyone-betterverlet-doublesqrt-msgmgmt

test-gals-onebyone-betterverlet-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-gals-onebyone-betterverlet-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs: test-gals-onebyone-betterverlet-doublesqrt-msgmgmt-floatonly

test-gals-onebyone-betterverlet-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-onebyone-betterverlet-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs-singleforceloop: test-gals-onebyone-betterverlet-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs

test-gals-onebyone-betterverlet-doublesqrt-msgmgmt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-onebyone-betterverlet-doublesqrt-msgmgmt-floatonly-singleforceloop: test-gals-onebyone-betterverlet-doublesqrt-msgmgmt-floatonly

test-gals-onebyone-betterverlet-doublesqrt-msgmgmt-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-gals-onebyone-betterverlet-doublesqrt-msgmgmt-reducedlocalcalcs: test-gals-onebyone-betterverlet-doublesqrt-msgmgmt

test-gals-onebyone-betterverlet-doublesqrt-msgmgmt-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-onebyone-betterverlet-doublesqrt-msgmgmt-reducedlocalcalcs-singleforceloop: test-gals-onebyone-betterverlet-doublesqrt-msgmgmt-reducedlocalcalcs

test-gals-onebyone-betterverlet-doublesqrt-msgmgmt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-onebyone-betterverlet-doublesqrt-msgmgmt-singleforceloop: test-gals-onebyone-betterverlet-doublesqrt-msgmgmt

test-gals-onebyone-betterverlet-doublesqrt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-gals-onebyone-betterverlet-doublesqrt-floatonly: test-gals-onebyone-betterverlet-doublesqrt

test-gals-onebyone-betterverlet-doublesqrt-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-gals-onebyone-betterverlet-doublesqrt-floatonly-reducedlocalcalcs: test-gals-onebyone-betterverlet-doublesqrt-floatonly

test-gals-onebyone-betterverlet-doublesqrt-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-onebyone-betterverlet-doublesqrt-floatonly-reducedlocalcalcs-singleforceloop: test-gals-onebyone-betterverlet-doublesqrt-floatonly-reducedlocalcalcs

test-gals-onebyone-betterverlet-doublesqrt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-onebyone-betterverlet-doublesqrt-floatonly-singleforceloop: test-gals-onebyone-betterverlet-doublesqrt-floatonly

test-gals-onebyone-betterverlet-doublesqrt-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-gals-onebyone-betterverlet-doublesqrt-reducedlocalcalcs: test-gals-onebyone-betterverlet-doublesqrt

test-gals-onebyone-betterverlet-doublesqrt-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-onebyone-betterverlet-doublesqrt-reducedlocalcalcs-singleforceloop: test-gals-onebyone-betterverlet-doublesqrt-reducedlocalcalcs

test-gals-onebyone-betterverlet-doublesqrt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-onebyone-betterverlet-doublesqrt-singleforceloop: test-gals-onebyone-betterverlet-doublesqrt

test-gals-onebyone-betterverlet-msgmgmt: DFLAGS+=-DMESSAGE_MANAGEMENT
test-gals-onebyone-betterverlet-msgmgmt: test-gals-onebyone-betterverlet

test-gals-onebyone-betterverlet-msgmgmt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-gals-onebyone-betterverlet-msgmgmt-floatonly: test-gals-onebyone-betterverlet-msgmgmt

test-gals-onebyone-betterverlet-msgmgmt-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-gals-onebyone-betterverlet-msgmgmt-floatonly-reducedlocalcalcs: test-gals-onebyone-betterverlet-msgmgmt-floatonly

test-gals-onebyone-betterverlet-msgmgmt-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-onebyone-betterverlet-msgmgmt-floatonly-reducedlocalcalcs-singleforceloop: test-gals-onebyone-betterverlet-msgmgmt-floatonly-reducedlocalcalcs

test-gals-onebyone-betterverlet-msgmgmt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-onebyone-betterverlet-msgmgmt-floatonly-singleforceloop: test-gals-onebyone-betterverlet-msgmgmt-floatonly

test-gals-onebyone-betterverlet-msgmgmt-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-gals-onebyone-betterverlet-msgmgmt-reducedlocalcalcs: test-gals-onebyone-betterverlet-msgmgmt

test-gals-onebyone-betterverlet-msgmgmt-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-onebyone-betterverlet-msgmgmt-reducedlocalcalcs-singleforceloop: test-gals-onebyone-betterverlet-msgmgmt-reducedlocalcalcs

test-gals-onebyone-betterverlet-msgmgmt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-onebyone-betterverlet-msgmgmt-singleforceloop: test-gals-onebyone-betterverlet-msgmgmt

test-gals-onebyone-betterverlet-floatonly: DFLAGS+=-DFLOAT_ONLY
test-gals-onebyone-betterverlet-floatonly: test-gals-onebyone-betterverlet

test-gals-onebyone-betterverlet-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-gals-onebyone-betterverlet-floatonly-reducedlocalcalcs: test-gals-onebyone-betterverlet-floatonly

test-gals-onebyone-betterverlet-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-onebyone-betterverlet-floatonly-reducedlocalcalcs-singleforceloop: test-gals-onebyone-betterverlet-floatonly-reducedlocalcalcs

test-gals-onebyone-betterverlet-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-onebyone-betterverlet-floatonly-singleforceloop: test-gals-onebyone-betterverlet-floatonly

test-gals-onebyone-betterverlet-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-gals-onebyone-betterverlet-reducedlocalcalcs: test-gals-onebyone-betterverlet

test-gals-onebyone-betterverlet-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-onebyone-betterverlet-reducedlocalcalcs-singleforceloop: test-gals-onebyone-betterverlet-reducedlocalcalcs

test-gals-onebyone-betterverlet-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-onebyone-betterverlet-singleforceloop: test-gals-onebyone-betterverlet

test-gals-onebyone-dtchange: DFLAGS+=-DSMALL_DT_EARLY
test-gals-onebyone-dtchange: test-gals-onebyone

test-gals-onebyone-dtchange-doublesqrt: DFLAGS+=-DOUBLE_SQRT
test-gals-onebyone-dtchange-doublesqrt: test-gals-onebyone-dtchange

test-gals-onebyone-dtchange-doublesqrt-msgmgmt: DFLAGS+=-DMESSAGE_MANAGEMENT
test-gals-onebyone-dtchange-doublesqrt-msgmgmt: test-gals-onebyone-dtchange-doublesqrt

test-gals-onebyone-dtchange-doublesqrt-msgmgmt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-gals-onebyone-dtchange-doublesqrt-msgmgmt-floatonly: test-gals-onebyone-dtchange-doublesqrt-msgmgmt

test-gals-onebyone-dtchange-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-gals-onebyone-dtchange-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs: test-gals-onebyone-dtchange-doublesqrt-msgmgmt-floatonly

test-gals-onebyone-dtchange-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-onebyone-dtchange-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs-singleforceloop: test-gals-onebyone-dtchange-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs

test-gals-onebyone-dtchange-doublesqrt-msgmgmt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-onebyone-dtchange-doublesqrt-msgmgmt-floatonly-singleforceloop: test-gals-onebyone-dtchange-doublesqrt-msgmgmt-floatonly

test-gals-onebyone-dtchange-doublesqrt-msgmgmt-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-gals-onebyone-dtchange-doublesqrt-msgmgmt-reducedlocalcalcs: test-gals-onebyone-dtchange-doublesqrt-msgmgmt

test-gals-onebyone-dtchange-doublesqrt-msgmgmt-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-onebyone-dtchange-doublesqrt-msgmgmt-reducedlocalcalcs-singleforceloop: test-gals-onebyone-dtchange-doublesqrt-msgmgmt-reducedlocalcalcs

test-gals-onebyone-dtchange-doublesqrt-msgmgmt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-onebyone-dtchange-doublesqrt-msgmgmt-singleforceloop: test-gals-onebyone-dtchange-doublesqrt-msgmgmt

test-gals-onebyone-dtchange-doublesqrt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-gals-onebyone-dtchange-doublesqrt-floatonly: test-gals-onebyone-dtchange-doublesqrt

test-gals-onebyone-dtchange-doublesqrt-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-gals-onebyone-dtchange-doublesqrt-floatonly-reducedlocalcalcs: test-gals-onebyone-dtchange-doublesqrt-floatonly

test-gals-onebyone-dtchange-doublesqrt-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-onebyone-dtchange-doublesqrt-floatonly-reducedlocalcalcs-singleforceloop: test-gals-onebyone-dtchange-doublesqrt-floatonly-reducedlocalcalcs

test-gals-onebyone-dtchange-doublesqrt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-onebyone-dtchange-doublesqrt-floatonly-singleforceloop: test-gals-onebyone-dtchange-doublesqrt-floatonly

test-gals-onebyone-dtchange-doublesqrt-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-gals-onebyone-dtchange-doublesqrt-reducedlocalcalcs: test-gals-onebyone-dtchange-doublesqrt

test-gals-onebyone-dtchange-doublesqrt-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-onebyone-dtchange-doublesqrt-reducedlocalcalcs-singleforceloop: test-gals-onebyone-dtchange-doublesqrt-reducedlocalcalcs

test-gals-onebyone-dtchange-doublesqrt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-onebyone-dtchange-doublesqrt-singleforceloop: test-gals-onebyone-dtchange-doublesqrt

test-gals-onebyone-dtchange-msgmgmt: DFLAGS+=-DMESSAGE_MANAGEMENT
test-gals-onebyone-dtchange-msgmgmt: test-gals-onebyone-dtchange

test-gals-onebyone-dtchange-msgmgmt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-gals-onebyone-dtchange-msgmgmt-floatonly: test-gals-onebyone-dtchange-msgmgmt

test-gals-onebyone-dtchange-msgmgmt-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-gals-onebyone-dtchange-msgmgmt-floatonly-reducedlocalcalcs: test-gals-onebyone-dtchange-msgmgmt-floatonly

test-gals-onebyone-dtchange-msgmgmt-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-onebyone-dtchange-msgmgmt-floatonly-reducedlocalcalcs-singleforceloop: test-gals-onebyone-dtchange-msgmgmt-floatonly-reducedlocalcalcs

test-gals-onebyone-dtchange-msgmgmt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-onebyone-dtchange-msgmgmt-floatonly-singleforceloop: test-gals-onebyone-dtchange-msgmgmt-floatonly

test-gals-onebyone-dtchange-msgmgmt-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-gals-onebyone-dtchange-msgmgmt-reducedlocalcalcs: test-gals-onebyone-dtchange-msgmgmt

test-gals-onebyone-dtchange-msgmgmt-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-onebyone-dtchange-msgmgmt-reducedlocalcalcs-singleforceloop: test-gals-onebyone-dtchange-msgmgmt-reducedlocalcalcs

test-gals-onebyone-dtchange-msgmgmt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-onebyone-dtchange-msgmgmt-singleforceloop: test-gals-onebyone-dtchange-msgmgmt

test-gals-onebyone-dtchange-floatonly: DFLAGS+=-DFLOAT_ONLY
test-gals-onebyone-dtchange-floatonly: test-gals-onebyone-dtchange

test-gals-onebyone-dtchange-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-gals-onebyone-dtchange-floatonly-reducedlocalcalcs: test-gals-onebyone-dtchange-floatonly

test-gals-onebyone-dtchange-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-onebyone-dtchange-floatonly-reducedlocalcalcs-singleforceloop: test-gals-onebyone-dtchange-floatonly-reducedlocalcalcs

test-gals-onebyone-dtchange-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-onebyone-dtchange-floatonly-singleforceloop: test-gals-onebyone-dtchange-floatonly

test-gals-onebyone-dtchange-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-gals-onebyone-dtchange-reducedlocalcalcs: test-gals-onebyone-dtchange

test-gals-onebyone-dtchange-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-onebyone-dtchange-reducedlocalcalcs-singleforceloop: test-gals-onebyone-dtchange-reducedlocalcalcs

test-gals-onebyone-dtchange-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-onebyone-dtchange-singleforceloop: test-gals-onebyone-dtchange

test-gals-onebyone-doublesqrt: DFLAGS+=-DOUBLE_SQRT
test-gals-onebyone-doublesqrt: test-gals-onebyone

test-gals-onebyone-doublesqrt-msgmgmt: DFLAGS+=-DMESSAGE_MANAGEMENT
test-gals-onebyone-doublesqrt-msgmgmt: test-gals-onebyone-doublesqrt

test-gals-onebyone-doublesqrt-msgmgmt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-gals-onebyone-doublesqrt-msgmgmt-floatonly: test-gals-onebyone-doublesqrt-msgmgmt

test-gals-onebyone-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-gals-onebyone-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs: test-gals-onebyone-doublesqrt-msgmgmt-floatonly

test-gals-onebyone-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-onebyone-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs-singleforceloop: test-gals-onebyone-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs

test-gals-onebyone-doublesqrt-msgmgmt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-onebyone-doublesqrt-msgmgmt-floatonly-singleforceloop: test-gals-onebyone-doublesqrt-msgmgmt-floatonly

test-gals-onebyone-doublesqrt-msgmgmt-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-gals-onebyone-doublesqrt-msgmgmt-reducedlocalcalcs: test-gals-onebyone-doublesqrt-msgmgmt

test-gals-onebyone-doublesqrt-msgmgmt-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-onebyone-doublesqrt-msgmgmt-reducedlocalcalcs-singleforceloop: test-gals-onebyone-doublesqrt-msgmgmt-reducedlocalcalcs

test-gals-onebyone-doublesqrt-msgmgmt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-onebyone-doublesqrt-msgmgmt-singleforceloop: test-gals-onebyone-doublesqrt-msgmgmt

test-gals-onebyone-doublesqrt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-gals-onebyone-doublesqrt-floatonly: test-gals-onebyone-doublesqrt

test-gals-onebyone-doublesqrt-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-gals-onebyone-doublesqrt-floatonly-reducedlocalcalcs: test-gals-onebyone-doublesqrt-floatonly

test-gals-onebyone-doublesqrt-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-onebyone-doublesqrt-floatonly-reducedlocalcalcs-singleforceloop: test-gals-onebyone-doublesqrt-floatonly-reducedlocalcalcs

test-gals-onebyone-doublesqrt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-onebyone-doublesqrt-floatonly-singleforceloop: test-gals-onebyone-doublesqrt-floatonly

test-gals-onebyone-doublesqrt-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-gals-onebyone-doublesqrt-reducedlocalcalcs: test-gals-onebyone-doublesqrt

test-gals-onebyone-doublesqrt-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-onebyone-doublesqrt-reducedlocalcalcs-singleforceloop: test-gals-onebyone-doublesqrt-reducedlocalcalcs

test-gals-onebyone-doublesqrt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-onebyone-doublesqrt-singleforceloop: test-gals-onebyone-doublesqrt

test-gals-onebyone-msgmgmt: DFLAGS+=-DMESSAGE_MANAGEMENT
test-gals-onebyone-msgmgmt: test-gals-onebyone

test-gals-onebyone-msgmgmt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-gals-onebyone-msgmgmt-floatonly: test-gals-onebyone-msgmgmt

test-gals-onebyone-msgmgmt-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-gals-onebyone-msgmgmt-floatonly-reducedlocalcalcs: test-gals-onebyone-msgmgmt-floatonly

test-gals-onebyone-msgmgmt-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-onebyone-msgmgmt-floatonly-reducedlocalcalcs-singleforceloop: test-gals-onebyone-msgmgmt-floatonly-reducedlocalcalcs

test-gals-onebyone-msgmgmt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-onebyone-msgmgmt-floatonly-singleforceloop: test-gals-onebyone-msgmgmt-floatonly

test-gals-onebyone-msgmgmt-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-gals-onebyone-msgmgmt-reducedlocalcalcs: test-gals-onebyone-msgmgmt

test-gals-onebyone-msgmgmt-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-onebyone-msgmgmt-reducedlocalcalcs-singleforceloop: test-gals-onebyone-msgmgmt-reducedlocalcalcs

test-gals-onebyone-msgmgmt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-onebyone-msgmgmt-singleforceloop: test-gals-onebyone-msgmgmt

test-gals-onebyone-floatonly: DFLAGS+=-DFLOAT_ONLY
test-gals-onebyone-floatonly: test-gals-onebyone

test-gals-onebyone-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-gals-onebyone-floatonly-reducedlocalcalcs: test-gals-onebyone-floatonly

test-gals-onebyone-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-onebyone-floatonly-reducedlocalcalcs-singleforceloop: test-gals-onebyone-floatonly-reducedlocalcalcs

test-gals-onebyone-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-onebyone-floatonly-singleforceloop: test-gals-onebyone-floatonly

test-gals-onebyone-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-gals-onebyone-reducedlocalcalcs: test-gals-onebyone

test-gals-onebyone-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-onebyone-reducedlocalcalcs-singleforceloop: test-gals-onebyone-reducedlocalcalcs

test-gals-onebyone-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-onebyone-singleforceloop: test-gals-onebyone

test-gals-large: DFLAGS+=-DLARGE_TEST
test-gals-large: test-gals

test-gals-large-betterverlet: DFLAGS+=-DBETTER_VERLET
test-gals-large-betterverlet: test-gals-large

test-gals-large-betterverlet-dtchange: DFLAGS+=-DSMALL_DT_EARLY
test-gals-large-betterverlet-dtchange: test-gals-large-betterverlet

test-gals-large-betterverlet-dtchange-doublesqrt: DFLAGS+=-DOUBLE_SQRT
test-gals-large-betterverlet-dtchange-doublesqrt: test-gals-large-betterverlet-dtchange

test-gals-large-betterverlet-dtchange-doublesqrt-msgmgmt: DFLAGS+=-DMESSAGE_MANAGEMENT
test-gals-large-betterverlet-dtchange-doublesqrt-msgmgmt: test-gals-large-betterverlet-dtchange-doublesqrt

test-gals-large-betterverlet-dtchange-doublesqrt-msgmgmt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-gals-large-betterverlet-dtchange-doublesqrt-msgmgmt-floatonly: test-gals-large-betterverlet-dtchange-doublesqrt-msgmgmt

test-gals-large-betterverlet-dtchange-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-gals-large-betterverlet-dtchange-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs: test-gals-large-betterverlet-dtchange-doublesqrt-msgmgmt-floatonly

test-gals-large-betterverlet-dtchange-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-large-betterverlet-dtchange-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs-singleforceloop: test-gals-large-betterverlet-dtchange-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs

test-gals-large-betterverlet-dtchange-doublesqrt-msgmgmt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-large-betterverlet-dtchange-doublesqrt-msgmgmt-floatonly-singleforceloop: test-gals-large-betterverlet-dtchange-doublesqrt-msgmgmt-floatonly

test-gals-large-betterverlet-dtchange-doublesqrt-msgmgmt-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-gals-large-betterverlet-dtchange-doublesqrt-msgmgmt-reducedlocalcalcs: test-gals-large-betterverlet-dtchange-doublesqrt-msgmgmt

test-gals-large-betterverlet-dtchange-doublesqrt-msgmgmt-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-large-betterverlet-dtchange-doublesqrt-msgmgmt-reducedlocalcalcs-singleforceloop: test-gals-large-betterverlet-dtchange-doublesqrt-msgmgmt-reducedlocalcalcs

test-gals-large-betterverlet-dtchange-doublesqrt-msgmgmt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-large-betterverlet-dtchange-doublesqrt-msgmgmt-singleforceloop: test-gals-large-betterverlet-dtchange-doublesqrt-msgmgmt

test-gals-large-betterverlet-dtchange-doublesqrt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-gals-large-betterverlet-dtchange-doublesqrt-floatonly: test-gals-large-betterverlet-dtchange-doublesqrt

test-gals-large-betterverlet-dtchange-doublesqrt-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-gals-large-betterverlet-dtchange-doublesqrt-floatonly-reducedlocalcalcs: test-gals-large-betterverlet-dtchange-doublesqrt-floatonly

test-gals-large-betterverlet-dtchange-doublesqrt-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-large-betterverlet-dtchange-doublesqrt-floatonly-reducedlocalcalcs-singleforceloop: test-gals-large-betterverlet-dtchange-doublesqrt-floatonly-reducedlocalcalcs

test-gals-large-betterverlet-dtchange-doublesqrt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-large-betterverlet-dtchange-doublesqrt-floatonly-singleforceloop: test-gals-large-betterverlet-dtchange-doublesqrt-floatonly

test-gals-large-betterverlet-dtchange-doublesqrt-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-gals-large-betterverlet-dtchange-doublesqrt-reducedlocalcalcs: test-gals-large-betterverlet-dtchange-doublesqrt

test-gals-large-betterverlet-dtchange-doublesqrt-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-large-betterverlet-dtchange-doublesqrt-reducedlocalcalcs-singleforceloop: test-gals-large-betterverlet-dtchange-doublesqrt-reducedlocalcalcs

test-gals-large-betterverlet-dtchange-doublesqrt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-large-betterverlet-dtchange-doublesqrt-singleforceloop: test-gals-large-betterverlet-dtchange-doublesqrt

test-gals-large-betterverlet-dtchange-msgmgmt: DFLAGS+=-DMESSAGE_MANAGEMENT
test-gals-large-betterverlet-dtchange-msgmgmt: test-gals-large-betterverlet-dtchange

test-gals-large-betterverlet-dtchange-msgmgmt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-gals-large-betterverlet-dtchange-msgmgmt-floatonly: test-gals-large-betterverlet-dtchange-msgmgmt

test-gals-large-betterverlet-dtchange-msgmgmt-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-gals-large-betterverlet-dtchange-msgmgmt-floatonly-reducedlocalcalcs: test-gals-large-betterverlet-dtchange-msgmgmt-floatonly

test-gals-large-betterverlet-dtchange-msgmgmt-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-large-betterverlet-dtchange-msgmgmt-floatonly-reducedlocalcalcs-singleforceloop: test-gals-large-betterverlet-dtchange-msgmgmt-floatonly-reducedlocalcalcs

test-gals-large-betterverlet-dtchange-msgmgmt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-large-betterverlet-dtchange-msgmgmt-floatonly-singleforceloop: test-gals-large-betterverlet-dtchange-msgmgmt-floatonly

test-gals-large-betterverlet-dtchange-msgmgmt-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-gals-large-betterverlet-dtchange-msgmgmt-reducedlocalcalcs: test-gals-large-betterverlet-dtchange-msgmgmt

test-gals-large-betterverlet-dtchange-msgmgmt-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-large-betterverlet-dtchange-msgmgmt-reducedlocalcalcs-singleforceloop: test-gals-large-betterverlet-dtchange-msgmgmt-reducedlocalcalcs

test-gals-large-betterverlet-dtchange-msgmgmt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-large-betterverlet-dtchange-msgmgmt-singleforceloop: test-gals-large-betterverlet-dtchange-msgmgmt

test-gals-large-betterverlet-dtchange-floatonly: DFLAGS+=-DFLOAT_ONLY
test-gals-large-betterverlet-dtchange-floatonly: test-gals-large-betterverlet-dtchange

test-gals-large-betterverlet-dtchange-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-gals-large-betterverlet-dtchange-floatonly-reducedlocalcalcs: test-gals-large-betterverlet-dtchange-floatonly

test-gals-large-betterverlet-dtchange-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-large-betterverlet-dtchange-floatonly-reducedlocalcalcs-singleforceloop: test-gals-large-betterverlet-dtchange-floatonly-reducedlocalcalcs

test-gals-large-betterverlet-dtchange-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-large-betterverlet-dtchange-floatonly-singleforceloop: test-gals-large-betterverlet-dtchange-floatonly

test-gals-large-betterverlet-dtchange-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-gals-large-betterverlet-dtchange-reducedlocalcalcs: test-gals-large-betterverlet-dtchange

test-gals-large-betterverlet-dtchange-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-large-betterverlet-dtchange-reducedlocalcalcs-singleforceloop: test-gals-large-betterverlet-dtchange-reducedlocalcalcs

test-gals-large-betterverlet-dtchange-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-large-betterverlet-dtchange-singleforceloop: test-gals-large-betterverlet-dtchange

test-gals-large-betterverlet-doublesqrt: DFLAGS+=-DOUBLE_SQRT
test-gals-large-betterverlet-doublesqrt: test-gals-large-betterverlet

test-gals-large-betterverlet-doublesqrt-msgmgmt: DFLAGS+=-DMESSAGE_MANAGEMENT
test-gals-large-betterverlet-doublesqrt-msgmgmt: test-gals-large-betterverlet-doublesqrt

test-gals-large-betterverlet-doublesqrt-msgmgmt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-gals-large-betterverlet-doublesqrt-msgmgmt-floatonly: test-gals-large-betterverlet-doublesqrt-msgmgmt

test-gals-large-betterverlet-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-gals-large-betterverlet-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs: test-gals-large-betterverlet-doublesqrt-msgmgmt-floatonly

test-gals-large-betterverlet-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-large-betterverlet-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs-singleforceloop: test-gals-large-betterverlet-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs

test-gals-large-betterverlet-doublesqrt-msgmgmt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-large-betterverlet-doublesqrt-msgmgmt-floatonly-singleforceloop: test-gals-large-betterverlet-doublesqrt-msgmgmt-floatonly

test-gals-large-betterverlet-doublesqrt-msgmgmt-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-gals-large-betterverlet-doublesqrt-msgmgmt-reducedlocalcalcs: test-gals-large-betterverlet-doublesqrt-msgmgmt

test-gals-large-betterverlet-doublesqrt-msgmgmt-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-large-betterverlet-doublesqrt-msgmgmt-reducedlocalcalcs-singleforceloop: test-gals-large-betterverlet-doublesqrt-msgmgmt-reducedlocalcalcs

test-gals-large-betterverlet-doublesqrt-msgmgmt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-large-betterverlet-doublesqrt-msgmgmt-singleforceloop: test-gals-large-betterverlet-doublesqrt-msgmgmt

test-gals-large-betterverlet-doublesqrt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-gals-large-betterverlet-doublesqrt-floatonly: test-gals-large-betterverlet-doublesqrt

test-gals-large-betterverlet-doublesqrt-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-gals-large-betterverlet-doublesqrt-floatonly-reducedlocalcalcs: test-gals-large-betterverlet-doublesqrt-floatonly

test-gals-large-betterverlet-doublesqrt-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-large-betterverlet-doublesqrt-floatonly-reducedlocalcalcs-singleforceloop: test-gals-large-betterverlet-doublesqrt-floatonly-reducedlocalcalcs

test-gals-large-betterverlet-doublesqrt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-large-betterverlet-doublesqrt-floatonly-singleforceloop: test-gals-large-betterverlet-doublesqrt-floatonly

test-gals-large-betterverlet-doublesqrt-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-gals-large-betterverlet-doublesqrt-reducedlocalcalcs: test-gals-large-betterverlet-doublesqrt

test-gals-large-betterverlet-doublesqrt-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-large-betterverlet-doublesqrt-reducedlocalcalcs-singleforceloop: test-gals-large-betterverlet-doublesqrt-reducedlocalcalcs

test-gals-large-betterverlet-doublesqrt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-large-betterverlet-doublesqrt-singleforceloop: test-gals-large-betterverlet-doublesqrt

test-gals-large-betterverlet-msgmgmt: DFLAGS+=-DMESSAGE_MANAGEMENT
test-gals-large-betterverlet-msgmgmt: test-gals-large-betterverlet

test-gals-large-betterverlet-msgmgmt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-gals-large-betterverlet-msgmgmt-floatonly: test-gals-large-betterverlet-msgmgmt

test-gals-large-betterverlet-msgmgmt-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-gals-large-betterverlet-msgmgmt-floatonly-reducedlocalcalcs: test-gals-large-betterverlet-msgmgmt-floatonly

test-gals-large-betterverlet-msgmgmt-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-large-betterverlet-msgmgmt-floatonly-reducedlocalcalcs-singleforceloop: test-gals-large-betterverlet-msgmgmt-floatonly-reducedlocalcalcs

test-gals-large-betterverlet-msgmgmt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-large-betterverlet-msgmgmt-floatonly-singleforceloop: test-gals-large-betterverlet-msgmgmt-floatonly

test-gals-large-betterverlet-msgmgmt-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-gals-large-betterverlet-msgmgmt-reducedlocalcalcs: test-gals-large-betterverlet-msgmgmt

test-gals-large-betterverlet-msgmgmt-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-large-betterverlet-msgmgmt-reducedlocalcalcs-singleforceloop: test-gals-large-betterverlet-msgmgmt-reducedlocalcalcs

test-gals-large-betterverlet-msgmgmt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-large-betterverlet-msgmgmt-singleforceloop: test-gals-large-betterverlet-msgmgmt

test-gals-large-betterverlet-floatonly: DFLAGS+=-DFLOAT_ONLY
test-gals-large-betterverlet-floatonly: test-gals-large-betterverlet

test-gals-large-betterverlet-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-gals-large-betterverlet-floatonly-reducedlocalcalcs: test-gals-large-betterverlet-floatonly

test-gals-large-betterverlet-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-large-betterverlet-floatonly-reducedlocalcalcs-singleforceloop: test-gals-large-betterverlet-floatonly-reducedlocalcalcs

test-gals-large-betterverlet-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-large-betterverlet-floatonly-singleforceloop: test-gals-large-betterverlet-floatonly

test-gals-large-betterverlet-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-gals-large-betterverlet-reducedlocalcalcs: test-gals-large-betterverlet

test-gals-large-betterverlet-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-large-betterverlet-reducedlocalcalcs-singleforceloop: test-gals-large-betterverlet-reducedlocalcalcs

test-gals-large-betterverlet-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-large-betterverlet-singleforceloop: test-gals-large-betterverlet

test-gals-large-dtchange: DFLAGS+=-DSMALL_DT_EARLY
test-gals-large-dtchange: test-gals-large

test-gals-large-dtchange-doublesqrt: DFLAGS+=-DOUBLE_SQRT
test-gals-large-dtchange-doublesqrt: test-gals-large-dtchange

test-gals-large-dtchange-doublesqrt-msgmgmt: DFLAGS+=-DMESSAGE_MANAGEMENT
test-gals-large-dtchange-doublesqrt-msgmgmt: test-gals-large-dtchange-doublesqrt

test-gals-large-dtchange-doublesqrt-msgmgmt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-gals-large-dtchange-doublesqrt-msgmgmt-floatonly: test-gals-large-dtchange-doublesqrt-msgmgmt

test-gals-large-dtchange-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-gals-large-dtchange-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs: test-gals-large-dtchange-doublesqrt-msgmgmt-floatonly

test-gals-large-dtchange-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-large-dtchange-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs-singleforceloop: test-gals-large-dtchange-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs

test-gals-large-dtchange-doublesqrt-msgmgmt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-large-dtchange-doublesqrt-msgmgmt-floatonly-singleforceloop: test-gals-large-dtchange-doublesqrt-msgmgmt-floatonly

test-gals-large-dtchange-doublesqrt-msgmgmt-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-gals-large-dtchange-doublesqrt-msgmgmt-reducedlocalcalcs: test-gals-large-dtchange-doublesqrt-msgmgmt

test-gals-large-dtchange-doublesqrt-msgmgmt-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-large-dtchange-doublesqrt-msgmgmt-reducedlocalcalcs-singleforceloop: test-gals-large-dtchange-doublesqrt-msgmgmt-reducedlocalcalcs

test-gals-large-dtchange-doublesqrt-msgmgmt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-large-dtchange-doublesqrt-msgmgmt-singleforceloop: test-gals-large-dtchange-doublesqrt-msgmgmt

test-gals-large-dtchange-doublesqrt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-gals-large-dtchange-doublesqrt-floatonly: test-gals-large-dtchange-doublesqrt

test-gals-large-dtchange-doublesqrt-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-gals-large-dtchange-doublesqrt-floatonly-reducedlocalcalcs: test-gals-large-dtchange-doublesqrt-floatonly

test-gals-large-dtchange-doublesqrt-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-large-dtchange-doublesqrt-floatonly-reducedlocalcalcs-singleforceloop: test-gals-large-dtchange-doublesqrt-floatonly-reducedlocalcalcs

test-gals-large-dtchange-doublesqrt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-large-dtchange-doublesqrt-floatonly-singleforceloop: test-gals-large-dtchange-doublesqrt-floatonly

test-gals-large-dtchange-doublesqrt-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-gals-large-dtchange-doublesqrt-reducedlocalcalcs: test-gals-large-dtchange-doublesqrt

test-gals-large-dtchange-doublesqrt-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-large-dtchange-doublesqrt-reducedlocalcalcs-singleforceloop: test-gals-large-dtchange-doublesqrt-reducedlocalcalcs

test-gals-large-dtchange-doublesqrt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-large-dtchange-doublesqrt-singleforceloop: test-gals-large-dtchange-doublesqrt

test-gals-large-dtchange-msgmgmt: DFLAGS+=-DMESSAGE_MANAGEMENT
test-gals-large-dtchange-msgmgmt: test-gals-large-dtchange

test-gals-large-dtchange-msgmgmt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-gals-large-dtchange-msgmgmt-floatonly: test-gals-large-dtchange-msgmgmt

test-gals-large-dtchange-msgmgmt-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-gals-large-dtchange-msgmgmt-floatonly-reducedlocalcalcs: test-gals-large-dtchange-msgmgmt-floatonly

test-gals-large-dtchange-msgmgmt-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-large-dtchange-msgmgmt-floatonly-reducedlocalcalcs-singleforceloop: test-gals-large-dtchange-msgmgmt-floatonly-reducedlocalcalcs

test-gals-large-dtchange-msgmgmt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-large-dtchange-msgmgmt-floatonly-singleforceloop: test-gals-large-dtchange-msgmgmt-floatonly

test-gals-large-dtchange-msgmgmt-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-gals-large-dtchange-msgmgmt-reducedlocalcalcs: test-gals-large-dtchange-msgmgmt

test-gals-large-dtchange-msgmgmt-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-large-dtchange-msgmgmt-reducedlocalcalcs-singleforceloop: test-gals-large-dtchange-msgmgmt-reducedlocalcalcs

test-gals-large-dtchange-msgmgmt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-large-dtchange-msgmgmt-singleforceloop: test-gals-large-dtchange-msgmgmt

test-gals-large-dtchange-floatonly: DFLAGS+=-DFLOAT_ONLY
test-gals-large-dtchange-floatonly: test-gals-large-dtchange

test-gals-large-dtchange-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-gals-large-dtchange-floatonly-reducedlocalcalcs: test-gals-large-dtchange-floatonly

test-gals-large-dtchange-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-large-dtchange-floatonly-reducedlocalcalcs-singleforceloop: test-gals-large-dtchange-floatonly-reducedlocalcalcs

test-gals-large-dtchange-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-large-dtchange-floatonly-singleforceloop: test-gals-large-dtchange-floatonly

test-gals-large-dtchange-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-gals-large-dtchange-reducedlocalcalcs: test-gals-large-dtchange

test-gals-large-dtchange-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-large-dtchange-reducedlocalcalcs-singleforceloop: test-gals-large-dtchange-reducedlocalcalcs

test-gals-large-dtchange-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-large-dtchange-singleforceloop: test-gals-large-dtchange

test-gals-large-doublesqrt: DFLAGS+=-DOUBLE_SQRT
test-gals-large-doublesqrt: test-gals-large

test-gals-large-doublesqrt-msgmgmt: DFLAGS+=-DMESSAGE_MANAGEMENT
test-gals-large-doublesqrt-msgmgmt: test-gals-large-doublesqrt

test-gals-large-doublesqrt-msgmgmt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-gals-large-doublesqrt-msgmgmt-floatonly: test-gals-large-doublesqrt-msgmgmt

test-gals-large-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-gals-large-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs: test-gals-large-doublesqrt-msgmgmt-floatonly

test-gals-large-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-large-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs-singleforceloop: test-gals-large-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs

test-gals-large-doublesqrt-msgmgmt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-large-doublesqrt-msgmgmt-floatonly-singleforceloop: test-gals-large-doublesqrt-msgmgmt-floatonly

test-gals-large-doublesqrt-msgmgmt-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-gals-large-doublesqrt-msgmgmt-reducedlocalcalcs: test-gals-large-doublesqrt-msgmgmt

test-gals-large-doublesqrt-msgmgmt-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-large-doublesqrt-msgmgmt-reducedlocalcalcs-singleforceloop: test-gals-large-doublesqrt-msgmgmt-reducedlocalcalcs

test-gals-large-doublesqrt-msgmgmt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-large-doublesqrt-msgmgmt-singleforceloop: test-gals-large-doublesqrt-msgmgmt

test-gals-large-doublesqrt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-gals-large-doublesqrt-floatonly: test-gals-large-doublesqrt

test-gals-large-doublesqrt-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-gals-large-doublesqrt-floatonly-reducedlocalcalcs: test-gals-large-doublesqrt-floatonly

test-gals-large-doublesqrt-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-large-doublesqrt-floatonly-reducedlocalcalcs-singleforceloop: test-gals-large-doublesqrt-floatonly-reducedlocalcalcs

test-gals-large-doublesqrt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-large-doublesqrt-floatonly-singleforceloop: test-gals-large-doublesqrt-floatonly

test-gals-large-doublesqrt-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-gals-large-doublesqrt-reducedlocalcalcs: test-gals-large-doublesqrt

test-gals-large-doublesqrt-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-large-doublesqrt-reducedlocalcalcs-singleforceloop: test-gals-large-doublesqrt-reducedlocalcalcs

test-gals-large-doublesqrt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-large-doublesqrt-singleforceloop: test-gals-large-doublesqrt

test-gals-large-msgmgmt: DFLAGS+=-DMESSAGE_MANAGEMENT
test-gals-large-msgmgmt: test-gals-large

test-gals-large-msgmgmt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-gals-large-msgmgmt-floatonly: test-gals-large-msgmgmt

test-gals-large-msgmgmt-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-gals-large-msgmgmt-floatonly-reducedlocalcalcs: test-gals-large-msgmgmt-floatonly

test-gals-large-msgmgmt-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-large-msgmgmt-floatonly-reducedlocalcalcs-singleforceloop: test-gals-large-msgmgmt-floatonly-reducedlocalcalcs

test-gals-large-msgmgmt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-large-msgmgmt-floatonly-singleforceloop: test-gals-large-msgmgmt-floatonly

test-gals-large-msgmgmt-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-gals-large-msgmgmt-reducedlocalcalcs: test-gals-large-msgmgmt

test-gals-large-msgmgmt-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-large-msgmgmt-reducedlocalcalcs-singleforceloop: test-gals-large-msgmgmt-reducedlocalcalcs

test-gals-large-msgmgmt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-large-msgmgmt-singleforceloop: test-gals-large-msgmgmt

test-gals-large-floatonly: DFLAGS+=-DFLOAT_ONLY
test-gals-large-floatonly: test-gals-large

test-gals-large-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-gals-large-floatonly-reducedlocalcalcs: test-gals-large-floatonly

test-gals-large-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-large-floatonly-reducedlocalcalcs-singleforceloop: test-gals-large-floatonly-reducedlocalcalcs

test-gals-large-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-large-floatonly-singleforceloop: test-gals-large-floatonly

test-gals-large-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-gals-large-reducedlocalcalcs: test-gals-large

test-gals-large-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-large-reducedlocalcalcs-singleforceloop: test-gals-large-reducedlocalcalcs

test-gals-large-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-large-singleforceloop: test-gals-large

# **TEST Local calculation method: onebyone**
test-gals-large-onebyone: DFLAGS+=-DONE_BY_ONE
test-gals-large-onebyone: test-gals-large

test-gals-large-onebyone-betterverlet: DFLAGS+=-DBETTER_VERLET
test-gals-large-onebyone-betterverlet: test-gals-large-onebyone

test-gals-large-onebyone-betterverlet-dtchange: DFLAGS+=-DSMALL_DT_EARLY
test-gals-large-onebyone-betterverlet-dtchange: test-gals-large-onebyone-betterverlet

test-gals-large-onebyone-betterverlet-dtchange-doublesqrt: DFLAGS+=-DOUBLE_SQRT
test-gals-large-onebyone-betterverlet-dtchange-doublesqrt: test-gals-large-onebyone-betterverlet-dtchange

test-gals-large-onebyone-betterverlet-dtchange-doublesqrt-msgmgmt: DFLAGS+=-DMESSAGE_MANAGEMENT
test-gals-large-onebyone-betterverlet-dtchange-doublesqrt-msgmgmt: test-gals-large-onebyone-betterverlet-dtchange-doublesqrt

test-gals-large-onebyone-betterverlet-dtchange-doublesqrt-msgmgmt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-gals-large-onebyone-betterverlet-dtchange-doublesqrt-msgmgmt-floatonly: test-gals-large-onebyone-betterverlet-dtchange-doublesqrt-msgmgmt

test-gals-large-onebyone-betterverlet-dtchange-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-gals-large-onebyone-betterverlet-dtchange-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs: test-gals-large-onebyone-betterverlet-dtchange-doublesqrt-msgmgmt-floatonly

test-gals-large-onebyone-betterverlet-dtchange-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-large-onebyone-betterverlet-dtchange-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs-singleforceloop: test-gals-large-onebyone-betterverlet-dtchange-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs

test-gals-large-onebyone-betterverlet-dtchange-doublesqrt-msgmgmt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-large-onebyone-betterverlet-dtchange-doublesqrt-msgmgmt-floatonly-singleforceloop: test-gals-large-onebyone-betterverlet-dtchange-doublesqrt-msgmgmt-floatonly

test-gals-large-onebyone-betterverlet-dtchange-doublesqrt-msgmgmt-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-gals-large-onebyone-betterverlet-dtchange-doublesqrt-msgmgmt-reducedlocalcalcs: test-gals-large-onebyone-betterverlet-dtchange-doublesqrt-msgmgmt

test-gals-large-onebyone-betterverlet-dtchange-doublesqrt-msgmgmt-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-large-onebyone-betterverlet-dtchange-doublesqrt-msgmgmt-reducedlocalcalcs-singleforceloop: test-gals-large-onebyone-betterverlet-dtchange-doublesqrt-msgmgmt-reducedlocalcalcs

test-gals-large-onebyone-betterverlet-dtchange-doublesqrt-msgmgmt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-large-onebyone-betterverlet-dtchange-doublesqrt-msgmgmt-singleforceloop: test-gals-large-onebyone-betterverlet-dtchange-doublesqrt-msgmgmt

test-gals-large-onebyone-betterverlet-dtchange-doublesqrt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-gals-large-onebyone-betterverlet-dtchange-doublesqrt-floatonly: test-gals-large-onebyone-betterverlet-dtchange-doublesqrt

test-gals-large-onebyone-betterverlet-dtchange-doublesqrt-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-gals-large-onebyone-betterverlet-dtchange-doublesqrt-floatonly-reducedlocalcalcs: test-gals-large-onebyone-betterverlet-dtchange-doublesqrt-floatonly

test-gals-large-onebyone-betterverlet-dtchange-doublesqrt-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-large-onebyone-betterverlet-dtchange-doublesqrt-floatonly-reducedlocalcalcs-singleforceloop: test-gals-large-onebyone-betterverlet-dtchange-doublesqrt-floatonly-reducedlocalcalcs

test-gals-large-onebyone-betterverlet-dtchange-doublesqrt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-large-onebyone-betterverlet-dtchange-doublesqrt-floatonly-singleforceloop: test-gals-large-onebyone-betterverlet-dtchange-doublesqrt-floatonly

test-gals-large-onebyone-betterverlet-dtchange-doublesqrt-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-gals-large-onebyone-betterverlet-dtchange-doublesqrt-reducedlocalcalcs: test-gals-large-onebyone-betterverlet-dtchange-doublesqrt

test-gals-large-onebyone-betterverlet-dtchange-doublesqrt-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-large-onebyone-betterverlet-dtchange-doublesqrt-reducedlocalcalcs-singleforceloop: test-gals-large-onebyone-betterverlet-dtchange-doublesqrt-reducedlocalcalcs

test-gals-large-onebyone-betterverlet-dtchange-doublesqrt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-large-onebyone-betterverlet-dtchange-doublesqrt-singleforceloop: test-gals-large-onebyone-betterverlet-dtchange-doublesqrt

test-gals-large-onebyone-betterverlet-dtchange-msgmgmt: DFLAGS+=-DMESSAGE_MANAGEMENT
test-gals-large-onebyone-betterverlet-dtchange-msgmgmt: test-gals-large-onebyone-betterverlet-dtchange

test-gals-large-onebyone-betterverlet-dtchange-msgmgmt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-gals-large-onebyone-betterverlet-dtchange-msgmgmt-floatonly: test-gals-large-onebyone-betterverlet-dtchange-msgmgmt

test-gals-large-onebyone-betterverlet-dtchange-msgmgmt-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-gals-large-onebyone-betterverlet-dtchange-msgmgmt-floatonly-reducedlocalcalcs: test-gals-large-onebyone-betterverlet-dtchange-msgmgmt-floatonly

test-gals-large-onebyone-betterverlet-dtchange-msgmgmt-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-large-onebyone-betterverlet-dtchange-msgmgmt-floatonly-reducedlocalcalcs-singleforceloop: test-gals-large-onebyone-betterverlet-dtchange-msgmgmt-floatonly-reducedlocalcalcs

test-gals-large-onebyone-betterverlet-dtchange-msgmgmt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-large-onebyone-betterverlet-dtchange-msgmgmt-floatonly-singleforceloop: test-gals-large-onebyone-betterverlet-dtchange-msgmgmt-floatonly

test-gals-large-onebyone-betterverlet-dtchange-msgmgmt-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-gals-large-onebyone-betterverlet-dtchange-msgmgmt-reducedlocalcalcs: test-gals-large-onebyone-betterverlet-dtchange-msgmgmt

test-gals-large-onebyone-betterverlet-dtchange-msgmgmt-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-large-onebyone-betterverlet-dtchange-msgmgmt-reducedlocalcalcs-singleforceloop: test-gals-large-onebyone-betterverlet-dtchange-msgmgmt-reducedlocalcalcs

test-gals-large-onebyone-betterverlet-dtchange-msgmgmt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-large-onebyone-betterverlet-dtchange-msgmgmt-singleforceloop: test-gals-large-onebyone-betterverlet-dtchange-msgmgmt

test-gals-large-onebyone-betterverlet-dtchange-floatonly: DFLAGS+=-DFLOAT_ONLY
test-gals-large-onebyone-betterverlet-dtchange-floatonly: test-gals-large-onebyone-betterverlet-dtchange

test-gals-large-onebyone-betterverlet-dtchange-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-gals-large-onebyone-betterverlet-dtchange-floatonly-reducedlocalcalcs: test-gals-large-onebyone-betterverlet-dtchange-floatonly

test-gals-large-onebyone-betterverlet-dtchange-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-large-onebyone-betterverlet-dtchange-floatonly-reducedlocalcalcs-singleforceloop: test-gals-large-onebyone-betterverlet-dtchange-floatonly-reducedlocalcalcs

test-gals-large-onebyone-betterverlet-dtchange-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-large-onebyone-betterverlet-dtchange-floatonly-singleforceloop: test-gals-large-onebyone-betterverlet-dtchange-floatonly

test-gals-large-onebyone-betterverlet-dtchange-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-gals-large-onebyone-betterverlet-dtchange-reducedlocalcalcs: test-gals-large-onebyone-betterverlet-dtchange

test-gals-large-onebyone-betterverlet-dtchange-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-large-onebyone-betterverlet-dtchange-reducedlocalcalcs-singleforceloop: test-gals-large-onebyone-betterverlet-dtchange-reducedlocalcalcs

test-gals-large-onebyone-betterverlet-dtchange-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-large-onebyone-betterverlet-dtchange-singleforceloop: test-gals-large-onebyone-betterverlet-dtchange

test-gals-large-onebyone-betterverlet-doublesqrt: DFLAGS+=-DOUBLE_SQRT
test-gals-large-onebyone-betterverlet-doublesqrt: test-gals-large-onebyone-betterverlet

test-gals-large-onebyone-betterverlet-doublesqrt-msgmgmt: DFLAGS+=-DMESSAGE_MANAGEMENT
test-gals-large-onebyone-betterverlet-doublesqrt-msgmgmt: test-gals-large-onebyone-betterverlet-doublesqrt

test-gals-large-onebyone-betterverlet-doublesqrt-msgmgmt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-gals-large-onebyone-betterverlet-doublesqrt-msgmgmt-floatonly: test-gals-large-onebyone-betterverlet-doublesqrt-msgmgmt

test-gals-large-onebyone-betterverlet-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-gals-large-onebyone-betterverlet-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs: test-gals-large-onebyone-betterverlet-doublesqrt-msgmgmt-floatonly

test-gals-large-onebyone-betterverlet-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-large-onebyone-betterverlet-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs-singleforceloop: test-gals-large-onebyone-betterverlet-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs

test-gals-large-onebyone-betterverlet-doublesqrt-msgmgmt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-large-onebyone-betterverlet-doublesqrt-msgmgmt-floatonly-singleforceloop: test-gals-large-onebyone-betterverlet-doublesqrt-msgmgmt-floatonly

test-gals-large-onebyone-betterverlet-doublesqrt-msgmgmt-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-gals-large-onebyone-betterverlet-doublesqrt-msgmgmt-reducedlocalcalcs: test-gals-large-onebyone-betterverlet-doublesqrt-msgmgmt

test-gals-large-onebyone-betterverlet-doublesqrt-msgmgmt-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-large-onebyone-betterverlet-doublesqrt-msgmgmt-reducedlocalcalcs-singleforceloop: test-gals-large-onebyone-betterverlet-doublesqrt-msgmgmt-reducedlocalcalcs

test-gals-large-onebyone-betterverlet-doublesqrt-msgmgmt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-large-onebyone-betterverlet-doublesqrt-msgmgmt-singleforceloop: test-gals-large-onebyone-betterverlet-doublesqrt-msgmgmt

test-gals-large-onebyone-betterverlet-doublesqrt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-gals-large-onebyone-betterverlet-doublesqrt-floatonly: test-gals-large-onebyone-betterverlet-doublesqrt

test-gals-large-onebyone-betterverlet-doublesqrt-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-gals-large-onebyone-betterverlet-doublesqrt-floatonly-reducedlocalcalcs: test-gals-large-onebyone-betterverlet-doublesqrt-floatonly

test-gals-large-onebyone-betterverlet-doublesqrt-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-large-onebyone-betterverlet-doublesqrt-floatonly-reducedlocalcalcs-singleforceloop: test-gals-large-onebyone-betterverlet-doublesqrt-floatonly-reducedlocalcalcs

test-gals-large-onebyone-betterverlet-doublesqrt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-large-onebyone-betterverlet-doublesqrt-floatonly-singleforceloop: test-gals-large-onebyone-betterverlet-doublesqrt-floatonly

test-gals-large-onebyone-betterverlet-doublesqrt-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-gals-large-onebyone-betterverlet-doublesqrt-reducedlocalcalcs: test-gals-large-onebyone-betterverlet-doublesqrt

test-gals-large-onebyone-betterverlet-doublesqrt-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-large-onebyone-betterverlet-doublesqrt-reducedlocalcalcs-singleforceloop: test-gals-large-onebyone-betterverlet-doublesqrt-reducedlocalcalcs

test-gals-large-onebyone-betterverlet-doublesqrt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-large-onebyone-betterverlet-doublesqrt-singleforceloop: test-gals-large-onebyone-betterverlet-doublesqrt

test-gals-large-onebyone-betterverlet-msgmgmt: DFLAGS+=-DMESSAGE_MANAGEMENT
test-gals-large-onebyone-betterverlet-msgmgmt: test-gals-large-onebyone-betterverlet

test-gals-large-onebyone-betterverlet-msgmgmt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-gals-large-onebyone-betterverlet-msgmgmt-floatonly: test-gals-large-onebyone-betterverlet-msgmgmt

test-gals-large-onebyone-betterverlet-msgmgmt-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-gals-large-onebyone-betterverlet-msgmgmt-floatonly-reducedlocalcalcs: test-gals-large-onebyone-betterverlet-msgmgmt-floatonly

test-gals-large-onebyone-betterverlet-msgmgmt-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-large-onebyone-betterverlet-msgmgmt-floatonly-reducedlocalcalcs-singleforceloop: test-gals-large-onebyone-betterverlet-msgmgmt-floatonly-reducedlocalcalcs

test-gals-large-onebyone-betterverlet-msgmgmt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-large-onebyone-betterverlet-msgmgmt-floatonly-singleforceloop: test-gals-large-onebyone-betterverlet-msgmgmt-floatonly

test-gals-large-onebyone-betterverlet-msgmgmt-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-gals-large-onebyone-betterverlet-msgmgmt-reducedlocalcalcs: test-gals-large-onebyone-betterverlet-msgmgmt

test-gals-large-onebyone-betterverlet-msgmgmt-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-large-onebyone-betterverlet-msgmgmt-reducedlocalcalcs-singleforceloop: test-gals-large-onebyone-betterverlet-msgmgmt-reducedlocalcalcs

test-gals-large-onebyone-betterverlet-msgmgmt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-large-onebyone-betterverlet-msgmgmt-singleforceloop: test-gals-large-onebyone-betterverlet-msgmgmt

test-gals-large-onebyone-betterverlet-floatonly: DFLAGS+=-DFLOAT_ONLY
test-gals-large-onebyone-betterverlet-floatonly: test-gals-large-onebyone-betterverlet

test-gals-large-onebyone-betterverlet-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-gals-large-onebyone-betterverlet-floatonly-reducedlocalcalcs: test-gals-large-onebyone-betterverlet-floatonly

test-gals-large-onebyone-betterverlet-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-large-onebyone-betterverlet-floatonly-reducedlocalcalcs-singleforceloop: test-gals-large-onebyone-betterverlet-floatonly-reducedlocalcalcs

test-gals-large-onebyone-betterverlet-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-large-onebyone-betterverlet-floatonly-singleforceloop: test-gals-large-onebyone-betterverlet-floatonly

test-gals-large-onebyone-betterverlet-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-gals-large-onebyone-betterverlet-reducedlocalcalcs: test-gals-large-onebyone-betterverlet

test-gals-large-onebyone-betterverlet-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-large-onebyone-betterverlet-reducedlocalcalcs-singleforceloop: test-gals-large-onebyone-betterverlet-reducedlocalcalcs

test-gals-large-onebyone-betterverlet-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-large-onebyone-betterverlet-singleforceloop: test-gals-large-onebyone-betterverlet

test-gals-large-onebyone-dtchange: DFLAGS+=-DSMALL_DT_EARLY
test-gals-large-onebyone-dtchange: test-gals-large-onebyone

test-gals-large-onebyone-dtchange-doublesqrt: DFLAGS+=-DOUBLE_SQRT
test-gals-large-onebyone-dtchange-doublesqrt: test-gals-large-onebyone-dtchange

test-gals-large-onebyone-dtchange-doublesqrt-msgmgmt: DFLAGS+=-DMESSAGE_MANAGEMENT
test-gals-large-onebyone-dtchange-doublesqrt-msgmgmt: test-gals-large-onebyone-dtchange-doublesqrt

test-gals-large-onebyone-dtchange-doublesqrt-msgmgmt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-gals-large-onebyone-dtchange-doublesqrt-msgmgmt-floatonly: test-gals-large-onebyone-dtchange-doublesqrt-msgmgmt

test-gals-large-onebyone-dtchange-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-gals-large-onebyone-dtchange-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs: test-gals-large-onebyone-dtchange-doublesqrt-msgmgmt-floatonly

test-gals-large-onebyone-dtchange-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-large-onebyone-dtchange-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs-singleforceloop: test-gals-large-onebyone-dtchange-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs

test-gals-large-onebyone-dtchange-doublesqrt-msgmgmt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-large-onebyone-dtchange-doublesqrt-msgmgmt-floatonly-singleforceloop: test-gals-large-onebyone-dtchange-doublesqrt-msgmgmt-floatonly

test-gals-large-onebyone-dtchange-doublesqrt-msgmgmt-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-gals-large-onebyone-dtchange-doublesqrt-msgmgmt-reducedlocalcalcs: test-gals-large-onebyone-dtchange-doublesqrt-msgmgmt

test-gals-large-onebyone-dtchange-doublesqrt-msgmgmt-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-large-onebyone-dtchange-doublesqrt-msgmgmt-reducedlocalcalcs-singleforceloop: test-gals-large-onebyone-dtchange-doublesqrt-msgmgmt-reducedlocalcalcs

test-gals-large-onebyone-dtchange-doublesqrt-msgmgmt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-large-onebyone-dtchange-doublesqrt-msgmgmt-singleforceloop: test-gals-large-onebyone-dtchange-doublesqrt-msgmgmt

test-gals-large-onebyone-dtchange-doublesqrt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-gals-large-onebyone-dtchange-doublesqrt-floatonly: test-gals-large-onebyone-dtchange-doublesqrt

test-gals-large-onebyone-dtchange-doublesqrt-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-gals-large-onebyone-dtchange-doublesqrt-floatonly-reducedlocalcalcs: test-gals-large-onebyone-dtchange-doublesqrt-floatonly

test-gals-large-onebyone-dtchange-doublesqrt-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-large-onebyone-dtchange-doublesqrt-floatonly-reducedlocalcalcs-singleforceloop: test-gals-large-onebyone-dtchange-doublesqrt-floatonly-reducedlocalcalcs

test-gals-large-onebyone-dtchange-doublesqrt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-large-onebyone-dtchange-doublesqrt-floatonly-singleforceloop: test-gals-large-onebyone-dtchange-doublesqrt-floatonly

test-gals-large-onebyone-dtchange-doublesqrt-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-gals-large-onebyone-dtchange-doublesqrt-reducedlocalcalcs: test-gals-large-onebyone-dtchange-doublesqrt

test-gals-large-onebyone-dtchange-doublesqrt-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-large-onebyone-dtchange-doublesqrt-reducedlocalcalcs-singleforceloop: test-gals-large-onebyone-dtchange-doublesqrt-reducedlocalcalcs

test-gals-large-onebyone-dtchange-doublesqrt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-large-onebyone-dtchange-doublesqrt-singleforceloop: test-gals-large-onebyone-dtchange-doublesqrt

test-gals-large-onebyone-dtchange-msgmgmt: DFLAGS+=-DMESSAGE_MANAGEMENT
test-gals-large-onebyone-dtchange-msgmgmt: test-gals-large-onebyone-dtchange

test-gals-large-onebyone-dtchange-msgmgmt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-gals-large-onebyone-dtchange-msgmgmt-floatonly: test-gals-large-onebyone-dtchange-msgmgmt

test-gals-large-onebyone-dtchange-msgmgmt-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-gals-large-onebyone-dtchange-msgmgmt-floatonly-reducedlocalcalcs: test-gals-large-onebyone-dtchange-msgmgmt-floatonly

test-gals-large-onebyone-dtchange-msgmgmt-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-large-onebyone-dtchange-msgmgmt-floatonly-reducedlocalcalcs-singleforceloop: test-gals-large-onebyone-dtchange-msgmgmt-floatonly-reducedlocalcalcs

test-gals-large-onebyone-dtchange-msgmgmt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-large-onebyone-dtchange-msgmgmt-floatonly-singleforceloop: test-gals-large-onebyone-dtchange-msgmgmt-floatonly

test-gals-large-onebyone-dtchange-msgmgmt-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-gals-large-onebyone-dtchange-msgmgmt-reducedlocalcalcs: test-gals-large-onebyone-dtchange-msgmgmt

test-gals-large-onebyone-dtchange-msgmgmt-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-large-onebyone-dtchange-msgmgmt-reducedlocalcalcs-singleforceloop: test-gals-large-onebyone-dtchange-msgmgmt-reducedlocalcalcs

test-gals-large-onebyone-dtchange-msgmgmt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-large-onebyone-dtchange-msgmgmt-singleforceloop: test-gals-large-onebyone-dtchange-msgmgmt

test-gals-large-onebyone-dtchange-floatonly: DFLAGS+=-DFLOAT_ONLY
test-gals-large-onebyone-dtchange-floatonly: test-gals-large-onebyone-dtchange

test-gals-large-onebyone-dtchange-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-gals-large-onebyone-dtchange-floatonly-reducedlocalcalcs: test-gals-large-onebyone-dtchange-floatonly

test-gals-large-onebyone-dtchange-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-large-onebyone-dtchange-floatonly-reducedlocalcalcs-singleforceloop: test-gals-large-onebyone-dtchange-floatonly-reducedlocalcalcs

test-gals-large-onebyone-dtchange-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-large-onebyone-dtchange-floatonly-singleforceloop: test-gals-large-onebyone-dtchange-floatonly

test-gals-large-onebyone-dtchange-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-gals-large-onebyone-dtchange-reducedlocalcalcs: test-gals-large-onebyone-dtchange

test-gals-large-onebyone-dtchange-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-large-onebyone-dtchange-reducedlocalcalcs-singleforceloop: test-gals-large-onebyone-dtchange-reducedlocalcalcs

test-gals-large-onebyone-dtchange-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-large-onebyone-dtchange-singleforceloop: test-gals-large-onebyone-dtchange

test-gals-large-onebyone-doublesqrt: DFLAGS+=-DOUBLE_SQRT
test-gals-large-onebyone-doublesqrt: test-gals-large-onebyone

test-gals-large-onebyone-doublesqrt-msgmgmt: DFLAGS+=-DMESSAGE_MANAGEMENT
test-gals-large-onebyone-doublesqrt-msgmgmt: test-gals-large-onebyone-doublesqrt

test-gals-large-onebyone-doublesqrt-msgmgmt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-gals-large-onebyone-doublesqrt-msgmgmt-floatonly: test-gals-large-onebyone-doublesqrt-msgmgmt

test-gals-large-onebyone-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-gals-large-onebyone-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs: test-gals-large-onebyone-doublesqrt-msgmgmt-floatonly

test-gals-large-onebyone-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-large-onebyone-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs-singleforceloop: test-gals-large-onebyone-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs

test-gals-large-onebyone-doublesqrt-msgmgmt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-large-onebyone-doublesqrt-msgmgmt-floatonly-singleforceloop: test-gals-large-onebyone-doublesqrt-msgmgmt-floatonly

test-gals-large-onebyone-doublesqrt-msgmgmt-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-gals-large-onebyone-doublesqrt-msgmgmt-reducedlocalcalcs: test-gals-large-onebyone-doublesqrt-msgmgmt

test-gals-large-onebyone-doublesqrt-msgmgmt-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-large-onebyone-doublesqrt-msgmgmt-reducedlocalcalcs-singleforceloop: test-gals-large-onebyone-doublesqrt-msgmgmt-reducedlocalcalcs

test-gals-large-onebyone-doublesqrt-msgmgmt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-large-onebyone-doublesqrt-msgmgmt-singleforceloop: test-gals-large-onebyone-doublesqrt-msgmgmt

test-gals-large-onebyone-doublesqrt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-gals-large-onebyone-doublesqrt-floatonly: test-gals-large-onebyone-doublesqrt

test-gals-large-onebyone-doublesqrt-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-gals-large-onebyone-doublesqrt-floatonly-reducedlocalcalcs: test-gals-large-onebyone-doublesqrt-floatonly

test-gals-large-onebyone-doublesqrt-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-large-onebyone-doublesqrt-floatonly-reducedlocalcalcs-singleforceloop: test-gals-large-onebyone-doublesqrt-floatonly-reducedlocalcalcs

test-gals-large-onebyone-doublesqrt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-large-onebyone-doublesqrt-floatonly-singleforceloop: test-gals-large-onebyone-doublesqrt-floatonly

test-gals-large-onebyone-doublesqrt-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-gals-large-onebyone-doublesqrt-reducedlocalcalcs: test-gals-large-onebyone-doublesqrt

test-gals-large-onebyone-doublesqrt-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-large-onebyone-doublesqrt-reducedlocalcalcs-singleforceloop: test-gals-large-onebyone-doublesqrt-reducedlocalcalcs

test-gals-large-onebyone-doublesqrt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-large-onebyone-doublesqrt-singleforceloop: test-gals-large-onebyone-doublesqrt

test-gals-large-onebyone-msgmgmt: DFLAGS+=-DMESSAGE_MANAGEMENT
test-gals-large-onebyone-msgmgmt: test-gals-large-onebyone

test-gals-large-onebyone-msgmgmt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-gals-large-onebyone-msgmgmt-floatonly: test-gals-large-onebyone-msgmgmt

test-gals-large-onebyone-msgmgmt-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-gals-large-onebyone-msgmgmt-floatonly-reducedlocalcalcs: test-gals-large-onebyone-msgmgmt-floatonly

test-gals-large-onebyone-msgmgmt-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-large-onebyone-msgmgmt-floatonly-reducedlocalcalcs-singleforceloop: test-gals-large-onebyone-msgmgmt-floatonly-reducedlocalcalcs

test-gals-large-onebyone-msgmgmt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-large-onebyone-msgmgmt-floatonly-singleforceloop: test-gals-large-onebyone-msgmgmt-floatonly

test-gals-large-onebyone-msgmgmt-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-gals-large-onebyone-msgmgmt-reducedlocalcalcs: test-gals-large-onebyone-msgmgmt

test-gals-large-onebyone-msgmgmt-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-large-onebyone-msgmgmt-reducedlocalcalcs-singleforceloop: test-gals-large-onebyone-msgmgmt-reducedlocalcalcs

test-gals-large-onebyone-msgmgmt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-large-onebyone-msgmgmt-singleforceloop: test-gals-large-onebyone-msgmgmt

test-gals-large-onebyone-floatonly: DFLAGS+=-DFLOAT_ONLY
test-gals-large-onebyone-floatonly: test-gals-large-onebyone

test-gals-large-onebyone-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-gals-large-onebyone-floatonly-reducedlocalcalcs: test-gals-large-onebyone-floatonly

test-gals-large-onebyone-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-large-onebyone-floatonly-reducedlocalcalcs-singleforceloop: test-gals-large-onebyone-floatonly-reducedlocalcalcs

test-gals-large-onebyone-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-large-onebyone-floatonly-singleforceloop: test-gals-large-onebyone-floatonly

test-gals-large-onebyone-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-gals-large-onebyone-reducedlocalcalcs: test-gals-large-onebyone

test-gals-large-onebyone-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-large-onebyone-reducedlocalcalcs-singleforceloop: test-gals-large-onebyone-reducedlocalcalcs

test-gals-large-onebyone-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-large-onebyone-singleforceloop: test-gals-large-onebyone

test-gals-bonds: DFLAGS+=-DBONDS
test-gals-bonds: test-gals

test-gals-bonds-betterverlet: DFLAGS+=-DBETTER_VERLET
test-gals-bonds-betterverlet: test-gals-bonds

test-gals-bonds-betterverlet-dtchange: DFLAGS+=-DSMALL_DT_EARLY
test-gals-bonds-betterverlet-dtchange: test-gals-bonds-betterverlet

test-gals-bonds-betterverlet-dtchange-doublesqrt: DFLAGS+=-DOUBLE_SQRT
test-gals-bonds-betterverlet-dtchange-doublesqrt: test-gals-bonds-betterverlet-dtchange

test-gals-bonds-betterverlet-dtchange-doublesqrt-msgmgmt: DFLAGS+=-DMESSAGE_MANAGEMENT
test-gals-bonds-betterverlet-dtchange-doublesqrt-msgmgmt: test-gals-bonds-betterverlet-dtchange-doublesqrt

test-gals-bonds-betterverlet-dtchange-doublesqrt-msgmgmt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-gals-bonds-betterverlet-dtchange-doublesqrt-msgmgmt-floatonly: test-gals-bonds-betterverlet-dtchange-doublesqrt-msgmgmt

test-gals-bonds-betterverlet-dtchange-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-gals-bonds-betterverlet-dtchange-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs: test-gals-bonds-betterverlet-dtchange-doublesqrt-msgmgmt-floatonly

test-gals-bonds-betterverlet-dtchange-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-bonds-betterverlet-dtchange-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs-singleforceloop: test-gals-bonds-betterverlet-dtchange-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs

test-gals-bonds-betterverlet-dtchange-doublesqrt-msgmgmt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-bonds-betterverlet-dtchange-doublesqrt-msgmgmt-floatonly-singleforceloop: test-gals-bonds-betterverlet-dtchange-doublesqrt-msgmgmt-floatonly

test-gals-bonds-betterverlet-dtchange-doublesqrt-msgmgmt-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-gals-bonds-betterverlet-dtchange-doublesqrt-msgmgmt-reducedlocalcalcs: test-gals-bonds-betterverlet-dtchange-doublesqrt-msgmgmt

test-gals-bonds-betterverlet-dtchange-doublesqrt-msgmgmt-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-bonds-betterverlet-dtchange-doublesqrt-msgmgmt-reducedlocalcalcs-singleforceloop: test-gals-bonds-betterverlet-dtchange-doublesqrt-msgmgmt-reducedlocalcalcs

test-gals-bonds-betterverlet-dtchange-doublesqrt-msgmgmt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-bonds-betterverlet-dtchange-doublesqrt-msgmgmt-singleforceloop: test-gals-bonds-betterverlet-dtchange-doublesqrt-msgmgmt

test-gals-bonds-betterverlet-dtchange-doublesqrt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-gals-bonds-betterverlet-dtchange-doublesqrt-floatonly: test-gals-bonds-betterverlet-dtchange-doublesqrt

test-gals-bonds-betterverlet-dtchange-doublesqrt-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-gals-bonds-betterverlet-dtchange-doublesqrt-floatonly-reducedlocalcalcs: test-gals-bonds-betterverlet-dtchange-doublesqrt-floatonly

test-gals-bonds-betterverlet-dtchange-doublesqrt-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-bonds-betterverlet-dtchange-doublesqrt-floatonly-reducedlocalcalcs-singleforceloop: test-gals-bonds-betterverlet-dtchange-doublesqrt-floatonly-reducedlocalcalcs

test-gals-bonds-betterverlet-dtchange-doublesqrt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-bonds-betterverlet-dtchange-doublesqrt-floatonly-singleforceloop: test-gals-bonds-betterverlet-dtchange-doublesqrt-floatonly

test-gals-bonds-betterverlet-dtchange-doublesqrt-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-gals-bonds-betterverlet-dtchange-doublesqrt-reducedlocalcalcs: test-gals-bonds-betterverlet-dtchange-doublesqrt

test-gals-bonds-betterverlet-dtchange-doublesqrt-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-bonds-betterverlet-dtchange-doublesqrt-reducedlocalcalcs-singleforceloop: test-gals-bonds-betterverlet-dtchange-doublesqrt-reducedlocalcalcs

test-gals-bonds-betterverlet-dtchange-doublesqrt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-bonds-betterverlet-dtchange-doublesqrt-singleforceloop: test-gals-bonds-betterverlet-dtchange-doublesqrt

test-gals-bonds-betterverlet-dtchange-msgmgmt: DFLAGS+=-DMESSAGE_MANAGEMENT
test-gals-bonds-betterverlet-dtchange-msgmgmt: test-gals-bonds-betterverlet-dtchange

test-gals-bonds-betterverlet-dtchange-msgmgmt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-gals-bonds-betterverlet-dtchange-msgmgmt-floatonly: test-gals-bonds-betterverlet-dtchange-msgmgmt

test-gals-bonds-betterverlet-dtchange-msgmgmt-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-gals-bonds-betterverlet-dtchange-msgmgmt-floatonly-reducedlocalcalcs: test-gals-bonds-betterverlet-dtchange-msgmgmt-floatonly

test-gals-bonds-betterverlet-dtchange-msgmgmt-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-bonds-betterverlet-dtchange-msgmgmt-floatonly-reducedlocalcalcs-singleforceloop: test-gals-bonds-betterverlet-dtchange-msgmgmt-floatonly-reducedlocalcalcs

test-gals-bonds-betterverlet-dtchange-msgmgmt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-bonds-betterverlet-dtchange-msgmgmt-floatonly-singleforceloop: test-gals-bonds-betterverlet-dtchange-msgmgmt-floatonly

test-gals-bonds-betterverlet-dtchange-msgmgmt-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-gals-bonds-betterverlet-dtchange-msgmgmt-reducedlocalcalcs: test-gals-bonds-betterverlet-dtchange-msgmgmt

test-gals-bonds-betterverlet-dtchange-msgmgmt-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-bonds-betterverlet-dtchange-msgmgmt-reducedlocalcalcs-singleforceloop: test-gals-bonds-betterverlet-dtchange-msgmgmt-reducedlocalcalcs

test-gals-bonds-betterverlet-dtchange-msgmgmt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-bonds-betterverlet-dtchange-msgmgmt-singleforceloop: test-gals-bonds-betterverlet-dtchange-msgmgmt

test-gals-bonds-betterverlet-dtchange-floatonly: DFLAGS+=-DFLOAT_ONLY
test-gals-bonds-betterverlet-dtchange-floatonly: test-gals-bonds-betterverlet-dtchange

test-gals-bonds-betterverlet-dtchange-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-gals-bonds-betterverlet-dtchange-floatonly-reducedlocalcalcs: test-gals-bonds-betterverlet-dtchange-floatonly

test-gals-bonds-betterverlet-dtchange-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-bonds-betterverlet-dtchange-floatonly-reducedlocalcalcs-singleforceloop: test-gals-bonds-betterverlet-dtchange-floatonly-reducedlocalcalcs

test-gals-bonds-betterverlet-dtchange-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-bonds-betterverlet-dtchange-floatonly-singleforceloop: test-gals-bonds-betterverlet-dtchange-floatonly

test-gals-bonds-betterverlet-dtchange-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-gals-bonds-betterverlet-dtchange-reducedlocalcalcs: test-gals-bonds-betterverlet-dtchange

test-gals-bonds-betterverlet-dtchange-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-bonds-betterverlet-dtchange-reducedlocalcalcs-singleforceloop: test-gals-bonds-betterverlet-dtchange-reducedlocalcalcs

test-gals-bonds-betterverlet-dtchange-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-bonds-betterverlet-dtchange-singleforceloop: test-gals-bonds-betterverlet-dtchange

test-gals-bonds-betterverlet-doublesqrt: DFLAGS+=-DOUBLE_SQRT
test-gals-bonds-betterverlet-doublesqrt: test-gals-bonds-betterverlet

test-gals-bonds-betterverlet-doublesqrt-msgmgmt: DFLAGS+=-DMESSAGE_MANAGEMENT
test-gals-bonds-betterverlet-doublesqrt-msgmgmt: test-gals-bonds-betterverlet-doublesqrt

test-gals-bonds-betterverlet-doublesqrt-msgmgmt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-gals-bonds-betterverlet-doublesqrt-msgmgmt-floatonly: test-gals-bonds-betterverlet-doublesqrt-msgmgmt

test-gals-bonds-betterverlet-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-gals-bonds-betterverlet-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs: test-gals-bonds-betterverlet-doublesqrt-msgmgmt-floatonly

test-gals-bonds-betterverlet-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-bonds-betterverlet-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs-singleforceloop: test-gals-bonds-betterverlet-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs

test-gals-bonds-betterverlet-doublesqrt-msgmgmt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-bonds-betterverlet-doublesqrt-msgmgmt-floatonly-singleforceloop: test-gals-bonds-betterverlet-doublesqrt-msgmgmt-floatonly

test-gals-bonds-betterverlet-doublesqrt-msgmgmt-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-gals-bonds-betterverlet-doublesqrt-msgmgmt-reducedlocalcalcs: test-gals-bonds-betterverlet-doublesqrt-msgmgmt

test-gals-bonds-betterverlet-doublesqrt-msgmgmt-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-bonds-betterverlet-doublesqrt-msgmgmt-reducedlocalcalcs-singleforceloop: test-gals-bonds-betterverlet-doublesqrt-msgmgmt-reducedlocalcalcs

test-gals-bonds-betterverlet-doublesqrt-msgmgmt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-bonds-betterverlet-doublesqrt-msgmgmt-singleforceloop: test-gals-bonds-betterverlet-doublesqrt-msgmgmt

test-gals-bonds-betterverlet-doublesqrt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-gals-bonds-betterverlet-doublesqrt-floatonly: test-gals-bonds-betterverlet-doublesqrt

test-gals-bonds-betterverlet-doublesqrt-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-gals-bonds-betterverlet-doublesqrt-floatonly-reducedlocalcalcs: test-gals-bonds-betterverlet-doublesqrt-floatonly

test-gals-bonds-betterverlet-doublesqrt-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-bonds-betterverlet-doublesqrt-floatonly-reducedlocalcalcs-singleforceloop: test-gals-bonds-betterverlet-doublesqrt-floatonly-reducedlocalcalcs

test-gals-bonds-betterverlet-doublesqrt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-bonds-betterverlet-doublesqrt-floatonly-singleforceloop: test-gals-bonds-betterverlet-doublesqrt-floatonly

test-gals-bonds-betterverlet-doublesqrt-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-gals-bonds-betterverlet-doublesqrt-reducedlocalcalcs: test-gals-bonds-betterverlet-doublesqrt

test-gals-bonds-betterverlet-doublesqrt-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-bonds-betterverlet-doublesqrt-reducedlocalcalcs-singleforceloop: test-gals-bonds-betterverlet-doublesqrt-reducedlocalcalcs

test-gals-bonds-betterverlet-doublesqrt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-bonds-betterverlet-doublesqrt-singleforceloop: test-gals-bonds-betterverlet-doublesqrt

test-gals-bonds-betterverlet-msgmgmt: DFLAGS+=-DMESSAGE_MANAGEMENT
test-gals-bonds-betterverlet-msgmgmt: test-gals-bonds-betterverlet

test-gals-bonds-betterverlet-msgmgmt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-gals-bonds-betterverlet-msgmgmt-floatonly: test-gals-bonds-betterverlet-msgmgmt

test-gals-bonds-betterverlet-msgmgmt-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-gals-bonds-betterverlet-msgmgmt-floatonly-reducedlocalcalcs: test-gals-bonds-betterverlet-msgmgmt-floatonly

test-gals-bonds-betterverlet-msgmgmt-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-bonds-betterverlet-msgmgmt-floatonly-reducedlocalcalcs-singleforceloop: test-gals-bonds-betterverlet-msgmgmt-floatonly-reducedlocalcalcs

test-gals-bonds-betterverlet-msgmgmt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-bonds-betterverlet-msgmgmt-floatonly-singleforceloop: test-gals-bonds-betterverlet-msgmgmt-floatonly

test-gals-bonds-betterverlet-msgmgmt-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-gals-bonds-betterverlet-msgmgmt-reducedlocalcalcs: test-gals-bonds-betterverlet-msgmgmt

test-gals-bonds-betterverlet-msgmgmt-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-bonds-betterverlet-msgmgmt-reducedlocalcalcs-singleforceloop: test-gals-bonds-betterverlet-msgmgmt-reducedlocalcalcs

test-gals-bonds-betterverlet-msgmgmt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-bonds-betterverlet-msgmgmt-singleforceloop: test-gals-bonds-betterverlet-msgmgmt

test-gals-bonds-betterverlet-floatonly: DFLAGS+=-DFLOAT_ONLY
test-gals-bonds-betterverlet-floatonly: test-gals-bonds-betterverlet

test-gals-bonds-betterverlet-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-gals-bonds-betterverlet-floatonly-reducedlocalcalcs: test-gals-bonds-betterverlet-floatonly

test-gals-bonds-betterverlet-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-bonds-betterverlet-floatonly-reducedlocalcalcs-singleforceloop: test-gals-bonds-betterverlet-floatonly-reducedlocalcalcs

test-gals-bonds-betterverlet-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-bonds-betterverlet-floatonly-singleforceloop: test-gals-bonds-betterverlet-floatonly

test-gals-bonds-betterverlet-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-gals-bonds-betterverlet-reducedlocalcalcs: test-gals-bonds-betterverlet

test-gals-bonds-betterverlet-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-bonds-betterverlet-reducedlocalcalcs-singleforceloop: test-gals-bonds-betterverlet-reducedlocalcalcs

test-gals-bonds-betterverlet-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-bonds-betterverlet-singleforceloop: test-gals-bonds-betterverlet

test-gals-bonds-dtchange: DFLAGS+=-DSMALL_DT_EARLY
test-gals-bonds-dtchange: test-gals-bonds

test-gals-bonds-dtchange-doublesqrt: DFLAGS+=-DOUBLE_SQRT
test-gals-bonds-dtchange-doublesqrt: test-gals-bonds-dtchange

test-gals-bonds-dtchange-doublesqrt-msgmgmt: DFLAGS+=-DMESSAGE_MANAGEMENT
test-gals-bonds-dtchange-doublesqrt-msgmgmt: test-gals-bonds-dtchange-doublesqrt

test-gals-bonds-dtchange-doublesqrt-msgmgmt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-gals-bonds-dtchange-doublesqrt-msgmgmt-floatonly: test-gals-bonds-dtchange-doublesqrt-msgmgmt

test-gals-bonds-dtchange-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-gals-bonds-dtchange-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs: test-gals-bonds-dtchange-doublesqrt-msgmgmt-floatonly

test-gals-bonds-dtchange-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-bonds-dtchange-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs-singleforceloop: test-gals-bonds-dtchange-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs

test-gals-bonds-dtchange-doublesqrt-msgmgmt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-bonds-dtchange-doublesqrt-msgmgmt-floatonly-singleforceloop: test-gals-bonds-dtchange-doublesqrt-msgmgmt-floatonly

test-gals-bonds-dtchange-doublesqrt-msgmgmt-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-gals-bonds-dtchange-doublesqrt-msgmgmt-reducedlocalcalcs: test-gals-bonds-dtchange-doublesqrt-msgmgmt

test-gals-bonds-dtchange-doublesqrt-msgmgmt-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-bonds-dtchange-doublesqrt-msgmgmt-reducedlocalcalcs-singleforceloop: test-gals-bonds-dtchange-doublesqrt-msgmgmt-reducedlocalcalcs

test-gals-bonds-dtchange-doublesqrt-msgmgmt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-bonds-dtchange-doublesqrt-msgmgmt-singleforceloop: test-gals-bonds-dtchange-doublesqrt-msgmgmt

test-gals-bonds-dtchange-doublesqrt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-gals-bonds-dtchange-doublesqrt-floatonly: test-gals-bonds-dtchange-doublesqrt

test-gals-bonds-dtchange-doublesqrt-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-gals-bonds-dtchange-doublesqrt-floatonly-reducedlocalcalcs: test-gals-bonds-dtchange-doublesqrt-floatonly

test-gals-bonds-dtchange-doublesqrt-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-bonds-dtchange-doublesqrt-floatonly-reducedlocalcalcs-singleforceloop: test-gals-bonds-dtchange-doublesqrt-floatonly-reducedlocalcalcs

test-gals-bonds-dtchange-doublesqrt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-bonds-dtchange-doublesqrt-floatonly-singleforceloop: test-gals-bonds-dtchange-doublesqrt-floatonly

test-gals-bonds-dtchange-doublesqrt-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-gals-bonds-dtchange-doublesqrt-reducedlocalcalcs: test-gals-bonds-dtchange-doublesqrt

test-gals-bonds-dtchange-doublesqrt-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-bonds-dtchange-doublesqrt-reducedlocalcalcs-singleforceloop: test-gals-bonds-dtchange-doublesqrt-reducedlocalcalcs

test-gals-bonds-dtchange-doublesqrt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-bonds-dtchange-doublesqrt-singleforceloop: test-gals-bonds-dtchange-doublesqrt

test-gals-bonds-dtchange-msgmgmt: DFLAGS+=-DMESSAGE_MANAGEMENT
test-gals-bonds-dtchange-msgmgmt: test-gals-bonds-dtchange

test-gals-bonds-dtchange-msgmgmt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-gals-bonds-dtchange-msgmgmt-floatonly: test-gals-bonds-dtchange-msgmgmt

test-gals-bonds-dtchange-msgmgmt-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-gals-bonds-dtchange-msgmgmt-floatonly-reducedlocalcalcs: test-gals-bonds-dtchange-msgmgmt-floatonly

test-gals-bonds-dtchange-msgmgmt-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-bonds-dtchange-msgmgmt-floatonly-reducedlocalcalcs-singleforceloop: test-gals-bonds-dtchange-msgmgmt-floatonly-reducedlocalcalcs

test-gals-bonds-dtchange-msgmgmt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-bonds-dtchange-msgmgmt-floatonly-singleforceloop: test-gals-bonds-dtchange-msgmgmt-floatonly

test-gals-bonds-dtchange-msgmgmt-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-gals-bonds-dtchange-msgmgmt-reducedlocalcalcs: test-gals-bonds-dtchange-msgmgmt

test-gals-bonds-dtchange-msgmgmt-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-bonds-dtchange-msgmgmt-reducedlocalcalcs-singleforceloop: test-gals-bonds-dtchange-msgmgmt-reducedlocalcalcs

test-gals-bonds-dtchange-msgmgmt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-bonds-dtchange-msgmgmt-singleforceloop: test-gals-bonds-dtchange-msgmgmt

test-gals-bonds-dtchange-floatonly: DFLAGS+=-DFLOAT_ONLY
test-gals-bonds-dtchange-floatonly: test-gals-bonds-dtchange

test-gals-bonds-dtchange-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-gals-bonds-dtchange-floatonly-reducedlocalcalcs: test-gals-bonds-dtchange-floatonly

test-gals-bonds-dtchange-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-bonds-dtchange-floatonly-reducedlocalcalcs-singleforceloop: test-gals-bonds-dtchange-floatonly-reducedlocalcalcs

test-gals-bonds-dtchange-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-bonds-dtchange-floatonly-singleforceloop: test-gals-bonds-dtchange-floatonly

test-gals-bonds-dtchange-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-gals-bonds-dtchange-reducedlocalcalcs: test-gals-bonds-dtchange

test-gals-bonds-dtchange-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-bonds-dtchange-reducedlocalcalcs-singleforceloop: test-gals-bonds-dtchange-reducedlocalcalcs

test-gals-bonds-dtchange-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-bonds-dtchange-singleforceloop: test-gals-bonds-dtchange

test-gals-bonds-doublesqrt: DFLAGS+=-DOUBLE_SQRT
test-gals-bonds-doublesqrt: test-gals-bonds

test-gals-bonds-doublesqrt-msgmgmt: DFLAGS+=-DMESSAGE_MANAGEMENT
test-gals-bonds-doublesqrt-msgmgmt: test-gals-bonds-doublesqrt

test-gals-bonds-doublesqrt-msgmgmt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-gals-bonds-doublesqrt-msgmgmt-floatonly: test-gals-bonds-doublesqrt-msgmgmt

test-gals-bonds-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-gals-bonds-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs: test-gals-bonds-doublesqrt-msgmgmt-floatonly

test-gals-bonds-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-bonds-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs-singleforceloop: test-gals-bonds-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs

test-gals-bonds-doublesqrt-msgmgmt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-bonds-doublesqrt-msgmgmt-floatonly-singleforceloop: test-gals-bonds-doublesqrt-msgmgmt-floatonly

test-gals-bonds-doublesqrt-msgmgmt-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-gals-bonds-doublesqrt-msgmgmt-reducedlocalcalcs: test-gals-bonds-doublesqrt-msgmgmt

test-gals-bonds-doublesqrt-msgmgmt-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-bonds-doublesqrt-msgmgmt-reducedlocalcalcs-singleforceloop: test-gals-bonds-doublesqrt-msgmgmt-reducedlocalcalcs

test-gals-bonds-doublesqrt-msgmgmt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-bonds-doublesqrt-msgmgmt-singleforceloop: test-gals-bonds-doublesqrt-msgmgmt

test-gals-bonds-doublesqrt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-gals-bonds-doublesqrt-floatonly: test-gals-bonds-doublesqrt

test-gals-bonds-doublesqrt-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-gals-bonds-doublesqrt-floatonly-reducedlocalcalcs: test-gals-bonds-doublesqrt-floatonly

test-gals-bonds-doublesqrt-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-bonds-doublesqrt-floatonly-reducedlocalcalcs-singleforceloop: test-gals-bonds-doublesqrt-floatonly-reducedlocalcalcs

test-gals-bonds-doublesqrt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-bonds-doublesqrt-floatonly-singleforceloop: test-gals-bonds-doublesqrt-floatonly

test-gals-bonds-doublesqrt-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-gals-bonds-doublesqrt-reducedlocalcalcs: test-gals-bonds-doublesqrt

test-gals-bonds-doublesqrt-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-bonds-doublesqrt-reducedlocalcalcs-singleforceloop: test-gals-bonds-doublesqrt-reducedlocalcalcs

test-gals-bonds-doublesqrt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-bonds-doublesqrt-singleforceloop: test-gals-bonds-doublesqrt

test-gals-bonds-msgmgmt: DFLAGS+=-DMESSAGE_MANAGEMENT
test-gals-bonds-msgmgmt: test-gals-bonds

test-gals-bonds-msgmgmt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-gals-bonds-msgmgmt-floatonly: test-gals-bonds-msgmgmt

test-gals-bonds-msgmgmt-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-gals-bonds-msgmgmt-floatonly-reducedlocalcalcs: test-gals-bonds-msgmgmt-floatonly

test-gals-bonds-msgmgmt-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-bonds-msgmgmt-floatonly-reducedlocalcalcs-singleforceloop: test-gals-bonds-msgmgmt-floatonly-reducedlocalcalcs

test-gals-bonds-msgmgmt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-bonds-msgmgmt-floatonly-singleforceloop: test-gals-bonds-msgmgmt-floatonly

test-gals-bonds-msgmgmt-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-gals-bonds-msgmgmt-reducedlocalcalcs: test-gals-bonds-msgmgmt

test-gals-bonds-msgmgmt-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-bonds-msgmgmt-reducedlocalcalcs-singleforceloop: test-gals-bonds-msgmgmt-reducedlocalcalcs

test-gals-bonds-msgmgmt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-bonds-msgmgmt-singleforceloop: test-gals-bonds-msgmgmt

test-gals-bonds-floatonly: DFLAGS+=-DFLOAT_ONLY
test-gals-bonds-floatonly: test-gals-bonds

test-gals-bonds-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-gals-bonds-floatonly-reducedlocalcalcs: test-gals-bonds-floatonly

test-gals-bonds-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-bonds-floatonly-reducedlocalcalcs-singleforceloop: test-gals-bonds-floatonly-reducedlocalcalcs

test-gals-bonds-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-bonds-floatonly-singleforceloop: test-gals-bonds-floatonly

test-gals-bonds-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-gals-bonds-reducedlocalcalcs: test-gals-bonds

test-gals-bonds-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-bonds-reducedlocalcalcs-singleforceloop: test-gals-bonds-reducedlocalcalcs

test-gals-bonds-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-bonds-singleforceloop: test-gals-bonds

# **TEST Local calculation method: onebyone**
test-gals-bonds-onebyone: DFLAGS+=-DONE_BY_ONE
test-gals-bonds-onebyone: test-gals-bonds

test-gals-bonds-onebyone-betterverlet: DFLAGS+=-DBETTER_VERLET
test-gals-bonds-onebyone-betterverlet: test-gals-bonds-onebyone

test-gals-bonds-onebyone-betterverlet-dtchange: DFLAGS+=-DSMALL_DT_EARLY
test-gals-bonds-onebyone-betterverlet-dtchange: test-gals-bonds-onebyone-betterverlet

test-gals-bonds-onebyone-betterverlet-dtchange-doublesqrt: DFLAGS+=-DOUBLE_SQRT
test-gals-bonds-onebyone-betterverlet-dtchange-doublesqrt: test-gals-bonds-onebyone-betterverlet-dtchange

test-gals-bonds-onebyone-betterverlet-dtchange-doublesqrt-msgmgmt: DFLAGS+=-DMESSAGE_MANAGEMENT
test-gals-bonds-onebyone-betterverlet-dtchange-doublesqrt-msgmgmt: test-gals-bonds-onebyone-betterverlet-dtchange-doublesqrt

test-gals-bonds-onebyone-betterverlet-dtchange-doublesqrt-msgmgmt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-gals-bonds-onebyone-betterverlet-dtchange-doublesqrt-msgmgmt-floatonly: test-gals-bonds-onebyone-betterverlet-dtchange-doublesqrt-msgmgmt

test-gals-bonds-onebyone-betterverlet-dtchange-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-gals-bonds-onebyone-betterverlet-dtchange-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs: test-gals-bonds-onebyone-betterverlet-dtchange-doublesqrt-msgmgmt-floatonly

test-gals-bonds-onebyone-betterverlet-dtchange-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-bonds-onebyone-betterverlet-dtchange-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs-singleforceloop: test-gals-bonds-onebyone-betterverlet-dtchange-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs

test-gals-bonds-onebyone-betterverlet-dtchange-doublesqrt-msgmgmt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-bonds-onebyone-betterverlet-dtchange-doublesqrt-msgmgmt-floatonly-singleforceloop: test-gals-bonds-onebyone-betterverlet-dtchange-doublesqrt-msgmgmt-floatonly

test-gals-bonds-onebyone-betterverlet-dtchange-doublesqrt-msgmgmt-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-gals-bonds-onebyone-betterverlet-dtchange-doublesqrt-msgmgmt-reducedlocalcalcs: test-gals-bonds-onebyone-betterverlet-dtchange-doublesqrt-msgmgmt

test-gals-bonds-onebyone-betterverlet-dtchange-doublesqrt-msgmgmt-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-bonds-onebyone-betterverlet-dtchange-doublesqrt-msgmgmt-reducedlocalcalcs-singleforceloop: test-gals-bonds-onebyone-betterverlet-dtchange-doublesqrt-msgmgmt-reducedlocalcalcs

test-gals-bonds-onebyone-betterverlet-dtchange-doublesqrt-msgmgmt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-bonds-onebyone-betterverlet-dtchange-doublesqrt-msgmgmt-singleforceloop: test-gals-bonds-onebyone-betterverlet-dtchange-doublesqrt-msgmgmt

test-gals-bonds-onebyone-betterverlet-dtchange-doublesqrt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-gals-bonds-onebyone-betterverlet-dtchange-doublesqrt-floatonly: test-gals-bonds-onebyone-betterverlet-dtchange-doublesqrt

test-gals-bonds-onebyone-betterverlet-dtchange-doublesqrt-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-gals-bonds-onebyone-betterverlet-dtchange-doublesqrt-floatonly-reducedlocalcalcs: test-gals-bonds-onebyone-betterverlet-dtchange-doublesqrt-floatonly

test-gals-bonds-onebyone-betterverlet-dtchange-doublesqrt-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-bonds-onebyone-betterverlet-dtchange-doublesqrt-floatonly-reducedlocalcalcs-singleforceloop: test-gals-bonds-onebyone-betterverlet-dtchange-doublesqrt-floatonly-reducedlocalcalcs

test-gals-bonds-onebyone-betterverlet-dtchange-doublesqrt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-bonds-onebyone-betterverlet-dtchange-doublesqrt-floatonly-singleforceloop: test-gals-bonds-onebyone-betterverlet-dtchange-doublesqrt-floatonly

test-gals-bonds-onebyone-betterverlet-dtchange-doublesqrt-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-gals-bonds-onebyone-betterverlet-dtchange-doublesqrt-reducedlocalcalcs: test-gals-bonds-onebyone-betterverlet-dtchange-doublesqrt

test-gals-bonds-onebyone-betterverlet-dtchange-doublesqrt-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-bonds-onebyone-betterverlet-dtchange-doublesqrt-reducedlocalcalcs-singleforceloop: test-gals-bonds-onebyone-betterverlet-dtchange-doublesqrt-reducedlocalcalcs

test-gals-bonds-onebyone-betterverlet-dtchange-doublesqrt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-bonds-onebyone-betterverlet-dtchange-doublesqrt-singleforceloop: test-gals-bonds-onebyone-betterverlet-dtchange-doublesqrt

test-gals-bonds-onebyone-betterverlet-dtchange-msgmgmt: DFLAGS+=-DMESSAGE_MANAGEMENT
test-gals-bonds-onebyone-betterverlet-dtchange-msgmgmt: test-gals-bonds-onebyone-betterverlet-dtchange

test-gals-bonds-onebyone-betterverlet-dtchange-msgmgmt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-gals-bonds-onebyone-betterverlet-dtchange-msgmgmt-floatonly: test-gals-bonds-onebyone-betterverlet-dtchange-msgmgmt

test-gals-bonds-onebyone-betterverlet-dtchange-msgmgmt-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-gals-bonds-onebyone-betterverlet-dtchange-msgmgmt-floatonly-reducedlocalcalcs: test-gals-bonds-onebyone-betterverlet-dtchange-msgmgmt-floatonly

test-gals-bonds-onebyone-betterverlet-dtchange-msgmgmt-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-bonds-onebyone-betterverlet-dtchange-msgmgmt-floatonly-reducedlocalcalcs-singleforceloop: test-gals-bonds-onebyone-betterverlet-dtchange-msgmgmt-floatonly-reducedlocalcalcs

test-gals-bonds-onebyone-betterverlet-dtchange-msgmgmt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-bonds-onebyone-betterverlet-dtchange-msgmgmt-floatonly-singleforceloop: test-gals-bonds-onebyone-betterverlet-dtchange-msgmgmt-floatonly

test-gals-bonds-onebyone-betterverlet-dtchange-msgmgmt-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-gals-bonds-onebyone-betterverlet-dtchange-msgmgmt-reducedlocalcalcs: test-gals-bonds-onebyone-betterverlet-dtchange-msgmgmt

test-gals-bonds-onebyone-betterverlet-dtchange-msgmgmt-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-bonds-onebyone-betterverlet-dtchange-msgmgmt-reducedlocalcalcs-singleforceloop: test-gals-bonds-onebyone-betterverlet-dtchange-msgmgmt-reducedlocalcalcs

test-gals-bonds-onebyone-betterverlet-dtchange-msgmgmt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-bonds-onebyone-betterverlet-dtchange-msgmgmt-singleforceloop: test-gals-bonds-onebyone-betterverlet-dtchange-msgmgmt

test-gals-bonds-onebyone-betterverlet-dtchange-floatonly: DFLAGS+=-DFLOAT_ONLY
test-gals-bonds-onebyone-betterverlet-dtchange-floatonly: test-gals-bonds-onebyone-betterverlet-dtchange

test-gals-bonds-onebyone-betterverlet-dtchange-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-gals-bonds-onebyone-betterverlet-dtchange-floatonly-reducedlocalcalcs: test-gals-bonds-onebyone-betterverlet-dtchange-floatonly

test-gals-bonds-onebyone-betterverlet-dtchange-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-bonds-onebyone-betterverlet-dtchange-floatonly-reducedlocalcalcs-singleforceloop: test-gals-bonds-onebyone-betterverlet-dtchange-floatonly-reducedlocalcalcs

test-gals-bonds-onebyone-betterverlet-dtchange-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-bonds-onebyone-betterverlet-dtchange-floatonly-singleforceloop: test-gals-bonds-onebyone-betterverlet-dtchange-floatonly

test-gals-bonds-onebyone-betterverlet-dtchange-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-gals-bonds-onebyone-betterverlet-dtchange-reducedlocalcalcs: test-gals-bonds-onebyone-betterverlet-dtchange

test-gals-bonds-onebyone-betterverlet-dtchange-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-bonds-onebyone-betterverlet-dtchange-reducedlocalcalcs-singleforceloop: test-gals-bonds-onebyone-betterverlet-dtchange-reducedlocalcalcs

test-gals-bonds-onebyone-betterverlet-dtchange-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-bonds-onebyone-betterverlet-dtchange-singleforceloop: test-gals-bonds-onebyone-betterverlet-dtchange

test-gals-bonds-onebyone-betterverlet-doublesqrt: DFLAGS+=-DOUBLE_SQRT
test-gals-bonds-onebyone-betterverlet-doublesqrt: test-gals-bonds-onebyone-betterverlet

test-gals-bonds-onebyone-betterverlet-doublesqrt-msgmgmt: DFLAGS+=-DMESSAGE_MANAGEMENT
test-gals-bonds-onebyone-betterverlet-doublesqrt-msgmgmt: test-gals-bonds-onebyone-betterverlet-doublesqrt

test-gals-bonds-onebyone-betterverlet-doublesqrt-msgmgmt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-gals-bonds-onebyone-betterverlet-doublesqrt-msgmgmt-floatonly: test-gals-bonds-onebyone-betterverlet-doublesqrt-msgmgmt

test-gals-bonds-onebyone-betterverlet-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-gals-bonds-onebyone-betterverlet-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs: test-gals-bonds-onebyone-betterverlet-doublesqrt-msgmgmt-floatonly

test-gals-bonds-onebyone-betterverlet-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-bonds-onebyone-betterverlet-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs-singleforceloop: test-gals-bonds-onebyone-betterverlet-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs

test-gals-bonds-onebyone-betterverlet-doublesqrt-msgmgmt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-bonds-onebyone-betterverlet-doublesqrt-msgmgmt-floatonly-singleforceloop: test-gals-bonds-onebyone-betterverlet-doublesqrt-msgmgmt-floatonly

test-gals-bonds-onebyone-betterverlet-doublesqrt-msgmgmt-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-gals-bonds-onebyone-betterverlet-doublesqrt-msgmgmt-reducedlocalcalcs: test-gals-bonds-onebyone-betterverlet-doublesqrt-msgmgmt

test-gals-bonds-onebyone-betterverlet-doublesqrt-msgmgmt-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-bonds-onebyone-betterverlet-doublesqrt-msgmgmt-reducedlocalcalcs-singleforceloop: test-gals-bonds-onebyone-betterverlet-doublesqrt-msgmgmt-reducedlocalcalcs

test-gals-bonds-onebyone-betterverlet-doublesqrt-msgmgmt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-bonds-onebyone-betterverlet-doublesqrt-msgmgmt-singleforceloop: test-gals-bonds-onebyone-betterverlet-doublesqrt-msgmgmt

test-gals-bonds-onebyone-betterverlet-doublesqrt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-gals-bonds-onebyone-betterverlet-doublesqrt-floatonly: test-gals-bonds-onebyone-betterverlet-doublesqrt

test-gals-bonds-onebyone-betterverlet-doublesqrt-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-gals-bonds-onebyone-betterverlet-doublesqrt-floatonly-reducedlocalcalcs: test-gals-bonds-onebyone-betterverlet-doublesqrt-floatonly

test-gals-bonds-onebyone-betterverlet-doublesqrt-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-bonds-onebyone-betterverlet-doublesqrt-floatonly-reducedlocalcalcs-singleforceloop: test-gals-bonds-onebyone-betterverlet-doublesqrt-floatonly-reducedlocalcalcs

test-gals-bonds-onebyone-betterverlet-doublesqrt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-bonds-onebyone-betterverlet-doublesqrt-floatonly-singleforceloop: test-gals-bonds-onebyone-betterverlet-doublesqrt-floatonly

test-gals-bonds-onebyone-betterverlet-doublesqrt-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-gals-bonds-onebyone-betterverlet-doublesqrt-reducedlocalcalcs: test-gals-bonds-onebyone-betterverlet-doublesqrt

test-gals-bonds-onebyone-betterverlet-doublesqrt-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-bonds-onebyone-betterverlet-doublesqrt-reducedlocalcalcs-singleforceloop: test-gals-bonds-onebyone-betterverlet-doublesqrt-reducedlocalcalcs

test-gals-bonds-onebyone-betterverlet-doublesqrt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-bonds-onebyone-betterverlet-doublesqrt-singleforceloop: test-gals-bonds-onebyone-betterverlet-doublesqrt

test-gals-bonds-onebyone-betterverlet-msgmgmt: DFLAGS+=-DMESSAGE_MANAGEMENT
test-gals-bonds-onebyone-betterverlet-msgmgmt: test-gals-bonds-onebyone-betterverlet

test-gals-bonds-onebyone-betterverlet-msgmgmt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-gals-bonds-onebyone-betterverlet-msgmgmt-floatonly: test-gals-bonds-onebyone-betterverlet-msgmgmt

test-gals-bonds-onebyone-betterverlet-msgmgmt-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-gals-bonds-onebyone-betterverlet-msgmgmt-floatonly-reducedlocalcalcs: test-gals-bonds-onebyone-betterverlet-msgmgmt-floatonly

test-gals-bonds-onebyone-betterverlet-msgmgmt-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-bonds-onebyone-betterverlet-msgmgmt-floatonly-reducedlocalcalcs-singleforceloop: test-gals-bonds-onebyone-betterverlet-msgmgmt-floatonly-reducedlocalcalcs

test-gals-bonds-onebyone-betterverlet-msgmgmt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-bonds-onebyone-betterverlet-msgmgmt-floatonly-singleforceloop: test-gals-bonds-onebyone-betterverlet-msgmgmt-floatonly

test-gals-bonds-onebyone-betterverlet-msgmgmt-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-gals-bonds-onebyone-betterverlet-msgmgmt-reducedlocalcalcs: test-gals-bonds-onebyone-betterverlet-msgmgmt

test-gals-bonds-onebyone-betterverlet-msgmgmt-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-bonds-onebyone-betterverlet-msgmgmt-reducedlocalcalcs-singleforceloop: test-gals-bonds-onebyone-betterverlet-msgmgmt-reducedlocalcalcs

test-gals-bonds-onebyone-betterverlet-msgmgmt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-bonds-onebyone-betterverlet-msgmgmt-singleforceloop: test-gals-bonds-onebyone-betterverlet-msgmgmt

test-gals-bonds-onebyone-betterverlet-floatonly: DFLAGS+=-DFLOAT_ONLY
test-gals-bonds-onebyone-betterverlet-floatonly: test-gals-bonds-onebyone-betterverlet

test-gals-bonds-onebyone-betterverlet-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-gals-bonds-onebyone-betterverlet-floatonly-reducedlocalcalcs: test-gals-bonds-onebyone-betterverlet-floatonly

test-gals-bonds-onebyone-betterverlet-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-bonds-onebyone-betterverlet-floatonly-reducedlocalcalcs-singleforceloop: test-gals-bonds-onebyone-betterverlet-floatonly-reducedlocalcalcs

test-gals-bonds-onebyone-betterverlet-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-bonds-onebyone-betterverlet-floatonly-singleforceloop: test-gals-bonds-onebyone-betterverlet-floatonly

test-gals-bonds-onebyone-betterverlet-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-gals-bonds-onebyone-betterverlet-reducedlocalcalcs: test-gals-bonds-onebyone-betterverlet

test-gals-bonds-onebyone-betterverlet-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-bonds-onebyone-betterverlet-reducedlocalcalcs-singleforceloop: test-gals-bonds-onebyone-betterverlet-reducedlocalcalcs

test-gals-bonds-onebyone-betterverlet-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-bonds-onebyone-betterverlet-singleforceloop: test-gals-bonds-onebyone-betterverlet

test-gals-bonds-onebyone-dtchange: DFLAGS+=-DSMALL_DT_EARLY
test-gals-bonds-onebyone-dtchange: test-gals-bonds-onebyone

test-gals-bonds-onebyone-dtchange-doublesqrt: DFLAGS+=-DOUBLE_SQRT
test-gals-bonds-onebyone-dtchange-doublesqrt: test-gals-bonds-onebyone-dtchange

test-gals-bonds-onebyone-dtchange-doublesqrt-msgmgmt: DFLAGS+=-DMESSAGE_MANAGEMENT
test-gals-bonds-onebyone-dtchange-doublesqrt-msgmgmt: test-gals-bonds-onebyone-dtchange-doublesqrt

test-gals-bonds-onebyone-dtchange-doublesqrt-msgmgmt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-gals-bonds-onebyone-dtchange-doublesqrt-msgmgmt-floatonly: test-gals-bonds-onebyone-dtchange-doublesqrt-msgmgmt

test-gals-bonds-onebyone-dtchange-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-gals-bonds-onebyone-dtchange-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs: test-gals-bonds-onebyone-dtchange-doublesqrt-msgmgmt-floatonly

test-gals-bonds-onebyone-dtchange-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-bonds-onebyone-dtchange-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs-singleforceloop: test-gals-bonds-onebyone-dtchange-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs

test-gals-bonds-onebyone-dtchange-doublesqrt-msgmgmt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-bonds-onebyone-dtchange-doublesqrt-msgmgmt-floatonly-singleforceloop: test-gals-bonds-onebyone-dtchange-doublesqrt-msgmgmt-floatonly

test-gals-bonds-onebyone-dtchange-doublesqrt-msgmgmt-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-gals-bonds-onebyone-dtchange-doublesqrt-msgmgmt-reducedlocalcalcs: test-gals-bonds-onebyone-dtchange-doublesqrt-msgmgmt

test-gals-bonds-onebyone-dtchange-doublesqrt-msgmgmt-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-bonds-onebyone-dtchange-doublesqrt-msgmgmt-reducedlocalcalcs-singleforceloop: test-gals-bonds-onebyone-dtchange-doublesqrt-msgmgmt-reducedlocalcalcs

test-gals-bonds-onebyone-dtchange-doublesqrt-msgmgmt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-bonds-onebyone-dtchange-doublesqrt-msgmgmt-singleforceloop: test-gals-bonds-onebyone-dtchange-doublesqrt-msgmgmt

test-gals-bonds-onebyone-dtchange-doublesqrt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-gals-bonds-onebyone-dtchange-doublesqrt-floatonly: test-gals-bonds-onebyone-dtchange-doublesqrt

test-gals-bonds-onebyone-dtchange-doublesqrt-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-gals-bonds-onebyone-dtchange-doublesqrt-floatonly-reducedlocalcalcs: test-gals-bonds-onebyone-dtchange-doublesqrt-floatonly

test-gals-bonds-onebyone-dtchange-doublesqrt-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-bonds-onebyone-dtchange-doublesqrt-floatonly-reducedlocalcalcs-singleforceloop: test-gals-bonds-onebyone-dtchange-doublesqrt-floatonly-reducedlocalcalcs

test-gals-bonds-onebyone-dtchange-doublesqrt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-bonds-onebyone-dtchange-doublesqrt-floatonly-singleforceloop: test-gals-bonds-onebyone-dtchange-doublesqrt-floatonly

test-gals-bonds-onebyone-dtchange-doublesqrt-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-gals-bonds-onebyone-dtchange-doublesqrt-reducedlocalcalcs: test-gals-bonds-onebyone-dtchange-doublesqrt

test-gals-bonds-onebyone-dtchange-doublesqrt-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-bonds-onebyone-dtchange-doublesqrt-reducedlocalcalcs-singleforceloop: test-gals-bonds-onebyone-dtchange-doublesqrt-reducedlocalcalcs

test-gals-bonds-onebyone-dtchange-doublesqrt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-bonds-onebyone-dtchange-doublesqrt-singleforceloop: test-gals-bonds-onebyone-dtchange-doublesqrt

test-gals-bonds-onebyone-dtchange-msgmgmt: DFLAGS+=-DMESSAGE_MANAGEMENT
test-gals-bonds-onebyone-dtchange-msgmgmt: test-gals-bonds-onebyone-dtchange

test-gals-bonds-onebyone-dtchange-msgmgmt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-gals-bonds-onebyone-dtchange-msgmgmt-floatonly: test-gals-bonds-onebyone-dtchange-msgmgmt

test-gals-bonds-onebyone-dtchange-msgmgmt-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-gals-bonds-onebyone-dtchange-msgmgmt-floatonly-reducedlocalcalcs: test-gals-bonds-onebyone-dtchange-msgmgmt-floatonly

test-gals-bonds-onebyone-dtchange-msgmgmt-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-bonds-onebyone-dtchange-msgmgmt-floatonly-reducedlocalcalcs-singleforceloop: test-gals-bonds-onebyone-dtchange-msgmgmt-floatonly-reducedlocalcalcs

test-gals-bonds-onebyone-dtchange-msgmgmt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-bonds-onebyone-dtchange-msgmgmt-floatonly-singleforceloop: test-gals-bonds-onebyone-dtchange-msgmgmt-floatonly

test-gals-bonds-onebyone-dtchange-msgmgmt-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-gals-bonds-onebyone-dtchange-msgmgmt-reducedlocalcalcs: test-gals-bonds-onebyone-dtchange-msgmgmt

test-gals-bonds-onebyone-dtchange-msgmgmt-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-bonds-onebyone-dtchange-msgmgmt-reducedlocalcalcs-singleforceloop: test-gals-bonds-onebyone-dtchange-msgmgmt-reducedlocalcalcs

test-gals-bonds-onebyone-dtchange-msgmgmt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-bonds-onebyone-dtchange-msgmgmt-singleforceloop: test-gals-bonds-onebyone-dtchange-msgmgmt

test-gals-bonds-onebyone-dtchange-floatonly: DFLAGS+=-DFLOAT_ONLY
test-gals-bonds-onebyone-dtchange-floatonly: test-gals-bonds-onebyone-dtchange

test-gals-bonds-onebyone-dtchange-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-gals-bonds-onebyone-dtchange-floatonly-reducedlocalcalcs: test-gals-bonds-onebyone-dtchange-floatonly

test-gals-bonds-onebyone-dtchange-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-bonds-onebyone-dtchange-floatonly-reducedlocalcalcs-singleforceloop: test-gals-bonds-onebyone-dtchange-floatonly-reducedlocalcalcs

test-gals-bonds-onebyone-dtchange-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-bonds-onebyone-dtchange-floatonly-singleforceloop: test-gals-bonds-onebyone-dtchange-floatonly

test-gals-bonds-onebyone-dtchange-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-gals-bonds-onebyone-dtchange-reducedlocalcalcs: test-gals-bonds-onebyone-dtchange

test-gals-bonds-onebyone-dtchange-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-bonds-onebyone-dtchange-reducedlocalcalcs-singleforceloop: test-gals-bonds-onebyone-dtchange-reducedlocalcalcs

test-gals-bonds-onebyone-dtchange-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-bonds-onebyone-dtchange-singleforceloop: test-gals-bonds-onebyone-dtchange

test-gals-bonds-onebyone-doublesqrt: DFLAGS+=-DOUBLE_SQRT
test-gals-bonds-onebyone-doublesqrt: test-gals-bonds-onebyone

test-gals-bonds-onebyone-doublesqrt-msgmgmt: DFLAGS+=-DMESSAGE_MANAGEMENT
test-gals-bonds-onebyone-doublesqrt-msgmgmt: test-gals-bonds-onebyone-doublesqrt

test-gals-bonds-onebyone-doublesqrt-msgmgmt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-gals-bonds-onebyone-doublesqrt-msgmgmt-floatonly: test-gals-bonds-onebyone-doublesqrt-msgmgmt

test-gals-bonds-onebyone-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-gals-bonds-onebyone-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs: test-gals-bonds-onebyone-doublesqrt-msgmgmt-floatonly

test-gals-bonds-onebyone-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-bonds-onebyone-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs-singleforceloop: test-gals-bonds-onebyone-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs

test-gals-bonds-onebyone-doublesqrt-msgmgmt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-bonds-onebyone-doublesqrt-msgmgmt-floatonly-singleforceloop: test-gals-bonds-onebyone-doublesqrt-msgmgmt-floatonly

test-gals-bonds-onebyone-doublesqrt-msgmgmt-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-gals-bonds-onebyone-doublesqrt-msgmgmt-reducedlocalcalcs: test-gals-bonds-onebyone-doublesqrt-msgmgmt

test-gals-bonds-onebyone-doublesqrt-msgmgmt-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-bonds-onebyone-doublesqrt-msgmgmt-reducedlocalcalcs-singleforceloop: test-gals-bonds-onebyone-doublesqrt-msgmgmt-reducedlocalcalcs

test-gals-bonds-onebyone-doublesqrt-msgmgmt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-bonds-onebyone-doublesqrt-msgmgmt-singleforceloop: test-gals-bonds-onebyone-doublesqrt-msgmgmt

test-gals-bonds-onebyone-doublesqrt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-gals-bonds-onebyone-doublesqrt-floatonly: test-gals-bonds-onebyone-doublesqrt

test-gals-bonds-onebyone-doublesqrt-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-gals-bonds-onebyone-doublesqrt-floatonly-reducedlocalcalcs: test-gals-bonds-onebyone-doublesqrt-floatonly

test-gals-bonds-onebyone-doublesqrt-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-bonds-onebyone-doublesqrt-floatonly-reducedlocalcalcs-singleforceloop: test-gals-bonds-onebyone-doublesqrt-floatonly-reducedlocalcalcs

test-gals-bonds-onebyone-doublesqrt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-bonds-onebyone-doublesqrt-floatonly-singleforceloop: test-gals-bonds-onebyone-doublesqrt-floatonly

test-gals-bonds-onebyone-doublesqrt-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-gals-bonds-onebyone-doublesqrt-reducedlocalcalcs: test-gals-bonds-onebyone-doublesqrt

test-gals-bonds-onebyone-doublesqrt-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-bonds-onebyone-doublesqrt-reducedlocalcalcs-singleforceloop: test-gals-bonds-onebyone-doublesqrt-reducedlocalcalcs

test-gals-bonds-onebyone-doublesqrt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-bonds-onebyone-doublesqrt-singleforceloop: test-gals-bonds-onebyone-doublesqrt

test-gals-bonds-onebyone-msgmgmt: DFLAGS+=-DMESSAGE_MANAGEMENT
test-gals-bonds-onebyone-msgmgmt: test-gals-bonds-onebyone

test-gals-bonds-onebyone-msgmgmt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-gals-bonds-onebyone-msgmgmt-floatonly: test-gals-bonds-onebyone-msgmgmt

test-gals-bonds-onebyone-msgmgmt-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-gals-bonds-onebyone-msgmgmt-floatonly-reducedlocalcalcs: test-gals-bonds-onebyone-msgmgmt-floatonly

test-gals-bonds-onebyone-msgmgmt-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-bonds-onebyone-msgmgmt-floatonly-reducedlocalcalcs-singleforceloop: test-gals-bonds-onebyone-msgmgmt-floatonly-reducedlocalcalcs

test-gals-bonds-onebyone-msgmgmt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-bonds-onebyone-msgmgmt-floatonly-singleforceloop: test-gals-bonds-onebyone-msgmgmt-floatonly

test-gals-bonds-onebyone-msgmgmt-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-gals-bonds-onebyone-msgmgmt-reducedlocalcalcs: test-gals-bonds-onebyone-msgmgmt

test-gals-bonds-onebyone-msgmgmt-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-bonds-onebyone-msgmgmt-reducedlocalcalcs-singleforceloop: test-gals-bonds-onebyone-msgmgmt-reducedlocalcalcs

test-gals-bonds-onebyone-msgmgmt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-bonds-onebyone-msgmgmt-singleforceloop: test-gals-bonds-onebyone-msgmgmt

test-gals-bonds-onebyone-floatonly: DFLAGS+=-DFLOAT_ONLY
test-gals-bonds-onebyone-floatonly: test-gals-bonds-onebyone

test-gals-bonds-onebyone-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-gals-bonds-onebyone-floatonly-reducedlocalcalcs: test-gals-bonds-onebyone-floatonly

test-gals-bonds-onebyone-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-bonds-onebyone-floatonly-reducedlocalcalcs-singleforceloop: test-gals-bonds-onebyone-floatonly-reducedlocalcalcs

test-gals-bonds-onebyone-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-bonds-onebyone-floatonly-singleforceloop: test-gals-bonds-onebyone-floatonly

test-gals-bonds-onebyone-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-gals-bonds-onebyone-reducedlocalcalcs: test-gals-bonds-onebyone

test-gals-bonds-onebyone-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-bonds-onebyone-reducedlocalcalcs-singleforceloop: test-gals-bonds-onebyone-reducedlocalcalcs

test-gals-bonds-onebyone-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-bonds-onebyone-singleforceloop: test-gals-bonds-onebyone


# ************** TEST Simulator: improvedgals**************

test-improvedgals: OBJS=$(-lmetis)
test-improvedgals: DFLAGS+=-DTESTING -DGALS -DIMPROVED_GALS
test-improvedgals: base-gals test

test-improvedgals-betterverlet: DFLAGS+=-DBETTER_VERLET
test-improvedgals-betterverlet: test-improvedgals

test-improvedgals-betterverlet-dtchange: DFLAGS+=-DSMALL_DT_EARLY
test-improvedgals-betterverlet-dtchange: test-improvedgals-betterverlet

test-improvedgals-betterverlet-dtchange-doublesqrt: DFLAGS+=-DOUBLE_SQRT
test-improvedgals-betterverlet-dtchange-doublesqrt: test-improvedgals-betterverlet-dtchange

test-improvedgals-betterverlet-dtchange-doublesqrt-msgmgmt: DFLAGS+=-DMESSAGE_MANAGEMENT
test-improvedgals-betterverlet-dtchange-doublesqrt-msgmgmt: test-improvedgals-betterverlet-dtchange-doublesqrt

test-improvedgals-betterverlet-dtchange-doublesqrt-msgmgmt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-improvedgals-betterverlet-dtchange-doublesqrt-msgmgmt-floatonly: test-improvedgals-betterverlet-dtchange-doublesqrt-msgmgmt

test-improvedgals-betterverlet-dtchange-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-improvedgals-betterverlet-dtchange-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs: test-improvedgals-betterverlet-dtchange-doublesqrt-msgmgmt-floatonly

test-improvedgals-betterverlet-dtchange-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-betterverlet-dtchange-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs-singleforceloop: test-improvedgals-betterverlet-dtchange-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs

test-improvedgals-betterverlet-dtchange-doublesqrt-msgmgmt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-betterverlet-dtchange-doublesqrt-msgmgmt-floatonly-singleforceloop: test-improvedgals-betterverlet-dtchange-doublesqrt-msgmgmt-floatonly

test-improvedgals-betterverlet-dtchange-doublesqrt-msgmgmt-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-improvedgals-betterverlet-dtchange-doublesqrt-msgmgmt-reducedlocalcalcs: test-improvedgals-betterverlet-dtchange-doublesqrt-msgmgmt

test-improvedgals-betterverlet-dtchange-doublesqrt-msgmgmt-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-betterverlet-dtchange-doublesqrt-msgmgmt-reducedlocalcalcs-singleforceloop: test-improvedgals-betterverlet-dtchange-doublesqrt-msgmgmt-reducedlocalcalcs

test-improvedgals-betterverlet-dtchange-doublesqrt-msgmgmt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-betterverlet-dtchange-doublesqrt-msgmgmt-singleforceloop: test-improvedgals-betterverlet-dtchange-doublesqrt-msgmgmt

test-improvedgals-betterverlet-dtchange-doublesqrt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-improvedgals-betterverlet-dtchange-doublesqrt-floatonly: test-improvedgals-betterverlet-dtchange-doublesqrt

test-improvedgals-betterverlet-dtchange-doublesqrt-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-improvedgals-betterverlet-dtchange-doublesqrt-floatonly-reducedlocalcalcs: test-improvedgals-betterverlet-dtchange-doublesqrt-floatonly

test-improvedgals-betterverlet-dtchange-doublesqrt-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-betterverlet-dtchange-doublesqrt-floatonly-reducedlocalcalcs-singleforceloop: test-improvedgals-betterverlet-dtchange-doublesqrt-floatonly-reducedlocalcalcs

test-improvedgals-betterverlet-dtchange-doublesqrt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-betterverlet-dtchange-doublesqrt-floatonly-singleforceloop: test-improvedgals-betterverlet-dtchange-doublesqrt-floatonly

test-improvedgals-betterverlet-dtchange-doublesqrt-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-improvedgals-betterverlet-dtchange-doublesqrt-reducedlocalcalcs: test-improvedgals-betterverlet-dtchange-doublesqrt

test-improvedgals-betterverlet-dtchange-doublesqrt-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-betterverlet-dtchange-doublesqrt-reducedlocalcalcs-singleforceloop: test-improvedgals-betterverlet-dtchange-doublesqrt-reducedlocalcalcs

test-improvedgals-betterverlet-dtchange-doublesqrt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-betterverlet-dtchange-doublesqrt-singleforceloop: test-improvedgals-betterverlet-dtchange-doublesqrt

test-improvedgals-betterverlet-dtchange-msgmgmt: DFLAGS+=-DMESSAGE_MANAGEMENT
test-improvedgals-betterverlet-dtchange-msgmgmt: test-improvedgals-betterverlet-dtchange

test-improvedgals-betterverlet-dtchange-msgmgmt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-improvedgals-betterverlet-dtchange-msgmgmt-floatonly: test-improvedgals-betterverlet-dtchange-msgmgmt

test-improvedgals-betterverlet-dtchange-msgmgmt-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-improvedgals-betterverlet-dtchange-msgmgmt-floatonly-reducedlocalcalcs: test-improvedgals-betterverlet-dtchange-msgmgmt-floatonly

test-improvedgals-betterverlet-dtchange-msgmgmt-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-betterverlet-dtchange-msgmgmt-floatonly-reducedlocalcalcs-singleforceloop: test-improvedgals-betterverlet-dtchange-msgmgmt-floatonly-reducedlocalcalcs

test-improvedgals-betterverlet-dtchange-msgmgmt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-betterverlet-dtchange-msgmgmt-floatonly-singleforceloop: test-improvedgals-betterverlet-dtchange-msgmgmt-floatonly

test-improvedgals-betterverlet-dtchange-msgmgmt-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-improvedgals-betterverlet-dtchange-msgmgmt-reducedlocalcalcs: test-improvedgals-betterverlet-dtchange-msgmgmt

test-improvedgals-betterverlet-dtchange-msgmgmt-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-betterverlet-dtchange-msgmgmt-reducedlocalcalcs-singleforceloop: test-improvedgals-betterverlet-dtchange-msgmgmt-reducedlocalcalcs

test-improvedgals-betterverlet-dtchange-msgmgmt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-betterverlet-dtchange-msgmgmt-singleforceloop: test-improvedgals-betterverlet-dtchange-msgmgmt

test-improvedgals-betterverlet-dtchange-floatonly: DFLAGS+=-DFLOAT_ONLY
test-improvedgals-betterverlet-dtchange-floatonly: test-improvedgals-betterverlet-dtchange

test-improvedgals-betterverlet-dtchange-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-improvedgals-betterverlet-dtchange-floatonly-reducedlocalcalcs: test-improvedgals-betterverlet-dtchange-floatonly

test-improvedgals-betterverlet-dtchange-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-betterverlet-dtchange-floatonly-reducedlocalcalcs-singleforceloop: test-improvedgals-betterverlet-dtchange-floatonly-reducedlocalcalcs

test-improvedgals-betterverlet-dtchange-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-betterverlet-dtchange-floatonly-singleforceloop: test-improvedgals-betterverlet-dtchange-floatonly

test-improvedgals-betterverlet-dtchange-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-improvedgals-betterverlet-dtchange-reducedlocalcalcs: test-improvedgals-betterverlet-dtchange

test-improvedgals-betterverlet-dtchange-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-betterverlet-dtchange-reducedlocalcalcs-singleforceloop: test-improvedgals-betterverlet-dtchange-reducedlocalcalcs

test-improvedgals-betterverlet-dtchange-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-betterverlet-dtchange-singleforceloop: test-improvedgals-betterverlet-dtchange

test-improvedgals-betterverlet-doublesqrt: DFLAGS+=-DOUBLE_SQRT
test-improvedgals-betterverlet-doublesqrt: test-improvedgals-betterverlet

test-improvedgals-betterverlet-doublesqrt-msgmgmt: DFLAGS+=-DMESSAGE_MANAGEMENT
test-improvedgals-betterverlet-doublesqrt-msgmgmt: test-improvedgals-betterverlet-doublesqrt

test-improvedgals-betterverlet-doublesqrt-msgmgmt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-improvedgals-betterverlet-doublesqrt-msgmgmt-floatonly: test-improvedgals-betterverlet-doublesqrt-msgmgmt

test-improvedgals-betterverlet-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-improvedgals-betterverlet-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs: test-improvedgals-betterverlet-doublesqrt-msgmgmt-floatonly

test-improvedgals-betterverlet-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-betterverlet-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs-singleforceloop: test-improvedgals-betterverlet-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs

test-improvedgals-betterverlet-doublesqrt-msgmgmt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-betterverlet-doublesqrt-msgmgmt-floatonly-singleforceloop: test-improvedgals-betterverlet-doublesqrt-msgmgmt-floatonly

test-improvedgals-betterverlet-doublesqrt-msgmgmt-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-improvedgals-betterverlet-doublesqrt-msgmgmt-reducedlocalcalcs: test-improvedgals-betterverlet-doublesqrt-msgmgmt

test-improvedgals-betterverlet-doublesqrt-msgmgmt-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-betterverlet-doublesqrt-msgmgmt-reducedlocalcalcs-singleforceloop: test-improvedgals-betterverlet-doublesqrt-msgmgmt-reducedlocalcalcs

test-improvedgals-betterverlet-doublesqrt-msgmgmt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-betterverlet-doublesqrt-msgmgmt-singleforceloop: test-improvedgals-betterverlet-doublesqrt-msgmgmt

test-improvedgals-betterverlet-doublesqrt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-improvedgals-betterverlet-doublesqrt-floatonly: test-improvedgals-betterverlet-doublesqrt

test-improvedgals-betterverlet-doublesqrt-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-improvedgals-betterverlet-doublesqrt-floatonly-reducedlocalcalcs: test-improvedgals-betterverlet-doublesqrt-floatonly

test-improvedgals-betterverlet-doublesqrt-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-betterverlet-doublesqrt-floatonly-reducedlocalcalcs-singleforceloop: test-improvedgals-betterverlet-doublesqrt-floatonly-reducedlocalcalcs

test-improvedgals-betterverlet-doublesqrt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-betterverlet-doublesqrt-floatonly-singleforceloop: test-improvedgals-betterverlet-doublesqrt-floatonly

test-improvedgals-betterverlet-doublesqrt-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-improvedgals-betterverlet-doublesqrt-reducedlocalcalcs: test-improvedgals-betterverlet-doublesqrt

test-improvedgals-betterverlet-doublesqrt-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-betterverlet-doublesqrt-reducedlocalcalcs-singleforceloop: test-improvedgals-betterverlet-doublesqrt-reducedlocalcalcs

test-improvedgals-betterverlet-doublesqrt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-betterverlet-doublesqrt-singleforceloop: test-improvedgals-betterverlet-doublesqrt

test-improvedgals-betterverlet-msgmgmt: DFLAGS+=-DMESSAGE_MANAGEMENT
test-improvedgals-betterverlet-msgmgmt: test-improvedgals-betterverlet

test-improvedgals-betterverlet-msgmgmt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-improvedgals-betterverlet-msgmgmt-floatonly: test-improvedgals-betterverlet-msgmgmt

test-improvedgals-betterverlet-msgmgmt-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-improvedgals-betterverlet-msgmgmt-floatonly-reducedlocalcalcs: test-improvedgals-betterverlet-msgmgmt-floatonly

test-improvedgals-betterverlet-msgmgmt-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-betterverlet-msgmgmt-floatonly-reducedlocalcalcs-singleforceloop: test-improvedgals-betterverlet-msgmgmt-floatonly-reducedlocalcalcs

test-improvedgals-betterverlet-msgmgmt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-betterverlet-msgmgmt-floatonly-singleforceloop: test-improvedgals-betterverlet-msgmgmt-floatonly

test-improvedgals-betterverlet-msgmgmt-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-improvedgals-betterverlet-msgmgmt-reducedlocalcalcs: test-improvedgals-betterverlet-msgmgmt

test-improvedgals-betterverlet-msgmgmt-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-betterverlet-msgmgmt-reducedlocalcalcs-singleforceloop: test-improvedgals-betterverlet-msgmgmt-reducedlocalcalcs

test-improvedgals-betterverlet-msgmgmt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-betterverlet-msgmgmt-singleforceloop: test-improvedgals-betterverlet-msgmgmt

test-improvedgals-betterverlet-floatonly: DFLAGS+=-DFLOAT_ONLY
test-improvedgals-betterverlet-floatonly: test-improvedgals-betterverlet

test-improvedgals-betterverlet-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-improvedgals-betterverlet-floatonly-reducedlocalcalcs: test-improvedgals-betterverlet-floatonly

test-improvedgals-betterverlet-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-betterverlet-floatonly-reducedlocalcalcs-singleforceloop: test-improvedgals-betterverlet-floatonly-reducedlocalcalcs

test-improvedgals-betterverlet-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-betterverlet-floatonly-singleforceloop: test-improvedgals-betterverlet-floatonly

test-improvedgals-betterverlet-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-improvedgals-betterverlet-reducedlocalcalcs: test-improvedgals-betterverlet

test-improvedgals-betterverlet-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-betterverlet-reducedlocalcalcs-singleforceloop: test-improvedgals-betterverlet-reducedlocalcalcs

test-improvedgals-betterverlet-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-betterverlet-singleforceloop: test-improvedgals-betterverlet

test-improvedgals-dtchange: DFLAGS+=-DSMALL_DT_EARLY
test-improvedgals-dtchange: test-improvedgals

test-improvedgals-dtchange-doublesqrt: DFLAGS+=-DOUBLE_SQRT
test-improvedgals-dtchange-doublesqrt: test-improvedgals-dtchange

test-improvedgals-dtchange-doublesqrt-msgmgmt: DFLAGS+=-DMESSAGE_MANAGEMENT
test-improvedgals-dtchange-doublesqrt-msgmgmt: test-improvedgals-dtchange-doublesqrt

test-improvedgals-dtchange-doublesqrt-msgmgmt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-improvedgals-dtchange-doublesqrt-msgmgmt-floatonly: test-improvedgals-dtchange-doublesqrt-msgmgmt

test-improvedgals-dtchange-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-improvedgals-dtchange-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs: test-improvedgals-dtchange-doublesqrt-msgmgmt-floatonly

test-improvedgals-dtchange-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-dtchange-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs-singleforceloop: test-improvedgals-dtchange-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs

test-improvedgals-dtchange-doublesqrt-msgmgmt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-dtchange-doublesqrt-msgmgmt-floatonly-singleforceloop: test-improvedgals-dtchange-doublesqrt-msgmgmt-floatonly

test-improvedgals-dtchange-doublesqrt-msgmgmt-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-improvedgals-dtchange-doublesqrt-msgmgmt-reducedlocalcalcs: test-improvedgals-dtchange-doublesqrt-msgmgmt

test-improvedgals-dtchange-doublesqrt-msgmgmt-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-dtchange-doublesqrt-msgmgmt-reducedlocalcalcs-singleforceloop: test-improvedgals-dtchange-doublesqrt-msgmgmt-reducedlocalcalcs

test-improvedgals-dtchange-doublesqrt-msgmgmt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-dtchange-doublesqrt-msgmgmt-singleforceloop: test-improvedgals-dtchange-doublesqrt-msgmgmt

test-improvedgals-dtchange-doublesqrt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-improvedgals-dtchange-doublesqrt-floatonly: test-improvedgals-dtchange-doublesqrt

test-improvedgals-dtchange-doublesqrt-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-improvedgals-dtchange-doublesqrt-floatonly-reducedlocalcalcs: test-improvedgals-dtchange-doublesqrt-floatonly

test-improvedgals-dtchange-doublesqrt-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-dtchange-doublesqrt-floatonly-reducedlocalcalcs-singleforceloop: test-improvedgals-dtchange-doublesqrt-floatonly-reducedlocalcalcs

test-improvedgals-dtchange-doublesqrt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-dtchange-doublesqrt-floatonly-singleforceloop: test-improvedgals-dtchange-doublesqrt-floatonly

test-improvedgals-dtchange-doublesqrt-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-improvedgals-dtchange-doublesqrt-reducedlocalcalcs: test-improvedgals-dtchange-doublesqrt

test-improvedgals-dtchange-doublesqrt-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-dtchange-doublesqrt-reducedlocalcalcs-singleforceloop: test-improvedgals-dtchange-doublesqrt-reducedlocalcalcs

test-improvedgals-dtchange-doublesqrt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-dtchange-doublesqrt-singleforceloop: test-improvedgals-dtchange-doublesqrt

test-improvedgals-dtchange-msgmgmt: DFLAGS+=-DMESSAGE_MANAGEMENT
test-improvedgals-dtchange-msgmgmt: test-improvedgals-dtchange

test-improvedgals-dtchange-msgmgmt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-improvedgals-dtchange-msgmgmt-floatonly: test-improvedgals-dtchange-msgmgmt

test-improvedgals-dtchange-msgmgmt-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-improvedgals-dtchange-msgmgmt-floatonly-reducedlocalcalcs: test-improvedgals-dtchange-msgmgmt-floatonly

test-improvedgals-dtchange-msgmgmt-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-dtchange-msgmgmt-floatonly-reducedlocalcalcs-singleforceloop: test-improvedgals-dtchange-msgmgmt-floatonly-reducedlocalcalcs

test-improvedgals-dtchange-msgmgmt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-dtchange-msgmgmt-floatonly-singleforceloop: test-improvedgals-dtchange-msgmgmt-floatonly

test-improvedgals-dtchange-msgmgmt-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-improvedgals-dtchange-msgmgmt-reducedlocalcalcs: test-improvedgals-dtchange-msgmgmt

test-improvedgals-dtchange-msgmgmt-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-dtchange-msgmgmt-reducedlocalcalcs-singleforceloop: test-improvedgals-dtchange-msgmgmt-reducedlocalcalcs

test-improvedgals-dtchange-msgmgmt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-dtchange-msgmgmt-singleforceloop: test-improvedgals-dtchange-msgmgmt

test-improvedgals-dtchange-floatonly: DFLAGS+=-DFLOAT_ONLY
test-improvedgals-dtchange-floatonly: test-improvedgals-dtchange

test-improvedgals-dtchange-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-improvedgals-dtchange-floatonly-reducedlocalcalcs: test-improvedgals-dtchange-floatonly

test-improvedgals-dtchange-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-dtchange-floatonly-reducedlocalcalcs-singleforceloop: test-improvedgals-dtchange-floatonly-reducedlocalcalcs

test-improvedgals-dtchange-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-dtchange-floatonly-singleforceloop: test-improvedgals-dtchange-floatonly

test-improvedgals-dtchange-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-improvedgals-dtchange-reducedlocalcalcs: test-improvedgals-dtchange

test-improvedgals-dtchange-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-dtchange-reducedlocalcalcs-singleforceloop: test-improvedgals-dtchange-reducedlocalcalcs

test-improvedgals-dtchange-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-dtchange-singleforceloop: test-improvedgals-dtchange

test-improvedgals-doublesqrt: DFLAGS+=-DOUBLE_SQRT
test-improvedgals-doublesqrt: test-improvedgals

test-improvedgals-doublesqrt-msgmgmt: DFLAGS+=-DMESSAGE_MANAGEMENT
test-improvedgals-doublesqrt-msgmgmt: test-improvedgals-doublesqrt

test-improvedgals-doublesqrt-msgmgmt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-improvedgals-doublesqrt-msgmgmt-floatonly: test-improvedgals-doublesqrt-msgmgmt

test-improvedgals-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-improvedgals-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs: test-improvedgals-doublesqrt-msgmgmt-floatonly

test-improvedgals-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs-singleforceloop: test-improvedgals-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs

test-improvedgals-doublesqrt-msgmgmt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-doublesqrt-msgmgmt-floatonly-singleforceloop: test-improvedgals-doublesqrt-msgmgmt-floatonly

test-improvedgals-doublesqrt-msgmgmt-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-improvedgals-doublesqrt-msgmgmt-reducedlocalcalcs: test-improvedgals-doublesqrt-msgmgmt

test-improvedgals-doublesqrt-msgmgmt-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-doublesqrt-msgmgmt-reducedlocalcalcs-singleforceloop: test-improvedgals-doublesqrt-msgmgmt-reducedlocalcalcs

test-improvedgals-doublesqrt-msgmgmt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-doublesqrt-msgmgmt-singleforceloop: test-improvedgals-doublesqrt-msgmgmt

test-improvedgals-doublesqrt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-improvedgals-doublesqrt-floatonly: test-improvedgals-doublesqrt

test-improvedgals-doublesqrt-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-improvedgals-doublesqrt-floatonly-reducedlocalcalcs: test-improvedgals-doublesqrt-floatonly

test-improvedgals-doublesqrt-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-doublesqrt-floatonly-reducedlocalcalcs-singleforceloop: test-improvedgals-doublesqrt-floatonly-reducedlocalcalcs

test-improvedgals-doublesqrt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-doublesqrt-floatonly-singleforceloop: test-improvedgals-doublesqrt-floatonly

test-improvedgals-doublesqrt-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-improvedgals-doublesqrt-reducedlocalcalcs: test-improvedgals-doublesqrt

test-improvedgals-doublesqrt-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-doublesqrt-reducedlocalcalcs-singleforceloop: test-improvedgals-doublesqrt-reducedlocalcalcs

test-improvedgals-doublesqrt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-doublesqrt-singleforceloop: test-improvedgals-doublesqrt

test-improvedgals-msgmgmt: DFLAGS+=-DMESSAGE_MANAGEMENT
test-improvedgals-msgmgmt: test-improvedgals

test-improvedgals-msgmgmt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-improvedgals-msgmgmt-floatonly: test-improvedgals-msgmgmt

test-improvedgals-msgmgmt-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-improvedgals-msgmgmt-floatonly-reducedlocalcalcs: test-improvedgals-msgmgmt-floatonly

test-improvedgals-msgmgmt-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-msgmgmt-floatonly-reducedlocalcalcs-singleforceloop: test-improvedgals-msgmgmt-floatonly-reducedlocalcalcs

test-improvedgals-msgmgmt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-msgmgmt-floatonly-singleforceloop: test-improvedgals-msgmgmt-floatonly

test-improvedgals-msgmgmt-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-improvedgals-msgmgmt-reducedlocalcalcs: test-improvedgals-msgmgmt

test-improvedgals-msgmgmt-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-msgmgmt-reducedlocalcalcs-singleforceloop: test-improvedgals-msgmgmt-reducedlocalcalcs

test-improvedgals-msgmgmt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-msgmgmt-singleforceloop: test-improvedgals-msgmgmt

test-improvedgals-floatonly: DFLAGS+=-DFLOAT_ONLY
test-improvedgals-floatonly: test-improvedgals

test-improvedgals-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-improvedgals-floatonly-reducedlocalcalcs: test-improvedgals-floatonly

test-improvedgals-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-floatonly-reducedlocalcalcs-singleforceloop: test-improvedgals-floatonly-reducedlocalcalcs

test-improvedgals-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-floatonly-singleforceloop: test-improvedgals-floatonly

test-improvedgals-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-improvedgals-reducedlocalcalcs: test-improvedgals

test-improvedgals-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-reducedlocalcalcs-singleforceloop: test-improvedgals-reducedlocalcalcs

test-improvedgals-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-singleforceloop: test-improvedgals

# **TEST Local calculation method: onebyone**
test-improvedgals-onebyone: DFLAGS+=-DONE_BY_ONE
test-improvedgals-onebyone: test-improvedgals

test-improvedgals-onebyone-betterverlet: DFLAGS+=-DBETTER_VERLET
test-improvedgals-onebyone-betterverlet: test-improvedgals-onebyone

test-improvedgals-onebyone-betterverlet-dtchange: DFLAGS+=-DSMALL_DT_EARLY
test-improvedgals-onebyone-betterverlet-dtchange: test-improvedgals-onebyone-betterverlet

test-improvedgals-onebyone-betterverlet-dtchange-doublesqrt: DFLAGS+=-DOUBLE_SQRT
test-improvedgals-onebyone-betterverlet-dtchange-doublesqrt: test-improvedgals-onebyone-betterverlet-dtchange

test-improvedgals-onebyone-betterverlet-dtchange-doublesqrt-msgmgmt: DFLAGS+=-DMESSAGE_MANAGEMENT
test-improvedgals-onebyone-betterverlet-dtchange-doublesqrt-msgmgmt: test-improvedgals-onebyone-betterverlet-dtchange-doublesqrt

test-improvedgals-onebyone-betterverlet-dtchange-doublesqrt-msgmgmt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-improvedgals-onebyone-betterverlet-dtchange-doublesqrt-msgmgmt-floatonly: test-improvedgals-onebyone-betterverlet-dtchange-doublesqrt-msgmgmt

test-improvedgals-onebyone-betterverlet-dtchange-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-improvedgals-onebyone-betterverlet-dtchange-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs: test-improvedgals-onebyone-betterverlet-dtchange-doublesqrt-msgmgmt-floatonly

test-improvedgals-onebyone-betterverlet-dtchange-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-onebyone-betterverlet-dtchange-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs-singleforceloop: test-improvedgals-onebyone-betterverlet-dtchange-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs

test-improvedgals-onebyone-betterverlet-dtchange-doublesqrt-msgmgmt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-onebyone-betterverlet-dtchange-doublesqrt-msgmgmt-floatonly-singleforceloop: test-improvedgals-onebyone-betterverlet-dtchange-doublesqrt-msgmgmt-floatonly

test-improvedgals-onebyone-betterverlet-dtchange-doublesqrt-msgmgmt-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-improvedgals-onebyone-betterverlet-dtchange-doublesqrt-msgmgmt-reducedlocalcalcs: test-improvedgals-onebyone-betterverlet-dtchange-doublesqrt-msgmgmt

test-improvedgals-onebyone-betterverlet-dtchange-doublesqrt-msgmgmt-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-onebyone-betterverlet-dtchange-doublesqrt-msgmgmt-reducedlocalcalcs-singleforceloop: test-improvedgals-onebyone-betterverlet-dtchange-doublesqrt-msgmgmt-reducedlocalcalcs

test-improvedgals-onebyone-betterverlet-dtchange-doublesqrt-msgmgmt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-onebyone-betterverlet-dtchange-doublesqrt-msgmgmt-singleforceloop: test-improvedgals-onebyone-betterverlet-dtchange-doublesqrt-msgmgmt

test-improvedgals-onebyone-betterverlet-dtchange-doublesqrt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-improvedgals-onebyone-betterverlet-dtchange-doublesqrt-floatonly: test-improvedgals-onebyone-betterverlet-dtchange-doublesqrt

test-improvedgals-onebyone-betterverlet-dtchange-doublesqrt-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-improvedgals-onebyone-betterverlet-dtchange-doublesqrt-floatonly-reducedlocalcalcs: test-improvedgals-onebyone-betterverlet-dtchange-doublesqrt-floatonly

test-improvedgals-onebyone-betterverlet-dtchange-doublesqrt-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-onebyone-betterverlet-dtchange-doublesqrt-floatonly-reducedlocalcalcs-singleforceloop: test-improvedgals-onebyone-betterverlet-dtchange-doublesqrt-floatonly-reducedlocalcalcs

test-improvedgals-onebyone-betterverlet-dtchange-doublesqrt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-onebyone-betterverlet-dtchange-doublesqrt-floatonly-singleforceloop: test-improvedgals-onebyone-betterverlet-dtchange-doublesqrt-floatonly

test-improvedgals-onebyone-betterverlet-dtchange-doublesqrt-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-improvedgals-onebyone-betterverlet-dtchange-doublesqrt-reducedlocalcalcs: test-improvedgals-onebyone-betterverlet-dtchange-doublesqrt

test-improvedgals-onebyone-betterverlet-dtchange-doublesqrt-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-onebyone-betterverlet-dtchange-doublesqrt-reducedlocalcalcs-singleforceloop: test-improvedgals-onebyone-betterverlet-dtchange-doublesqrt-reducedlocalcalcs

test-improvedgals-onebyone-betterverlet-dtchange-doublesqrt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-onebyone-betterverlet-dtchange-doublesqrt-singleforceloop: test-improvedgals-onebyone-betterverlet-dtchange-doublesqrt

test-improvedgals-onebyone-betterverlet-dtchange-msgmgmt: DFLAGS+=-DMESSAGE_MANAGEMENT
test-improvedgals-onebyone-betterverlet-dtchange-msgmgmt: test-improvedgals-onebyone-betterverlet-dtchange

test-improvedgals-onebyone-betterverlet-dtchange-msgmgmt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-improvedgals-onebyone-betterverlet-dtchange-msgmgmt-floatonly: test-improvedgals-onebyone-betterverlet-dtchange-msgmgmt

test-improvedgals-onebyone-betterverlet-dtchange-msgmgmt-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-improvedgals-onebyone-betterverlet-dtchange-msgmgmt-floatonly-reducedlocalcalcs: test-improvedgals-onebyone-betterverlet-dtchange-msgmgmt-floatonly

test-improvedgals-onebyone-betterverlet-dtchange-msgmgmt-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-onebyone-betterverlet-dtchange-msgmgmt-floatonly-reducedlocalcalcs-singleforceloop: test-improvedgals-onebyone-betterverlet-dtchange-msgmgmt-floatonly-reducedlocalcalcs

test-improvedgals-onebyone-betterverlet-dtchange-msgmgmt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-onebyone-betterverlet-dtchange-msgmgmt-floatonly-singleforceloop: test-improvedgals-onebyone-betterverlet-dtchange-msgmgmt-floatonly

test-improvedgals-onebyone-betterverlet-dtchange-msgmgmt-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-improvedgals-onebyone-betterverlet-dtchange-msgmgmt-reducedlocalcalcs: test-improvedgals-onebyone-betterverlet-dtchange-msgmgmt

test-improvedgals-onebyone-betterverlet-dtchange-msgmgmt-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-onebyone-betterverlet-dtchange-msgmgmt-reducedlocalcalcs-singleforceloop: test-improvedgals-onebyone-betterverlet-dtchange-msgmgmt-reducedlocalcalcs

test-improvedgals-onebyone-betterverlet-dtchange-msgmgmt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-onebyone-betterverlet-dtchange-msgmgmt-singleforceloop: test-improvedgals-onebyone-betterverlet-dtchange-msgmgmt

test-improvedgals-onebyone-betterverlet-dtchange-floatonly: DFLAGS+=-DFLOAT_ONLY
test-improvedgals-onebyone-betterverlet-dtchange-floatonly: test-improvedgals-onebyone-betterverlet-dtchange

test-improvedgals-onebyone-betterverlet-dtchange-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-improvedgals-onebyone-betterverlet-dtchange-floatonly-reducedlocalcalcs: test-improvedgals-onebyone-betterverlet-dtchange-floatonly

test-improvedgals-onebyone-betterverlet-dtchange-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-onebyone-betterverlet-dtchange-floatonly-reducedlocalcalcs-singleforceloop: test-improvedgals-onebyone-betterverlet-dtchange-floatonly-reducedlocalcalcs

test-improvedgals-onebyone-betterverlet-dtchange-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-onebyone-betterverlet-dtchange-floatonly-singleforceloop: test-improvedgals-onebyone-betterverlet-dtchange-floatonly

test-improvedgals-onebyone-betterverlet-dtchange-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-improvedgals-onebyone-betterverlet-dtchange-reducedlocalcalcs: test-improvedgals-onebyone-betterverlet-dtchange

test-improvedgals-onebyone-betterverlet-dtchange-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-onebyone-betterverlet-dtchange-reducedlocalcalcs-singleforceloop: test-improvedgals-onebyone-betterverlet-dtchange-reducedlocalcalcs

test-improvedgals-onebyone-betterverlet-dtchange-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-onebyone-betterverlet-dtchange-singleforceloop: test-improvedgals-onebyone-betterverlet-dtchange

test-improvedgals-onebyone-betterverlet-doublesqrt: DFLAGS+=-DOUBLE_SQRT
test-improvedgals-onebyone-betterverlet-doublesqrt: test-improvedgals-onebyone-betterverlet

test-improvedgals-onebyone-betterverlet-doublesqrt-msgmgmt: DFLAGS+=-DMESSAGE_MANAGEMENT
test-improvedgals-onebyone-betterverlet-doublesqrt-msgmgmt: test-improvedgals-onebyone-betterverlet-doublesqrt

test-improvedgals-onebyone-betterverlet-doublesqrt-msgmgmt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-improvedgals-onebyone-betterverlet-doublesqrt-msgmgmt-floatonly: test-improvedgals-onebyone-betterverlet-doublesqrt-msgmgmt

test-improvedgals-onebyone-betterverlet-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-improvedgals-onebyone-betterverlet-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs: test-improvedgals-onebyone-betterverlet-doublesqrt-msgmgmt-floatonly

test-improvedgals-onebyone-betterverlet-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-onebyone-betterverlet-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs-singleforceloop: test-improvedgals-onebyone-betterverlet-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs

test-improvedgals-onebyone-betterverlet-doublesqrt-msgmgmt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-onebyone-betterverlet-doublesqrt-msgmgmt-floatonly-singleforceloop: test-improvedgals-onebyone-betterverlet-doublesqrt-msgmgmt-floatonly

test-improvedgals-onebyone-betterverlet-doublesqrt-msgmgmt-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-improvedgals-onebyone-betterverlet-doublesqrt-msgmgmt-reducedlocalcalcs: test-improvedgals-onebyone-betterverlet-doublesqrt-msgmgmt

test-improvedgals-onebyone-betterverlet-doublesqrt-msgmgmt-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-onebyone-betterverlet-doublesqrt-msgmgmt-reducedlocalcalcs-singleforceloop: test-improvedgals-onebyone-betterverlet-doublesqrt-msgmgmt-reducedlocalcalcs

test-improvedgals-onebyone-betterverlet-doublesqrt-msgmgmt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-onebyone-betterverlet-doublesqrt-msgmgmt-singleforceloop: test-improvedgals-onebyone-betterverlet-doublesqrt-msgmgmt

test-improvedgals-onebyone-betterverlet-doublesqrt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-improvedgals-onebyone-betterverlet-doublesqrt-floatonly: test-improvedgals-onebyone-betterverlet-doublesqrt

test-improvedgals-onebyone-betterverlet-doublesqrt-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-improvedgals-onebyone-betterverlet-doublesqrt-floatonly-reducedlocalcalcs: test-improvedgals-onebyone-betterverlet-doublesqrt-floatonly

test-improvedgals-onebyone-betterverlet-doublesqrt-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-onebyone-betterverlet-doublesqrt-floatonly-reducedlocalcalcs-singleforceloop: test-improvedgals-onebyone-betterverlet-doublesqrt-floatonly-reducedlocalcalcs

test-improvedgals-onebyone-betterverlet-doublesqrt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-onebyone-betterverlet-doublesqrt-floatonly-singleforceloop: test-improvedgals-onebyone-betterverlet-doublesqrt-floatonly

test-improvedgals-onebyone-betterverlet-doublesqrt-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-improvedgals-onebyone-betterverlet-doublesqrt-reducedlocalcalcs: test-improvedgals-onebyone-betterverlet-doublesqrt

test-improvedgals-onebyone-betterverlet-doublesqrt-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-onebyone-betterverlet-doublesqrt-reducedlocalcalcs-singleforceloop: test-improvedgals-onebyone-betterverlet-doublesqrt-reducedlocalcalcs

test-improvedgals-onebyone-betterverlet-doublesqrt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-onebyone-betterverlet-doublesqrt-singleforceloop: test-improvedgals-onebyone-betterverlet-doublesqrt

test-improvedgals-onebyone-betterverlet-msgmgmt: DFLAGS+=-DMESSAGE_MANAGEMENT
test-improvedgals-onebyone-betterverlet-msgmgmt: test-improvedgals-onebyone-betterverlet

test-improvedgals-onebyone-betterverlet-msgmgmt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-improvedgals-onebyone-betterverlet-msgmgmt-floatonly: test-improvedgals-onebyone-betterverlet-msgmgmt

test-improvedgals-onebyone-betterverlet-msgmgmt-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-improvedgals-onebyone-betterverlet-msgmgmt-floatonly-reducedlocalcalcs: test-improvedgals-onebyone-betterverlet-msgmgmt-floatonly

test-improvedgals-onebyone-betterverlet-msgmgmt-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-onebyone-betterverlet-msgmgmt-floatonly-reducedlocalcalcs-singleforceloop: test-improvedgals-onebyone-betterverlet-msgmgmt-floatonly-reducedlocalcalcs

test-improvedgals-onebyone-betterverlet-msgmgmt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-onebyone-betterverlet-msgmgmt-floatonly-singleforceloop: test-improvedgals-onebyone-betterverlet-msgmgmt-floatonly

test-improvedgals-onebyone-betterverlet-msgmgmt-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-improvedgals-onebyone-betterverlet-msgmgmt-reducedlocalcalcs: test-improvedgals-onebyone-betterverlet-msgmgmt

test-improvedgals-onebyone-betterverlet-msgmgmt-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-onebyone-betterverlet-msgmgmt-reducedlocalcalcs-singleforceloop: test-improvedgals-onebyone-betterverlet-msgmgmt-reducedlocalcalcs

test-improvedgals-onebyone-betterverlet-msgmgmt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-onebyone-betterverlet-msgmgmt-singleforceloop: test-improvedgals-onebyone-betterverlet-msgmgmt

test-improvedgals-onebyone-betterverlet-floatonly: DFLAGS+=-DFLOAT_ONLY
test-improvedgals-onebyone-betterverlet-floatonly: test-improvedgals-onebyone-betterverlet

test-improvedgals-onebyone-betterverlet-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-improvedgals-onebyone-betterverlet-floatonly-reducedlocalcalcs: test-improvedgals-onebyone-betterverlet-floatonly

test-improvedgals-onebyone-betterverlet-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-onebyone-betterverlet-floatonly-reducedlocalcalcs-singleforceloop: test-improvedgals-onebyone-betterverlet-floatonly-reducedlocalcalcs

test-improvedgals-onebyone-betterverlet-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-onebyone-betterverlet-floatonly-singleforceloop: test-improvedgals-onebyone-betterverlet-floatonly

test-improvedgals-onebyone-betterverlet-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-improvedgals-onebyone-betterverlet-reducedlocalcalcs: test-improvedgals-onebyone-betterverlet

test-improvedgals-onebyone-betterverlet-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-onebyone-betterverlet-reducedlocalcalcs-singleforceloop: test-improvedgals-onebyone-betterverlet-reducedlocalcalcs

test-improvedgals-onebyone-betterverlet-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-onebyone-betterverlet-singleforceloop: test-improvedgals-onebyone-betterverlet

test-improvedgals-onebyone-dtchange: DFLAGS+=-DSMALL_DT_EARLY
test-improvedgals-onebyone-dtchange: test-improvedgals-onebyone

test-improvedgals-onebyone-dtchange-doublesqrt: DFLAGS+=-DOUBLE_SQRT
test-improvedgals-onebyone-dtchange-doublesqrt: test-improvedgals-onebyone-dtchange

test-improvedgals-onebyone-dtchange-doublesqrt-msgmgmt: DFLAGS+=-DMESSAGE_MANAGEMENT
test-improvedgals-onebyone-dtchange-doublesqrt-msgmgmt: test-improvedgals-onebyone-dtchange-doublesqrt

test-improvedgals-onebyone-dtchange-doublesqrt-msgmgmt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-improvedgals-onebyone-dtchange-doublesqrt-msgmgmt-floatonly: test-improvedgals-onebyone-dtchange-doublesqrt-msgmgmt

test-improvedgals-onebyone-dtchange-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-improvedgals-onebyone-dtchange-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs: test-improvedgals-onebyone-dtchange-doublesqrt-msgmgmt-floatonly

test-improvedgals-onebyone-dtchange-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-onebyone-dtchange-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs-singleforceloop: test-improvedgals-onebyone-dtchange-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs

test-improvedgals-onebyone-dtchange-doublesqrt-msgmgmt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-onebyone-dtchange-doublesqrt-msgmgmt-floatonly-singleforceloop: test-improvedgals-onebyone-dtchange-doublesqrt-msgmgmt-floatonly

test-improvedgals-onebyone-dtchange-doublesqrt-msgmgmt-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-improvedgals-onebyone-dtchange-doublesqrt-msgmgmt-reducedlocalcalcs: test-improvedgals-onebyone-dtchange-doublesqrt-msgmgmt

test-improvedgals-onebyone-dtchange-doublesqrt-msgmgmt-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-onebyone-dtchange-doublesqrt-msgmgmt-reducedlocalcalcs-singleforceloop: test-improvedgals-onebyone-dtchange-doublesqrt-msgmgmt-reducedlocalcalcs

test-improvedgals-onebyone-dtchange-doublesqrt-msgmgmt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-onebyone-dtchange-doublesqrt-msgmgmt-singleforceloop: test-improvedgals-onebyone-dtchange-doublesqrt-msgmgmt

test-improvedgals-onebyone-dtchange-doublesqrt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-improvedgals-onebyone-dtchange-doublesqrt-floatonly: test-improvedgals-onebyone-dtchange-doublesqrt

test-improvedgals-onebyone-dtchange-doublesqrt-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-improvedgals-onebyone-dtchange-doublesqrt-floatonly-reducedlocalcalcs: test-improvedgals-onebyone-dtchange-doublesqrt-floatonly

test-improvedgals-onebyone-dtchange-doublesqrt-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-onebyone-dtchange-doublesqrt-floatonly-reducedlocalcalcs-singleforceloop: test-improvedgals-onebyone-dtchange-doublesqrt-floatonly-reducedlocalcalcs

test-improvedgals-onebyone-dtchange-doublesqrt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-onebyone-dtchange-doublesqrt-floatonly-singleforceloop: test-improvedgals-onebyone-dtchange-doublesqrt-floatonly

test-improvedgals-onebyone-dtchange-doublesqrt-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-improvedgals-onebyone-dtchange-doublesqrt-reducedlocalcalcs: test-improvedgals-onebyone-dtchange-doublesqrt

test-improvedgals-onebyone-dtchange-doublesqrt-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-onebyone-dtchange-doublesqrt-reducedlocalcalcs-singleforceloop: test-improvedgals-onebyone-dtchange-doublesqrt-reducedlocalcalcs

test-improvedgals-onebyone-dtchange-doublesqrt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-onebyone-dtchange-doublesqrt-singleforceloop: test-improvedgals-onebyone-dtchange-doublesqrt

test-improvedgals-onebyone-dtchange-msgmgmt: DFLAGS+=-DMESSAGE_MANAGEMENT
test-improvedgals-onebyone-dtchange-msgmgmt: test-improvedgals-onebyone-dtchange

test-improvedgals-onebyone-dtchange-msgmgmt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-improvedgals-onebyone-dtchange-msgmgmt-floatonly: test-improvedgals-onebyone-dtchange-msgmgmt

test-improvedgals-onebyone-dtchange-msgmgmt-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-improvedgals-onebyone-dtchange-msgmgmt-floatonly-reducedlocalcalcs: test-improvedgals-onebyone-dtchange-msgmgmt-floatonly

test-improvedgals-onebyone-dtchange-msgmgmt-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-onebyone-dtchange-msgmgmt-floatonly-reducedlocalcalcs-singleforceloop: test-improvedgals-onebyone-dtchange-msgmgmt-floatonly-reducedlocalcalcs

test-improvedgals-onebyone-dtchange-msgmgmt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-onebyone-dtchange-msgmgmt-floatonly-singleforceloop: test-improvedgals-onebyone-dtchange-msgmgmt-floatonly

test-improvedgals-onebyone-dtchange-msgmgmt-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-improvedgals-onebyone-dtchange-msgmgmt-reducedlocalcalcs: test-improvedgals-onebyone-dtchange-msgmgmt

test-improvedgals-onebyone-dtchange-msgmgmt-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-onebyone-dtchange-msgmgmt-reducedlocalcalcs-singleforceloop: test-improvedgals-onebyone-dtchange-msgmgmt-reducedlocalcalcs

test-improvedgals-onebyone-dtchange-msgmgmt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-onebyone-dtchange-msgmgmt-singleforceloop: test-improvedgals-onebyone-dtchange-msgmgmt

test-improvedgals-onebyone-dtchange-floatonly: DFLAGS+=-DFLOAT_ONLY
test-improvedgals-onebyone-dtchange-floatonly: test-improvedgals-onebyone-dtchange

test-improvedgals-onebyone-dtchange-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-improvedgals-onebyone-dtchange-floatonly-reducedlocalcalcs: test-improvedgals-onebyone-dtchange-floatonly

test-improvedgals-onebyone-dtchange-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-onebyone-dtchange-floatonly-reducedlocalcalcs-singleforceloop: test-improvedgals-onebyone-dtchange-floatonly-reducedlocalcalcs

test-improvedgals-onebyone-dtchange-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-onebyone-dtchange-floatonly-singleforceloop: test-improvedgals-onebyone-dtchange-floatonly

test-improvedgals-onebyone-dtchange-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-improvedgals-onebyone-dtchange-reducedlocalcalcs: test-improvedgals-onebyone-dtchange

test-improvedgals-onebyone-dtchange-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-onebyone-dtchange-reducedlocalcalcs-singleforceloop: test-improvedgals-onebyone-dtchange-reducedlocalcalcs

test-improvedgals-onebyone-dtchange-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-onebyone-dtchange-singleforceloop: test-improvedgals-onebyone-dtchange

test-improvedgals-onebyone-doublesqrt: DFLAGS+=-DOUBLE_SQRT
test-improvedgals-onebyone-doublesqrt: test-improvedgals-onebyone

test-improvedgals-onebyone-doublesqrt-msgmgmt: DFLAGS+=-DMESSAGE_MANAGEMENT
test-improvedgals-onebyone-doublesqrt-msgmgmt: test-improvedgals-onebyone-doublesqrt

test-improvedgals-onebyone-doublesqrt-msgmgmt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-improvedgals-onebyone-doublesqrt-msgmgmt-floatonly: test-improvedgals-onebyone-doublesqrt-msgmgmt

test-improvedgals-onebyone-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-improvedgals-onebyone-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs: test-improvedgals-onebyone-doublesqrt-msgmgmt-floatonly

test-improvedgals-onebyone-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-onebyone-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs-singleforceloop: test-improvedgals-onebyone-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs

test-improvedgals-onebyone-doublesqrt-msgmgmt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-onebyone-doublesqrt-msgmgmt-floatonly-singleforceloop: test-improvedgals-onebyone-doublesqrt-msgmgmt-floatonly

test-improvedgals-onebyone-doublesqrt-msgmgmt-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-improvedgals-onebyone-doublesqrt-msgmgmt-reducedlocalcalcs: test-improvedgals-onebyone-doublesqrt-msgmgmt

test-improvedgals-onebyone-doublesqrt-msgmgmt-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-onebyone-doublesqrt-msgmgmt-reducedlocalcalcs-singleforceloop: test-improvedgals-onebyone-doublesqrt-msgmgmt-reducedlocalcalcs

test-improvedgals-onebyone-doublesqrt-msgmgmt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-onebyone-doublesqrt-msgmgmt-singleforceloop: test-improvedgals-onebyone-doublesqrt-msgmgmt

test-improvedgals-onebyone-doublesqrt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-improvedgals-onebyone-doublesqrt-floatonly: test-improvedgals-onebyone-doublesqrt

test-improvedgals-onebyone-doublesqrt-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-improvedgals-onebyone-doublesqrt-floatonly-reducedlocalcalcs: test-improvedgals-onebyone-doublesqrt-floatonly

test-improvedgals-onebyone-doublesqrt-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-onebyone-doublesqrt-floatonly-reducedlocalcalcs-singleforceloop: test-improvedgals-onebyone-doublesqrt-floatonly-reducedlocalcalcs

test-improvedgals-onebyone-doublesqrt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-onebyone-doublesqrt-floatonly-singleforceloop: test-improvedgals-onebyone-doublesqrt-floatonly

test-improvedgals-onebyone-doublesqrt-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-improvedgals-onebyone-doublesqrt-reducedlocalcalcs: test-improvedgals-onebyone-doublesqrt

test-improvedgals-onebyone-doublesqrt-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-onebyone-doublesqrt-reducedlocalcalcs-singleforceloop: test-improvedgals-onebyone-doublesqrt-reducedlocalcalcs

test-improvedgals-onebyone-doublesqrt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-onebyone-doublesqrt-singleforceloop: test-improvedgals-onebyone-doublesqrt

test-improvedgals-onebyone-msgmgmt: DFLAGS+=-DMESSAGE_MANAGEMENT
test-improvedgals-onebyone-msgmgmt: test-improvedgals-onebyone

test-improvedgals-onebyone-msgmgmt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-improvedgals-onebyone-msgmgmt-floatonly: test-improvedgals-onebyone-msgmgmt

test-improvedgals-onebyone-msgmgmt-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-improvedgals-onebyone-msgmgmt-floatonly-reducedlocalcalcs: test-improvedgals-onebyone-msgmgmt-floatonly

test-improvedgals-onebyone-msgmgmt-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-onebyone-msgmgmt-floatonly-reducedlocalcalcs-singleforceloop: test-improvedgals-onebyone-msgmgmt-floatonly-reducedlocalcalcs

test-improvedgals-onebyone-msgmgmt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-onebyone-msgmgmt-floatonly-singleforceloop: test-improvedgals-onebyone-msgmgmt-floatonly

test-improvedgals-onebyone-msgmgmt-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-improvedgals-onebyone-msgmgmt-reducedlocalcalcs: test-improvedgals-onebyone-msgmgmt

test-improvedgals-onebyone-msgmgmt-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-onebyone-msgmgmt-reducedlocalcalcs-singleforceloop: test-improvedgals-onebyone-msgmgmt-reducedlocalcalcs

test-improvedgals-onebyone-msgmgmt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-onebyone-msgmgmt-singleforceloop: test-improvedgals-onebyone-msgmgmt

test-improvedgals-onebyone-floatonly: DFLAGS+=-DFLOAT_ONLY
test-improvedgals-onebyone-floatonly: test-improvedgals-onebyone

test-improvedgals-onebyone-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-improvedgals-onebyone-floatonly-reducedlocalcalcs: test-improvedgals-onebyone-floatonly

test-improvedgals-onebyone-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-onebyone-floatonly-reducedlocalcalcs-singleforceloop: test-improvedgals-onebyone-floatonly-reducedlocalcalcs

test-improvedgals-onebyone-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-onebyone-floatonly-singleforceloop: test-improvedgals-onebyone-floatonly

test-improvedgals-onebyone-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-improvedgals-onebyone-reducedlocalcalcs: test-improvedgals-onebyone

test-improvedgals-onebyone-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-onebyone-reducedlocalcalcs-singleforceloop: test-improvedgals-onebyone-reducedlocalcalcs

test-improvedgals-onebyone-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-onebyone-singleforceloop: test-improvedgals-onebyone

test-improvedgals-large: DFLAGS+=-DLARGE_TEST
test-improvedgals-large: test-improvedgals

test-improvedgals-large-betterverlet: DFLAGS+=-DBETTER_VERLET
test-improvedgals-large-betterverlet: test-improvedgals-large

test-improvedgals-large-betterverlet-dtchange: DFLAGS+=-DSMALL_DT_EARLY
test-improvedgals-large-betterverlet-dtchange: test-improvedgals-large-betterverlet

test-improvedgals-large-betterverlet-dtchange-doublesqrt: DFLAGS+=-DOUBLE_SQRT
test-improvedgals-large-betterverlet-dtchange-doublesqrt: test-improvedgals-large-betterverlet-dtchange

test-improvedgals-large-betterverlet-dtchange-doublesqrt-msgmgmt: DFLAGS+=-DMESSAGE_MANAGEMENT
test-improvedgals-large-betterverlet-dtchange-doublesqrt-msgmgmt: test-improvedgals-large-betterverlet-dtchange-doublesqrt

test-improvedgals-large-betterverlet-dtchange-doublesqrt-msgmgmt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-improvedgals-large-betterverlet-dtchange-doublesqrt-msgmgmt-floatonly: test-improvedgals-large-betterverlet-dtchange-doublesqrt-msgmgmt

test-improvedgals-large-betterverlet-dtchange-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-improvedgals-large-betterverlet-dtchange-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs: test-improvedgals-large-betterverlet-dtchange-doublesqrt-msgmgmt-floatonly

test-improvedgals-large-betterverlet-dtchange-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-large-betterverlet-dtchange-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs-singleforceloop: test-improvedgals-large-betterverlet-dtchange-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs

test-improvedgals-large-betterverlet-dtchange-doublesqrt-msgmgmt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-large-betterverlet-dtchange-doublesqrt-msgmgmt-floatonly-singleforceloop: test-improvedgals-large-betterverlet-dtchange-doublesqrt-msgmgmt-floatonly

test-improvedgals-large-betterverlet-dtchange-doublesqrt-msgmgmt-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-improvedgals-large-betterverlet-dtchange-doublesqrt-msgmgmt-reducedlocalcalcs: test-improvedgals-large-betterverlet-dtchange-doublesqrt-msgmgmt

test-improvedgals-large-betterverlet-dtchange-doublesqrt-msgmgmt-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-large-betterverlet-dtchange-doublesqrt-msgmgmt-reducedlocalcalcs-singleforceloop: test-improvedgals-large-betterverlet-dtchange-doublesqrt-msgmgmt-reducedlocalcalcs

test-improvedgals-large-betterverlet-dtchange-doublesqrt-msgmgmt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-large-betterverlet-dtchange-doublesqrt-msgmgmt-singleforceloop: test-improvedgals-large-betterverlet-dtchange-doublesqrt-msgmgmt

test-improvedgals-large-betterverlet-dtchange-doublesqrt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-improvedgals-large-betterverlet-dtchange-doublesqrt-floatonly: test-improvedgals-large-betterverlet-dtchange-doublesqrt

test-improvedgals-large-betterverlet-dtchange-doublesqrt-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-improvedgals-large-betterverlet-dtchange-doublesqrt-floatonly-reducedlocalcalcs: test-improvedgals-large-betterverlet-dtchange-doublesqrt-floatonly

test-improvedgals-large-betterverlet-dtchange-doublesqrt-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-large-betterverlet-dtchange-doublesqrt-floatonly-reducedlocalcalcs-singleforceloop: test-improvedgals-large-betterverlet-dtchange-doublesqrt-floatonly-reducedlocalcalcs

test-improvedgals-large-betterverlet-dtchange-doublesqrt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-large-betterverlet-dtchange-doublesqrt-floatonly-singleforceloop: test-improvedgals-large-betterverlet-dtchange-doublesqrt-floatonly

test-improvedgals-large-betterverlet-dtchange-doublesqrt-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-improvedgals-large-betterverlet-dtchange-doublesqrt-reducedlocalcalcs: test-improvedgals-large-betterverlet-dtchange-doublesqrt

test-improvedgals-large-betterverlet-dtchange-doublesqrt-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-large-betterverlet-dtchange-doublesqrt-reducedlocalcalcs-singleforceloop: test-improvedgals-large-betterverlet-dtchange-doublesqrt-reducedlocalcalcs

test-improvedgals-large-betterverlet-dtchange-doublesqrt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-large-betterverlet-dtchange-doublesqrt-singleforceloop: test-improvedgals-large-betterverlet-dtchange-doublesqrt

test-improvedgals-large-betterverlet-dtchange-msgmgmt: DFLAGS+=-DMESSAGE_MANAGEMENT
test-improvedgals-large-betterverlet-dtchange-msgmgmt: test-improvedgals-large-betterverlet-dtchange

test-improvedgals-large-betterverlet-dtchange-msgmgmt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-improvedgals-large-betterverlet-dtchange-msgmgmt-floatonly: test-improvedgals-large-betterverlet-dtchange-msgmgmt

test-improvedgals-large-betterverlet-dtchange-msgmgmt-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-improvedgals-large-betterverlet-dtchange-msgmgmt-floatonly-reducedlocalcalcs: test-improvedgals-large-betterverlet-dtchange-msgmgmt-floatonly

test-improvedgals-large-betterverlet-dtchange-msgmgmt-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-large-betterverlet-dtchange-msgmgmt-floatonly-reducedlocalcalcs-singleforceloop: test-improvedgals-large-betterverlet-dtchange-msgmgmt-floatonly-reducedlocalcalcs

test-improvedgals-large-betterverlet-dtchange-msgmgmt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-large-betterverlet-dtchange-msgmgmt-floatonly-singleforceloop: test-improvedgals-large-betterverlet-dtchange-msgmgmt-floatonly

test-improvedgals-large-betterverlet-dtchange-msgmgmt-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-improvedgals-large-betterverlet-dtchange-msgmgmt-reducedlocalcalcs: test-improvedgals-large-betterverlet-dtchange-msgmgmt

test-improvedgals-large-betterverlet-dtchange-msgmgmt-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-large-betterverlet-dtchange-msgmgmt-reducedlocalcalcs-singleforceloop: test-improvedgals-large-betterverlet-dtchange-msgmgmt-reducedlocalcalcs

test-improvedgals-large-betterverlet-dtchange-msgmgmt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-large-betterverlet-dtchange-msgmgmt-singleforceloop: test-improvedgals-large-betterverlet-dtchange-msgmgmt

test-improvedgals-large-betterverlet-dtchange-floatonly: DFLAGS+=-DFLOAT_ONLY
test-improvedgals-large-betterverlet-dtchange-floatonly: test-improvedgals-large-betterverlet-dtchange

test-improvedgals-large-betterverlet-dtchange-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-improvedgals-large-betterverlet-dtchange-floatonly-reducedlocalcalcs: test-improvedgals-large-betterverlet-dtchange-floatonly

test-improvedgals-large-betterverlet-dtchange-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-large-betterverlet-dtchange-floatonly-reducedlocalcalcs-singleforceloop: test-improvedgals-large-betterverlet-dtchange-floatonly-reducedlocalcalcs

test-improvedgals-large-betterverlet-dtchange-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-large-betterverlet-dtchange-floatonly-singleforceloop: test-improvedgals-large-betterverlet-dtchange-floatonly

test-improvedgals-large-betterverlet-dtchange-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-improvedgals-large-betterverlet-dtchange-reducedlocalcalcs: test-improvedgals-large-betterverlet-dtchange

test-improvedgals-large-betterverlet-dtchange-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-large-betterverlet-dtchange-reducedlocalcalcs-singleforceloop: test-improvedgals-large-betterverlet-dtchange-reducedlocalcalcs

test-improvedgals-large-betterverlet-dtchange-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-large-betterverlet-dtchange-singleforceloop: test-improvedgals-large-betterverlet-dtchange

test-improvedgals-large-betterverlet-doublesqrt: DFLAGS+=-DOUBLE_SQRT
test-improvedgals-large-betterverlet-doublesqrt: test-improvedgals-large-betterverlet

test-improvedgals-large-betterverlet-doublesqrt-msgmgmt: DFLAGS+=-DMESSAGE_MANAGEMENT
test-improvedgals-large-betterverlet-doublesqrt-msgmgmt: test-improvedgals-large-betterverlet-doublesqrt

test-improvedgals-large-betterverlet-doublesqrt-msgmgmt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-improvedgals-large-betterverlet-doublesqrt-msgmgmt-floatonly: test-improvedgals-large-betterverlet-doublesqrt-msgmgmt

test-improvedgals-large-betterverlet-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-improvedgals-large-betterverlet-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs: test-improvedgals-large-betterverlet-doublesqrt-msgmgmt-floatonly

test-improvedgals-large-betterverlet-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-large-betterverlet-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs-singleforceloop: test-improvedgals-large-betterverlet-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs

test-improvedgals-large-betterverlet-doublesqrt-msgmgmt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-large-betterverlet-doublesqrt-msgmgmt-floatonly-singleforceloop: test-improvedgals-large-betterverlet-doublesqrt-msgmgmt-floatonly

test-improvedgals-large-betterverlet-doublesqrt-msgmgmt-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-improvedgals-large-betterverlet-doublesqrt-msgmgmt-reducedlocalcalcs: test-improvedgals-large-betterverlet-doublesqrt-msgmgmt

test-improvedgals-large-betterverlet-doublesqrt-msgmgmt-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-large-betterverlet-doublesqrt-msgmgmt-reducedlocalcalcs-singleforceloop: test-improvedgals-large-betterverlet-doublesqrt-msgmgmt-reducedlocalcalcs

test-improvedgals-large-betterverlet-doublesqrt-msgmgmt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-large-betterverlet-doublesqrt-msgmgmt-singleforceloop: test-improvedgals-large-betterverlet-doublesqrt-msgmgmt

test-improvedgals-large-betterverlet-doublesqrt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-improvedgals-large-betterverlet-doublesqrt-floatonly: test-improvedgals-large-betterverlet-doublesqrt

test-improvedgals-large-betterverlet-doublesqrt-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-improvedgals-large-betterverlet-doublesqrt-floatonly-reducedlocalcalcs: test-improvedgals-large-betterverlet-doublesqrt-floatonly

test-improvedgals-large-betterverlet-doublesqrt-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-large-betterverlet-doublesqrt-floatonly-reducedlocalcalcs-singleforceloop: test-improvedgals-large-betterverlet-doublesqrt-floatonly-reducedlocalcalcs

test-improvedgals-large-betterverlet-doublesqrt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-large-betterverlet-doublesqrt-floatonly-singleforceloop: test-improvedgals-large-betterverlet-doublesqrt-floatonly

test-improvedgals-large-betterverlet-doublesqrt-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-improvedgals-large-betterverlet-doublesqrt-reducedlocalcalcs: test-improvedgals-large-betterverlet-doublesqrt

test-improvedgals-large-betterverlet-doublesqrt-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-large-betterverlet-doublesqrt-reducedlocalcalcs-singleforceloop: test-improvedgals-large-betterverlet-doublesqrt-reducedlocalcalcs

test-improvedgals-large-betterverlet-doublesqrt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-large-betterverlet-doublesqrt-singleforceloop: test-improvedgals-large-betterverlet-doublesqrt

test-improvedgals-large-betterverlet-msgmgmt: DFLAGS+=-DMESSAGE_MANAGEMENT
test-improvedgals-large-betterverlet-msgmgmt: test-improvedgals-large-betterverlet

test-improvedgals-large-betterverlet-msgmgmt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-improvedgals-large-betterverlet-msgmgmt-floatonly: test-improvedgals-large-betterverlet-msgmgmt

test-improvedgals-large-betterverlet-msgmgmt-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-improvedgals-large-betterverlet-msgmgmt-floatonly-reducedlocalcalcs: test-improvedgals-large-betterverlet-msgmgmt-floatonly

test-improvedgals-large-betterverlet-msgmgmt-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-large-betterverlet-msgmgmt-floatonly-reducedlocalcalcs-singleforceloop: test-improvedgals-large-betterverlet-msgmgmt-floatonly-reducedlocalcalcs

test-improvedgals-large-betterverlet-msgmgmt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-large-betterverlet-msgmgmt-floatonly-singleforceloop: test-improvedgals-large-betterverlet-msgmgmt-floatonly

test-improvedgals-large-betterverlet-msgmgmt-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-improvedgals-large-betterverlet-msgmgmt-reducedlocalcalcs: test-improvedgals-large-betterverlet-msgmgmt

test-improvedgals-large-betterverlet-msgmgmt-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-large-betterverlet-msgmgmt-reducedlocalcalcs-singleforceloop: test-improvedgals-large-betterverlet-msgmgmt-reducedlocalcalcs

test-improvedgals-large-betterverlet-msgmgmt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-large-betterverlet-msgmgmt-singleforceloop: test-improvedgals-large-betterverlet-msgmgmt

test-improvedgals-large-betterverlet-floatonly: DFLAGS+=-DFLOAT_ONLY
test-improvedgals-large-betterverlet-floatonly: test-improvedgals-large-betterverlet

test-improvedgals-large-betterverlet-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-improvedgals-large-betterverlet-floatonly-reducedlocalcalcs: test-improvedgals-large-betterverlet-floatonly

test-improvedgals-large-betterverlet-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-large-betterverlet-floatonly-reducedlocalcalcs-singleforceloop: test-improvedgals-large-betterverlet-floatonly-reducedlocalcalcs

test-improvedgals-large-betterverlet-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-large-betterverlet-floatonly-singleforceloop: test-improvedgals-large-betterverlet-floatonly

test-improvedgals-large-betterverlet-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-improvedgals-large-betterverlet-reducedlocalcalcs: test-improvedgals-large-betterverlet

test-improvedgals-large-betterverlet-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-large-betterverlet-reducedlocalcalcs-singleforceloop: test-improvedgals-large-betterverlet-reducedlocalcalcs

test-improvedgals-large-betterverlet-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-large-betterverlet-singleforceloop: test-improvedgals-large-betterverlet

test-improvedgals-large-dtchange: DFLAGS+=-DSMALL_DT_EARLY
test-improvedgals-large-dtchange: test-improvedgals-large

test-improvedgals-large-dtchange-doublesqrt: DFLAGS+=-DOUBLE_SQRT
test-improvedgals-large-dtchange-doublesqrt: test-improvedgals-large-dtchange

test-improvedgals-large-dtchange-doublesqrt-msgmgmt: DFLAGS+=-DMESSAGE_MANAGEMENT
test-improvedgals-large-dtchange-doublesqrt-msgmgmt: test-improvedgals-large-dtchange-doublesqrt

test-improvedgals-large-dtchange-doublesqrt-msgmgmt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-improvedgals-large-dtchange-doublesqrt-msgmgmt-floatonly: test-improvedgals-large-dtchange-doublesqrt-msgmgmt

test-improvedgals-large-dtchange-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-improvedgals-large-dtchange-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs: test-improvedgals-large-dtchange-doublesqrt-msgmgmt-floatonly

test-improvedgals-large-dtchange-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-large-dtchange-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs-singleforceloop: test-improvedgals-large-dtchange-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs

test-improvedgals-large-dtchange-doublesqrt-msgmgmt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-large-dtchange-doublesqrt-msgmgmt-floatonly-singleforceloop: test-improvedgals-large-dtchange-doublesqrt-msgmgmt-floatonly

test-improvedgals-large-dtchange-doublesqrt-msgmgmt-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-improvedgals-large-dtchange-doublesqrt-msgmgmt-reducedlocalcalcs: test-improvedgals-large-dtchange-doublesqrt-msgmgmt

test-improvedgals-large-dtchange-doublesqrt-msgmgmt-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-large-dtchange-doublesqrt-msgmgmt-reducedlocalcalcs-singleforceloop: test-improvedgals-large-dtchange-doublesqrt-msgmgmt-reducedlocalcalcs

test-improvedgals-large-dtchange-doublesqrt-msgmgmt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-large-dtchange-doublesqrt-msgmgmt-singleforceloop: test-improvedgals-large-dtchange-doublesqrt-msgmgmt

test-improvedgals-large-dtchange-doublesqrt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-improvedgals-large-dtchange-doublesqrt-floatonly: test-improvedgals-large-dtchange-doublesqrt

test-improvedgals-large-dtchange-doublesqrt-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-improvedgals-large-dtchange-doublesqrt-floatonly-reducedlocalcalcs: test-improvedgals-large-dtchange-doublesqrt-floatonly

test-improvedgals-large-dtchange-doublesqrt-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-large-dtchange-doublesqrt-floatonly-reducedlocalcalcs-singleforceloop: test-improvedgals-large-dtchange-doublesqrt-floatonly-reducedlocalcalcs

test-improvedgals-large-dtchange-doublesqrt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-large-dtchange-doublesqrt-floatonly-singleforceloop: test-improvedgals-large-dtchange-doublesqrt-floatonly

test-improvedgals-large-dtchange-doublesqrt-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-improvedgals-large-dtchange-doublesqrt-reducedlocalcalcs: test-improvedgals-large-dtchange-doublesqrt

test-improvedgals-large-dtchange-doublesqrt-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-large-dtchange-doublesqrt-reducedlocalcalcs-singleforceloop: test-improvedgals-large-dtchange-doublesqrt-reducedlocalcalcs

test-improvedgals-large-dtchange-doublesqrt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-large-dtchange-doublesqrt-singleforceloop: test-improvedgals-large-dtchange-doublesqrt

test-improvedgals-large-dtchange-msgmgmt: DFLAGS+=-DMESSAGE_MANAGEMENT
test-improvedgals-large-dtchange-msgmgmt: test-improvedgals-large-dtchange

test-improvedgals-large-dtchange-msgmgmt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-improvedgals-large-dtchange-msgmgmt-floatonly: test-improvedgals-large-dtchange-msgmgmt

test-improvedgals-large-dtchange-msgmgmt-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-improvedgals-large-dtchange-msgmgmt-floatonly-reducedlocalcalcs: test-improvedgals-large-dtchange-msgmgmt-floatonly

test-improvedgals-large-dtchange-msgmgmt-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-large-dtchange-msgmgmt-floatonly-reducedlocalcalcs-singleforceloop: test-improvedgals-large-dtchange-msgmgmt-floatonly-reducedlocalcalcs

test-improvedgals-large-dtchange-msgmgmt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-large-dtchange-msgmgmt-floatonly-singleforceloop: test-improvedgals-large-dtchange-msgmgmt-floatonly

test-improvedgals-large-dtchange-msgmgmt-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-improvedgals-large-dtchange-msgmgmt-reducedlocalcalcs: test-improvedgals-large-dtchange-msgmgmt

test-improvedgals-large-dtchange-msgmgmt-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-large-dtchange-msgmgmt-reducedlocalcalcs-singleforceloop: test-improvedgals-large-dtchange-msgmgmt-reducedlocalcalcs

test-improvedgals-large-dtchange-msgmgmt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-large-dtchange-msgmgmt-singleforceloop: test-improvedgals-large-dtchange-msgmgmt

test-improvedgals-large-dtchange-floatonly: DFLAGS+=-DFLOAT_ONLY
test-improvedgals-large-dtchange-floatonly: test-improvedgals-large-dtchange

test-improvedgals-large-dtchange-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-improvedgals-large-dtchange-floatonly-reducedlocalcalcs: test-improvedgals-large-dtchange-floatonly

test-improvedgals-large-dtchange-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-large-dtchange-floatonly-reducedlocalcalcs-singleforceloop: test-improvedgals-large-dtchange-floatonly-reducedlocalcalcs

test-improvedgals-large-dtchange-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-large-dtchange-floatonly-singleforceloop: test-improvedgals-large-dtchange-floatonly

test-improvedgals-large-dtchange-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-improvedgals-large-dtchange-reducedlocalcalcs: test-improvedgals-large-dtchange

test-improvedgals-large-dtchange-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-large-dtchange-reducedlocalcalcs-singleforceloop: test-improvedgals-large-dtchange-reducedlocalcalcs

test-improvedgals-large-dtchange-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-large-dtchange-singleforceloop: test-improvedgals-large-dtchange

test-improvedgals-large-doublesqrt: DFLAGS+=-DOUBLE_SQRT
test-improvedgals-large-doublesqrt: test-improvedgals-large

test-improvedgals-large-doublesqrt-msgmgmt: DFLAGS+=-DMESSAGE_MANAGEMENT
test-improvedgals-large-doublesqrt-msgmgmt: test-improvedgals-large-doublesqrt

test-improvedgals-large-doublesqrt-msgmgmt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-improvedgals-large-doublesqrt-msgmgmt-floatonly: test-improvedgals-large-doublesqrt-msgmgmt

test-improvedgals-large-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-improvedgals-large-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs: test-improvedgals-large-doublesqrt-msgmgmt-floatonly

test-improvedgals-large-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-large-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs-singleforceloop: test-improvedgals-large-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs

test-improvedgals-large-doublesqrt-msgmgmt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-large-doublesqrt-msgmgmt-floatonly-singleforceloop: test-improvedgals-large-doublesqrt-msgmgmt-floatonly

test-improvedgals-large-doublesqrt-msgmgmt-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-improvedgals-large-doublesqrt-msgmgmt-reducedlocalcalcs: test-improvedgals-large-doublesqrt-msgmgmt

test-improvedgals-large-doublesqrt-msgmgmt-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-large-doublesqrt-msgmgmt-reducedlocalcalcs-singleforceloop: test-improvedgals-large-doublesqrt-msgmgmt-reducedlocalcalcs

test-improvedgals-large-doublesqrt-msgmgmt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-large-doublesqrt-msgmgmt-singleforceloop: test-improvedgals-large-doublesqrt-msgmgmt

test-improvedgals-large-doublesqrt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-improvedgals-large-doublesqrt-floatonly: test-improvedgals-large-doublesqrt

test-improvedgals-large-doublesqrt-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-improvedgals-large-doublesqrt-floatonly-reducedlocalcalcs: test-improvedgals-large-doublesqrt-floatonly

test-improvedgals-large-doublesqrt-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-large-doublesqrt-floatonly-reducedlocalcalcs-singleforceloop: test-improvedgals-large-doublesqrt-floatonly-reducedlocalcalcs

test-improvedgals-large-doublesqrt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-large-doublesqrt-floatonly-singleforceloop: test-improvedgals-large-doublesqrt-floatonly

test-improvedgals-large-doublesqrt-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-improvedgals-large-doublesqrt-reducedlocalcalcs: test-improvedgals-large-doublesqrt

test-improvedgals-large-doublesqrt-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-large-doublesqrt-reducedlocalcalcs-singleforceloop: test-improvedgals-large-doublesqrt-reducedlocalcalcs

test-improvedgals-large-doublesqrt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-large-doublesqrt-singleforceloop: test-improvedgals-large-doublesqrt

test-improvedgals-large-msgmgmt: DFLAGS+=-DMESSAGE_MANAGEMENT
test-improvedgals-large-msgmgmt: test-improvedgals-large

test-improvedgals-large-msgmgmt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-improvedgals-large-msgmgmt-floatonly: test-improvedgals-large-msgmgmt

test-improvedgals-large-msgmgmt-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-improvedgals-large-msgmgmt-floatonly-reducedlocalcalcs: test-improvedgals-large-msgmgmt-floatonly

test-improvedgals-large-msgmgmt-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-large-msgmgmt-floatonly-reducedlocalcalcs-singleforceloop: test-improvedgals-large-msgmgmt-floatonly-reducedlocalcalcs

test-improvedgals-large-msgmgmt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-large-msgmgmt-floatonly-singleforceloop: test-improvedgals-large-msgmgmt-floatonly

test-improvedgals-large-msgmgmt-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-improvedgals-large-msgmgmt-reducedlocalcalcs: test-improvedgals-large-msgmgmt

test-improvedgals-large-msgmgmt-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-large-msgmgmt-reducedlocalcalcs-singleforceloop: test-improvedgals-large-msgmgmt-reducedlocalcalcs

test-improvedgals-large-msgmgmt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-large-msgmgmt-singleforceloop: test-improvedgals-large-msgmgmt

test-improvedgals-large-floatonly: DFLAGS+=-DFLOAT_ONLY
test-improvedgals-large-floatonly: test-improvedgals-large

test-improvedgals-large-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-improvedgals-large-floatonly-reducedlocalcalcs: test-improvedgals-large-floatonly

test-improvedgals-large-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-large-floatonly-reducedlocalcalcs-singleforceloop: test-improvedgals-large-floatonly-reducedlocalcalcs

test-improvedgals-large-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-large-floatonly-singleforceloop: test-improvedgals-large-floatonly

test-improvedgals-large-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-improvedgals-large-reducedlocalcalcs: test-improvedgals-large

test-improvedgals-large-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-large-reducedlocalcalcs-singleforceloop: test-improvedgals-large-reducedlocalcalcs

test-improvedgals-large-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-large-singleforceloop: test-improvedgals-large

# **TEST Local calculation method: onebyone**
test-improvedgals-large-onebyone: DFLAGS+=-DONE_BY_ONE
test-improvedgals-large-onebyone: test-improvedgals-large

test-improvedgals-large-onebyone-betterverlet: DFLAGS+=-DBETTER_VERLET
test-improvedgals-large-onebyone-betterverlet: test-improvedgals-large-onebyone

test-improvedgals-large-onebyone-betterverlet-dtchange: DFLAGS+=-DSMALL_DT_EARLY
test-improvedgals-large-onebyone-betterverlet-dtchange: test-improvedgals-large-onebyone-betterverlet

test-improvedgals-large-onebyone-betterverlet-dtchange-doublesqrt: DFLAGS+=-DOUBLE_SQRT
test-improvedgals-large-onebyone-betterverlet-dtchange-doublesqrt: test-improvedgals-large-onebyone-betterverlet-dtchange

test-improvedgals-large-onebyone-betterverlet-dtchange-doublesqrt-msgmgmt: DFLAGS+=-DMESSAGE_MANAGEMENT
test-improvedgals-large-onebyone-betterverlet-dtchange-doublesqrt-msgmgmt: test-improvedgals-large-onebyone-betterverlet-dtchange-doublesqrt

test-improvedgals-large-onebyone-betterverlet-dtchange-doublesqrt-msgmgmt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-improvedgals-large-onebyone-betterverlet-dtchange-doublesqrt-msgmgmt-floatonly: test-improvedgals-large-onebyone-betterverlet-dtchange-doublesqrt-msgmgmt

test-improvedgals-large-onebyone-betterverlet-dtchange-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-improvedgals-large-onebyone-betterverlet-dtchange-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs: test-improvedgals-large-onebyone-betterverlet-dtchange-doublesqrt-msgmgmt-floatonly

test-improvedgals-large-onebyone-betterverlet-dtchange-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-large-onebyone-betterverlet-dtchange-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs-singleforceloop: test-improvedgals-large-onebyone-betterverlet-dtchange-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs

test-improvedgals-large-onebyone-betterverlet-dtchange-doublesqrt-msgmgmt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-large-onebyone-betterverlet-dtchange-doublesqrt-msgmgmt-floatonly-singleforceloop: test-improvedgals-large-onebyone-betterverlet-dtchange-doublesqrt-msgmgmt-floatonly

test-improvedgals-large-onebyone-betterverlet-dtchange-doublesqrt-msgmgmt-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-improvedgals-large-onebyone-betterverlet-dtchange-doublesqrt-msgmgmt-reducedlocalcalcs: test-improvedgals-large-onebyone-betterverlet-dtchange-doublesqrt-msgmgmt

test-improvedgals-large-onebyone-betterverlet-dtchange-doublesqrt-msgmgmt-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-large-onebyone-betterverlet-dtchange-doublesqrt-msgmgmt-reducedlocalcalcs-singleforceloop: test-improvedgals-large-onebyone-betterverlet-dtchange-doublesqrt-msgmgmt-reducedlocalcalcs

test-improvedgals-large-onebyone-betterverlet-dtchange-doublesqrt-msgmgmt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-large-onebyone-betterverlet-dtchange-doublesqrt-msgmgmt-singleforceloop: test-improvedgals-large-onebyone-betterverlet-dtchange-doublesqrt-msgmgmt

test-improvedgals-large-onebyone-betterverlet-dtchange-doublesqrt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-improvedgals-large-onebyone-betterverlet-dtchange-doublesqrt-floatonly: test-improvedgals-large-onebyone-betterverlet-dtchange-doublesqrt

test-improvedgals-large-onebyone-betterverlet-dtchange-doublesqrt-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-improvedgals-large-onebyone-betterverlet-dtchange-doublesqrt-floatonly-reducedlocalcalcs: test-improvedgals-large-onebyone-betterverlet-dtchange-doublesqrt-floatonly

test-improvedgals-large-onebyone-betterverlet-dtchange-doublesqrt-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-large-onebyone-betterverlet-dtchange-doublesqrt-floatonly-reducedlocalcalcs-singleforceloop: test-improvedgals-large-onebyone-betterverlet-dtchange-doublesqrt-floatonly-reducedlocalcalcs

test-improvedgals-large-onebyone-betterverlet-dtchange-doublesqrt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-large-onebyone-betterverlet-dtchange-doublesqrt-floatonly-singleforceloop: test-improvedgals-large-onebyone-betterverlet-dtchange-doublesqrt-floatonly

test-improvedgals-large-onebyone-betterverlet-dtchange-doublesqrt-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-improvedgals-large-onebyone-betterverlet-dtchange-doublesqrt-reducedlocalcalcs: test-improvedgals-large-onebyone-betterverlet-dtchange-doublesqrt

test-improvedgals-large-onebyone-betterverlet-dtchange-doublesqrt-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-large-onebyone-betterverlet-dtchange-doublesqrt-reducedlocalcalcs-singleforceloop: test-improvedgals-large-onebyone-betterverlet-dtchange-doublesqrt-reducedlocalcalcs

test-improvedgals-large-onebyone-betterverlet-dtchange-doublesqrt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-large-onebyone-betterverlet-dtchange-doublesqrt-singleforceloop: test-improvedgals-large-onebyone-betterverlet-dtchange-doublesqrt

test-improvedgals-large-onebyone-betterverlet-dtchange-msgmgmt: DFLAGS+=-DMESSAGE_MANAGEMENT
test-improvedgals-large-onebyone-betterverlet-dtchange-msgmgmt: test-improvedgals-large-onebyone-betterverlet-dtchange

test-improvedgals-large-onebyone-betterverlet-dtchange-msgmgmt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-improvedgals-large-onebyone-betterverlet-dtchange-msgmgmt-floatonly: test-improvedgals-large-onebyone-betterverlet-dtchange-msgmgmt

test-improvedgals-large-onebyone-betterverlet-dtchange-msgmgmt-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-improvedgals-large-onebyone-betterverlet-dtchange-msgmgmt-floatonly-reducedlocalcalcs: test-improvedgals-large-onebyone-betterverlet-dtchange-msgmgmt-floatonly

test-improvedgals-large-onebyone-betterverlet-dtchange-msgmgmt-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-large-onebyone-betterverlet-dtchange-msgmgmt-floatonly-reducedlocalcalcs-singleforceloop: test-improvedgals-large-onebyone-betterverlet-dtchange-msgmgmt-floatonly-reducedlocalcalcs

test-improvedgals-large-onebyone-betterverlet-dtchange-msgmgmt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-large-onebyone-betterverlet-dtchange-msgmgmt-floatonly-singleforceloop: test-improvedgals-large-onebyone-betterverlet-dtchange-msgmgmt-floatonly

test-improvedgals-large-onebyone-betterverlet-dtchange-msgmgmt-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-improvedgals-large-onebyone-betterverlet-dtchange-msgmgmt-reducedlocalcalcs: test-improvedgals-large-onebyone-betterverlet-dtchange-msgmgmt

test-improvedgals-large-onebyone-betterverlet-dtchange-msgmgmt-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-large-onebyone-betterverlet-dtchange-msgmgmt-reducedlocalcalcs-singleforceloop: test-improvedgals-large-onebyone-betterverlet-dtchange-msgmgmt-reducedlocalcalcs

test-improvedgals-large-onebyone-betterverlet-dtchange-msgmgmt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-large-onebyone-betterverlet-dtchange-msgmgmt-singleforceloop: test-improvedgals-large-onebyone-betterverlet-dtchange-msgmgmt

test-improvedgals-large-onebyone-betterverlet-dtchange-floatonly: DFLAGS+=-DFLOAT_ONLY
test-improvedgals-large-onebyone-betterverlet-dtchange-floatonly: test-improvedgals-large-onebyone-betterverlet-dtchange

test-improvedgals-large-onebyone-betterverlet-dtchange-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-improvedgals-large-onebyone-betterverlet-dtchange-floatonly-reducedlocalcalcs: test-improvedgals-large-onebyone-betterverlet-dtchange-floatonly

test-improvedgals-large-onebyone-betterverlet-dtchange-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-large-onebyone-betterverlet-dtchange-floatonly-reducedlocalcalcs-singleforceloop: test-improvedgals-large-onebyone-betterverlet-dtchange-floatonly-reducedlocalcalcs

test-improvedgals-large-onebyone-betterverlet-dtchange-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-large-onebyone-betterverlet-dtchange-floatonly-singleforceloop: test-improvedgals-large-onebyone-betterverlet-dtchange-floatonly

test-improvedgals-large-onebyone-betterverlet-dtchange-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-improvedgals-large-onebyone-betterverlet-dtchange-reducedlocalcalcs: test-improvedgals-large-onebyone-betterverlet-dtchange

test-improvedgals-large-onebyone-betterverlet-dtchange-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-large-onebyone-betterverlet-dtchange-reducedlocalcalcs-singleforceloop: test-improvedgals-large-onebyone-betterverlet-dtchange-reducedlocalcalcs

test-improvedgals-large-onebyone-betterverlet-dtchange-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-large-onebyone-betterverlet-dtchange-singleforceloop: test-improvedgals-large-onebyone-betterverlet-dtchange

test-improvedgals-large-onebyone-betterverlet-doublesqrt: DFLAGS+=-DOUBLE_SQRT
test-improvedgals-large-onebyone-betterverlet-doublesqrt: test-improvedgals-large-onebyone-betterverlet

test-improvedgals-large-onebyone-betterverlet-doublesqrt-msgmgmt: DFLAGS+=-DMESSAGE_MANAGEMENT
test-improvedgals-large-onebyone-betterverlet-doublesqrt-msgmgmt: test-improvedgals-large-onebyone-betterverlet-doublesqrt

test-improvedgals-large-onebyone-betterverlet-doublesqrt-msgmgmt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-improvedgals-large-onebyone-betterverlet-doublesqrt-msgmgmt-floatonly: test-improvedgals-large-onebyone-betterverlet-doublesqrt-msgmgmt

test-improvedgals-large-onebyone-betterverlet-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-improvedgals-large-onebyone-betterverlet-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs: test-improvedgals-large-onebyone-betterverlet-doublesqrt-msgmgmt-floatonly

test-improvedgals-large-onebyone-betterverlet-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-large-onebyone-betterverlet-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs-singleforceloop: test-improvedgals-large-onebyone-betterverlet-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs

test-improvedgals-large-onebyone-betterverlet-doublesqrt-msgmgmt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-large-onebyone-betterverlet-doublesqrt-msgmgmt-floatonly-singleforceloop: test-improvedgals-large-onebyone-betterverlet-doublesqrt-msgmgmt-floatonly

test-improvedgals-large-onebyone-betterverlet-doublesqrt-msgmgmt-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-improvedgals-large-onebyone-betterverlet-doublesqrt-msgmgmt-reducedlocalcalcs: test-improvedgals-large-onebyone-betterverlet-doublesqrt-msgmgmt

test-improvedgals-large-onebyone-betterverlet-doublesqrt-msgmgmt-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-large-onebyone-betterverlet-doublesqrt-msgmgmt-reducedlocalcalcs-singleforceloop: test-improvedgals-large-onebyone-betterverlet-doublesqrt-msgmgmt-reducedlocalcalcs

test-improvedgals-large-onebyone-betterverlet-doublesqrt-msgmgmt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-large-onebyone-betterverlet-doublesqrt-msgmgmt-singleforceloop: test-improvedgals-large-onebyone-betterverlet-doublesqrt-msgmgmt

test-improvedgals-large-onebyone-betterverlet-doublesqrt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-improvedgals-large-onebyone-betterverlet-doublesqrt-floatonly: test-improvedgals-large-onebyone-betterverlet-doublesqrt

test-improvedgals-large-onebyone-betterverlet-doublesqrt-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-improvedgals-large-onebyone-betterverlet-doublesqrt-floatonly-reducedlocalcalcs: test-improvedgals-large-onebyone-betterverlet-doublesqrt-floatonly

test-improvedgals-large-onebyone-betterverlet-doublesqrt-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-large-onebyone-betterverlet-doublesqrt-floatonly-reducedlocalcalcs-singleforceloop: test-improvedgals-large-onebyone-betterverlet-doublesqrt-floatonly-reducedlocalcalcs

test-improvedgals-large-onebyone-betterverlet-doublesqrt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-large-onebyone-betterverlet-doublesqrt-floatonly-singleforceloop: test-improvedgals-large-onebyone-betterverlet-doublesqrt-floatonly

test-improvedgals-large-onebyone-betterverlet-doublesqrt-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-improvedgals-large-onebyone-betterverlet-doublesqrt-reducedlocalcalcs: test-improvedgals-large-onebyone-betterverlet-doublesqrt

test-improvedgals-large-onebyone-betterverlet-doublesqrt-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-large-onebyone-betterverlet-doublesqrt-reducedlocalcalcs-singleforceloop: test-improvedgals-large-onebyone-betterverlet-doublesqrt-reducedlocalcalcs

test-improvedgals-large-onebyone-betterverlet-doublesqrt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-large-onebyone-betterverlet-doublesqrt-singleforceloop: test-improvedgals-large-onebyone-betterverlet-doublesqrt

test-improvedgals-large-onebyone-betterverlet-msgmgmt: DFLAGS+=-DMESSAGE_MANAGEMENT
test-improvedgals-large-onebyone-betterverlet-msgmgmt: test-improvedgals-large-onebyone-betterverlet

test-improvedgals-large-onebyone-betterverlet-msgmgmt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-improvedgals-large-onebyone-betterverlet-msgmgmt-floatonly: test-improvedgals-large-onebyone-betterverlet-msgmgmt

test-improvedgals-large-onebyone-betterverlet-msgmgmt-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-improvedgals-large-onebyone-betterverlet-msgmgmt-floatonly-reducedlocalcalcs: test-improvedgals-large-onebyone-betterverlet-msgmgmt-floatonly

test-improvedgals-large-onebyone-betterverlet-msgmgmt-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-large-onebyone-betterverlet-msgmgmt-floatonly-reducedlocalcalcs-singleforceloop: test-improvedgals-large-onebyone-betterverlet-msgmgmt-floatonly-reducedlocalcalcs

test-improvedgals-large-onebyone-betterverlet-msgmgmt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-large-onebyone-betterverlet-msgmgmt-floatonly-singleforceloop: test-improvedgals-large-onebyone-betterverlet-msgmgmt-floatonly

test-improvedgals-large-onebyone-betterverlet-msgmgmt-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-improvedgals-large-onebyone-betterverlet-msgmgmt-reducedlocalcalcs: test-improvedgals-large-onebyone-betterverlet-msgmgmt

test-improvedgals-large-onebyone-betterverlet-msgmgmt-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-large-onebyone-betterverlet-msgmgmt-reducedlocalcalcs-singleforceloop: test-improvedgals-large-onebyone-betterverlet-msgmgmt-reducedlocalcalcs

test-improvedgals-large-onebyone-betterverlet-msgmgmt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-large-onebyone-betterverlet-msgmgmt-singleforceloop: test-improvedgals-large-onebyone-betterverlet-msgmgmt

test-improvedgals-large-onebyone-betterverlet-floatonly: DFLAGS+=-DFLOAT_ONLY
test-improvedgals-large-onebyone-betterverlet-floatonly: test-improvedgals-large-onebyone-betterverlet

test-improvedgals-large-onebyone-betterverlet-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-improvedgals-large-onebyone-betterverlet-floatonly-reducedlocalcalcs: test-improvedgals-large-onebyone-betterverlet-floatonly

test-improvedgals-large-onebyone-betterverlet-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-large-onebyone-betterverlet-floatonly-reducedlocalcalcs-singleforceloop: test-improvedgals-large-onebyone-betterverlet-floatonly-reducedlocalcalcs

test-improvedgals-large-onebyone-betterverlet-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-large-onebyone-betterverlet-floatonly-singleforceloop: test-improvedgals-large-onebyone-betterverlet-floatonly

test-improvedgals-large-onebyone-betterverlet-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-improvedgals-large-onebyone-betterverlet-reducedlocalcalcs: test-improvedgals-large-onebyone-betterverlet

test-improvedgals-large-onebyone-betterverlet-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-large-onebyone-betterverlet-reducedlocalcalcs-singleforceloop: test-improvedgals-large-onebyone-betterverlet-reducedlocalcalcs

test-improvedgals-large-onebyone-betterverlet-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-large-onebyone-betterverlet-singleforceloop: test-improvedgals-large-onebyone-betterverlet

test-improvedgals-large-onebyone-dtchange: DFLAGS+=-DSMALL_DT_EARLY
test-improvedgals-large-onebyone-dtchange: test-improvedgals-large-onebyone

test-improvedgals-large-onebyone-dtchange-doublesqrt: DFLAGS+=-DOUBLE_SQRT
test-improvedgals-large-onebyone-dtchange-doublesqrt: test-improvedgals-large-onebyone-dtchange

test-improvedgals-large-onebyone-dtchange-doublesqrt-msgmgmt: DFLAGS+=-DMESSAGE_MANAGEMENT
test-improvedgals-large-onebyone-dtchange-doublesqrt-msgmgmt: test-improvedgals-large-onebyone-dtchange-doublesqrt

test-improvedgals-large-onebyone-dtchange-doublesqrt-msgmgmt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-improvedgals-large-onebyone-dtchange-doublesqrt-msgmgmt-floatonly: test-improvedgals-large-onebyone-dtchange-doublesqrt-msgmgmt

test-improvedgals-large-onebyone-dtchange-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-improvedgals-large-onebyone-dtchange-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs: test-improvedgals-large-onebyone-dtchange-doublesqrt-msgmgmt-floatonly

test-improvedgals-large-onebyone-dtchange-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-large-onebyone-dtchange-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs-singleforceloop: test-improvedgals-large-onebyone-dtchange-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs

test-improvedgals-large-onebyone-dtchange-doublesqrt-msgmgmt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-large-onebyone-dtchange-doublesqrt-msgmgmt-floatonly-singleforceloop: test-improvedgals-large-onebyone-dtchange-doublesqrt-msgmgmt-floatonly

test-improvedgals-large-onebyone-dtchange-doublesqrt-msgmgmt-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-improvedgals-large-onebyone-dtchange-doublesqrt-msgmgmt-reducedlocalcalcs: test-improvedgals-large-onebyone-dtchange-doublesqrt-msgmgmt

test-improvedgals-large-onebyone-dtchange-doublesqrt-msgmgmt-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-large-onebyone-dtchange-doublesqrt-msgmgmt-reducedlocalcalcs-singleforceloop: test-improvedgals-large-onebyone-dtchange-doublesqrt-msgmgmt-reducedlocalcalcs

test-improvedgals-large-onebyone-dtchange-doublesqrt-msgmgmt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-large-onebyone-dtchange-doublesqrt-msgmgmt-singleforceloop: test-improvedgals-large-onebyone-dtchange-doublesqrt-msgmgmt

test-improvedgals-large-onebyone-dtchange-doublesqrt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-improvedgals-large-onebyone-dtchange-doublesqrt-floatonly: test-improvedgals-large-onebyone-dtchange-doublesqrt

test-improvedgals-large-onebyone-dtchange-doublesqrt-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-improvedgals-large-onebyone-dtchange-doublesqrt-floatonly-reducedlocalcalcs: test-improvedgals-large-onebyone-dtchange-doublesqrt-floatonly

test-improvedgals-large-onebyone-dtchange-doublesqrt-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-large-onebyone-dtchange-doublesqrt-floatonly-reducedlocalcalcs-singleforceloop: test-improvedgals-large-onebyone-dtchange-doublesqrt-floatonly-reducedlocalcalcs

test-improvedgals-large-onebyone-dtchange-doublesqrt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-large-onebyone-dtchange-doublesqrt-floatonly-singleforceloop: test-improvedgals-large-onebyone-dtchange-doublesqrt-floatonly

test-improvedgals-large-onebyone-dtchange-doublesqrt-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-improvedgals-large-onebyone-dtchange-doublesqrt-reducedlocalcalcs: test-improvedgals-large-onebyone-dtchange-doublesqrt

test-improvedgals-large-onebyone-dtchange-doublesqrt-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-large-onebyone-dtchange-doublesqrt-reducedlocalcalcs-singleforceloop: test-improvedgals-large-onebyone-dtchange-doublesqrt-reducedlocalcalcs

test-improvedgals-large-onebyone-dtchange-doublesqrt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-large-onebyone-dtchange-doublesqrt-singleforceloop: test-improvedgals-large-onebyone-dtchange-doublesqrt

test-improvedgals-large-onebyone-dtchange-msgmgmt: DFLAGS+=-DMESSAGE_MANAGEMENT
test-improvedgals-large-onebyone-dtchange-msgmgmt: test-improvedgals-large-onebyone-dtchange

test-improvedgals-large-onebyone-dtchange-msgmgmt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-improvedgals-large-onebyone-dtchange-msgmgmt-floatonly: test-improvedgals-large-onebyone-dtchange-msgmgmt

test-improvedgals-large-onebyone-dtchange-msgmgmt-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-improvedgals-large-onebyone-dtchange-msgmgmt-floatonly-reducedlocalcalcs: test-improvedgals-large-onebyone-dtchange-msgmgmt-floatonly

test-improvedgals-large-onebyone-dtchange-msgmgmt-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-large-onebyone-dtchange-msgmgmt-floatonly-reducedlocalcalcs-singleforceloop: test-improvedgals-large-onebyone-dtchange-msgmgmt-floatonly-reducedlocalcalcs

test-improvedgals-large-onebyone-dtchange-msgmgmt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-large-onebyone-dtchange-msgmgmt-floatonly-singleforceloop: test-improvedgals-large-onebyone-dtchange-msgmgmt-floatonly

test-improvedgals-large-onebyone-dtchange-msgmgmt-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-improvedgals-large-onebyone-dtchange-msgmgmt-reducedlocalcalcs: test-improvedgals-large-onebyone-dtchange-msgmgmt

test-improvedgals-large-onebyone-dtchange-msgmgmt-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-large-onebyone-dtchange-msgmgmt-reducedlocalcalcs-singleforceloop: test-improvedgals-large-onebyone-dtchange-msgmgmt-reducedlocalcalcs

test-improvedgals-large-onebyone-dtchange-msgmgmt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-large-onebyone-dtchange-msgmgmt-singleforceloop: test-improvedgals-large-onebyone-dtchange-msgmgmt

test-improvedgals-large-onebyone-dtchange-floatonly: DFLAGS+=-DFLOAT_ONLY
test-improvedgals-large-onebyone-dtchange-floatonly: test-improvedgals-large-onebyone-dtchange

test-improvedgals-large-onebyone-dtchange-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-improvedgals-large-onebyone-dtchange-floatonly-reducedlocalcalcs: test-improvedgals-large-onebyone-dtchange-floatonly

test-improvedgals-large-onebyone-dtchange-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-large-onebyone-dtchange-floatonly-reducedlocalcalcs-singleforceloop: test-improvedgals-large-onebyone-dtchange-floatonly-reducedlocalcalcs

test-improvedgals-large-onebyone-dtchange-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-large-onebyone-dtchange-floatonly-singleforceloop: test-improvedgals-large-onebyone-dtchange-floatonly

test-improvedgals-large-onebyone-dtchange-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-improvedgals-large-onebyone-dtchange-reducedlocalcalcs: test-improvedgals-large-onebyone-dtchange

test-improvedgals-large-onebyone-dtchange-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-large-onebyone-dtchange-reducedlocalcalcs-singleforceloop: test-improvedgals-large-onebyone-dtchange-reducedlocalcalcs

test-improvedgals-large-onebyone-dtchange-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-large-onebyone-dtchange-singleforceloop: test-improvedgals-large-onebyone-dtchange

test-improvedgals-large-onebyone-doublesqrt: DFLAGS+=-DOUBLE_SQRT
test-improvedgals-large-onebyone-doublesqrt: test-improvedgals-large-onebyone

test-improvedgals-large-onebyone-doublesqrt-msgmgmt: DFLAGS+=-DMESSAGE_MANAGEMENT
test-improvedgals-large-onebyone-doublesqrt-msgmgmt: test-improvedgals-large-onebyone-doublesqrt

test-improvedgals-large-onebyone-doublesqrt-msgmgmt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-improvedgals-large-onebyone-doublesqrt-msgmgmt-floatonly: test-improvedgals-large-onebyone-doublesqrt-msgmgmt

test-improvedgals-large-onebyone-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-improvedgals-large-onebyone-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs: test-improvedgals-large-onebyone-doublesqrt-msgmgmt-floatonly

test-improvedgals-large-onebyone-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-large-onebyone-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs-singleforceloop: test-improvedgals-large-onebyone-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs

test-improvedgals-large-onebyone-doublesqrt-msgmgmt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-large-onebyone-doublesqrt-msgmgmt-floatonly-singleforceloop: test-improvedgals-large-onebyone-doublesqrt-msgmgmt-floatonly

test-improvedgals-large-onebyone-doublesqrt-msgmgmt-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-improvedgals-large-onebyone-doublesqrt-msgmgmt-reducedlocalcalcs: test-improvedgals-large-onebyone-doublesqrt-msgmgmt

test-improvedgals-large-onebyone-doublesqrt-msgmgmt-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-large-onebyone-doublesqrt-msgmgmt-reducedlocalcalcs-singleforceloop: test-improvedgals-large-onebyone-doublesqrt-msgmgmt-reducedlocalcalcs

test-improvedgals-large-onebyone-doublesqrt-msgmgmt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-large-onebyone-doublesqrt-msgmgmt-singleforceloop: test-improvedgals-large-onebyone-doublesqrt-msgmgmt

test-improvedgals-large-onebyone-doublesqrt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-improvedgals-large-onebyone-doublesqrt-floatonly: test-improvedgals-large-onebyone-doublesqrt

test-improvedgals-large-onebyone-doublesqrt-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-improvedgals-large-onebyone-doublesqrt-floatonly-reducedlocalcalcs: test-improvedgals-large-onebyone-doublesqrt-floatonly

test-improvedgals-large-onebyone-doublesqrt-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-large-onebyone-doublesqrt-floatonly-reducedlocalcalcs-singleforceloop: test-improvedgals-large-onebyone-doublesqrt-floatonly-reducedlocalcalcs

test-improvedgals-large-onebyone-doublesqrt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-large-onebyone-doublesqrt-floatonly-singleforceloop: test-improvedgals-large-onebyone-doublesqrt-floatonly

test-improvedgals-large-onebyone-doublesqrt-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-improvedgals-large-onebyone-doublesqrt-reducedlocalcalcs: test-improvedgals-large-onebyone-doublesqrt

test-improvedgals-large-onebyone-doublesqrt-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-large-onebyone-doublesqrt-reducedlocalcalcs-singleforceloop: test-improvedgals-large-onebyone-doublesqrt-reducedlocalcalcs

test-improvedgals-large-onebyone-doublesqrt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-large-onebyone-doublesqrt-singleforceloop: test-improvedgals-large-onebyone-doublesqrt

test-improvedgals-large-onebyone-msgmgmt: DFLAGS+=-DMESSAGE_MANAGEMENT
test-improvedgals-large-onebyone-msgmgmt: test-improvedgals-large-onebyone

test-improvedgals-large-onebyone-msgmgmt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-improvedgals-large-onebyone-msgmgmt-floatonly: test-improvedgals-large-onebyone-msgmgmt

test-improvedgals-large-onebyone-msgmgmt-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-improvedgals-large-onebyone-msgmgmt-floatonly-reducedlocalcalcs: test-improvedgals-large-onebyone-msgmgmt-floatonly

test-improvedgals-large-onebyone-msgmgmt-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-large-onebyone-msgmgmt-floatonly-reducedlocalcalcs-singleforceloop: test-improvedgals-large-onebyone-msgmgmt-floatonly-reducedlocalcalcs

test-improvedgals-large-onebyone-msgmgmt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-large-onebyone-msgmgmt-floatonly-singleforceloop: test-improvedgals-large-onebyone-msgmgmt-floatonly

test-improvedgals-large-onebyone-msgmgmt-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-improvedgals-large-onebyone-msgmgmt-reducedlocalcalcs: test-improvedgals-large-onebyone-msgmgmt

test-improvedgals-large-onebyone-msgmgmt-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-large-onebyone-msgmgmt-reducedlocalcalcs-singleforceloop: test-improvedgals-large-onebyone-msgmgmt-reducedlocalcalcs

test-improvedgals-large-onebyone-msgmgmt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-large-onebyone-msgmgmt-singleforceloop: test-improvedgals-large-onebyone-msgmgmt

test-improvedgals-large-onebyone-floatonly: DFLAGS+=-DFLOAT_ONLY
test-improvedgals-large-onebyone-floatonly: test-improvedgals-large-onebyone

test-improvedgals-large-onebyone-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-improvedgals-large-onebyone-floatonly-reducedlocalcalcs: test-improvedgals-large-onebyone-floatonly

test-improvedgals-large-onebyone-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-large-onebyone-floatonly-reducedlocalcalcs-singleforceloop: test-improvedgals-large-onebyone-floatonly-reducedlocalcalcs

test-improvedgals-large-onebyone-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-large-onebyone-floatonly-singleforceloop: test-improvedgals-large-onebyone-floatonly

test-improvedgals-large-onebyone-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-improvedgals-large-onebyone-reducedlocalcalcs: test-improvedgals-large-onebyone

test-improvedgals-large-onebyone-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-large-onebyone-reducedlocalcalcs-singleforceloop: test-improvedgals-large-onebyone-reducedlocalcalcs

test-improvedgals-large-onebyone-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-large-onebyone-singleforceloop: test-improvedgals-large-onebyone

test-improvedgals-bonds: DFLAGS+=-DBONDS
test-improvedgals-bonds: test-improvedgals

test-improvedgals-bonds-betterverlet: DFLAGS+=-DBETTER_VERLET
test-improvedgals-bonds-betterverlet: test-improvedgals-bonds

test-improvedgals-bonds-betterverlet-dtchange: DFLAGS+=-DSMALL_DT_EARLY
test-improvedgals-bonds-betterverlet-dtchange: test-improvedgals-bonds-betterverlet

test-improvedgals-bonds-betterverlet-dtchange-doublesqrt: DFLAGS+=-DOUBLE_SQRT
test-improvedgals-bonds-betterverlet-dtchange-doublesqrt: test-improvedgals-bonds-betterverlet-dtchange

test-improvedgals-bonds-betterverlet-dtchange-doublesqrt-msgmgmt: DFLAGS+=-DMESSAGE_MANAGEMENT
test-improvedgals-bonds-betterverlet-dtchange-doublesqrt-msgmgmt: test-improvedgals-bonds-betterverlet-dtchange-doublesqrt

test-improvedgals-bonds-betterverlet-dtchange-doublesqrt-msgmgmt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-improvedgals-bonds-betterverlet-dtchange-doublesqrt-msgmgmt-floatonly: test-improvedgals-bonds-betterverlet-dtchange-doublesqrt-msgmgmt

test-improvedgals-bonds-betterverlet-dtchange-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-improvedgals-bonds-betterverlet-dtchange-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs: test-improvedgals-bonds-betterverlet-dtchange-doublesqrt-msgmgmt-floatonly

test-improvedgals-bonds-betterverlet-dtchange-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-bonds-betterverlet-dtchange-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs-singleforceloop: test-improvedgals-bonds-betterverlet-dtchange-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs

test-improvedgals-bonds-betterverlet-dtchange-doublesqrt-msgmgmt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-bonds-betterverlet-dtchange-doublesqrt-msgmgmt-floatonly-singleforceloop: test-improvedgals-bonds-betterverlet-dtchange-doublesqrt-msgmgmt-floatonly

test-improvedgals-bonds-betterverlet-dtchange-doublesqrt-msgmgmt-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-improvedgals-bonds-betterverlet-dtchange-doublesqrt-msgmgmt-reducedlocalcalcs: test-improvedgals-bonds-betterverlet-dtchange-doublesqrt-msgmgmt

test-improvedgals-bonds-betterverlet-dtchange-doublesqrt-msgmgmt-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-bonds-betterverlet-dtchange-doublesqrt-msgmgmt-reducedlocalcalcs-singleforceloop: test-improvedgals-bonds-betterverlet-dtchange-doublesqrt-msgmgmt-reducedlocalcalcs

test-improvedgals-bonds-betterverlet-dtchange-doublesqrt-msgmgmt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-bonds-betterverlet-dtchange-doublesqrt-msgmgmt-singleforceloop: test-improvedgals-bonds-betterverlet-dtchange-doublesqrt-msgmgmt

test-improvedgals-bonds-betterverlet-dtchange-doublesqrt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-improvedgals-bonds-betterverlet-dtchange-doublesqrt-floatonly: test-improvedgals-bonds-betterverlet-dtchange-doublesqrt

test-improvedgals-bonds-betterverlet-dtchange-doublesqrt-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-improvedgals-bonds-betterverlet-dtchange-doublesqrt-floatonly-reducedlocalcalcs: test-improvedgals-bonds-betterverlet-dtchange-doublesqrt-floatonly

test-improvedgals-bonds-betterverlet-dtchange-doublesqrt-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-bonds-betterverlet-dtchange-doublesqrt-floatonly-reducedlocalcalcs-singleforceloop: test-improvedgals-bonds-betterverlet-dtchange-doublesqrt-floatonly-reducedlocalcalcs

test-improvedgals-bonds-betterverlet-dtchange-doublesqrt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-bonds-betterverlet-dtchange-doublesqrt-floatonly-singleforceloop: test-improvedgals-bonds-betterverlet-dtchange-doublesqrt-floatonly

test-improvedgals-bonds-betterverlet-dtchange-doublesqrt-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-improvedgals-bonds-betterverlet-dtchange-doublesqrt-reducedlocalcalcs: test-improvedgals-bonds-betterverlet-dtchange-doublesqrt

test-improvedgals-bonds-betterverlet-dtchange-doublesqrt-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-bonds-betterverlet-dtchange-doublesqrt-reducedlocalcalcs-singleforceloop: test-improvedgals-bonds-betterverlet-dtchange-doublesqrt-reducedlocalcalcs

test-improvedgals-bonds-betterverlet-dtchange-doublesqrt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-bonds-betterverlet-dtchange-doublesqrt-singleforceloop: test-improvedgals-bonds-betterverlet-dtchange-doublesqrt

test-improvedgals-bonds-betterverlet-dtchange-msgmgmt: DFLAGS+=-DMESSAGE_MANAGEMENT
test-improvedgals-bonds-betterverlet-dtchange-msgmgmt: test-improvedgals-bonds-betterverlet-dtchange

test-improvedgals-bonds-betterverlet-dtchange-msgmgmt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-improvedgals-bonds-betterverlet-dtchange-msgmgmt-floatonly: test-improvedgals-bonds-betterverlet-dtchange-msgmgmt

test-improvedgals-bonds-betterverlet-dtchange-msgmgmt-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-improvedgals-bonds-betterverlet-dtchange-msgmgmt-floatonly-reducedlocalcalcs: test-improvedgals-bonds-betterverlet-dtchange-msgmgmt-floatonly

test-improvedgals-bonds-betterverlet-dtchange-msgmgmt-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-bonds-betterverlet-dtchange-msgmgmt-floatonly-reducedlocalcalcs-singleforceloop: test-improvedgals-bonds-betterverlet-dtchange-msgmgmt-floatonly-reducedlocalcalcs

test-improvedgals-bonds-betterverlet-dtchange-msgmgmt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-bonds-betterverlet-dtchange-msgmgmt-floatonly-singleforceloop: test-improvedgals-bonds-betterverlet-dtchange-msgmgmt-floatonly

test-improvedgals-bonds-betterverlet-dtchange-msgmgmt-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-improvedgals-bonds-betterverlet-dtchange-msgmgmt-reducedlocalcalcs: test-improvedgals-bonds-betterverlet-dtchange-msgmgmt

test-improvedgals-bonds-betterverlet-dtchange-msgmgmt-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-bonds-betterverlet-dtchange-msgmgmt-reducedlocalcalcs-singleforceloop: test-improvedgals-bonds-betterverlet-dtchange-msgmgmt-reducedlocalcalcs

test-improvedgals-bonds-betterverlet-dtchange-msgmgmt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-bonds-betterverlet-dtchange-msgmgmt-singleforceloop: test-improvedgals-bonds-betterverlet-dtchange-msgmgmt

test-improvedgals-bonds-betterverlet-dtchange-floatonly: DFLAGS+=-DFLOAT_ONLY
test-improvedgals-bonds-betterverlet-dtchange-floatonly: test-improvedgals-bonds-betterverlet-dtchange

test-improvedgals-bonds-betterverlet-dtchange-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-improvedgals-bonds-betterverlet-dtchange-floatonly-reducedlocalcalcs: test-improvedgals-bonds-betterverlet-dtchange-floatonly

test-improvedgals-bonds-betterverlet-dtchange-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-bonds-betterverlet-dtchange-floatonly-reducedlocalcalcs-singleforceloop: test-improvedgals-bonds-betterverlet-dtchange-floatonly-reducedlocalcalcs

test-improvedgals-bonds-betterverlet-dtchange-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-bonds-betterverlet-dtchange-floatonly-singleforceloop: test-improvedgals-bonds-betterverlet-dtchange-floatonly

test-improvedgals-bonds-betterverlet-dtchange-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-improvedgals-bonds-betterverlet-dtchange-reducedlocalcalcs: test-improvedgals-bonds-betterverlet-dtchange

test-improvedgals-bonds-betterverlet-dtchange-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-bonds-betterverlet-dtchange-reducedlocalcalcs-singleforceloop: test-improvedgals-bonds-betterverlet-dtchange-reducedlocalcalcs

test-improvedgals-bonds-betterverlet-dtchange-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-bonds-betterverlet-dtchange-singleforceloop: test-improvedgals-bonds-betterverlet-dtchange

test-improvedgals-bonds-betterverlet-doublesqrt: DFLAGS+=-DOUBLE_SQRT
test-improvedgals-bonds-betterverlet-doublesqrt: test-improvedgals-bonds-betterverlet

test-improvedgals-bonds-betterverlet-doublesqrt-msgmgmt: DFLAGS+=-DMESSAGE_MANAGEMENT
test-improvedgals-bonds-betterverlet-doublesqrt-msgmgmt: test-improvedgals-bonds-betterverlet-doublesqrt

test-improvedgals-bonds-betterverlet-doublesqrt-msgmgmt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-improvedgals-bonds-betterverlet-doublesqrt-msgmgmt-floatonly: test-improvedgals-bonds-betterverlet-doublesqrt-msgmgmt

test-improvedgals-bonds-betterverlet-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-improvedgals-bonds-betterverlet-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs: test-improvedgals-bonds-betterverlet-doublesqrt-msgmgmt-floatonly

test-improvedgals-bonds-betterverlet-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-bonds-betterverlet-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs-singleforceloop: test-improvedgals-bonds-betterverlet-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs

test-improvedgals-bonds-betterverlet-doublesqrt-msgmgmt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-bonds-betterverlet-doublesqrt-msgmgmt-floatonly-singleforceloop: test-improvedgals-bonds-betterverlet-doublesqrt-msgmgmt-floatonly

test-improvedgals-bonds-betterverlet-doublesqrt-msgmgmt-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-improvedgals-bonds-betterverlet-doublesqrt-msgmgmt-reducedlocalcalcs: test-improvedgals-bonds-betterverlet-doublesqrt-msgmgmt

test-improvedgals-bonds-betterverlet-doublesqrt-msgmgmt-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-bonds-betterverlet-doublesqrt-msgmgmt-reducedlocalcalcs-singleforceloop: test-improvedgals-bonds-betterverlet-doublesqrt-msgmgmt-reducedlocalcalcs

test-improvedgals-bonds-betterverlet-doublesqrt-msgmgmt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-bonds-betterverlet-doublesqrt-msgmgmt-singleforceloop: test-improvedgals-bonds-betterverlet-doublesqrt-msgmgmt

test-improvedgals-bonds-betterverlet-doublesqrt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-improvedgals-bonds-betterverlet-doublesqrt-floatonly: test-improvedgals-bonds-betterverlet-doublesqrt

test-improvedgals-bonds-betterverlet-doublesqrt-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-improvedgals-bonds-betterverlet-doublesqrt-floatonly-reducedlocalcalcs: test-improvedgals-bonds-betterverlet-doublesqrt-floatonly

test-improvedgals-bonds-betterverlet-doublesqrt-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-bonds-betterverlet-doublesqrt-floatonly-reducedlocalcalcs-singleforceloop: test-improvedgals-bonds-betterverlet-doublesqrt-floatonly-reducedlocalcalcs

test-improvedgals-bonds-betterverlet-doublesqrt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-bonds-betterverlet-doublesqrt-floatonly-singleforceloop: test-improvedgals-bonds-betterverlet-doublesqrt-floatonly

test-improvedgals-bonds-betterverlet-doublesqrt-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-improvedgals-bonds-betterverlet-doublesqrt-reducedlocalcalcs: test-improvedgals-bonds-betterverlet-doublesqrt

test-improvedgals-bonds-betterverlet-doublesqrt-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-bonds-betterverlet-doublesqrt-reducedlocalcalcs-singleforceloop: test-improvedgals-bonds-betterverlet-doublesqrt-reducedlocalcalcs

test-improvedgals-bonds-betterverlet-doublesqrt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-bonds-betterverlet-doublesqrt-singleforceloop: test-improvedgals-bonds-betterverlet-doublesqrt

test-improvedgals-bonds-betterverlet-msgmgmt: DFLAGS+=-DMESSAGE_MANAGEMENT
test-improvedgals-bonds-betterverlet-msgmgmt: test-improvedgals-bonds-betterverlet

test-improvedgals-bonds-betterverlet-msgmgmt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-improvedgals-bonds-betterverlet-msgmgmt-floatonly: test-improvedgals-bonds-betterverlet-msgmgmt

test-improvedgals-bonds-betterverlet-msgmgmt-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-improvedgals-bonds-betterverlet-msgmgmt-floatonly-reducedlocalcalcs: test-improvedgals-bonds-betterverlet-msgmgmt-floatonly

test-improvedgals-bonds-betterverlet-msgmgmt-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-bonds-betterverlet-msgmgmt-floatonly-reducedlocalcalcs-singleforceloop: test-improvedgals-bonds-betterverlet-msgmgmt-floatonly-reducedlocalcalcs

test-improvedgals-bonds-betterverlet-msgmgmt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-bonds-betterverlet-msgmgmt-floatonly-singleforceloop: test-improvedgals-bonds-betterverlet-msgmgmt-floatonly

test-improvedgals-bonds-betterverlet-msgmgmt-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-improvedgals-bonds-betterverlet-msgmgmt-reducedlocalcalcs: test-improvedgals-bonds-betterverlet-msgmgmt

test-improvedgals-bonds-betterverlet-msgmgmt-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-bonds-betterverlet-msgmgmt-reducedlocalcalcs-singleforceloop: test-improvedgals-bonds-betterverlet-msgmgmt-reducedlocalcalcs

test-improvedgals-bonds-betterverlet-msgmgmt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-bonds-betterverlet-msgmgmt-singleforceloop: test-improvedgals-bonds-betterverlet-msgmgmt

test-improvedgals-bonds-betterverlet-floatonly: DFLAGS+=-DFLOAT_ONLY
test-improvedgals-bonds-betterverlet-floatonly: test-improvedgals-bonds-betterverlet

test-improvedgals-bonds-betterverlet-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-improvedgals-bonds-betterverlet-floatonly-reducedlocalcalcs: test-improvedgals-bonds-betterverlet-floatonly

test-improvedgals-bonds-betterverlet-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-bonds-betterverlet-floatonly-reducedlocalcalcs-singleforceloop: test-improvedgals-bonds-betterverlet-floatonly-reducedlocalcalcs

test-improvedgals-bonds-betterverlet-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-bonds-betterverlet-floatonly-singleforceloop: test-improvedgals-bonds-betterverlet-floatonly

test-improvedgals-bonds-betterverlet-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-improvedgals-bonds-betterverlet-reducedlocalcalcs: test-improvedgals-bonds-betterverlet

test-improvedgals-bonds-betterverlet-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-bonds-betterverlet-reducedlocalcalcs-singleforceloop: test-improvedgals-bonds-betterverlet-reducedlocalcalcs

test-improvedgals-bonds-betterverlet-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-bonds-betterverlet-singleforceloop: test-improvedgals-bonds-betterverlet

test-improvedgals-bonds-dtchange: DFLAGS+=-DSMALL_DT_EARLY
test-improvedgals-bonds-dtchange: test-improvedgals-bonds

test-improvedgals-bonds-dtchange-doublesqrt: DFLAGS+=-DOUBLE_SQRT
test-improvedgals-bonds-dtchange-doublesqrt: test-improvedgals-bonds-dtchange

test-improvedgals-bonds-dtchange-doublesqrt-msgmgmt: DFLAGS+=-DMESSAGE_MANAGEMENT
test-improvedgals-bonds-dtchange-doublesqrt-msgmgmt: test-improvedgals-bonds-dtchange-doublesqrt

test-improvedgals-bonds-dtchange-doublesqrt-msgmgmt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-improvedgals-bonds-dtchange-doublesqrt-msgmgmt-floatonly: test-improvedgals-bonds-dtchange-doublesqrt-msgmgmt

test-improvedgals-bonds-dtchange-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-improvedgals-bonds-dtchange-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs: test-improvedgals-bonds-dtchange-doublesqrt-msgmgmt-floatonly

test-improvedgals-bonds-dtchange-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-bonds-dtchange-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs-singleforceloop: test-improvedgals-bonds-dtchange-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs

test-improvedgals-bonds-dtchange-doublesqrt-msgmgmt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-bonds-dtchange-doublesqrt-msgmgmt-floatonly-singleforceloop: test-improvedgals-bonds-dtchange-doublesqrt-msgmgmt-floatonly

test-improvedgals-bonds-dtchange-doublesqrt-msgmgmt-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-improvedgals-bonds-dtchange-doublesqrt-msgmgmt-reducedlocalcalcs: test-improvedgals-bonds-dtchange-doublesqrt-msgmgmt

test-improvedgals-bonds-dtchange-doublesqrt-msgmgmt-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-bonds-dtchange-doublesqrt-msgmgmt-reducedlocalcalcs-singleforceloop: test-improvedgals-bonds-dtchange-doublesqrt-msgmgmt-reducedlocalcalcs

test-improvedgals-bonds-dtchange-doublesqrt-msgmgmt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-bonds-dtchange-doublesqrt-msgmgmt-singleforceloop: test-improvedgals-bonds-dtchange-doublesqrt-msgmgmt

test-improvedgals-bonds-dtchange-doublesqrt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-improvedgals-bonds-dtchange-doublesqrt-floatonly: test-improvedgals-bonds-dtchange-doublesqrt

test-improvedgals-bonds-dtchange-doublesqrt-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-improvedgals-bonds-dtchange-doublesqrt-floatonly-reducedlocalcalcs: test-improvedgals-bonds-dtchange-doublesqrt-floatonly

test-improvedgals-bonds-dtchange-doublesqrt-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-bonds-dtchange-doublesqrt-floatonly-reducedlocalcalcs-singleforceloop: test-improvedgals-bonds-dtchange-doublesqrt-floatonly-reducedlocalcalcs

test-improvedgals-bonds-dtchange-doublesqrt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-bonds-dtchange-doublesqrt-floatonly-singleforceloop: test-improvedgals-bonds-dtchange-doublesqrt-floatonly

test-improvedgals-bonds-dtchange-doublesqrt-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-improvedgals-bonds-dtchange-doublesqrt-reducedlocalcalcs: test-improvedgals-bonds-dtchange-doublesqrt

test-improvedgals-bonds-dtchange-doublesqrt-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-bonds-dtchange-doublesqrt-reducedlocalcalcs-singleforceloop: test-improvedgals-bonds-dtchange-doublesqrt-reducedlocalcalcs

test-improvedgals-bonds-dtchange-doublesqrt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-bonds-dtchange-doublesqrt-singleforceloop: test-improvedgals-bonds-dtchange-doublesqrt

test-improvedgals-bonds-dtchange-msgmgmt: DFLAGS+=-DMESSAGE_MANAGEMENT
test-improvedgals-bonds-dtchange-msgmgmt: test-improvedgals-bonds-dtchange

test-improvedgals-bonds-dtchange-msgmgmt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-improvedgals-bonds-dtchange-msgmgmt-floatonly: test-improvedgals-bonds-dtchange-msgmgmt

test-improvedgals-bonds-dtchange-msgmgmt-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-improvedgals-bonds-dtchange-msgmgmt-floatonly-reducedlocalcalcs: test-improvedgals-bonds-dtchange-msgmgmt-floatonly

test-improvedgals-bonds-dtchange-msgmgmt-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-bonds-dtchange-msgmgmt-floatonly-reducedlocalcalcs-singleforceloop: test-improvedgals-bonds-dtchange-msgmgmt-floatonly-reducedlocalcalcs

test-improvedgals-bonds-dtchange-msgmgmt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-bonds-dtchange-msgmgmt-floatonly-singleforceloop: test-improvedgals-bonds-dtchange-msgmgmt-floatonly

test-improvedgals-bonds-dtchange-msgmgmt-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-improvedgals-bonds-dtchange-msgmgmt-reducedlocalcalcs: test-improvedgals-bonds-dtchange-msgmgmt

test-improvedgals-bonds-dtchange-msgmgmt-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-bonds-dtchange-msgmgmt-reducedlocalcalcs-singleforceloop: test-improvedgals-bonds-dtchange-msgmgmt-reducedlocalcalcs

test-improvedgals-bonds-dtchange-msgmgmt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-bonds-dtchange-msgmgmt-singleforceloop: test-improvedgals-bonds-dtchange-msgmgmt

test-improvedgals-bonds-dtchange-floatonly: DFLAGS+=-DFLOAT_ONLY
test-improvedgals-bonds-dtchange-floatonly: test-improvedgals-bonds-dtchange

test-improvedgals-bonds-dtchange-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-improvedgals-bonds-dtchange-floatonly-reducedlocalcalcs: test-improvedgals-bonds-dtchange-floatonly

test-improvedgals-bonds-dtchange-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-bonds-dtchange-floatonly-reducedlocalcalcs-singleforceloop: test-improvedgals-bonds-dtchange-floatonly-reducedlocalcalcs

test-improvedgals-bonds-dtchange-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-bonds-dtchange-floatonly-singleforceloop: test-improvedgals-bonds-dtchange-floatonly

test-improvedgals-bonds-dtchange-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-improvedgals-bonds-dtchange-reducedlocalcalcs: test-improvedgals-bonds-dtchange

test-improvedgals-bonds-dtchange-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-bonds-dtchange-reducedlocalcalcs-singleforceloop: test-improvedgals-bonds-dtchange-reducedlocalcalcs

test-improvedgals-bonds-dtchange-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-bonds-dtchange-singleforceloop: test-improvedgals-bonds-dtchange

test-improvedgals-bonds-doublesqrt: DFLAGS+=-DOUBLE_SQRT
test-improvedgals-bonds-doublesqrt: test-improvedgals-bonds

test-improvedgals-bonds-doublesqrt-msgmgmt: DFLAGS+=-DMESSAGE_MANAGEMENT
test-improvedgals-bonds-doublesqrt-msgmgmt: test-improvedgals-bonds-doublesqrt

test-improvedgals-bonds-doublesqrt-msgmgmt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-improvedgals-bonds-doublesqrt-msgmgmt-floatonly: test-improvedgals-bonds-doublesqrt-msgmgmt

test-improvedgals-bonds-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-improvedgals-bonds-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs: test-improvedgals-bonds-doublesqrt-msgmgmt-floatonly

test-improvedgals-bonds-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-bonds-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs-singleforceloop: test-improvedgals-bonds-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs

test-improvedgals-bonds-doublesqrt-msgmgmt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-bonds-doublesqrt-msgmgmt-floatonly-singleforceloop: test-improvedgals-bonds-doublesqrt-msgmgmt-floatonly

test-improvedgals-bonds-doublesqrt-msgmgmt-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-improvedgals-bonds-doublesqrt-msgmgmt-reducedlocalcalcs: test-improvedgals-bonds-doublesqrt-msgmgmt

test-improvedgals-bonds-doublesqrt-msgmgmt-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-bonds-doublesqrt-msgmgmt-reducedlocalcalcs-singleforceloop: test-improvedgals-bonds-doublesqrt-msgmgmt-reducedlocalcalcs

test-improvedgals-bonds-doublesqrt-msgmgmt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-bonds-doublesqrt-msgmgmt-singleforceloop: test-improvedgals-bonds-doublesqrt-msgmgmt

test-improvedgals-bonds-doublesqrt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-improvedgals-bonds-doublesqrt-floatonly: test-improvedgals-bonds-doublesqrt

test-improvedgals-bonds-doublesqrt-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-improvedgals-bonds-doublesqrt-floatonly-reducedlocalcalcs: test-improvedgals-bonds-doublesqrt-floatonly

test-improvedgals-bonds-doublesqrt-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-bonds-doublesqrt-floatonly-reducedlocalcalcs-singleforceloop: test-improvedgals-bonds-doublesqrt-floatonly-reducedlocalcalcs

test-improvedgals-bonds-doublesqrt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-bonds-doublesqrt-floatonly-singleforceloop: test-improvedgals-bonds-doublesqrt-floatonly

test-improvedgals-bonds-doublesqrt-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-improvedgals-bonds-doublesqrt-reducedlocalcalcs: test-improvedgals-bonds-doublesqrt

test-improvedgals-bonds-doublesqrt-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-bonds-doublesqrt-reducedlocalcalcs-singleforceloop: test-improvedgals-bonds-doublesqrt-reducedlocalcalcs

test-improvedgals-bonds-doublesqrt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-bonds-doublesqrt-singleforceloop: test-improvedgals-bonds-doublesqrt

test-improvedgals-bonds-msgmgmt: DFLAGS+=-DMESSAGE_MANAGEMENT
test-improvedgals-bonds-msgmgmt: test-improvedgals-bonds

test-improvedgals-bonds-msgmgmt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-improvedgals-bonds-msgmgmt-floatonly: test-improvedgals-bonds-msgmgmt

test-improvedgals-bonds-msgmgmt-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-improvedgals-bonds-msgmgmt-floatonly-reducedlocalcalcs: test-improvedgals-bonds-msgmgmt-floatonly

test-improvedgals-bonds-msgmgmt-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-bonds-msgmgmt-floatonly-reducedlocalcalcs-singleforceloop: test-improvedgals-bonds-msgmgmt-floatonly-reducedlocalcalcs

test-improvedgals-bonds-msgmgmt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-bonds-msgmgmt-floatonly-singleforceloop: test-improvedgals-bonds-msgmgmt-floatonly

test-improvedgals-bonds-msgmgmt-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-improvedgals-bonds-msgmgmt-reducedlocalcalcs: test-improvedgals-bonds-msgmgmt

test-improvedgals-bonds-msgmgmt-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-bonds-msgmgmt-reducedlocalcalcs-singleforceloop: test-improvedgals-bonds-msgmgmt-reducedlocalcalcs

test-improvedgals-bonds-msgmgmt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-bonds-msgmgmt-singleforceloop: test-improvedgals-bonds-msgmgmt

test-improvedgals-bonds-floatonly: DFLAGS+=-DFLOAT_ONLY
test-improvedgals-bonds-floatonly: test-improvedgals-bonds

test-improvedgals-bonds-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-improvedgals-bonds-floatonly-reducedlocalcalcs: test-improvedgals-bonds-floatonly

test-improvedgals-bonds-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-bonds-floatonly-reducedlocalcalcs-singleforceloop: test-improvedgals-bonds-floatonly-reducedlocalcalcs

test-improvedgals-bonds-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-bonds-floatonly-singleforceloop: test-improvedgals-bonds-floatonly

test-improvedgals-bonds-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-improvedgals-bonds-reducedlocalcalcs: test-improvedgals-bonds

test-improvedgals-bonds-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-bonds-reducedlocalcalcs-singleforceloop: test-improvedgals-bonds-reducedlocalcalcs

test-improvedgals-bonds-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-bonds-singleforceloop: test-improvedgals-bonds

# **TEST Local calculation method: onebyone**
test-improvedgals-bonds-onebyone: DFLAGS+=-DONE_BY_ONE
test-improvedgals-bonds-onebyone: test-improvedgals-bonds

test-improvedgals-bonds-onebyone-betterverlet: DFLAGS+=-DBETTER_VERLET
test-improvedgals-bonds-onebyone-betterverlet: test-improvedgals-bonds-onebyone

test-improvedgals-bonds-onebyone-betterverlet-dtchange: DFLAGS+=-DSMALL_DT_EARLY
test-improvedgals-bonds-onebyone-betterverlet-dtchange: test-improvedgals-bonds-onebyone-betterverlet

test-improvedgals-bonds-onebyone-betterverlet-dtchange-doublesqrt: DFLAGS+=-DOUBLE_SQRT
test-improvedgals-bonds-onebyone-betterverlet-dtchange-doublesqrt: test-improvedgals-bonds-onebyone-betterverlet-dtchange

test-improvedgals-bonds-onebyone-betterverlet-dtchange-doublesqrt-msgmgmt: DFLAGS+=-DMESSAGE_MANAGEMENT
test-improvedgals-bonds-onebyone-betterverlet-dtchange-doublesqrt-msgmgmt: test-improvedgals-bonds-onebyone-betterverlet-dtchange-doublesqrt

test-improvedgals-bonds-onebyone-betterverlet-dtchange-doublesqrt-msgmgmt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-improvedgals-bonds-onebyone-betterverlet-dtchange-doublesqrt-msgmgmt-floatonly: test-improvedgals-bonds-onebyone-betterverlet-dtchange-doublesqrt-msgmgmt

test-improvedgals-bonds-onebyone-betterverlet-dtchange-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-improvedgals-bonds-onebyone-betterverlet-dtchange-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs: test-improvedgals-bonds-onebyone-betterverlet-dtchange-doublesqrt-msgmgmt-floatonly

test-improvedgals-bonds-onebyone-betterverlet-dtchange-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-bonds-onebyone-betterverlet-dtchange-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs-singleforceloop: test-improvedgals-bonds-onebyone-betterverlet-dtchange-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs

test-improvedgals-bonds-onebyone-betterverlet-dtchange-doublesqrt-msgmgmt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-bonds-onebyone-betterverlet-dtchange-doublesqrt-msgmgmt-floatonly-singleforceloop: test-improvedgals-bonds-onebyone-betterverlet-dtchange-doublesqrt-msgmgmt-floatonly

test-improvedgals-bonds-onebyone-betterverlet-dtchange-doublesqrt-msgmgmt-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-improvedgals-bonds-onebyone-betterverlet-dtchange-doublesqrt-msgmgmt-reducedlocalcalcs: test-improvedgals-bonds-onebyone-betterverlet-dtchange-doublesqrt-msgmgmt

test-improvedgals-bonds-onebyone-betterverlet-dtchange-doublesqrt-msgmgmt-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-bonds-onebyone-betterverlet-dtchange-doublesqrt-msgmgmt-reducedlocalcalcs-singleforceloop: test-improvedgals-bonds-onebyone-betterverlet-dtchange-doublesqrt-msgmgmt-reducedlocalcalcs

test-improvedgals-bonds-onebyone-betterverlet-dtchange-doublesqrt-msgmgmt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-bonds-onebyone-betterverlet-dtchange-doublesqrt-msgmgmt-singleforceloop: test-improvedgals-bonds-onebyone-betterverlet-dtchange-doublesqrt-msgmgmt

test-improvedgals-bonds-onebyone-betterverlet-dtchange-doublesqrt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-improvedgals-bonds-onebyone-betterverlet-dtchange-doublesqrt-floatonly: test-improvedgals-bonds-onebyone-betterverlet-dtchange-doublesqrt

test-improvedgals-bonds-onebyone-betterverlet-dtchange-doublesqrt-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-improvedgals-bonds-onebyone-betterverlet-dtchange-doublesqrt-floatonly-reducedlocalcalcs: test-improvedgals-bonds-onebyone-betterverlet-dtchange-doublesqrt-floatonly

test-improvedgals-bonds-onebyone-betterverlet-dtchange-doublesqrt-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-bonds-onebyone-betterverlet-dtchange-doublesqrt-floatonly-reducedlocalcalcs-singleforceloop: test-improvedgals-bonds-onebyone-betterverlet-dtchange-doublesqrt-floatonly-reducedlocalcalcs

test-improvedgals-bonds-onebyone-betterverlet-dtchange-doublesqrt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-bonds-onebyone-betterverlet-dtchange-doublesqrt-floatonly-singleforceloop: test-improvedgals-bonds-onebyone-betterverlet-dtchange-doublesqrt-floatonly

test-improvedgals-bonds-onebyone-betterverlet-dtchange-doublesqrt-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-improvedgals-bonds-onebyone-betterverlet-dtchange-doublesqrt-reducedlocalcalcs: test-improvedgals-bonds-onebyone-betterverlet-dtchange-doublesqrt

test-improvedgals-bonds-onebyone-betterverlet-dtchange-doublesqrt-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-bonds-onebyone-betterverlet-dtchange-doublesqrt-reducedlocalcalcs-singleforceloop: test-improvedgals-bonds-onebyone-betterverlet-dtchange-doublesqrt-reducedlocalcalcs

test-improvedgals-bonds-onebyone-betterverlet-dtchange-doublesqrt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-bonds-onebyone-betterverlet-dtchange-doublesqrt-singleforceloop: test-improvedgals-bonds-onebyone-betterverlet-dtchange-doublesqrt

test-improvedgals-bonds-onebyone-betterverlet-dtchange-msgmgmt: DFLAGS+=-DMESSAGE_MANAGEMENT
test-improvedgals-bonds-onebyone-betterverlet-dtchange-msgmgmt: test-improvedgals-bonds-onebyone-betterverlet-dtchange

test-improvedgals-bonds-onebyone-betterverlet-dtchange-msgmgmt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-improvedgals-bonds-onebyone-betterverlet-dtchange-msgmgmt-floatonly: test-improvedgals-bonds-onebyone-betterverlet-dtchange-msgmgmt

test-improvedgals-bonds-onebyone-betterverlet-dtchange-msgmgmt-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-improvedgals-bonds-onebyone-betterverlet-dtchange-msgmgmt-floatonly-reducedlocalcalcs: test-improvedgals-bonds-onebyone-betterverlet-dtchange-msgmgmt-floatonly

test-improvedgals-bonds-onebyone-betterverlet-dtchange-msgmgmt-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-bonds-onebyone-betterverlet-dtchange-msgmgmt-floatonly-reducedlocalcalcs-singleforceloop: test-improvedgals-bonds-onebyone-betterverlet-dtchange-msgmgmt-floatonly-reducedlocalcalcs

test-improvedgals-bonds-onebyone-betterverlet-dtchange-msgmgmt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-bonds-onebyone-betterverlet-dtchange-msgmgmt-floatonly-singleforceloop: test-improvedgals-bonds-onebyone-betterverlet-dtchange-msgmgmt-floatonly

test-improvedgals-bonds-onebyone-betterverlet-dtchange-msgmgmt-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-improvedgals-bonds-onebyone-betterverlet-dtchange-msgmgmt-reducedlocalcalcs: test-improvedgals-bonds-onebyone-betterverlet-dtchange-msgmgmt

test-improvedgals-bonds-onebyone-betterverlet-dtchange-msgmgmt-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-bonds-onebyone-betterverlet-dtchange-msgmgmt-reducedlocalcalcs-singleforceloop: test-improvedgals-bonds-onebyone-betterverlet-dtchange-msgmgmt-reducedlocalcalcs

test-improvedgals-bonds-onebyone-betterverlet-dtchange-msgmgmt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-bonds-onebyone-betterverlet-dtchange-msgmgmt-singleforceloop: test-improvedgals-bonds-onebyone-betterverlet-dtchange-msgmgmt

test-improvedgals-bonds-onebyone-betterverlet-dtchange-floatonly: DFLAGS+=-DFLOAT_ONLY
test-improvedgals-bonds-onebyone-betterverlet-dtchange-floatonly: test-improvedgals-bonds-onebyone-betterverlet-dtchange

test-improvedgals-bonds-onebyone-betterverlet-dtchange-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-improvedgals-bonds-onebyone-betterverlet-dtchange-floatonly-reducedlocalcalcs: test-improvedgals-bonds-onebyone-betterverlet-dtchange-floatonly

test-improvedgals-bonds-onebyone-betterverlet-dtchange-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-bonds-onebyone-betterverlet-dtchange-floatonly-reducedlocalcalcs-singleforceloop: test-improvedgals-bonds-onebyone-betterverlet-dtchange-floatonly-reducedlocalcalcs

test-improvedgals-bonds-onebyone-betterverlet-dtchange-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-bonds-onebyone-betterverlet-dtchange-floatonly-singleforceloop: test-improvedgals-bonds-onebyone-betterverlet-dtchange-floatonly

test-improvedgals-bonds-onebyone-betterverlet-dtchange-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-improvedgals-bonds-onebyone-betterverlet-dtchange-reducedlocalcalcs: test-improvedgals-bonds-onebyone-betterverlet-dtchange

test-improvedgals-bonds-onebyone-betterverlet-dtchange-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-bonds-onebyone-betterverlet-dtchange-reducedlocalcalcs-singleforceloop: test-improvedgals-bonds-onebyone-betterverlet-dtchange-reducedlocalcalcs

test-improvedgals-bonds-onebyone-betterverlet-dtchange-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-bonds-onebyone-betterverlet-dtchange-singleforceloop: test-improvedgals-bonds-onebyone-betterverlet-dtchange

test-improvedgals-bonds-onebyone-betterverlet-doublesqrt: DFLAGS+=-DOUBLE_SQRT
test-improvedgals-bonds-onebyone-betterverlet-doublesqrt: test-improvedgals-bonds-onebyone-betterverlet

test-improvedgals-bonds-onebyone-betterverlet-doublesqrt-msgmgmt: DFLAGS+=-DMESSAGE_MANAGEMENT
test-improvedgals-bonds-onebyone-betterverlet-doublesqrt-msgmgmt: test-improvedgals-bonds-onebyone-betterverlet-doublesqrt

test-improvedgals-bonds-onebyone-betterverlet-doublesqrt-msgmgmt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-improvedgals-bonds-onebyone-betterverlet-doublesqrt-msgmgmt-floatonly: test-improvedgals-bonds-onebyone-betterverlet-doublesqrt-msgmgmt

test-improvedgals-bonds-onebyone-betterverlet-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-improvedgals-bonds-onebyone-betterverlet-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs: test-improvedgals-bonds-onebyone-betterverlet-doublesqrt-msgmgmt-floatonly

test-improvedgals-bonds-onebyone-betterverlet-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-bonds-onebyone-betterverlet-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs-singleforceloop: test-improvedgals-bonds-onebyone-betterverlet-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs

test-improvedgals-bonds-onebyone-betterverlet-doublesqrt-msgmgmt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-bonds-onebyone-betterverlet-doublesqrt-msgmgmt-floatonly-singleforceloop: test-improvedgals-bonds-onebyone-betterverlet-doublesqrt-msgmgmt-floatonly

test-improvedgals-bonds-onebyone-betterverlet-doublesqrt-msgmgmt-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-improvedgals-bonds-onebyone-betterverlet-doublesqrt-msgmgmt-reducedlocalcalcs: test-improvedgals-bonds-onebyone-betterverlet-doublesqrt-msgmgmt

test-improvedgals-bonds-onebyone-betterverlet-doublesqrt-msgmgmt-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-bonds-onebyone-betterverlet-doublesqrt-msgmgmt-reducedlocalcalcs-singleforceloop: test-improvedgals-bonds-onebyone-betterverlet-doublesqrt-msgmgmt-reducedlocalcalcs

test-improvedgals-bonds-onebyone-betterverlet-doublesqrt-msgmgmt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-bonds-onebyone-betterverlet-doublesqrt-msgmgmt-singleforceloop: test-improvedgals-bonds-onebyone-betterverlet-doublesqrt-msgmgmt

test-improvedgals-bonds-onebyone-betterverlet-doublesqrt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-improvedgals-bonds-onebyone-betterverlet-doublesqrt-floatonly: test-improvedgals-bonds-onebyone-betterverlet-doublesqrt

test-improvedgals-bonds-onebyone-betterverlet-doublesqrt-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-improvedgals-bonds-onebyone-betterverlet-doublesqrt-floatonly-reducedlocalcalcs: test-improvedgals-bonds-onebyone-betterverlet-doublesqrt-floatonly

test-improvedgals-bonds-onebyone-betterverlet-doublesqrt-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-bonds-onebyone-betterverlet-doublesqrt-floatonly-reducedlocalcalcs-singleforceloop: test-improvedgals-bonds-onebyone-betterverlet-doublesqrt-floatonly-reducedlocalcalcs

test-improvedgals-bonds-onebyone-betterverlet-doublesqrt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-bonds-onebyone-betterverlet-doublesqrt-floatonly-singleforceloop: test-improvedgals-bonds-onebyone-betterverlet-doublesqrt-floatonly

test-improvedgals-bonds-onebyone-betterverlet-doublesqrt-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-improvedgals-bonds-onebyone-betterverlet-doublesqrt-reducedlocalcalcs: test-improvedgals-bonds-onebyone-betterverlet-doublesqrt

test-improvedgals-bonds-onebyone-betterverlet-doublesqrt-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-bonds-onebyone-betterverlet-doublesqrt-reducedlocalcalcs-singleforceloop: test-improvedgals-bonds-onebyone-betterverlet-doublesqrt-reducedlocalcalcs

test-improvedgals-bonds-onebyone-betterverlet-doublesqrt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-bonds-onebyone-betterverlet-doublesqrt-singleforceloop: test-improvedgals-bonds-onebyone-betterverlet-doublesqrt

test-improvedgals-bonds-onebyone-betterverlet-msgmgmt: DFLAGS+=-DMESSAGE_MANAGEMENT
test-improvedgals-bonds-onebyone-betterverlet-msgmgmt: test-improvedgals-bonds-onebyone-betterverlet

test-improvedgals-bonds-onebyone-betterverlet-msgmgmt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-improvedgals-bonds-onebyone-betterverlet-msgmgmt-floatonly: test-improvedgals-bonds-onebyone-betterverlet-msgmgmt

test-improvedgals-bonds-onebyone-betterverlet-msgmgmt-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-improvedgals-bonds-onebyone-betterverlet-msgmgmt-floatonly-reducedlocalcalcs: test-improvedgals-bonds-onebyone-betterverlet-msgmgmt-floatonly

test-improvedgals-bonds-onebyone-betterverlet-msgmgmt-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-bonds-onebyone-betterverlet-msgmgmt-floatonly-reducedlocalcalcs-singleforceloop: test-improvedgals-bonds-onebyone-betterverlet-msgmgmt-floatonly-reducedlocalcalcs

test-improvedgals-bonds-onebyone-betterverlet-msgmgmt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-bonds-onebyone-betterverlet-msgmgmt-floatonly-singleforceloop: test-improvedgals-bonds-onebyone-betterverlet-msgmgmt-floatonly

test-improvedgals-bonds-onebyone-betterverlet-msgmgmt-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-improvedgals-bonds-onebyone-betterverlet-msgmgmt-reducedlocalcalcs: test-improvedgals-bonds-onebyone-betterverlet-msgmgmt

test-improvedgals-bonds-onebyone-betterverlet-msgmgmt-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-bonds-onebyone-betterverlet-msgmgmt-reducedlocalcalcs-singleforceloop: test-improvedgals-bonds-onebyone-betterverlet-msgmgmt-reducedlocalcalcs

test-improvedgals-bonds-onebyone-betterverlet-msgmgmt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-bonds-onebyone-betterverlet-msgmgmt-singleforceloop: test-improvedgals-bonds-onebyone-betterverlet-msgmgmt

test-improvedgals-bonds-onebyone-betterverlet-floatonly: DFLAGS+=-DFLOAT_ONLY
test-improvedgals-bonds-onebyone-betterverlet-floatonly: test-improvedgals-bonds-onebyone-betterverlet

test-improvedgals-bonds-onebyone-betterverlet-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-improvedgals-bonds-onebyone-betterverlet-floatonly-reducedlocalcalcs: test-improvedgals-bonds-onebyone-betterverlet-floatonly

test-improvedgals-bonds-onebyone-betterverlet-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-bonds-onebyone-betterverlet-floatonly-reducedlocalcalcs-singleforceloop: test-improvedgals-bonds-onebyone-betterverlet-floatonly-reducedlocalcalcs

test-improvedgals-bonds-onebyone-betterverlet-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-bonds-onebyone-betterverlet-floatonly-singleforceloop: test-improvedgals-bonds-onebyone-betterverlet-floatonly

test-improvedgals-bonds-onebyone-betterverlet-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-improvedgals-bonds-onebyone-betterverlet-reducedlocalcalcs: test-improvedgals-bonds-onebyone-betterverlet

test-improvedgals-bonds-onebyone-betterverlet-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-bonds-onebyone-betterverlet-reducedlocalcalcs-singleforceloop: test-improvedgals-bonds-onebyone-betterverlet-reducedlocalcalcs

test-improvedgals-bonds-onebyone-betterverlet-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-bonds-onebyone-betterverlet-singleforceloop: test-improvedgals-bonds-onebyone-betterverlet

test-improvedgals-bonds-onebyone-dtchange: DFLAGS+=-DSMALL_DT_EARLY
test-improvedgals-bonds-onebyone-dtchange: test-improvedgals-bonds-onebyone

test-improvedgals-bonds-onebyone-dtchange-doublesqrt: DFLAGS+=-DOUBLE_SQRT
test-improvedgals-bonds-onebyone-dtchange-doublesqrt: test-improvedgals-bonds-onebyone-dtchange

test-improvedgals-bonds-onebyone-dtchange-doublesqrt-msgmgmt: DFLAGS+=-DMESSAGE_MANAGEMENT
test-improvedgals-bonds-onebyone-dtchange-doublesqrt-msgmgmt: test-improvedgals-bonds-onebyone-dtchange-doublesqrt

test-improvedgals-bonds-onebyone-dtchange-doublesqrt-msgmgmt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-improvedgals-bonds-onebyone-dtchange-doublesqrt-msgmgmt-floatonly: test-improvedgals-bonds-onebyone-dtchange-doublesqrt-msgmgmt

test-improvedgals-bonds-onebyone-dtchange-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-improvedgals-bonds-onebyone-dtchange-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs: test-improvedgals-bonds-onebyone-dtchange-doublesqrt-msgmgmt-floatonly

test-improvedgals-bonds-onebyone-dtchange-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-bonds-onebyone-dtchange-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs-singleforceloop: test-improvedgals-bonds-onebyone-dtchange-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs

test-improvedgals-bonds-onebyone-dtchange-doublesqrt-msgmgmt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-bonds-onebyone-dtchange-doublesqrt-msgmgmt-floatonly-singleforceloop: test-improvedgals-bonds-onebyone-dtchange-doublesqrt-msgmgmt-floatonly

test-improvedgals-bonds-onebyone-dtchange-doublesqrt-msgmgmt-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-improvedgals-bonds-onebyone-dtchange-doublesqrt-msgmgmt-reducedlocalcalcs: test-improvedgals-bonds-onebyone-dtchange-doublesqrt-msgmgmt

test-improvedgals-bonds-onebyone-dtchange-doublesqrt-msgmgmt-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-bonds-onebyone-dtchange-doublesqrt-msgmgmt-reducedlocalcalcs-singleforceloop: test-improvedgals-bonds-onebyone-dtchange-doublesqrt-msgmgmt-reducedlocalcalcs

test-improvedgals-bonds-onebyone-dtchange-doublesqrt-msgmgmt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-bonds-onebyone-dtchange-doublesqrt-msgmgmt-singleforceloop: test-improvedgals-bonds-onebyone-dtchange-doublesqrt-msgmgmt

test-improvedgals-bonds-onebyone-dtchange-doublesqrt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-improvedgals-bonds-onebyone-dtchange-doublesqrt-floatonly: test-improvedgals-bonds-onebyone-dtchange-doublesqrt

test-improvedgals-bonds-onebyone-dtchange-doublesqrt-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-improvedgals-bonds-onebyone-dtchange-doublesqrt-floatonly-reducedlocalcalcs: test-improvedgals-bonds-onebyone-dtchange-doublesqrt-floatonly

test-improvedgals-bonds-onebyone-dtchange-doublesqrt-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-bonds-onebyone-dtchange-doublesqrt-floatonly-reducedlocalcalcs-singleforceloop: test-improvedgals-bonds-onebyone-dtchange-doublesqrt-floatonly-reducedlocalcalcs

test-improvedgals-bonds-onebyone-dtchange-doublesqrt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-bonds-onebyone-dtchange-doublesqrt-floatonly-singleforceloop: test-improvedgals-bonds-onebyone-dtchange-doublesqrt-floatonly

test-improvedgals-bonds-onebyone-dtchange-doublesqrt-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-improvedgals-bonds-onebyone-dtchange-doublesqrt-reducedlocalcalcs: test-improvedgals-bonds-onebyone-dtchange-doublesqrt

test-improvedgals-bonds-onebyone-dtchange-doublesqrt-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-bonds-onebyone-dtchange-doublesqrt-reducedlocalcalcs-singleforceloop: test-improvedgals-bonds-onebyone-dtchange-doublesqrt-reducedlocalcalcs

test-improvedgals-bonds-onebyone-dtchange-doublesqrt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-bonds-onebyone-dtchange-doublesqrt-singleforceloop: test-improvedgals-bonds-onebyone-dtchange-doublesqrt

test-improvedgals-bonds-onebyone-dtchange-msgmgmt: DFLAGS+=-DMESSAGE_MANAGEMENT
test-improvedgals-bonds-onebyone-dtchange-msgmgmt: test-improvedgals-bonds-onebyone-dtchange

test-improvedgals-bonds-onebyone-dtchange-msgmgmt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-improvedgals-bonds-onebyone-dtchange-msgmgmt-floatonly: test-improvedgals-bonds-onebyone-dtchange-msgmgmt

test-improvedgals-bonds-onebyone-dtchange-msgmgmt-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-improvedgals-bonds-onebyone-dtchange-msgmgmt-floatonly-reducedlocalcalcs: test-improvedgals-bonds-onebyone-dtchange-msgmgmt-floatonly

test-improvedgals-bonds-onebyone-dtchange-msgmgmt-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-bonds-onebyone-dtchange-msgmgmt-floatonly-reducedlocalcalcs-singleforceloop: test-improvedgals-bonds-onebyone-dtchange-msgmgmt-floatonly-reducedlocalcalcs

test-improvedgals-bonds-onebyone-dtchange-msgmgmt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-bonds-onebyone-dtchange-msgmgmt-floatonly-singleforceloop: test-improvedgals-bonds-onebyone-dtchange-msgmgmt-floatonly

test-improvedgals-bonds-onebyone-dtchange-msgmgmt-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-improvedgals-bonds-onebyone-dtchange-msgmgmt-reducedlocalcalcs: test-improvedgals-bonds-onebyone-dtchange-msgmgmt

test-improvedgals-bonds-onebyone-dtchange-msgmgmt-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-bonds-onebyone-dtchange-msgmgmt-reducedlocalcalcs-singleforceloop: test-improvedgals-bonds-onebyone-dtchange-msgmgmt-reducedlocalcalcs

test-improvedgals-bonds-onebyone-dtchange-msgmgmt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-bonds-onebyone-dtchange-msgmgmt-singleforceloop: test-improvedgals-bonds-onebyone-dtchange-msgmgmt

test-improvedgals-bonds-onebyone-dtchange-floatonly: DFLAGS+=-DFLOAT_ONLY
test-improvedgals-bonds-onebyone-dtchange-floatonly: test-improvedgals-bonds-onebyone-dtchange

test-improvedgals-bonds-onebyone-dtchange-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-improvedgals-bonds-onebyone-dtchange-floatonly-reducedlocalcalcs: test-improvedgals-bonds-onebyone-dtchange-floatonly

test-improvedgals-bonds-onebyone-dtchange-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-bonds-onebyone-dtchange-floatonly-reducedlocalcalcs-singleforceloop: test-improvedgals-bonds-onebyone-dtchange-floatonly-reducedlocalcalcs

test-improvedgals-bonds-onebyone-dtchange-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-bonds-onebyone-dtchange-floatonly-singleforceloop: test-improvedgals-bonds-onebyone-dtchange-floatonly

test-improvedgals-bonds-onebyone-dtchange-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-improvedgals-bonds-onebyone-dtchange-reducedlocalcalcs: test-improvedgals-bonds-onebyone-dtchange

test-improvedgals-bonds-onebyone-dtchange-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-bonds-onebyone-dtchange-reducedlocalcalcs-singleforceloop: test-improvedgals-bonds-onebyone-dtchange-reducedlocalcalcs

test-improvedgals-bonds-onebyone-dtchange-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-bonds-onebyone-dtchange-singleforceloop: test-improvedgals-bonds-onebyone-dtchange

test-improvedgals-bonds-onebyone-doublesqrt: DFLAGS+=-DOUBLE_SQRT
test-improvedgals-bonds-onebyone-doublesqrt: test-improvedgals-bonds-onebyone

test-improvedgals-bonds-onebyone-doublesqrt-msgmgmt: DFLAGS+=-DMESSAGE_MANAGEMENT
test-improvedgals-bonds-onebyone-doublesqrt-msgmgmt: test-improvedgals-bonds-onebyone-doublesqrt

test-improvedgals-bonds-onebyone-doublesqrt-msgmgmt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-improvedgals-bonds-onebyone-doublesqrt-msgmgmt-floatonly: test-improvedgals-bonds-onebyone-doublesqrt-msgmgmt

test-improvedgals-bonds-onebyone-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-improvedgals-bonds-onebyone-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs: test-improvedgals-bonds-onebyone-doublesqrt-msgmgmt-floatonly

test-improvedgals-bonds-onebyone-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-bonds-onebyone-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs-singleforceloop: test-improvedgals-bonds-onebyone-doublesqrt-msgmgmt-floatonly-reducedlocalcalcs

test-improvedgals-bonds-onebyone-doublesqrt-msgmgmt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-bonds-onebyone-doublesqrt-msgmgmt-floatonly-singleforceloop: test-improvedgals-bonds-onebyone-doublesqrt-msgmgmt-floatonly

test-improvedgals-bonds-onebyone-doublesqrt-msgmgmt-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-improvedgals-bonds-onebyone-doublesqrt-msgmgmt-reducedlocalcalcs: test-improvedgals-bonds-onebyone-doublesqrt-msgmgmt

test-improvedgals-bonds-onebyone-doublesqrt-msgmgmt-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-bonds-onebyone-doublesqrt-msgmgmt-reducedlocalcalcs-singleforceloop: test-improvedgals-bonds-onebyone-doublesqrt-msgmgmt-reducedlocalcalcs

test-improvedgals-bonds-onebyone-doublesqrt-msgmgmt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-bonds-onebyone-doublesqrt-msgmgmt-singleforceloop: test-improvedgals-bonds-onebyone-doublesqrt-msgmgmt

test-improvedgals-bonds-onebyone-doublesqrt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-improvedgals-bonds-onebyone-doublesqrt-floatonly: test-improvedgals-bonds-onebyone-doublesqrt

test-improvedgals-bonds-onebyone-doublesqrt-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-improvedgals-bonds-onebyone-doublesqrt-floatonly-reducedlocalcalcs: test-improvedgals-bonds-onebyone-doublesqrt-floatonly

test-improvedgals-bonds-onebyone-doublesqrt-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-bonds-onebyone-doublesqrt-floatonly-reducedlocalcalcs-singleforceloop: test-improvedgals-bonds-onebyone-doublesqrt-floatonly-reducedlocalcalcs

test-improvedgals-bonds-onebyone-doublesqrt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-bonds-onebyone-doublesqrt-floatonly-singleforceloop: test-improvedgals-bonds-onebyone-doublesqrt-floatonly

test-improvedgals-bonds-onebyone-doublesqrt-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-improvedgals-bonds-onebyone-doublesqrt-reducedlocalcalcs: test-improvedgals-bonds-onebyone-doublesqrt

test-improvedgals-bonds-onebyone-doublesqrt-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-bonds-onebyone-doublesqrt-reducedlocalcalcs-singleforceloop: test-improvedgals-bonds-onebyone-doublesqrt-reducedlocalcalcs

test-improvedgals-bonds-onebyone-doublesqrt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-bonds-onebyone-doublesqrt-singleforceloop: test-improvedgals-bonds-onebyone-doublesqrt

test-improvedgals-bonds-onebyone-msgmgmt: DFLAGS+=-DMESSAGE_MANAGEMENT
test-improvedgals-bonds-onebyone-msgmgmt: test-improvedgals-bonds-onebyone

test-improvedgals-bonds-onebyone-msgmgmt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-improvedgals-bonds-onebyone-msgmgmt-floatonly: test-improvedgals-bonds-onebyone-msgmgmt

test-improvedgals-bonds-onebyone-msgmgmt-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-improvedgals-bonds-onebyone-msgmgmt-floatonly-reducedlocalcalcs: test-improvedgals-bonds-onebyone-msgmgmt-floatonly

test-improvedgals-bonds-onebyone-msgmgmt-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-bonds-onebyone-msgmgmt-floatonly-reducedlocalcalcs-singleforceloop: test-improvedgals-bonds-onebyone-msgmgmt-floatonly-reducedlocalcalcs

test-improvedgals-bonds-onebyone-msgmgmt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-bonds-onebyone-msgmgmt-floatonly-singleforceloop: test-improvedgals-bonds-onebyone-msgmgmt-floatonly

test-improvedgals-bonds-onebyone-msgmgmt-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-improvedgals-bonds-onebyone-msgmgmt-reducedlocalcalcs: test-improvedgals-bonds-onebyone-msgmgmt

test-improvedgals-bonds-onebyone-msgmgmt-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-bonds-onebyone-msgmgmt-reducedlocalcalcs-singleforceloop: test-improvedgals-bonds-onebyone-msgmgmt-reducedlocalcalcs

test-improvedgals-bonds-onebyone-msgmgmt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-bonds-onebyone-msgmgmt-singleforceloop: test-improvedgals-bonds-onebyone-msgmgmt

test-improvedgals-bonds-onebyone-floatonly: DFLAGS+=-DFLOAT_ONLY
test-improvedgals-bonds-onebyone-floatonly: test-improvedgals-bonds-onebyone

test-improvedgals-bonds-onebyone-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-improvedgals-bonds-onebyone-floatonly-reducedlocalcalcs: test-improvedgals-bonds-onebyone-floatonly

test-improvedgals-bonds-onebyone-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-bonds-onebyone-floatonly-reducedlocalcalcs-singleforceloop: test-improvedgals-bonds-onebyone-floatonly-reducedlocalcalcs

test-improvedgals-bonds-onebyone-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-bonds-onebyone-floatonly-singleforceloop: test-improvedgals-bonds-onebyone-floatonly

test-improvedgals-bonds-onebyone-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-improvedgals-bonds-onebyone-reducedlocalcalcs: test-improvedgals-bonds-onebyone

test-improvedgals-bonds-onebyone-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-bonds-onebyone-reducedlocalcalcs-singleforceloop: test-improvedgals-bonds-onebyone-reducedlocalcalcs

test-improvedgals-bonds-onebyone-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-bonds-onebyone-singleforceloop: test-improvedgals-bonds-onebyone


# ************** TEST Simulator: serial**************

test-serial: OBJS=$(SERIAL_OBJS)
test-serial: DFLAGS+=-DTESTING -DSERIAL
test-serial: serial test

test-serial-betterverlet: DFLAGS+=-DBETTER_VERLET
test-serial-betterverlet: test-serial

test-serial-betterverlet-dtchange: DFLAGS+=-DSMALL_DT_EARLY
test-serial-betterverlet-dtchange: test-serial-betterverlet

test-serial-betterverlet-dtchange-doublesqrt: DFLAGS+=-DOUBLE_SQRT
test-serial-betterverlet-dtchange-doublesqrt: test-serial-betterverlet-dtchange

test-serial-betterverlet-dtchange-doublesqrt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-serial-betterverlet-dtchange-doublesqrt-floatonly: test-serial-betterverlet-dtchange-doublesqrt

test-serial-betterverlet-dtchange-doublesqrt-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-serial-betterverlet-dtchange-doublesqrt-floatonly-reducedlocalcalcs: test-serial-betterverlet-dtchange-doublesqrt-floatonly

test-serial-betterverlet-dtchange-doublesqrt-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-serial-betterverlet-dtchange-doublesqrt-floatonly-reducedlocalcalcs-singleforceloop: test-serial-betterverlet-dtchange-doublesqrt-floatonly-reducedlocalcalcs

test-serial-betterverlet-dtchange-doublesqrt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-serial-betterverlet-dtchange-doublesqrt-floatonly-singleforceloop: test-serial-betterverlet-dtchange-doublesqrt-floatonly

test-serial-betterverlet-dtchange-doublesqrt-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-serial-betterverlet-dtchange-doublesqrt-reducedlocalcalcs: test-serial-betterverlet-dtchange-doublesqrt

test-serial-betterverlet-dtchange-doublesqrt-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-serial-betterverlet-dtchange-doublesqrt-reducedlocalcalcs-singleforceloop: test-serial-betterverlet-dtchange-doublesqrt-reducedlocalcalcs

test-serial-betterverlet-dtchange-doublesqrt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-serial-betterverlet-dtchange-doublesqrt-singleforceloop: test-serial-betterverlet-dtchange-doublesqrt

test-serial-betterverlet-dtchange-floatonly: DFLAGS+=-DFLOAT_ONLY
test-serial-betterverlet-dtchange-floatonly: test-serial-betterverlet-dtchange

test-serial-betterverlet-dtchange-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-serial-betterverlet-dtchange-floatonly-reducedlocalcalcs: test-serial-betterverlet-dtchange-floatonly

test-serial-betterverlet-dtchange-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-serial-betterverlet-dtchange-floatonly-reducedlocalcalcs-singleforceloop: test-serial-betterverlet-dtchange-floatonly-reducedlocalcalcs

test-serial-betterverlet-dtchange-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-serial-betterverlet-dtchange-floatonly-singleforceloop: test-serial-betterverlet-dtchange-floatonly

test-serial-betterverlet-dtchange-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-serial-betterverlet-dtchange-reducedlocalcalcs: test-serial-betterverlet-dtchange

test-serial-betterverlet-dtchange-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-serial-betterverlet-dtchange-reducedlocalcalcs-singleforceloop: test-serial-betterverlet-dtchange-reducedlocalcalcs

test-serial-betterverlet-dtchange-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-serial-betterverlet-dtchange-singleforceloop: test-serial-betterverlet-dtchange

test-serial-betterverlet-doublesqrt: DFLAGS+=-DOUBLE_SQRT
test-serial-betterverlet-doublesqrt: test-serial-betterverlet

test-serial-betterverlet-doublesqrt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-serial-betterverlet-doublesqrt-floatonly: test-serial-betterverlet-doublesqrt

test-serial-betterverlet-doublesqrt-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-serial-betterverlet-doublesqrt-floatonly-reducedlocalcalcs: test-serial-betterverlet-doublesqrt-floatonly

test-serial-betterverlet-doublesqrt-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-serial-betterverlet-doublesqrt-floatonly-reducedlocalcalcs-singleforceloop: test-serial-betterverlet-doublesqrt-floatonly-reducedlocalcalcs

test-serial-betterverlet-doublesqrt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-serial-betterverlet-doublesqrt-floatonly-singleforceloop: test-serial-betterverlet-doublesqrt-floatonly

test-serial-betterverlet-doublesqrt-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-serial-betterverlet-doublesqrt-reducedlocalcalcs: test-serial-betterverlet-doublesqrt

test-serial-betterverlet-doublesqrt-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-serial-betterverlet-doublesqrt-reducedlocalcalcs-singleforceloop: test-serial-betterverlet-doublesqrt-reducedlocalcalcs

test-serial-betterverlet-doublesqrt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-serial-betterverlet-doublesqrt-singleforceloop: test-serial-betterverlet-doublesqrt

test-serial-betterverlet-floatonly: DFLAGS+=-DFLOAT_ONLY
test-serial-betterverlet-floatonly: test-serial-betterverlet

test-serial-betterverlet-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-serial-betterverlet-floatonly-reducedlocalcalcs: test-serial-betterverlet-floatonly

test-serial-betterverlet-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-serial-betterverlet-floatonly-reducedlocalcalcs-singleforceloop: test-serial-betterverlet-floatonly-reducedlocalcalcs

test-serial-betterverlet-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-serial-betterverlet-floatonly-singleforceloop: test-serial-betterverlet-floatonly

test-serial-betterverlet-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-serial-betterverlet-reducedlocalcalcs: test-serial-betterverlet

test-serial-betterverlet-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-serial-betterverlet-reducedlocalcalcs-singleforceloop: test-serial-betterverlet-reducedlocalcalcs

test-serial-betterverlet-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-serial-betterverlet-singleforceloop: test-serial-betterverlet

test-serial-dtchange: DFLAGS+=-DSMALL_DT_EARLY
test-serial-dtchange: test-serial

test-serial-dtchange-doublesqrt: DFLAGS+=-DOUBLE_SQRT
test-serial-dtchange-doublesqrt: test-serial-dtchange

test-serial-dtchange-doublesqrt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-serial-dtchange-doublesqrt-floatonly: test-serial-dtchange-doublesqrt

test-serial-dtchange-doublesqrt-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-serial-dtchange-doublesqrt-floatonly-reducedlocalcalcs: test-serial-dtchange-doublesqrt-floatonly

test-serial-dtchange-doublesqrt-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-serial-dtchange-doublesqrt-floatonly-reducedlocalcalcs-singleforceloop: test-serial-dtchange-doublesqrt-floatonly-reducedlocalcalcs

test-serial-dtchange-doublesqrt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-serial-dtchange-doublesqrt-floatonly-singleforceloop: test-serial-dtchange-doublesqrt-floatonly

test-serial-dtchange-doublesqrt-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-serial-dtchange-doublesqrt-reducedlocalcalcs: test-serial-dtchange-doublesqrt

test-serial-dtchange-doublesqrt-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-serial-dtchange-doublesqrt-reducedlocalcalcs-singleforceloop: test-serial-dtchange-doublesqrt-reducedlocalcalcs

test-serial-dtchange-doublesqrt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-serial-dtchange-doublesqrt-singleforceloop: test-serial-dtchange-doublesqrt

test-serial-dtchange-floatonly: DFLAGS+=-DFLOAT_ONLY
test-serial-dtchange-floatonly: test-serial-dtchange

test-serial-dtchange-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-serial-dtchange-floatonly-reducedlocalcalcs: test-serial-dtchange-floatonly

test-serial-dtchange-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-serial-dtchange-floatonly-reducedlocalcalcs-singleforceloop: test-serial-dtchange-floatonly-reducedlocalcalcs

test-serial-dtchange-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-serial-dtchange-floatonly-singleforceloop: test-serial-dtchange-floatonly

test-serial-dtchange-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-serial-dtchange-reducedlocalcalcs: test-serial-dtchange

test-serial-dtchange-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-serial-dtchange-reducedlocalcalcs-singleforceloop: test-serial-dtchange-reducedlocalcalcs

test-serial-dtchange-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-serial-dtchange-singleforceloop: test-serial-dtchange

test-serial-doublesqrt: DFLAGS+=-DOUBLE_SQRT
test-serial-doublesqrt: test-serial

test-serial-doublesqrt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-serial-doublesqrt-floatonly: test-serial-doublesqrt

test-serial-doublesqrt-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-serial-doublesqrt-floatonly-reducedlocalcalcs: test-serial-doublesqrt-floatonly

test-serial-doublesqrt-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-serial-doublesqrt-floatonly-reducedlocalcalcs-singleforceloop: test-serial-doublesqrt-floatonly-reducedlocalcalcs

test-serial-doublesqrt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-serial-doublesqrt-floatonly-singleforceloop: test-serial-doublesqrt-floatonly

test-serial-doublesqrt-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-serial-doublesqrt-reducedlocalcalcs: test-serial-doublesqrt

test-serial-doublesqrt-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-serial-doublesqrt-reducedlocalcalcs-singleforceloop: test-serial-doublesqrt-reducedlocalcalcs

test-serial-doublesqrt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-serial-doublesqrt-singleforceloop: test-serial-doublesqrt

test-serial-floatonly: DFLAGS+=-DFLOAT_ONLY
test-serial-floatonly: test-serial

test-serial-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-serial-floatonly-reducedlocalcalcs: test-serial-floatonly

test-serial-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-serial-floatonly-reducedlocalcalcs-singleforceloop: test-serial-floatonly-reducedlocalcalcs

test-serial-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-serial-floatonly-singleforceloop: test-serial-floatonly

test-serial-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-serial-reducedlocalcalcs: test-serial

test-serial-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-serial-reducedlocalcalcs-singleforceloop: test-serial-reducedlocalcalcs

test-serial-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-serial-singleforceloop: test-serial

test-serial-large: DFLAGS+=-DLARGE_TEST
test-serial-large: test-serial

test-serial-large-betterverlet: DFLAGS+=-DBETTER_VERLET
test-serial-large-betterverlet: test-serial-large

test-serial-large-betterverlet-dtchange: DFLAGS+=-DSMALL_DT_EARLY
test-serial-large-betterverlet-dtchange: test-serial-large-betterverlet

test-serial-large-betterverlet-dtchange-doublesqrt: DFLAGS+=-DOUBLE_SQRT
test-serial-large-betterverlet-dtchange-doublesqrt: test-serial-large-betterverlet-dtchange

test-serial-large-betterverlet-dtchange-doublesqrt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-serial-large-betterverlet-dtchange-doublesqrt-floatonly: test-serial-large-betterverlet-dtchange-doublesqrt

test-serial-large-betterverlet-dtchange-doublesqrt-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-serial-large-betterverlet-dtchange-doublesqrt-floatonly-reducedlocalcalcs: test-serial-large-betterverlet-dtchange-doublesqrt-floatonly

test-serial-large-betterverlet-dtchange-doublesqrt-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-serial-large-betterverlet-dtchange-doublesqrt-floatonly-reducedlocalcalcs-singleforceloop: test-serial-large-betterverlet-dtchange-doublesqrt-floatonly-reducedlocalcalcs

test-serial-large-betterverlet-dtchange-doublesqrt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-serial-large-betterverlet-dtchange-doublesqrt-floatonly-singleforceloop: test-serial-large-betterverlet-dtchange-doublesqrt-floatonly

test-serial-large-betterverlet-dtchange-doublesqrt-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-serial-large-betterverlet-dtchange-doublesqrt-reducedlocalcalcs: test-serial-large-betterverlet-dtchange-doublesqrt

test-serial-large-betterverlet-dtchange-doublesqrt-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-serial-large-betterverlet-dtchange-doublesqrt-reducedlocalcalcs-singleforceloop: test-serial-large-betterverlet-dtchange-doublesqrt-reducedlocalcalcs

test-serial-large-betterverlet-dtchange-doublesqrt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-serial-large-betterverlet-dtchange-doublesqrt-singleforceloop: test-serial-large-betterverlet-dtchange-doublesqrt

test-serial-large-betterverlet-dtchange-floatonly: DFLAGS+=-DFLOAT_ONLY
test-serial-large-betterverlet-dtchange-floatonly: test-serial-large-betterverlet-dtchange

test-serial-large-betterverlet-dtchange-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-serial-large-betterverlet-dtchange-floatonly-reducedlocalcalcs: test-serial-large-betterverlet-dtchange-floatonly

test-serial-large-betterverlet-dtchange-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-serial-large-betterverlet-dtchange-floatonly-reducedlocalcalcs-singleforceloop: test-serial-large-betterverlet-dtchange-floatonly-reducedlocalcalcs

test-serial-large-betterverlet-dtchange-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-serial-large-betterverlet-dtchange-floatonly-singleforceloop: test-serial-large-betterverlet-dtchange-floatonly

test-serial-large-betterverlet-dtchange-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-serial-large-betterverlet-dtchange-reducedlocalcalcs: test-serial-large-betterverlet-dtchange

test-serial-large-betterverlet-dtchange-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-serial-large-betterverlet-dtchange-reducedlocalcalcs-singleforceloop: test-serial-large-betterverlet-dtchange-reducedlocalcalcs

test-serial-large-betterverlet-dtchange-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-serial-large-betterverlet-dtchange-singleforceloop: test-serial-large-betterverlet-dtchange

test-serial-large-betterverlet-doublesqrt: DFLAGS+=-DOUBLE_SQRT
test-serial-large-betterverlet-doublesqrt: test-serial-large-betterverlet

test-serial-large-betterverlet-doublesqrt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-serial-large-betterverlet-doublesqrt-floatonly: test-serial-large-betterverlet-doublesqrt

test-serial-large-betterverlet-doublesqrt-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-serial-large-betterverlet-doublesqrt-floatonly-reducedlocalcalcs: test-serial-large-betterverlet-doublesqrt-floatonly

test-serial-large-betterverlet-doublesqrt-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-serial-large-betterverlet-doublesqrt-floatonly-reducedlocalcalcs-singleforceloop: test-serial-large-betterverlet-doublesqrt-floatonly-reducedlocalcalcs

test-serial-large-betterverlet-doublesqrt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-serial-large-betterverlet-doublesqrt-floatonly-singleforceloop: test-serial-large-betterverlet-doublesqrt-floatonly

test-serial-large-betterverlet-doublesqrt-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-serial-large-betterverlet-doublesqrt-reducedlocalcalcs: test-serial-large-betterverlet-doublesqrt

test-serial-large-betterverlet-doublesqrt-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-serial-large-betterverlet-doublesqrt-reducedlocalcalcs-singleforceloop: test-serial-large-betterverlet-doublesqrt-reducedlocalcalcs

test-serial-large-betterverlet-doublesqrt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-serial-large-betterverlet-doublesqrt-singleforceloop: test-serial-large-betterverlet-doublesqrt

test-serial-large-betterverlet-floatonly: DFLAGS+=-DFLOAT_ONLY
test-serial-large-betterverlet-floatonly: test-serial-large-betterverlet

test-serial-large-betterverlet-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-serial-large-betterverlet-floatonly-reducedlocalcalcs: test-serial-large-betterverlet-floatonly

test-serial-large-betterverlet-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-serial-large-betterverlet-floatonly-reducedlocalcalcs-singleforceloop: test-serial-large-betterverlet-floatonly-reducedlocalcalcs

test-serial-large-betterverlet-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-serial-large-betterverlet-floatonly-singleforceloop: test-serial-large-betterverlet-floatonly

test-serial-large-betterverlet-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-serial-large-betterverlet-reducedlocalcalcs: test-serial-large-betterverlet

test-serial-large-betterverlet-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-serial-large-betterverlet-reducedlocalcalcs-singleforceloop: test-serial-large-betterverlet-reducedlocalcalcs

test-serial-large-betterverlet-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-serial-large-betterverlet-singleforceloop: test-serial-large-betterverlet

test-serial-large-dtchange: DFLAGS+=-DSMALL_DT_EARLY
test-serial-large-dtchange: test-serial-large

test-serial-large-dtchange-doublesqrt: DFLAGS+=-DOUBLE_SQRT
test-serial-large-dtchange-doublesqrt: test-serial-large-dtchange

test-serial-large-dtchange-doublesqrt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-serial-large-dtchange-doublesqrt-floatonly: test-serial-large-dtchange-doublesqrt

test-serial-large-dtchange-doublesqrt-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-serial-large-dtchange-doublesqrt-floatonly-reducedlocalcalcs: test-serial-large-dtchange-doublesqrt-floatonly

test-serial-large-dtchange-doublesqrt-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-serial-large-dtchange-doublesqrt-floatonly-reducedlocalcalcs-singleforceloop: test-serial-large-dtchange-doublesqrt-floatonly-reducedlocalcalcs

test-serial-large-dtchange-doublesqrt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-serial-large-dtchange-doublesqrt-floatonly-singleforceloop: test-serial-large-dtchange-doublesqrt-floatonly

test-serial-large-dtchange-doublesqrt-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-serial-large-dtchange-doublesqrt-reducedlocalcalcs: test-serial-large-dtchange-doublesqrt

test-serial-large-dtchange-doublesqrt-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-serial-large-dtchange-doublesqrt-reducedlocalcalcs-singleforceloop: test-serial-large-dtchange-doublesqrt-reducedlocalcalcs

test-serial-large-dtchange-doublesqrt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-serial-large-dtchange-doublesqrt-singleforceloop: test-serial-large-dtchange-doublesqrt

test-serial-large-dtchange-floatonly: DFLAGS+=-DFLOAT_ONLY
test-serial-large-dtchange-floatonly: test-serial-large-dtchange

test-serial-large-dtchange-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-serial-large-dtchange-floatonly-reducedlocalcalcs: test-serial-large-dtchange-floatonly

test-serial-large-dtchange-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-serial-large-dtchange-floatonly-reducedlocalcalcs-singleforceloop: test-serial-large-dtchange-floatonly-reducedlocalcalcs

test-serial-large-dtchange-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-serial-large-dtchange-floatonly-singleforceloop: test-serial-large-dtchange-floatonly

test-serial-large-dtchange-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-serial-large-dtchange-reducedlocalcalcs: test-serial-large-dtchange

test-serial-large-dtchange-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-serial-large-dtchange-reducedlocalcalcs-singleforceloop: test-serial-large-dtchange-reducedlocalcalcs

test-serial-large-dtchange-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-serial-large-dtchange-singleforceloop: test-serial-large-dtchange

test-serial-large-doublesqrt: DFLAGS+=-DOUBLE_SQRT
test-serial-large-doublesqrt: test-serial-large

test-serial-large-doublesqrt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-serial-large-doublesqrt-floatonly: test-serial-large-doublesqrt

test-serial-large-doublesqrt-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-serial-large-doublesqrt-floatonly-reducedlocalcalcs: test-serial-large-doublesqrt-floatonly

test-serial-large-doublesqrt-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-serial-large-doublesqrt-floatonly-reducedlocalcalcs-singleforceloop: test-serial-large-doublesqrt-floatonly-reducedlocalcalcs

test-serial-large-doublesqrt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-serial-large-doublesqrt-floatonly-singleforceloop: test-serial-large-doublesqrt-floatonly

test-serial-large-doublesqrt-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-serial-large-doublesqrt-reducedlocalcalcs: test-serial-large-doublesqrt

test-serial-large-doublesqrt-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-serial-large-doublesqrt-reducedlocalcalcs-singleforceloop: test-serial-large-doublesqrt-reducedlocalcalcs

test-serial-large-doublesqrt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-serial-large-doublesqrt-singleforceloop: test-serial-large-doublesqrt

test-serial-large-floatonly: DFLAGS+=-DFLOAT_ONLY
test-serial-large-floatonly: test-serial-large

test-serial-large-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-serial-large-floatonly-reducedlocalcalcs: test-serial-large-floatonly

test-serial-large-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-serial-large-floatonly-reducedlocalcalcs-singleforceloop: test-serial-large-floatonly-reducedlocalcalcs

test-serial-large-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-serial-large-floatonly-singleforceloop: test-serial-large-floatonly

test-serial-large-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-serial-large-reducedlocalcalcs: test-serial-large

test-serial-large-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-serial-large-reducedlocalcalcs-singleforceloop: test-serial-large-reducedlocalcalcs

test-serial-large-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-serial-large-singleforceloop: test-serial-large

test-serial-bonds: DFLAGS+=-DBONDS
test-serial-bonds: test-serial

test-serial-bonds-betterverlet: DFLAGS+=-DBETTER_VERLET
test-serial-bonds-betterverlet: test-serial-bonds

test-serial-bonds-betterverlet-dtchange: DFLAGS+=-DSMALL_DT_EARLY
test-serial-bonds-betterverlet-dtchange: test-serial-bonds-betterverlet

test-serial-bonds-betterverlet-dtchange-doublesqrt: DFLAGS+=-DOUBLE_SQRT
test-serial-bonds-betterverlet-dtchange-doublesqrt: test-serial-bonds-betterverlet-dtchange

test-serial-bonds-betterverlet-dtchange-doublesqrt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-serial-bonds-betterverlet-dtchange-doublesqrt-floatonly: test-serial-bonds-betterverlet-dtchange-doublesqrt

test-serial-bonds-betterverlet-dtchange-doublesqrt-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-serial-bonds-betterverlet-dtchange-doublesqrt-floatonly-reducedlocalcalcs: test-serial-bonds-betterverlet-dtchange-doublesqrt-floatonly

test-serial-bonds-betterverlet-dtchange-doublesqrt-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-serial-bonds-betterverlet-dtchange-doublesqrt-floatonly-reducedlocalcalcs-singleforceloop: test-serial-bonds-betterverlet-dtchange-doublesqrt-floatonly-reducedlocalcalcs

test-serial-bonds-betterverlet-dtchange-doublesqrt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-serial-bonds-betterverlet-dtchange-doublesqrt-floatonly-singleforceloop: test-serial-bonds-betterverlet-dtchange-doublesqrt-floatonly

test-serial-bonds-betterverlet-dtchange-doublesqrt-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-serial-bonds-betterverlet-dtchange-doublesqrt-reducedlocalcalcs: test-serial-bonds-betterverlet-dtchange-doublesqrt

test-serial-bonds-betterverlet-dtchange-doublesqrt-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-serial-bonds-betterverlet-dtchange-doublesqrt-reducedlocalcalcs-singleforceloop: test-serial-bonds-betterverlet-dtchange-doublesqrt-reducedlocalcalcs

test-serial-bonds-betterverlet-dtchange-doublesqrt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-serial-bonds-betterverlet-dtchange-doublesqrt-singleforceloop: test-serial-bonds-betterverlet-dtchange-doublesqrt

test-serial-bonds-betterverlet-dtchange-floatonly: DFLAGS+=-DFLOAT_ONLY
test-serial-bonds-betterverlet-dtchange-floatonly: test-serial-bonds-betterverlet-dtchange

test-serial-bonds-betterverlet-dtchange-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-serial-bonds-betterverlet-dtchange-floatonly-reducedlocalcalcs: test-serial-bonds-betterverlet-dtchange-floatonly

test-serial-bonds-betterverlet-dtchange-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-serial-bonds-betterverlet-dtchange-floatonly-reducedlocalcalcs-singleforceloop: test-serial-bonds-betterverlet-dtchange-floatonly-reducedlocalcalcs

test-serial-bonds-betterverlet-dtchange-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-serial-bonds-betterverlet-dtchange-floatonly-singleforceloop: test-serial-bonds-betterverlet-dtchange-floatonly

test-serial-bonds-betterverlet-dtchange-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-serial-bonds-betterverlet-dtchange-reducedlocalcalcs: test-serial-bonds-betterverlet-dtchange

test-serial-bonds-betterverlet-dtchange-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-serial-bonds-betterverlet-dtchange-reducedlocalcalcs-singleforceloop: test-serial-bonds-betterverlet-dtchange-reducedlocalcalcs

test-serial-bonds-betterverlet-dtchange-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-serial-bonds-betterverlet-dtchange-singleforceloop: test-serial-bonds-betterverlet-dtchange

test-serial-bonds-betterverlet-doublesqrt: DFLAGS+=-DOUBLE_SQRT
test-serial-bonds-betterverlet-doublesqrt: test-serial-bonds-betterverlet

test-serial-bonds-betterverlet-doublesqrt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-serial-bonds-betterverlet-doublesqrt-floatonly: test-serial-bonds-betterverlet-doublesqrt

test-serial-bonds-betterverlet-doublesqrt-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-serial-bonds-betterverlet-doublesqrt-floatonly-reducedlocalcalcs: test-serial-bonds-betterverlet-doublesqrt-floatonly

test-serial-bonds-betterverlet-doublesqrt-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-serial-bonds-betterverlet-doublesqrt-floatonly-reducedlocalcalcs-singleforceloop: test-serial-bonds-betterverlet-doublesqrt-floatonly-reducedlocalcalcs

test-serial-bonds-betterverlet-doublesqrt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-serial-bonds-betterverlet-doublesqrt-floatonly-singleforceloop: test-serial-bonds-betterverlet-doublesqrt-floatonly

test-serial-bonds-betterverlet-doublesqrt-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-serial-bonds-betterverlet-doublesqrt-reducedlocalcalcs: test-serial-bonds-betterverlet-doublesqrt

test-serial-bonds-betterverlet-doublesqrt-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-serial-bonds-betterverlet-doublesqrt-reducedlocalcalcs-singleforceloop: test-serial-bonds-betterverlet-doublesqrt-reducedlocalcalcs

test-serial-bonds-betterverlet-doublesqrt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-serial-bonds-betterverlet-doublesqrt-singleforceloop: test-serial-bonds-betterverlet-doublesqrt

test-serial-bonds-betterverlet-floatonly: DFLAGS+=-DFLOAT_ONLY
test-serial-bonds-betterverlet-floatonly: test-serial-bonds-betterverlet

test-serial-bonds-betterverlet-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-serial-bonds-betterverlet-floatonly-reducedlocalcalcs: test-serial-bonds-betterverlet-floatonly

test-serial-bonds-betterverlet-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-serial-bonds-betterverlet-floatonly-reducedlocalcalcs-singleforceloop: test-serial-bonds-betterverlet-floatonly-reducedlocalcalcs

test-serial-bonds-betterverlet-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-serial-bonds-betterverlet-floatonly-singleforceloop: test-serial-bonds-betterverlet-floatonly

test-serial-bonds-betterverlet-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-serial-bonds-betterverlet-reducedlocalcalcs: test-serial-bonds-betterverlet

test-serial-bonds-betterverlet-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-serial-bonds-betterverlet-reducedlocalcalcs-singleforceloop: test-serial-bonds-betterverlet-reducedlocalcalcs

test-serial-bonds-betterverlet-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-serial-bonds-betterverlet-singleforceloop: test-serial-bonds-betterverlet

test-serial-bonds-dtchange: DFLAGS+=-DSMALL_DT_EARLY
test-serial-bonds-dtchange: test-serial-bonds

test-serial-bonds-dtchange-doublesqrt: DFLAGS+=-DOUBLE_SQRT
test-serial-bonds-dtchange-doublesqrt: test-serial-bonds-dtchange

test-serial-bonds-dtchange-doublesqrt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-serial-bonds-dtchange-doublesqrt-floatonly: test-serial-bonds-dtchange-doublesqrt

test-serial-bonds-dtchange-doublesqrt-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-serial-bonds-dtchange-doublesqrt-floatonly-reducedlocalcalcs: test-serial-bonds-dtchange-doublesqrt-floatonly

test-serial-bonds-dtchange-doublesqrt-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-serial-bonds-dtchange-doublesqrt-floatonly-reducedlocalcalcs-singleforceloop: test-serial-bonds-dtchange-doublesqrt-floatonly-reducedlocalcalcs

test-serial-bonds-dtchange-doublesqrt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-serial-bonds-dtchange-doublesqrt-floatonly-singleforceloop: test-serial-bonds-dtchange-doublesqrt-floatonly

test-serial-bonds-dtchange-doublesqrt-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-serial-bonds-dtchange-doublesqrt-reducedlocalcalcs: test-serial-bonds-dtchange-doublesqrt

test-serial-bonds-dtchange-doublesqrt-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-serial-bonds-dtchange-doublesqrt-reducedlocalcalcs-singleforceloop: test-serial-bonds-dtchange-doublesqrt-reducedlocalcalcs

test-serial-bonds-dtchange-doublesqrt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-serial-bonds-dtchange-doublesqrt-singleforceloop: test-serial-bonds-dtchange-doublesqrt

test-serial-bonds-dtchange-floatonly: DFLAGS+=-DFLOAT_ONLY
test-serial-bonds-dtchange-floatonly: test-serial-bonds-dtchange

test-serial-bonds-dtchange-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-serial-bonds-dtchange-floatonly-reducedlocalcalcs: test-serial-bonds-dtchange-floatonly

test-serial-bonds-dtchange-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-serial-bonds-dtchange-floatonly-reducedlocalcalcs-singleforceloop: test-serial-bonds-dtchange-floatonly-reducedlocalcalcs

test-serial-bonds-dtchange-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-serial-bonds-dtchange-floatonly-singleforceloop: test-serial-bonds-dtchange-floatonly

test-serial-bonds-dtchange-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-serial-bonds-dtchange-reducedlocalcalcs: test-serial-bonds-dtchange

test-serial-bonds-dtchange-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-serial-bonds-dtchange-reducedlocalcalcs-singleforceloop: test-serial-bonds-dtchange-reducedlocalcalcs

test-serial-bonds-dtchange-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-serial-bonds-dtchange-singleforceloop: test-serial-bonds-dtchange

test-serial-bonds-doublesqrt: DFLAGS+=-DOUBLE_SQRT
test-serial-bonds-doublesqrt: test-serial-bonds

test-serial-bonds-doublesqrt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-serial-bonds-doublesqrt-floatonly: test-serial-bonds-doublesqrt

test-serial-bonds-doublesqrt-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-serial-bonds-doublesqrt-floatonly-reducedlocalcalcs: test-serial-bonds-doublesqrt-floatonly

test-serial-bonds-doublesqrt-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-serial-bonds-doublesqrt-floatonly-reducedlocalcalcs-singleforceloop: test-serial-bonds-doublesqrt-floatonly-reducedlocalcalcs

test-serial-bonds-doublesqrt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-serial-bonds-doublesqrt-floatonly-singleforceloop: test-serial-bonds-doublesqrt-floatonly

test-serial-bonds-doublesqrt-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-serial-bonds-doublesqrt-reducedlocalcalcs: test-serial-bonds-doublesqrt

test-serial-bonds-doublesqrt-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-serial-bonds-doublesqrt-reducedlocalcalcs-singleforceloop: test-serial-bonds-doublesqrt-reducedlocalcalcs

test-serial-bonds-doublesqrt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-serial-bonds-doublesqrt-singleforceloop: test-serial-bonds-doublesqrt

test-serial-bonds-floatonly: DFLAGS+=-DFLOAT_ONLY
test-serial-bonds-floatonly: test-serial-bonds

test-serial-bonds-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-serial-bonds-floatonly-reducedlocalcalcs: test-serial-bonds-floatonly

test-serial-bonds-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-serial-bonds-floatonly-reducedlocalcalcs-singleforceloop: test-serial-bonds-floatonly-reducedlocalcalcs

test-serial-bonds-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-serial-bonds-floatonly-singleforceloop: test-serial-bonds-floatonly

test-serial-bonds-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-serial-bonds-reducedlocalcalcs: test-serial-bonds

test-serial-bonds-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-serial-bonds-reducedlocalcalcs-singleforceloop: test-serial-bonds-reducedlocalcalcs

test-serial-bonds-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-serial-bonds-singleforceloop: test-serial-bonds

