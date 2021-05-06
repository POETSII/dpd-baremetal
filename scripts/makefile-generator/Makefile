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
POLITE_OBJS = $(COMMON_OBJS) $(HL)/*.o $(DPD_BIN)/POLiteCells.o $(DPD_BIN)/POLiteVolume.o $(DPD_BIN)/POLiteSimulator.o $(DPD_BIN)/ExternalClient.o $(DPD_BIN)/ExternalServer.o
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

test: DFLAGS+=-DTESTING -DONE_BY_ONE
test: $(INC)/config.h $(HL)/*.o $(POLITE_OBJS) base-sync
	g++ -O2 -std=c++11 $(DFLAGS) $(EXTERNAL_FLAGS) -I $(INC) -I $(QUEUE_INC) -I $(HL) -I $(DPD_INC) -c -o $(DPD_BIN)/test.o $(DPD_SRC)/test.cpp
	g++ -O2 -std=c++11 -o $(DPD_BIN)/test $(POLITE_OBJS) $(DPD_BIN)/test.o \
	  -static-libgcc -static-libstdc++ \
      -lscotch -L$(QUARTUS_ROOTDIR)/linux64 \
      -Wl,-rpath,$(QUARTUS_ROOTDIR)/linux64 -lmetis -lpthread -lboost_program_options -lboost_filesystem -lboost_system -fopenmp

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
		-static-libgcc -static-libstdc++ -lscotch -L$(QUARTUS_ROOTDIR)/linux64 \
		-Wl,-rpath,$(QUARTUS_ROOTDIR)/linux64 -lmetis -lpthread -lboost_program_options \
		-lboost_filesystem -lboost_system -fopenmp

vesicle: DFLAGS+=-DVESICLE_SELF_ASSEMBLY -DBONDS
vesicle: $(DPD_EXAMPLES)/VesicleSelfAssembly
	g++ -O2 -std=c++11 $(DFLAGS) $(EXTERNAL_FLAGS) \
	-I $(HL) -I $(DPD_INC) -c -o $(DPD_BIN)/VesicleSelfAssembly.o $(DPD_EXAMPLES)/VesicleSelfAssembly.cpp
	g++ -O2 -std=c++11 -o $(DPD_BIN)/run $(OBJS) $(DPD_BIN)/VesicleSelfAssembly.o \
		-static-libgcc -static-libstdc++ -lscotch -L$(QUARTUS_ROOTDIR)/linux64 \
		-Wl,-rpath,$(QUARTUS_ROOTDIR)/linux64 -lmetis -lpthread -lboost_program_options \
		-lboost_filesystem -lboost_system -fopenmp

corners: DFLAGS+=
corners: $(DPD_EXAMPLES)/corner-tests
	g++ -O2 -std=c++11 $(DFLAGS) $(EXTERNAL_FLAGS) \
	-I $(HL) -I $(DPD_INC) -c -o $(DPD_BIN)/corner-tests.o $(DPD_EXAMPLES)/corner-tests.cpp
	g++ -O2 -std=c++11 -o $(DPD_BIN)/run $(OBJS) $(DPD_BIN)/corner-tests.o \
		-static-libgcc -static-libstdc++ -lscotch -L$(QUARTUS_ROOTDIR)/linux64 \
		-Wl,-rpath,$(QUARTUS_ROOTDIR)/linux64 -lmetis -lpthread -lboost_program_options \
		-lboost_filesystem -lboost_system -fopenmp

gravity: DFLAGS+=-DGRAVITY
gravity: $(DPD_EXAMPLES)/BoxOilWaterGravity
	g++ -O2 -std=c++11 $(DFLAGS) $(EXTERNAL_FLAGS) \
	-I $(HL) -I $(DPD_INC) -c -o $(DPD_BIN)/BoxOilWaterGravity.o $(DPD_EXAMPLES)/BoxOilWaterGravity.cpp
	g++ -O2 -std=c++11 -o $(DPD_BIN)/run $(OBJS) $(DPD_BIN)/BoxOilWaterGravity.o \
		-static-libgcc -static-libstdc++ -lscotch -L$(QUARTUS_ROOTDIR)/linux64 \
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

improvedgals-visual: OBJS+=$(POLITE_OBJS)
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

improvedgals-timed: OBJS+=$(POLITE_OBJS)
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

improvedgals-stats: OBJS+=$(POLITE_OBJS)
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


# **********Operation: stats**********

serial-stats: OBJS+=$(SERIAL_OBJS)
serial-stats: DFLAGS+=-DSTATS
serial-stats: serial

# ******Example: oilwater******

serial-stats-oilwater: DFLAGS+=
serial-stats-oilwater: serial-stats oilwater

serial-stats-oilwater-fastest: DFLAGS+=-DBETTER_VERLET -DSMALL_DT_EARLY -DONE_BY_ONE -DREDUCED_LOCAL_CALCS 
serial-stats-oilwater-fastest: serial-stats-oilwater

serial-stats-oilwater-smallest: DFLAGS+=-DBETTER_VERLET -DSMALL_DT_EARLY -DSEND_TO_SELF -DSINGLE_FORCE_LOOP 
serial-stats-oilwater-smallest: serial-stats-oilwater


# ******Example: vesicle******

serial-stats-vesicle: DFLAGS+=-DVESICLE_SELF_ASSEMBLY -DBONDS
serial-stats-vesicle: serial-stats vesicle

serial-stats-vesicle-fastest: DFLAGS+=-DBETTER_VERLET -DSMALL_DT_EARLY -DONE_BY_ONE -DREDUCED_LOCAL_CALCS 
serial-stats-vesicle-fastest: serial-stats-vesicle

serial-stats-vesicle-smallest: DFLAGS+=-DBETTER_VERLET -DSMALL_DT_EARLY -DSEND_TO_SELF -DSINGLE_FORCE_LOOP 
serial-stats-vesicle-smallest: serial-stats-vesicle


# ******Example: corners******

serial-stats-corners: DFLAGS+=
serial-stats-corners: serial-stats corners

serial-stats-corners-fastest: DFLAGS+=-DBETTER_VERLET -DSMALL_DT_EARLY -DONE_BY_ONE -DREDUCED_LOCAL_CALCS 
serial-stats-corners-fastest: serial-stats-corners

serial-stats-corners-smallest: DFLAGS+=-DBETTER_VERLET -DSMALL_DT_EARLY -DSEND_TO_SELF -DSINGLE_FORCE_LOOP 
serial-stats-corners-smallest: serial-stats-corners


# ******Example: gravity******

serial-stats-gravity: DFLAGS+=-DGRAVITY
serial-stats-gravity: serial-stats gravity

serial-stats-gravity-fastest: DFLAGS+=-DBETTER_VERLET -DSMALL_DT_EARLY -DONE_BY_ONE -DREDUCED_LOCAL_CALCS 
serial-stats-gravity-fastest: serial-stats-gravity

serial-stats-gravity-smallest: DFLAGS+=-DBETTER_VERLET -DSMALL_DT_EARLY -DSEND_TO_SELF -DSINGLE_FORCE_LOOP 
serial-stats-gravity-smallest: serial-stats-gravity

#****************** REGRESSION TESTING ******************
# ************** TEST Simulator: sync**************

test-sync: OBJS=POLITE_OBJS
test-sync: DFLAGS+=-DTESTING 
test-sync: test

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

test-gals: OBJS=POLITE_OBJS
test-gals: DFLAGS+=-DTESTING -DGALS
test-gals: test

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

# **TEST Local calculation method: sendtoself**
test-gals-sendtoself: DFLAGS+=-DSEND_TO_SELF
test-gals-sendtoself: test-gals

test-gals-sendtoself-betterverlet: DFLAGS+=-DBETTER_VERLET
test-gals-sendtoself-betterverlet: test-gals-sendtoself

test-gals-sendtoself-betterverlet-dtchange: DFLAGS+=-DSMALL_DT_EARLY
test-gals-sendtoself-betterverlet-dtchange: test-gals-sendtoself-betterverlet

test-gals-sendtoself-betterverlet-dtchange-doublesqrt: DFLAGS+=-DOUBLE_SQRT
test-gals-sendtoself-betterverlet-dtchange-doublesqrt: test-gals-sendtoself-betterverlet-dtchange

test-gals-sendtoself-betterverlet-dtchange-doublesqrt-msgmgmt: DFLAGS+=-DMESSAGE_MANAGEMENT
test-gals-sendtoself-betterverlet-dtchange-doublesqrt-msgmgmt: test-gals-sendtoself-betterverlet-dtchange-doublesqrt

test-gals-sendtoself-betterverlet-dtchange-doublesqrt-msgmgmt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-gals-sendtoself-betterverlet-dtchange-doublesqrt-msgmgmt-floatonly: test-gals-sendtoself-betterverlet-dtchange-doublesqrt-msgmgmt

test-gals-sendtoself-betterverlet-dtchange-doublesqrt-msgmgmt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-sendtoself-betterverlet-dtchange-doublesqrt-msgmgmt-floatonly-singleforceloop: test-gals-sendtoself-betterverlet-dtchange-doublesqrt-msgmgmt-floatonly

test-gals-sendtoself-betterverlet-dtchange-doublesqrt-msgmgmt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-sendtoself-betterverlet-dtchange-doublesqrt-msgmgmt-singleforceloop: test-gals-sendtoself-betterverlet-dtchange-doublesqrt-msgmgmt

test-gals-sendtoself-betterverlet-dtchange-doublesqrt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-gals-sendtoself-betterverlet-dtchange-doublesqrt-floatonly: test-gals-sendtoself-betterverlet-dtchange-doublesqrt

test-gals-sendtoself-betterverlet-dtchange-doublesqrt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-sendtoself-betterverlet-dtchange-doublesqrt-floatonly-singleforceloop: test-gals-sendtoself-betterverlet-dtchange-doublesqrt-floatonly

test-gals-sendtoself-betterverlet-dtchange-doublesqrt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-sendtoself-betterverlet-dtchange-doublesqrt-singleforceloop: test-gals-sendtoself-betterverlet-dtchange-doublesqrt

test-gals-sendtoself-betterverlet-dtchange-msgmgmt: DFLAGS+=-DMESSAGE_MANAGEMENT
test-gals-sendtoself-betterverlet-dtchange-msgmgmt: test-gals-sendtoself-betterverlet-dtchange

test-gals-sendtoself-betterverlet-dtchange-msgmgmt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-gals-sendtoself-betterverlet-dtchange-msgmgmt-floatonly: test-gals-sendtoself-betterverlet-dtchange-msgmgmt

test-gals-sendtoself-betterverlet-dtchange-msgmgmt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-sendtoself-betterverlet-dtchange-msgmgmt-floatonly-singleforceloop: test-gals-sendtoself-betterverlet-dtchange-msgmgmt-floatonly

test-gals-sendtoself-betterverlet-dtchange-msgmgmt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-sendtoself-betterverlet-dtchange-msgmgmt-singleforceloop: test-gals-sendtoself-betterverlet-dtchange-msgmgmt

test-gals-sendtoself-betterverlet-dtchange-floatonly: DFLAGS+=-DFLOAT_ONLY
test-gals-sendtoself-betterverlet-dtchange-floatonly: test-gals-sendtoself-betterverlet-dtchange

test-gals-sendtoself-betterverlet-dtchange-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-sendtoself-betterverlet-dtchange-floatonly-singleforceloop: test-gals-sendtoself-betterverlet-dtchange-floatonly

test-gals-sendtoself-betterverlet-dtchange-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-sendtoself-betterverlet-dtchange-singleforceloop: test-gals-sendtoself-betterverlet-dtchange

test-gals-sendtoself-betterverlet-doublesqrt: DFLAGS+=-DOUBLE_SQRT
test-gals-sendtoself-betterverlet-doublesqrt: test-gals-sendtoself-betterverlet

test-gals-sendtoself-betterverlet-doublesqrt-msgmgmt: DFLAGS+=-DMESSAGE_MANAGEMENT
test-gals-sendtoself-betterverlet-doublesqrt-msgmgmt: test-gals-sendtoself-betterverlet-doublesqrt

test-gals-sendtoself-betterverlet-doublesqrt-msgmgmt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-gals-sendtoself-betterverlet-doublesqrt-msgmgmt-floatonly: test-gals-sendtoself-betterverlet-doublesqrt-msgmgmt

test-gals-sendtoself-betterverlet-doublesqrt-msgmgmt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-sendtoself-betterverlet-doublesqrt-msgmgmt-floatonly-singleforceloop: test-gals-sendtoself-betterverlet-doublesqrt-msgmgmt-floatonly

test-gals-sendtoself-betterverlet-doublesqrt-msgmgmt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-sendtoself-betterverlet-doublesqrt-msgmgmt-singleforceloop: test-gals-sendtoself-betterverlet-doublesqrt-msgmgmt

test-gals-sendtoself-betterverlet-doublesqrt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-gals-sendtoself-betterverlet-doublesqrt-floatonly: test-gals-sendtoself-betterverlet-doublesqrt

test-gals-sendtoself-betterverlet-doublesqrt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-sendtoself-betterverlet-doublesqrt-floatonly-singleforceloop: test-gals-sendtoself-betterverlet-doublesqrt-floatonly

test-gals-sendtoself-betterverlet-doublesqrt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-sendtoself-betterverlet-doublesqrt-singleforceloop: test-gals-sendtoself-betterverlet-doublesqrt

test-gals-sendtoself-betterverlet-msgmgmt: DFLAGS+=-DMESSAGE_MANAGEMENT
test-gals-sendtoself-betterverlet-msgmgmt: test-gals-sendtoself-betterverlet

test-gals-sendtoself-betterverlet-msgmgmt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-gals-sendtoself-betterverlet-msgmgmt-floatonly: test-gals-sendtoself-betterverlet-msgmgmt

test-gals-sendtoself-betterverlet-msgmgmt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-sendtoself-betterverlet-msgmgmt-floatonly-singleforceloop: test-gals-sendtoself-betterverlet-msgmgmt-floatonly

test-gals-sendtoself-betterverlet-msgmgmt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-sendtoself-betterverlet-msgmgmt-singleforceloop: test-gals-sendtoself-betterverlet-msgmgmt

test-gals-sendtoself-betterverlet-floatonly: DFLAGS+=-DFLOAT_ONLY
test-gals-sendtoself-betterverlet-floatonly: test-gals-sendtoself-betterverlet

test-gals-sendtoself-betterverlet-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-sendtoself-betterverlet-floatonly-singleforceloop: test-gals-sendtoself-betterverlet-floatonly

test-gals-sendtoself-betterverlet-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-sendtoself-betterverlet-singleforceloop: test-gals-sendtoself-betterverlet

test-gals-sendtoself-dtchange: DFLAGS+=-DSMALL_DT_EARLY
test-gals-sendtoself-dtchange: test-gals-sendtoself

test-gals-sendtoself-dtchange-doublesqrt: DFLAGS+=-DOUBLE_SQRT
test-gals-sendtoself-dtchange-doublesqrt: test-gals-sendtoself-dtchange

test-gals-sendtoself-dtchange-doublesqrt-msgmgmt: DFLAGS+=-DMESSAGE_MANAGEMENT
test-gals-sendtoself-dtchange-doublesqrt-msgmgmt: test-gals-sendtoself-dtchange-doublesqrt

test-gals-sendtoself-dtchange-doublesqrt-msgmgmt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-gals-sendtoself-dtchange-doublesqrt-msgmgmt-floatonly: test-gals-sendtoself-dtchange-doublesqrt-msgmgmt

test-gals-sendtoself-dtchange-doublesqrt-msgmgmt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-sendtoself-dtchange-doublesqrt-msgmgmt-floatonly-singleforceloop: test-gals-sendtoself-dtchange-doublesqrt-msgmgmt-floatonly

test-gals-sendtoself-dtchange-doublesqrt-msgmgmt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-sendtoself-dtchange-doublesqrt-msgmgmt-singleforceloop: test-gals-sendtoself-dtchange-doublesqrt-msgmgmt

test-gals-sendtoself-dtchange-doublesqrt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-gals-sendtoself-dtchange-doublesqrt-floatonly: test-gals-sendtoself-dtchange-doublesqrt

test-gals-sendtoself-dtchange-doublesqrt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-sendtoself-dtchange-doublesqrt-floatonly-singleforceloop: test-gals-sendtoself-dtchange-doublesqrt-floatonly

test-gals-sendtoself-dtchange-doublesqrt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-sendtoself-dtchange-doublesqrt-singleforceloop: test-gals-sendtoself-dtchange-doublesqrt

test-gals-sendtoself-dtchange-msgmgmt: DFLAGS+=-DMESSAGE_MANAGEMENT
test-gals-sendtoself-dtchange-msgmgmt: test-gals-sendtoself-dtchange

test-gals-sendtoself-dtchange-msgmgmt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-gals-sendtoself-dtchange-msgmgmt-floatonly: test-gals-sendtoself-dtchange-msgmgmt

test-gals-sendtoself-dtchange-msgmgmt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-sendtoself-dtchange-msgmgmt-floatonly-singleforceloop: test-gals-sendtoself-dtchange-msgmgmt-floatonly

test-gals-sendtoself-dtchange-msgmgmt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-sendtoself-dtchange-msgmgmt-singleforceloop: test-gals-sendtoself-dtchange-msgmgmt

test-gals-sendtoself-dtchange-floatonly: DFLAGS+=-DFLOAT_ONLY
test-gals-sendtoself-dtchange-floatonly: test-gals-sendtoself-dtchange

test-gals-sendtoself-dtchange-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-sendtoself-dtchange-floatonly-singleforceloop: test-gals-sendtoself-dtchange-floatonly

test-gals-sendtoself-dtchange-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-sendtoself-dtchange-singleforceloop: test-gals-sendtoself-dtchange

test-gals-sendtoself-doublesqrt: DFLAGS+=-DOUBLE_SQRT
test-gals-sendtoself-doublesqrt: test-gals-sendtoself

test-gals-sendtoself-doublesqrt-msgmgmt: DFLAGS+=-DMESSAGE_MANAGEMENT
test-gals-sendtoself-doublesqrt-msgmgmt: test-gals-sendtoself-doublesqrt

test-gals-sendtoself-doublesqrt-msgmgmt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-gals-sendtoself-doublesqrt-msgmgmt-floatonly: test-gals-sendtoself-doublesqrt-msgmgmt

test-gals-sendtoself-doublesqrt-msgmgmt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-sendtoself-doublesqrt-msgmgmt-floatonly-singleforceloop: test-gals-sendtoself-doublesqrt-msgmgmt-floatonly

test-gals-sendtoself-doublesqrt-msgmgmt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-sendtoself-doublesqrt-msgmgmt-singleforceloop: test-gals-sendtoself-doublesqrt-msgmgmt

test-gals-sendtoself-doublesqrt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-gals-sendtoself-doublesqrt-floatonly: test-gals-sendtoself-doublesqrt

test-gals-sendtoself-doublesqrt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-sendtoself-doublesqrt-floatonly-singleforceloop: test-gals-sendtoself-doublesqrt-floatonly

test-gals-sendtoself-doublesqrt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-sendtoself-doublesqrt-singleforceloop: test-gals-sendtoself-doublesqrt

test-gals-sendtoself-msgmgmt: DFLAGS+=-DMESSAGE_MANAGEMENT
test-gals-sendtoself-msgmgmt: test-gals-sendtoself

test-gals-sendtoself-msgmgmt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-gals-sendtoself-msgmgmt-floatonly: test-gals-sendtoself-msgmgmt

test-gals-sendtoself-msgmgmt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-sendtoself-msgmgmt-floatonly-singleforceloop: test-gals-sendtoself-msgmgmt-floatonly

test-gals-sendtoself-msgmgmt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-sendtoself-msgmgmt-singleforceloop: test-gals-sendtoself-msgmgmt

test-gals-sendtoself-floatonly: DFLAGS+=-DFLOAT_ONLY
test-gals-sendtoself-floatonly: test-gals-sendtoself

test-gals-sendtoself-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-sendtoself-floatonly-singleforceloop: test-gals-sendtoself-floatonly

test-gals-sendtoself-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-sendtoself-singleforceloop: test-gals-sendtoself

test-gals-large: DFLAGS+=-DLARGE_TEST
test-gals-large: test-gals

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

# **TEST Local calculation method: sendtoself**
test-gals-large-sendtoself: DFLAGS+=-DSEND_TO_SELF
test-gals-large-sendtoself: test-gals-large

test-gals-large-sendtoself-betterverlet: DFLAGS+=-DBETTER_VERLET
test-gals-large-sendtoself-betterverlet: test-gals-large-sendtoself

test-gals-large-sendtoself-betterverlet-dtchange: DFLAGS+=-DSMALL_DT_EARLY
test-gals-large-sendtoself-betterverlet-dtchange: test-gals-large-sendtoself-betterverlet

test-gals-large-sendtoself-betterverlet-dtchange-doublesqrt: DFLAGS+=-DOUBLE_SQRT
test-gals-large-sendtoself-betterverlet-dtchange-doublesqrt: test-gals-large-sendtoself-betterverlet-dtchange

test-gals-large-sendtoself-betterverlet-dtchange-doublesqrt-msgmgmt: DFLAGS+=-DMESSAGE_MANAGEMENT
test-gals-large-sendtoself-betterverlet-dtchange-doublesqrt-msgmgmt: test-gals-large-sendtoself-betterverlet-dtchange-doublesqrt

test-gals-large-sendtoself-betterverlet-dtchange-doublesqrt-msgmgmt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-gals-large-sendtoself-betterverlet-dtchange-doublesqrt-msgmgmt-floatonly: test-gals-large-sendtoself-betterverlet-dtchange-doublesqrt-msgmgmt

test-gals-large-sendtoself-betterverlet-dtchange-doublesqrt-msgmgmt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-large-sendtoself-betterverlet-dtchange-doublesqrt-msgmgmt-floatonly-singleforceloop: test-gals-large-sendtoself-betterverlet-dtchange-doublesqrt-msgmgmt-floatonly

test-gals-large-sendtoself-betterverlet-dtchange-doublesqrt-msgmgmt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-large-sendtoself-betterverlet-dtchange-doublesqrt-msgmgmt-singleforceloop: test-gals-large-sendtoself-betterverlet-dtchange-doublesqrt-msgmgmt

test-gals-large-sendtoself-betterverlet-dtchange-doublesqrt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-gals-large-sendtoself-betterverlet-dtchange-doublesqrt-floatonly: test-gals-large-sendtoself-betterverlet-dtchange-doublesqrt

test-gals-large-sendtoself-betterverlet-dtchange-doublesqrt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-large-sendtoself-betterverlet-dtchange-doublesqrt-floatonly-singleforceloop: test-gals-large-sendtoself-betterverlet-dtchange-doublesqrt-floatonly

test-gals-large-sendtoself-betterverlet-dtchange-doublesqrt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-large-sendtoself-betterverlet-dtchange-doublesqrt-singleforceloop: test-gals-large-sendtoself-betterverlet-dtchange-doublesqrt

test-gals-large-sendtoself-betterverlet-dtchange-msgmgmt: DFLAGS+=-DMESSAGE_MANAGEMENT
test-gals-large-sendtoself-betterverlet-dtchange-msgmgmt: test-gals-large-sendtoself-betterverlet-dtchange

test-gals-large-sendtoself-betterverlet-dtchange-msgmgmt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-gals-large-sendtoself-betterverlet-dtchange-msgmgmt-floatonly: test-gals-large-sendtoself-betterverlet-dtchange-msgmgmt

test-gals-large-sendtoself-betterverlet-dtchange-msgmgmt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-large-sendtoself-betterverlet-dtchange-msgmgmt-floatonly-singleforceloop: test-gals-large-sendtoself-betterverlet-dtchange-msgmgmt-floatonly

test-gals-large-sendtoself-betterverlet-dtchange-msgmgmt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-large-sendtoself-betterverlet-dtchange-msgmgmt-singleforceloop: test-gals-large-sendtoself-betterverlet-dtchange-msgmgmt

test-gals-large-sendtoself-betterverlet-dtchange-floatonly: DFLAGS+=-DFLOAT_ONLY
test-gals-large-sendtoself-betterverlet-dtchange-floatonly: test-gals-large-sendtoself-betterverlet-dtchange

test-gals-large-sendtoself-betterverlet-dtchange-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-large-sendtoself-betterverlet-dtchange-floatonly-singleforceloop: test-gals-large-sendtoself-betterverlet-dtchange-floatonly

test-gals-large-sendtoself-betterverlet-dtchange-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-large-sendtoself-betterverlet-dtchange-singleforceloop: test-gals-large-sendtoself-betterverlet-dtchange

test-gals-large-sendtoself-betterverlet-doublesqrt: DFLAGS+=-DOUBLE_SQRT
test-gals-large-sendtoself-betterverlet-doublesqrt: test-gals-large-sendtoself-betterverlet

test-gals-large-sendtoself-betterverlet-doublesqrt-msgmgmt: DFLAGS+=-DMESSAGE_MANAGEMENT
test-gals-large-sendtoself-betterverlet-doublesqrt-msgmgmt: test-gals-large-sendtoself-betterverlet-doublesqrt

test-gals-large-sendtoself-betterverlet-doublesqrt-msgmgmt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-gals-large-sendtoself-betterverlet-doublesqrt-msgmgmt-floatonly: test-gals-large-sendtoself-betterverlet-doublesqrt-msgmgmt

test-gals-large-sendtoself-betterverlet-doublesqrt-msgmgmt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-large-sendtoself-betterverlet-doublesqrt-msgmgmt-floatonly-singleforceloop: test-gals-large-sendtoself-betterverlet-doublesqrt-msgmgmt-floatonly

test-gals-large-sendtoself-betterverlet-doublesqrt-msgmgmt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-large-sendtoself-betterverlet-doublesqrt-msgmgmt-singleforceloop: test-gals-large-sendtoself-betterverlet-doublesqrt-msgmgmt

test-gals-large-sendtoself-betterverlet-doublesqrt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-gals-large-sendtoself-betterverlet-doublesqrt-floatonly: test-gals-large-sendtoself-betterverlet-doublesqrt

test-gals-large-sendtoself-betterverlet-doublesqrt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-large-sendtoself-betterverlet-doublesqrt-floatonly-singleforceloop: test-gals-large-sendtoself-betterverlet-doublesqrt-floatonly

test-gals-large-sendtoself-betterverlet-doublesqrt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-large-sendtoself-betterverlet-doublesqrt-singleforceloop: test-gals-large-sendtoself-betterverlet-doublesqrt

test-gals-large-sendtoself-betterverlet-msgmgmt: DFLAGS+=-DMESSAGE_MANAGEMENT
test-gals-large-sendtoself-betterverlet-msgmgmt: test-gals-large-sendtoself-betterverlet

test-gals-large-sendtoself-betterverlet-msgmgmt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-gals-large-sendtoself-betterverlet-msgmgmt-floatonly: test-gals-large-sendtoself-betterverlet-msgmgmt

test-gals-large-sendtoself-betterverlet-msgmgmt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-large-sendtoself-betterverlet-msgmgmt-floatonly-singleforceloop: test-gals-large-sendtoself-betterverlet-msgmgmt-floatonly

test-gals-large-sendtoself-betterverlet-msgmgmt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-large-sendtoself-betterverlet-msgmgmt-singleforceloop: test-gals-large-sendtoself-betterverlet-msgmgmt

test-gals-large-sendtoself-betterverlet-floatonly: DFLAGS+=-DFLOAT_ONLY
test-gals-large-sendtoself-betterverlet-floatonly: test-gals-large-sendtoself-betterverlet

test-gals-large-sendtoself-betterverlet-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-large-sendtoself-betterverlet-floatonly-singleforceloop: test-gals-large-sendtoself-betterverlet-floatonly

test-gals-large-sendtoself-betterverlet-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-large-sendtoself-betterverlet-singleforceloop: test-gals-large-sendtoself-betterverlet

test-gals-large-sendtoself-dtchange: DFLAGS+=-DSMALL_DT_EARLY
test-gals-large-sendtoself-dtchange: test-gals-large-sendtoself

test-gals-large-sendtoself-dtchange-doublesqrt: DFLAGS+=-DOUBLE_SQRT
test-gals-large-sendtoself-dtchange-doublesqrt: test-gals-large-sendtoself-dtchange

test-gals-large-sendtoself-dtchange-doublesqrt-msgmgmt: DFLAGS+=-DMESSAGE_MANAGEMENT
test-gals-large-sendtoself-dtchange-doublesqrt-msgmgmt: test-gals-large-sendtoself-dtchange-doublesqrt

test-gals-large-sendtoself-dtchange-doublesqrt-msgmgmt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-gals-large-sendtoself-dtchange-doublesqrt-msgmgmt-floatonly: test-gals-large-sendtoself-dtchange-doublesqrt-msgmgmt

test-gals-large-sendtoself-dtchange-doublesqrt-msgmgmt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-large-sendtoself-dtchange-doublesqrt-msgmgmt-floatonly-singleforceloop: test-gals-large-sendtoself-dtchange-doublesqrt-msgmgmt-floatonly

test-gals-large-sendtoself-dtchange-doublesqrt-msgmgmt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-large-sendtoself-dtchange-doublesqrt-msgmgmt-singleforceloop: test-gals-large-sendtoself-dtchange-doublesqrt-msgmgmt

test-gals-large-sendtoself-dtchange-doublesqrt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-gals-large-sendtoself-dtchange-doublesqrt-floatonly: test-gals-large-sendtoself-dtchange-doublesqrt

test-gals-large-sendtoself-dtchange-doublesqrt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-large-sendtoself-dtchange-doublesqrt-floatonly-singleforceloop: test-gals-large-sendtoself-dtchange-doublesqrt-floatonly

test-gals-large-sendtoself-dtchange-doublesqrt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-large-sendtoself-dtchange-doublesqrt-singleforceloop: test-gals-large-sendtoself-dtchange-doublesqrt

test-gals-large-sendtoself-dtchange-msgmgmt: DFLAGS+=-DMESSAGE_MANAGEMENT
test-gals-large-sendtoself-dtchange-msgmgmt: test-gals-large-sendtoself-dtchange

test-gals-large-sendtoself-dtchange-msgmgmt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-gals-large-sendtoself-dtchange-msgmgmt-floatonly: test-gals-large-sendtoself-dtchange-msgmgmt

test-gals-large-sendtoself-dtchange-msgmgmt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-large-sendtoself-dtchange-msgmgmt-floatonly-singleforceloop: test-gals-large-sendtoself-dtchange-msgmgmt-floatonly

test-gals-large-sendtoself-dtchange-msgmgmt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-large-sendtoself-dtchange-msgmgmt-singleforceloop: test-gals-large-sendtoself-dtchange-msgmgmt

test-gals-large-sendtoself-dtchange-floatonly: DFLAGS+=-DFLOAT_ONLY
test-gals-large-sendtoself-dtchange-floatonly: test-gals-large-sendtoself-dtchange

test-gals-large-sendtoself-dtchange-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-large-sendtoself-dtchange-floatonly-singleforceloop: test-gals-large-sendtoself-dtchange-floatonly

test-gals-large-sendtoself-dtchange-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-large-sendtoself-dtchange-singleforceloop: test-gals-large-sendtoself-dtchange

test-gals-large-sendtoself-doublesqrt: DFLAGS+=-DOUBLE_SQRT
test-gals-large-sendtoself-doublesqrt: test-gals-large-sendtoself

test-gals-large-sendtoself-doublesqrt-msgmgmt: DFLAGS+=-DMESSAGE_MANAGEMENT
test-gals-large-sendtoself-doublesqrt-msgmgmt: test-gals-large-sendtoself-doublesqrt

test-gals-large-sendtoself-doublesqrt-msgmgmt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-gals-large-sendtoself-doublesqrt-msgmgmt-floatonly: test-gals-large-sendtoself-doublesqrt-msgmgmt

test-gals-large-sendtoself-doublesqrt-msgmgmt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-large-sendtoself-doublesqrt-msgmgmt-floatonly-singleforceloop: test-gals-large-sendtoself-doublesqrt-msgmgmt-floatonly

test-gals-large-sendtoself-doublesqrt-msgmgmt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-large-sendtoself-doublesqrt-msgmgmt-singleforceloop: test-gals-large-sendtoself-doublesqrt-msgmgmt

test-gals-large-sendtoself-doublesqrt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-gals-large-sendtoself-doublesqrt-floatonly: test-gals-large-sendtoself-doublesqrt

test-gals-large-sendtoself-doublesqrt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-large-sendtoself-doublesqrt-floatonly-singleforceloop: test-gals-large-sendtoself-doublesqrt-floatonly

test-gals-large-sendtoself-doublesqrt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-large-sendtoself-doublesqrt-singleforceloop: test-gals-large-sendtoself-doublesqrt

test-gals-large-sendtoself-msgmgmt: DFLAGS+=-DMESSAGE_MANAGEMENT
test-gals-large-sendtoself-msgmgmt: test-gals-large-sendtoself

test-gals-large-sendtoself-msgmgmt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-gals-large-sendtoself-msgmgmt-floatonly: test-gals-large-sendtoself-msgmgmt

test-gals-large-sendtoself-msgmgmt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-large-sendtoself-msgmgmt-floatonly-singleforceloop: test-gals-large-sendtoself-msgmgmt-floatonly

test-gals-large-sendtoself-msgmgmt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-large-sendtoself-msgmgmt-singleforceloop: test-gals-large-sendtoself-msgmgmt

test-gals-large-sendtoself-floatonly: DFLAGS+=-DFLOAT_ONLY
test-gals-large-sendtoself-floatonly: test-gals-large-sendtoself

test-gals-large-sendtoself-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-large-sendtoself-floatonly-singleforceloop: test-gals-large-sendtoself-floatonly

test-gals-large-sendtoself-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-large-sendtoself-singleforceloop: test-gals-large-sendtoself

test-gals-bonds: DFLAGS+=-DBONDS
test-gals-bonds: test-gals

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

# **TEST Local calculation method: sendtoself**
test-gals-bonds-sendtoself: DFLAGS+=-DSEND_TO_SELF
test-gals-bonds-sendtoself: test-gals-bonds

test-gals-bonds-sendtoself-betterverlet: DFLAGS+=-DBETTER_VERLET
test-gals-bonds-sendtoself-betterverlet: test-gals-bonds-sendtoself

test-gals-bonds-sendtoself-betterverlet-dtchange: DFLAGS+=-DSMALL_DT_EARLY
test-gals-bonds-sendtoself-betterverlet-dtchange: test-gals-bonds-sendtoself-betterverlet

test-gals-bonds-sendtoself-betterverlet-dtchange-doublesqrt: DFLAGS+=-DOUBLE_SQRT
test-gals-bonds-sendtoself-betterverlet-dtchange-doublesqrt: test-gals-bonds-sendtoself-betterverlet-dtchange

test-gals-bonds-sendtoself-betterverlet-dtchange-doublesqrt-msgmgmt: DFLAGS+=-DMESSAGE_MANAGEMENT
test-gals-bonds-sendtoself-betterverlet-dtchange-doublesqrt-msgmgmt: test-gals-bonds-sendtoself-betterverlet-dtchange-doublesqrt

test-gals-bonds-sendtoself-betterverlet-dtchange-doublesqrt-msgmgmt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-gals-bonds-sendtoself-betterverlet-dtchange-doublesqrt-msgmgmt-floatonly: test-gals-bonds-sendtoself-betterverlet-dtchange-doublesqrt-msgmgmt

test-gals-bonds-sendtoself-betterverlet-dtchange-doublesqrt-msgmgmt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-bonds-sendtoself-betterverlet-dtchange-doublesqrt-msgmgmt-floatonly-singleforceloop: test-gals-bonds-sendtoself-betterverlet-dtchange-doublesqrt-msgmgmt-floatonly

test-gals-bonds-sendtoself-betterverlet-dtchange-doublesqrt-msgmgmt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-bonds-sendtoself-betterverlet-dtchange-doublesqrt-msgmgmt-singleforceloop: test-gals-bonds-sendtoself-betterverlet-dtchange-doublesqrt-msgmgmt

test-gals-bonds-sendtoself-betterverlet-dtchange-doublesqrt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-gals-bonds-sendtoself-betterverlet-dtchange-doublesqrt-floatonly: test-gals-bonds-sendtoself-betterverlet-dtchange-doublesqrt

test-gals-bonds-sendtoself-betterverlet-dtchange-doublesqrt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-bonds-sendtoself-betterverlet-dtchange-doublesqrt-floatonly-singleforceloop: test-gals-bonds-sendtoself-betterverlet-dtchange-doublesqrt-floatonly

test-gals-bonds-sendtoself-betterverlet-dtchange-doublesqrt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-bonds-sendtoself-betterverlet-dtchange-doublesqrt-singleforceloop: test-gals-bonds-sendtoself-betterverlet-dtchange-doublesqrt

test-gals-bonds-sendtoself-betterverlet-dtchange-msgmgmt: DFLAGS+=-DMESSAGE_MANAGEMENT
test-gals-bonds-sendtoself-betterverlet-dtchange-msgmgmt: test-gals-bonds-sendtoself-betterverlet-dtchange

test-gals-bonds-sendtoself-betterverlet-dtchange-msgmgmt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-gals-bonds-sendtoself-betterverlet-dtchange-msgmgmt-floatonly: test-gals-bonds-sendtoself-betterverlet-dtchange-msgmgmt

test-gals-bonds-sendtoself-betterverlet-dtchange-msgmgmt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-bonds-sendtoself-betterverlet-dtchange-msgmgmt-floatonly-singleforceloop: test-gals-bonds-sendtoself-betterverlet-dtchange-msgmgmt-floatonly

test-gals-bonds-sendtoself-betterverlet-dtchange-msgmgmt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-bonds-sendtoself-betterverlet-dtchange-msgmgmt-singleforceloop: test-gals-bonds-sendtoself-betterverlet-dtchange-msgmgmt

test-gals-bonds-sendtoself-betterverlet-dtchange-floatonly: DFLAGS+=-DFLOAT_ONLY
test-gals-bonds-sendtoself-betterverlet-dtchange-floatonly: test-gals-bonds-sendtoself-betterverlet-dtchange

test-gals-bonds-sendtoself-betterverlet-dtchange-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-bonds-sendtoself-betterverlet-dtchange-floatonly-singleforceloop: test-gals-bonds-sendtoself-betterverlet-dtchange-floatonly

test-gals-bonds-sendtoself-betterverlet-dtchange-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-bonds-sendtoself-betterverlet-dtchange-singleforceloop: test-gals-bonds-sendtoself-betterverlet-dtchange

test-gals-bonds-sendtoself-betterverlet-doublesqrt: DFLAGS+=-DOUBLE_SQRT
test-gals-bonds-sendtoself-betterverlet-doublesqrt: test-gals-bonds-sendtoself-betterverlet

test-gals-bonds-sendtoself-betterverlet-doublesqrt-msgmgmt: DFLAGS+=-DMESSAGE_MANAGEMENT
test-gals-bonds-sendtoself-betterverlet-doublesqrt-msgmgmt: test-gals-bonds-sendtoself-betterverlet-doublesqrt

test-gals-bonds-sendtoself-betterverlet-doublesqrt-msgmgmt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-gals-bonds-sendtoself-betterverlet-doublesqrt-msgmgmt-floatonly: test-gals-bonds-sendtoself-betterverlet-doublesqrt-msgmgmt

test-gals-bonds-sendtoself-betterverlet-doublesqrt-msgmgmt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-bonds-sendtoself-betterverlet-doublesqrt-msgmgmt-floatonly-singleforceloop: test-gals-bonds-sendtoself-betterverlet-doublesqrt-msgmgmt-floatonly

test-gals-bonds-sendtoself-betterverlet-doublesqrt-msgmgmt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-bonds-sendtoself-betterverlet-doublesqrt-msgmgmt-singleforceloop: test-gals-bonds-sendtoself-betterverlet-doublesqrt-msgmgmt

test-gals-bonds-sendtoself-betterverlet-doublesqrt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-gals-bonds-sendtoself-betterverlet-doublesqrt-floatonly: test-gals-bonds-sendtoself-betterverlet-doublesqrt

test-gals-bonds-sendtoself-betterverlet-doublesqrt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-bonds-sendtoself-betterverlet-doublesqrt-floatonly-singleforceloop: test-gals-bonds-sendtoself-betterverlet-doublesqrt-floatonly

test-gals-bonds-sendtoself-betterverlet-doublesqrt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-bonds-sendtoself-betterverlet-doublesqrt-singleforceloop: test-gals-bonds-sendtoself-betterverlet-doublesqrt

test-gals-bonds-sendtoself-betterverlet-msgmgmt: DFLAGS+=-DMESSAGE_MANAGEMENT
test-gals-bonds-sendtoself-betterverlet-msgmgmt: test-gals-bonds-sendtoself-betterverlet

test-gals-bonds-sendtoself-betterverlet-msgmgmt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-gals-bonds-sendtoself-betterverlet-msgmgmt-floatonly: test-gals-bonds-sendtoself-betterverlet-msgmgmt

test-gals-bonds-sendtoself-betterverlet-msgmgmt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-bonds-sendtoself-betterverlet-msgmgmt-floatonly-singleforceloop: test-gals-bonds-sendtoself-betterverlet-msgmgmt-floatonly

test-gals-bonds-sendtoself-betterverlet-msgmgmt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-bonds-sendtoself-betterverlet-msgmgmt-singleforceloop: test-gals-bonds-sendtoself-betterverlet-msgmgmt

test-gals-bonds-sendtoself-betterverlet-floatonly: DFLAGS+=-DFLOAT_ONLY
test-gals-bonds-sendtoself-betterverlet-floatonly: test-gals-bonds-sendtoself-betterverlet

test-gals-bonds-sendtoself-betterverlet-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-bonds-sendtoself-betterverlet-floatonly-singleforceloop: test-gals-bonds-sendtoself-betterverlet-floatonly

test-gals-bonds-sendtoself-betterverlet-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-bonds-sendtoself-betterverlet-singleforceloop: test-gals-bonds-sendtoself-betterverlet

test-gals-bonds-sendtoself-dtchange: DFLAGS+=-DSMALL_DT_EARLY
test-gals-bonds-sendtoself-dtchange: test-gals-bonds-sendtoself

test-gals-bonds-sendtoself-dtchange-doublesqrt: DFLAGS+=-DOUBLE_SQRT
test-gals-bonds-sendtoself-dtchange-doublesqrt: test-gals-bonds-sendtoself-dtchange

test-gals-bonds-sendtoself-dtchange-doublesqrt-msgmgmt: DFLAGS+=-DMESSAGE_MANAGEMENT
test-gals-bonds-sendtoself-dtchange-doublesqrt-msgmgmt: test-gals-bonds-sendtoself-dtchange-doublesqrt

test-gals-bonds-sendtoself-dtchange-doublesqrt-msgmgmt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-gals-bonds-sendtoself-dtchange-doublesqrt-msgmgmt-floatonly: test-gals-bonds-sendtoself-dtchange-doublesqrt-msgmgmt

test-gals-bonds-sendtoself-dtchange-doublesqrt-msgmgmt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-bonds-sendtoself-dtchange-doublesqrt-msgmgmt-floatonly-singleforceloop: test-gals-bonds-sendtoself-dtchange-doublesqrt-msgmgmt-floatonly

test-gals-bonds-sendtoself-dtchange-doublesqrt-msgmgmt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-bonds-sendtoself-dtchange-doublesqrt-msgmgmt-singleforceloop: test-gals-bonds-sendtoself-dtchange-doublesqrt-msgmgmt

test-gals-bonds-sendtoself-dtchange-doublesqrt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-gals-bonds-sendtoself-dtchange-doublesqrt-floatonly: test-gals-bonds-sendtoself-dtchange-doublesqrt

test-gals-bonds-sendtoself-dtchange-doublesqrt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-bonds-sendtoself-dtchange-doublesqrt-floatonly-singleforceloop: test-gals-bonds-sendtoself-dtchange-doublesqrt-floatonly

test-gals-bonds-sendtoself-dtchange-doublesqrt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-bonds-sendtoself-dtchange-doublesqrt-singleforceloop: test-gals-bonds-sendtoself-dtchange-doublesqrt

test-gals-bonds-sendtoself-dtchange-msgmgmt: DFLAGS+=-DMESSAGE_MANAGEMENT
test-gals-bonds-sendtoself-dtchange-msgmgmt: test-gals-bonds-sendtoself-dtchange

test-gals-bonds-sendtoself-dtchange-msgmgmt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-gals-bonds-sendtoself-dtchange-msgmgmt-floatonly: test-gals-bonds-sendtoself-dtchange-msgmgmt

test-gals-bonds-sendtoself-dtchange-msgmgmt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-bonds-sendtoself-dtchange-msgmgmt-floatonly-singleforceloop: test-gals-bonds-sendtoself-dtchange-msgmgmt-floatonly

test-gals-bonds-sendtoself-dtchange-msgmgmt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-bonds-sendtoself-dtchange-msgmgmt-singleforceloop: test-gals-bonds-sendtoself-dtchange-msgmgmt

test-gals-bonds-sendtoself-dtchange-floatonly: DFLAGS+=-DFLOAT_ONLY
test-gals-bonds-sendtoself-dtchange-floatonly: test-gals-bonds-sendtoself-dtchange

test-gals-bonds-sendtoself-dtchange-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-bonds-sendtoself-dtchange-floatonly-singleforceloop: test-gals-bonds-sendtoself-dtchange-floatonly

test-gals-bonds-sendtoself-dtchange-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-bonds-sendtoself-dtchange-singleforceloop: test-gals-bonds-sendtoself-dtchange

test-gals-bonds-sendtoself-doublesqrt: DFLAGS+=-DOUBLE_SQRT
test-gals-bonds-sendtoself-doublesqrt: test-gals-bonds-sendtoself

test-gals-bonds-sendtoself-doublesqrt-msgmgmt: DFLAGS+=-DMESSAGE_MANAGEMENT
test-gals-bonds-sendtoself-doublesqrt-msgmgmt: test-gals-bonds-sendtoself-doublesqrt

test-gals-bonds-sendtoself-doublesqrt-msgmgmt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-gals-bonds-sendtoself-doublesqrt-msgmgmt-floatonly: test-gals-bonds-sendtoself-doublesqrt-msgmgmt

test-gals-bonds-sendtoself-doublesqrt-msgmgmt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-bonds-sendtoself-doublesqrt-msgmgmt-floatonly-singleforceloop: test-gals-bonds-sendtoself-doublesqrt-msgmgmt-floatonly

test-gals-bonds-sendtoself-doublesqrt-msgmgmt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-bonds-sendtoself-doublesqrt-msgmgmt-singleforceloop: test-gals-bonds-sendtoself-doublesqrt-msgmgmt

test-gals-bonds-sendtoself-doublesqrt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-gals-bonds-sendtoself-doublesqrt-floatonly: test-gals-bonds-sendtoself-doublesqrt

test-gals-bonds-sendtoself-doublesqrt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-bonds-sendtoself-doublesqrt-floatonly-singleforceloop: test-gals-bonds-sendtoself-doublesqrt-floatonly

test-gals-bonds-sendtoself-doublesqrt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-bonds-sendtoself-doublesqrt-singleforceloop: test-gals-bonds-sendtoself-doublesqrt

test-gals-bonds-sendtoself-msgmgmt: DFLAGS+=-DMESSAGE_MANAGEMENT
test-gals-bonds-sendtoself-msgmgmt: test-gals-bonds-sendtoself

test-gals-bonds-sendtoself-msgmgmt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-gals-bonds-sendtoself-msgmgmt-floatonly: test-gals-bonds-sendtoself-msgmgmt

test-gals-bonds-sendtoself-msgmgmt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-bonds-sendtoself-msgmgmt-floatonly-singleforceloop: test-gals-bonds-sendtoself-msgmgmt-floatonly

test-gals-bonds-sendtoself-msgmgmt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-bonds-sendtoself-msgmgmt-singleforceloop: test-gals-bonds-sendtoself-msgmgmt

test-gals-bonds-sendtoself-floatonly: DFLAGS+=-DFLOAT_ONLY
test-gals-bonds-sendtoself-floatonly: test-gals-bonds-sendtoself

test-gals-bonds-sendtoself-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-bonds-sendtoself-floatonly-singleforceloop: test-gals-bonds-sendtoself-floatonly

test-gals-bonds-sendtoself-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-gals-bonds-sendtoself-singleforceloop: test-gals-bonds-sendtoself


# ************** TEST Simulator: improvedgals**************

test-improvedgals: OBJS=POLITE_OBJS
test-improvedgals: DFLAGS+=-DTESTING -DGALS -DIMPROVED_GALS
test-improvedgals: test

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

# **TEST Local calculation method: sendtoself**
test-improvedgals-sendtoself: DFLAGS+=-DSEND_TO_SELF
test-improvedgals-sendtoself: test-improvedgals

test-improvedgals-sendtoself-betterverlet: DFLAGS+=-DBETTER_VERLET
test-improvedgals-sendtoself-betterverlet: test-improvedgals-sendtoself

test-improvedgals-sendtoself-betterverlet-dtchange: DFLAGS+=-DSMALL_DT_EARLY
test-improvedgals-sendtoself-betterverlet-dtchange: test-improvedgals-sendtoself-betterverlet

test-improvedgals-sendtoself-betterverlet-dtchange-doublesqrt: DFLAGS+=-DOUBLE_SQRT
test-improvedgals-sendtoself-betterverlet-dtchange-doublesqrt: test-improvedgals-sendtoself-betterverlet-dtchange

test-improvedgals-sendtoself-betterverlet-dtchange-doublesqrt-msgmgmt: DFLAGS+=-DMESSAGE_MANAGEMENT
test-improvedgals-sendtoself-betterverlet-dtchange-doublesqrt-msgmgmt: test-improvedgals-sendtoself-betterverlet-dtchange-doublesqrt

test-improvedgals-sendtoself-betterverlet-dtchange-doublesqrt-msgmgmt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-improvedgals-sendtoself-betterverlet-dtchange-doublesqrt-msgmgmt-floatonly: test-improvedgals-sendtoself-betterverlet-dtchange-doublesqrt-msgmgmt

test-improvedgals-sendtoself-betterverlet-dtchange-doublesqrt-msgmgmt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-sendtoself-betterverlet-dtchange-doublesqrt-msgmgmt-floatonly-singleforceloop: test-improvedgals-sendtoself-betterverlet-dtchange-doublesqrt-msgmgmt-floatonly

test-improvedgals-sendtoself-betterverlet-dtchange-doublesqrt-msgmgmt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-sendtoself-betterverlet-dtchange-doublesqrt-msgmgmt-singleforceloop: test-improvedgals-sendtoself-betterverlet-dtchange-doublesqrt-msgmgmt

test-improvedgals-sendtoself-betterverlet-dtchange-doublesqrt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-improvedgals-sendtoself-betterverlet-dtchange-doublesqrt-floatonly: test-improvedgals-sendtoself-betterverlet-dtchange-doublesqrt

test-improvedgals-sendtoself-betterverlet-dtchange-doublesqrt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-sendtoself-betterverlet-dtchange-doublesqrt-floatonly-singleforceloop: test-improvedgals-sendtoself-betterverlet-dtchange-doublesqrt-floatonly

test-improvedgals-sendtoself-betterverlet-dtchange-doublesqrt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-sendtoself-betterverlet-dtchange-doublesqrt-singleforceloop: test-improvedgals-sendtoself-betterverlet-dtchange-doublesqrt

test-improvedgals-sendtoself-betterverlet-dtchange-msgmgmt: DFLAGS+=-DMESSAGE_MANAGEMENT
test-improvedgals-sendtoself-betterverlet-dtchange-msgmgmt: test-improvedgals-sendtoself-betterverlet-dtchange

test-improvedgals-sendtoself-betterverlet-dtchange-msgmgmt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-improvedgals-sendtoself-betterverlet-dtchange-msgmgmt-floatonly: test-improvedgals-sendtoself-betterverlet-dtchange-msgmgmt

test-improvedgals-sendtoself-betterverlet-dtchange-msgmgmt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-sendtoself-betterverlet-dtchange-msgmgmt-floatonly-singleforceloop: test-improvedgals-sendtoself-betterverlet-dtchange-msgmgmt-floatonly

test-improvedgals-sendtoself-betterverlet-dtchange-msgmgmt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-sendtoself-betterverlet-dtchange-msgmgmt-singleforceloop: test-improvedgals-sendtoself-betterverlet-dtchange-msgmgmt

test-improvedgals-sendtoself-betterverlet-dtchange-floatonly: DFLAGS+=-DFLOAT_ONLY
test-improvedgals-sendtoself-betterverlet-dtchange-floatonly: test-improvedgals-sendtoself-betterverlet-dtchange

test-improvedgals-sendtoself-betterverlet-dtchange-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-sendtoself-betterverlet-dtchange-floatonly-singleforceloop: test-improvedgals-sendtoself-betterverlet-dtchange-floatonly

test-improvedgals-sendtoself-betterverlet-dtchange-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-sendtoself-betterverlet-dtchange-singleforceloop: test-improvedgals-sendtoself-betterverlet-dtchange

test-improvedgals-sendtoself-betterverlet-doublesqrt: DFLAGS+=-DOUBLE_SQRT
test-improvedgals-sendtoself-betterverlet-doublesqrt: test-improvedgals-sendtoself-betterverlet

test-improvedgals-sendtoself-betterverlet-doublesqrt-msgmgmt: DFLAGS+=-DMESSAGE_MANAGEMENT
test-improvedgals-sendtoself-betterverlet-doublesqrt-msgmgmt: test-improvedgals-sendtoself-betterverlet-doublesqrt

test-improvedgals-sendtoself-betterverlet-doublesqrt-msgmgmt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-improvedgals-sendtoself-betterverlet-doublesqrt-msgmgmt-floatonly: test-improvedgals-sendtoself-betterverlet-doublesqrt-msgmgmt

test-improvedgals-sendtoself-betterverlet-doublesqrt-msgmgmt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-sendtoself-betterverlet-doublesqrt-msgmgmt-floatonly-singleforceloop: test-improvedgals-sendtoself-betterverlet-doublesqrt-msgmgmt-floatonly

test-improvedgals-sendtoself-betterverlet-doublesqrt-msgmgmt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-sendtoself-betterverlet-doublesqrt-msgmgmt-singleforceloop: test-improvedgals-sendtoself-betterverlet-doublesqrt-msgmgmt

test-improvedgals-sendtoself-betterverlet-doublesqrt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-improvedgals-sendtoself-betterverlet-doublesqrt-floatonly: test-improvedgals-sendtoself-betterverlet-doublesqrt

test-improvedgals-sendtoself-betterverlet-doublesqrt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-sendtoself-betterverlet-doublesqrt-floatonly-singleforceloop: test-improvedgals-sendtoself-betterverlet-doublesqrt-floatonly

test-improvedgals-sendtoself-betterverlet-doublesqrt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-sendtoself-betterverlet-doublesqrt-singleforceloop: test-improvedgals-sendtoself-betterverlet-doublesqrt

test-improvedgals-sendtoself-betterverlet-msgmgmt: DFLAGS+=-DMESSAGE_MANAGEMENT
test-improvedgals-sendtoself-betterverlet-msgmgmt: test-improvedgals-sendtoself-betterverlet

test-improvedgals-sendtoself-betterverlet-msgmgmt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-improvedgals-sendtoself-betterverlet-msgmgmt-floatonly: test-improvedgals-sendtoself-betterverlet-msgmgmt

test-improvedgals-sendtoself-betterverlet-msgmgmt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-sendtoself-betterverlet-msgmgmt-floatonly-singleforceloop: test-improvedgals-sendtoself-betterverlet-msgmgmt-floatonly

test-improvedgals-sendtoself-betterverlet-msgmgmt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-sendtoself-betterverlet-msgmgmt-singleforceloop: test-improvedgals-sendtoself-betterverlet-msgmgmt

test-improvedgals-sendtoself-betterverlet-floatonly: DFLAGS+=-DFLOAT_ONLY
test-improvedgals-sendtoself-betterverlet-floatonly: test-improvedgals-sendtoself-betterverlet

test-improvedgals-sendtoself-betterverlet-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-sendtoself-betterverlet-floatonly-singleforceloop: test-improvedgals-sendtoself-betterverlet-floatonly

test-improvedgals-sendtoself-betterverlet-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-sendtoself-betterverlet-singleforceloop: test-improvedgals-sendtoself-betterverlet

test-improvedgals-sendtoself-dtchange: DFLAGS+=-DSMALL_DT_EARLY
test-improvedgals-sendtoself-dtchange: test-improvedgals-sendtoself

test-improvedgals-sendtoself-dtchange-doublesqrt: DFLAGS+=-DOUBLE_SQRT
test-improvedgals-sendtoself-dtchange-doublesqrt: test-improvedgals-sendtoself-dtchange

test-improvedgals-sendtoself-dtchange-doublesqrt-msgmgmt: DFLAGS+=-DMESSAGE_MANAGEMENT
test-improvedgals-sendtoself-dtchange-doublesqrt-msgmgmt: test-improvedgals-sendtoself-dtchange-doublesqrt

test-improvedgals-sendtoself-dtchange-doublesqrt-msgmgmt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-improvedgals-sendtoself-dtchange-doublesqrt-msgmgmt-floatonly: test-improvedgals-sendtoself-dtchange-doublesqrt-msgmgmt

test-improvedgals-sendtoself-dtchange-doublesqrt-msgmgmt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-sendtoself-dtchange-doublesqrt-msgmgmt-floatonly-singleforceloop: test-improvedgals-sendtoself-dtchange-doublesqrt-msgmgmt-floatonly

test-improvedgals-sendtoself-dtchange-doublesqrt-msgmgmt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-sendtoself-dtchange-doublesqrt-msgmgmt-singleforceloop: test-improvedgals-sendtoself-dtchange-doublesqrt-msgmgmt

test-improvedgals-sendtoself-dtchange-doublesqrt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-improvedgals-sendtoself-dtchange-doublesqrt-floatonly: test-improvedgals-sendtoself-dtchange-doublesqrt

test-improvedgals-sendtoself-dtchange-doublesqrt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-sendtoself-dtchange-doublesqrt-floatonly-singleforceloop: test-improvedgals-sendtoself-dtchange-doublesqrt-floatonly

test-improvedgals-sendtoself-dtchange-doublesqrt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-sendtoself-dtchange-doublesqrt-singleforceloop: test-improvedgals-sendtoself-dtchange-doublesqrt

test-improvedgals-sendtoself-dtchange-msgmgmt: DFLAGS+=-DMESSAGE_MANAGEMENT
test-improvedgals-sendtoself-dtchange-msgmgmt: test-improvedgals-sendtoself-dtchange

test-improvedgals-sendtoself-dtchange-msgmgmt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-improvedgals-sendtoself-dtchange-msgmgmt-floatonly: test-improvedgals-sendtoself-dtchange-msgmgmt

test-improvedgals-sendtoself-dtchange-msgmgmt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-sendtoself-dtchange-msgmgmt-floatonly-singleforceloop: test-improvedgals-sendtoself-dtchange-msgmgmt-floatonly

test-improvedgals-sendtoself-dtchange-msgmgmt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-sendtoself-dtchange-msgmgmt-singleforceloop: test-improvedgals-sendtoself-dtchange-msgmgmt

test-improvedgals-sendtoself-dtchange-floatonly: DFLAGS+=-DFLOAT_ONLY
test-improvedgals-sendtoself-dtchange-floatonly: test-improvedgals-sendtoself-dtchange

test-improvedgals-sendtoself-dtchange-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-sendtoself-dtchange-floatonly-singleforceloop: test-improvedgals-sendtoself-dtchange-floatonly

test-improvedgals-sendtoself-dtchange-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-sendtoself-dtchange-singleforceloop: test-improvedgals-sendtoself-dtchange

test-improvedgals-sendtoself-doublesqrt: DFLAGS+=-DOUBLE_SQRT
test-improvedgals-sendtoself-doublesqrt: test-improvedgals-sendtoself

test-improvedgals-sendtoself-doublesqrt-msgmgmt: DFLAGS+=-DMESSAGE_MANAGEMENT
test-improvedgals-sendtoself-doublesqrt-msgmgmt: test-improvedgals-sendtoself-doublesqrt

test-improvedgals-sendtoself-doublesqrt-msgmgmt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-improvedgals-sendtoself-doublesqrt-msgmgmt-floatonly: test-improvedgals-sendtoself-doublesqrt-msgmgmt

test-improvedgals-sendtoself-doublesqrt-msgmgmt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-sendtoself-doublesqrt-msgmgmt-floatonly-singleforceloop: test-improvedgals-sendtoself-doublesqrt-msgmgmt-floatonly

test-improvedgals-sendtoself-doublesqrt-msgmgmt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-sendtoself-doublesqrt-msgmgmt-singleforceloop: test-improvedgals-sendtoself-doublesqrt-msgmgmt

test-improvedgals-sendtoself-doublesqrt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-improvedgals-sendtoself-doublesqrt-floatonly: test-improvedgals-sendtoself-doublesqrt

test-improvedgals-sendtoself-doublesqrt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-sendtoself-doublesqrt-floatonly-singleforceloop: test-improvedgals-sendtoself-doublesqrt-floatonly

test-improvedgals-sendtoself-doublesqrt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-sendtoself-doublesqrt-singleforceloop: test-improvedgals-sendtoself-doublesqrt

test-improvedgals-sendtoself-msgmgmt: DFLAGS+=-DMESSAGE_MANAGEMENT
test-improvedgals-sendtoself-msgmgmt: test-improvedgals-sendtoself

test-improvedgals-sendtoself-msgmgmt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-improvedgals-sendtoself-msgmgmt-floatonly: test-improvedgals-sendtoself-msgmgmt

test-improvedgals-sendtoself-msgmgmt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-sendtoself-msgmgmt-floatonly-singleforceloop: test-improvedgals-sendtoself-msgmgmt-floatonly

test-improvedgals-sendtoself-msgmgmt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-sendtoself-msgmgmt-singleforceloop: test-improvedgals-sendtoself-msgmgmt

test-improvedgals-sendtoself-floatonly: DFLAGS+=-DFLOAT_ONLY
test-improvedgals-sendtoself-floatonly: test-improvedgals-sendtoself

test-improvedgals-sendtoself-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-sendtoself-floatonly-singleforceloop: test-improvedgals-sendtoself-floatonly

test-improvedgals-sendtoself-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-sendtoself-singleforceloop: test-improvedgals-sendtoself

test-improvedgals-large: DFLAGS+=-DLARGE_TEST
test-improvedgals-large: test-improvedgals

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

# **TEST Local calculation method: sendtoself**
test-improvedgals-large-sendtoself: DFLAGS+=-DSEND_TO_SELF
test-improvedgals-large-sendtoself: test-improvedgals-large

test-improvedgals-large-sendtoself-betterverlet: DFLAGS+=-DBETTER_VERLET
test-improvedgals-large-sendtoself-betterverlet: test-improvedgals-large-sendtoself

test-improvedgals-large-sendtoself-betterverlet-dtchange: DFLAGS+=-DSMALL_DT_EARLY
test-improvedgals-large-sendtoself-betterverlet-dtchange: test-improvedgals-large-sendtoself-betterverlet

test-improvedgals-large-sendtoself-betterverlet-dtchange-doublesqrt: DFLAGS+=-DOUBLE_SQRT
test-improvedgals-large-sendtoself-betterverlet-dtchange-doublesqrt: test-improvedgals-large-sendtoself-betterverlet-dtchange

test-improvedgals-large-sendtoself-betterverlet-dtchange-doublesqrt-msgmgmt: DFLAGS+=-DMESSAGE_MANAGEMENT
test-improvedgals-large-sendtoself-betterverlet-dtchange-doublesqrt-msgmgmt: test-improvedgals-large-sendtoself-betterverlet-dtchange-doublesqrt

test-improvedgals-large-sendtoself-betterverlet-dtchange-doublesqrt-msgmgmt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-improvedgals-large-sendtoself-betterverlet-dtchange-doublesqrt-msgmgmt-floatonly: test-improvedgals-large-sendtoself-betterverlet-dtchange-doublesqrt-msgmgmt

test-improvedgals-large-sendtoself-betterverlet-dtchange-doublesqrt-msgmgmt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-large-sendtoself-betterverlet-dtchange-doublesqrt-msgmgmt-floatonly-singleforceloop: test-improvedgals-large-sendtoself-betterverlet-dtchange-doublesqrt-msgmgmt-floatonly

test-improvedgals-large-sendtoself-betterverlet-dtchange-doublesqrt-msgmgmt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-large-sendtoself-betterverlet-dtchange-doublesqrt-msgmgmt-singleforceloop: test-improvedgals-large-sendtoself-betterverlet-dtchange-doublesqrt-msgmgmt

test-improvedgals-large-sendtoself-betterverlet-dtchange-doublesqrt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-improvedgals-large-sendtoself-betterverlet-dtchange-doublesqrt-floatonly: test-improvedgals-large-sendtoself-betterverlet-dtchange-doublesqrt

test-improvedgals-large-sendtoself-betterverlet-dtchange-doublesqrt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-large-sendtoself-betterverlet-dtchange-doublesqrt-floatonly-singleforceloop: test-improvedgals-large-sendtoself-betterverlet-dtchange-doublesqrt-floatonly

test-improvedgals-large-sendtoself-betterverlet-dtchange-doublesqrt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-large-sendtoself-betterverlet-dtchange-doublesqrt-singleforceloop: test-improvedgals-large-sendtoself-betterverlet-dtchange-doublesqrt

test-improvedgals-large-sendtoself-betterverlet-dtchange-msgmgmt: DFLAGS+=-DMESSAGE_MANAGEMENT
test-improvedgals-large-sendtoself-betterverlet-dtchange-msgmgmt: test-improvedgals-large-sendtoself-betterverlet-dtchange

test-improvedgals-large-sendtoself-betterverlet-dtchange-msgmgmt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-improvedgals-large-sendtoself-betterverlet-dtchange-msgmgmt-floatonly: test-improvedgals-large-sendtoself-betterverlet-dtchange-msgmgmt

test-improvedgals-large-sendtoself-betterverlet-dtchange-msgmgmt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-large-sendtoself-betterverlet-dtchange-msgmgmt-floatonly-singleforceloop: test-improvedgals-large-sendtoself-betterverlet-dtchange-msgmgmt-floatonly

test-improvedgals-large-sendtoself-betterverlet-dtchange-msgmgmt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-large-sendtoself-betterverlet-dtchange-msgmgmt-singleforceloop: test-improvedgals-large-sendtoself-betterverlet-dtchange-msgmgmt

test-improvedgals-large-sendtoself-betterverlet-dtchange-floatonly: DFLAGS+=-DFLOAT_ONLY
test-improvedgals-large-sendtoself-betterverlet-dtchange-floatonly: test-improvedgals-large-sendtoself-betterverlet-dtchange

test-improvedgals-large-sendtoself-betterverlet-dtchange-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-large-sendtoself-betterverlet-dtchange-floatonly-singleforceloop: test-improvedgals-large-sendtoself-betterverlet-dtchange-floatonly

test-improvedgals-large-sendtoself-betterverlet-dtchange-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-large-sendtoself-betterverlet-dtchange-singleforceloop: test-improvedgals-large-sendtoself-betterverlet-dtchange

test-improvedgals-large-sendtoself-betterverlet-doublesqrt: DFLAGS+=-DOUBLE_SQRT
test-improvedgals-large-sendtoself-betterverlet-doublesqrt: test-improvedgals-large-sendtoself-betterverlet

test-improvedgals-large-sendtoself-betterverlet-doublesqrt-msgmgmt: DFLAGS+=-DMESSAGE_MANAGEMENT
test-improvedgals-large-sendtoself-betterverlet-doublesqrt-msgmgmt: test-improvedgals-large-sendtoself-betterverlet-doublesqrt

test-improvedgals-large-sendtoself-betterverlet-doublesqrt-msgmgmt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-improvedgals-large-sendtoself-betterverlet-doublesqrt-msgmgmt-floatonly: test-improvedgals-large-sendtoself-betterverlet-doublesqrt-msgmgmt

test-improvedgals-large-sendtoself-betterverlet-doublesqrt-msgmgmt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-large-sendtoself-betterverlet-doublesqrt-msgmgmt-floatonly-singleforceloop: test-improvedgals-large-sendtoself-betterverlet-doublesqrt-msgmgmt-floatonly

test-improvedgals-large-sendtoself-betterverlet-doublesqrt-msgmgmt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-large-sendtoself-betterverlet-doublesqrt-msgmgmt-singleforceloop: test-improvedgals-large-sendtoself-betterverlet-doublesqrt-msgmgmt

test-improvedgals-large-sendtoself-betterverlet-doublesqrt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-improvedgals-large-sendtoself-betterverlet-doublesqrt-floatonly: test-improvedgals-large-sendtoself-betterverlet-doublesqrt

test-improvedgals-large-sendtoself-betterverlet-doublesqrt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-large-sendtoself-betterverlet-doublesqrt-floatonly-singleforceloop: test-improvedgals-large-sendtoself-betterverlet-doublesqrt-floatonly

test-improvedgals-large-sendtoself-betterverlet-doublesqrt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-large-sendtoself-betterverlet-doublesqrt-singleforceloop: test-improvedgals-large-sendtoself-betterverlet-doublesqrt

test-improvedgals-large-sendtoself-betterverlet-msgmgmt: DFLAGS+=-DMESSAGE_MANAGEMENT
test-improvedgals-large-sendtoself-betterverlet-msgmgmt: test-improvedgals-large-sendtoself-betterverlet

test-improvedgals-large-sendtoself-betterverlet-msgmgmt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-improvedgals-large-sendtoself-betterverlet-msgmgmt-floatonly: test-improvedgals-large-sendtoself-betterverlet-msgmgmt

test-improvedgals-large-sendtoself-betterverlet-msgmgmt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-large-sendtoself-betterverlet-msgmgmt-floatonly-singleforceloop: test-improvedgals-large-sendtoself-betterverlet-msgmgmt-floatonly

test-improvedgals-large-sendtoself-betterverlet-msgmgmt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-large-sendtoself-betterverlet-msgmgmt-singleforceloop: test-improvedgals-large-sendtoself-betterverlet-msgmgmt

test-improvedgals-large-sendtoself-betterverlet-floatonly: DFLAGS+=-DFLOAT_ONLY
test-improvedgals-large-sendtoself-betterverlet-floatonly: test-improvedgals-large-sendtoself-betterverlet

test-improvedgals-large-sendtoself-betterverlet-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-large-sendtoself-betterverlet-floatonly-singleforceloop: test-improvedgals-large-sendtoself-betterverlet-floatonly

test-improvedgals-large-sendtoself-betterverlet-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-large-sendtoself-betterverlet-singleforceloop: test-improvedgals-large-sendtoself-betterverlet

test-improvedgals-large-sendtoself-dtchange: DFLAGS+=-DSMALL_DT_EARLY
test-improvedgals-large-sendtoself-dtchange: test-improvedgals-large-sendtoself

test-improvedgals-large-sendtoself-dtchange-doublesqrt: DFLAGS+=-DOUBLE_SQRT
test-improvedgals-large-sendtoself-dtchange-doublesqrt: test-improvedgals-large-sendtoself-dtchange

test-improvedgals-large-sendtoself-dtchange-doublesqrt-msgmgmt: DFLAGS+=-DMESSAGE_MANAGEMENT
test-improvedgals-large-sendtoself-dtchange-doublesqrt-msgmgmt: test-improvedgals-large-sendtoself-dtchange-doublesqrt

test-improvedgals-large-sendtoself-dtchange-doublesqrt-msgmgmt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-improvedgals-large-sendtoself-dtchange-doublesqrt-msgmgmt-floatonly: test-improvedgals-large-sendtoself-dtchange-doublesqrt-msgmgmt

test-improvedgals-large-sendtoself-dtchange-doublesqrt-msgmgmt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-large-sendtoself-dtchange-doublesqrt-msgmgmt-floatonly-singleforceloop: test-improvedgals-large-sendtoself-dtchange-doublesqrt-msgmgmt-floatonly

test-improvedgals-large-sendtoself-dtchange-doublesqrt-msgmgmt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-large-sendtoself-dtchange-doublesqrt-msgmgmt-singleforceloop: test-improvedgals-large-sendtoself-dtchange-doublesqrt-msgmgmt

test-improvedgals-large-sendtoself-dtchange-doublesqrt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-improvedgals-large-sendtoself-dtchange-doublesqrt-floatonly: test-improvedgals-large-sendtoself-dtchange-doublesqrt

test-improvedgals-large-sendtoself-dtchange-doublesqrt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-large-sendtoself-dtchange-doublesqrt-floatonly-singleforceloop: test-improvedgals-large-sendtoself-dtchange-doublesqrt-floatonly

test-improvedgals-large-sendtoself-dtchange-doublesqrt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-large-sendtoself-dtchange-doublesqrt-singleforceloop: test-improvedgals-large-sendtoself-dtchange-doublesqrt

test-improvedgals-large-sendtoself-dtchange-msgmgmt: DFLAGS+=-DMESSAGE_MANAGEMENT
test-improvedgals-large-sendtoself-dtchange-msgmgmt: test-improvedgals-large-sendtoself-dtchange

test-improvedgals-large-sendtoself-dtchange-msgmgmt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-improvedgals-large-sendtoself-dtchange-msgmgmt-floatonly: test-improvedgals-large-sendtoself-dtchange-msgmgmt

test-improvedgals-large-sendtoself-dtchange-msgmgmt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-large-sendtoself-dtchange-msgmgmt-floatonly-singleforceloop: test-improvedgals-large-sendtoself-dtchange-msgmgmt-floatonly

test-improvedgals-large-sendtoself-dtchange-msgmgmt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-large-sendtoself-dtchange-msgmgmt-singleforceloop: test-improvedgals-large-sendtoself-dtchange-msgmgmt

test-improvedgals-large-sendtoself-dtchange-floatonly: DFLAGS+=-DFLOAT_ONLY
test-improvedgals-large-sendtoself-dtchange-floatonly: test-improvedgals-large-sendtoself-dtchange

test-improvedgals-large-sendtoself-dtchange-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-large-sendtoself-dtchange-floatonly-singleforceloop: test-improvedgals-large-sendtoself-dtchange-floatonly

test-improvedgals-large-sendtoself-dtchange-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-large-sendtoself-dtchange-singleforceloop: test-improvedgals-large-sendtoself-dtchange

test-improvedgals-large-sendtoself-doublesqrt: DFLAGS+=-DOUBLE_SQRT
test-improvedgals-large-sendtoself-doublesqrt: test-improvedgals-large-sendtoself

test-improvedgals-large-sendtoself-doublesqrt-msgmgmt: DFLAGS+=-DMESSAGE_MANAGEMENT
test-improvedgals-large-sendtoself-doublesqrt-msgmgmt: test-improvedgals-large-sendtoself-doublesqrt

test-improvedgals-large-sendtoself-doublesqrt-msgmgmt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-improvedgals-large-sendtoself-doublesqrt-msgmgmt-floatonly: test-improvedgals-large-sendtoself-doublesqrt-msgmgmt

test-improvedgals-large-sendtoself-doublesqrt-msgmgmt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-large-sendtoself-doublesqrt-msgmgmt-floatonly-singleforceloop: test-improvedgals-large-sendtoself-doublesqrt-msgmgmt-floatonly

test-improvedgals-large-sendtoself-doublesqrt-msgmgmt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-large-sendtoself-doublesqrt-msgmgmt-singleforceloop: test-improvedgals-large-sendtoself-doublesqrt-msgmgmt

test-improvedgals-large-sendtoself-doublesqrt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-improvedgals-large-sendtoself-doublesqrt-floatonly: test-improvedgals-large-sendtoself-doublesqrt

test-improvedgals-large-sendtoself-doublesqrt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-large-sendtoself-doublesqrt-floatonly-singleforceloop: test-improvedgals-large-sendtoself-doublesqrt-floatonly

test-improvedgals-large-sendtoself-doublesqrt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-large-sendtoself-doublesqrt-singleforceloop: test-improvedgals-large-sendtoself-doublesqrt

test-improvedgals-large-sendtoself-msgmgmt: DFLAGS+=-DMESSAGE_MANAGEMENT
test-improvedgals-large-sendtoself-msgmgmt: test-improvedgals-large-sendtoself

test-improvedgals-large-sendtoself-msgmgmt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-improvedgals-large-sendtoself-msgmgmt-floatonly: test-improvedgals-large-sendtoself-msgmgmt

test-improvedgals-large-sendtoself-msgmgmt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-large-sendtoself-msgmgmt-floatonly-singleforceloop: test-improvedgals-large-sendtoself-msgmgmt-floatonly

test-improvedgals-large-sendtoself-msgmgmt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-large-sendtoself-msgmgmt-singleforceloop: test-improvedgals-large-sendtoself-msgmgmt

test-improvedgals-large-sendtoself-floatonly: DFLAGS+=-DFLOAT_ONLY
test-improvedgals-large-sendtoself-floatonly: test-improvedgals-large-sendtoself

test-improvedgals-large-sendtoself-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-large-sendtoself-floatonly-singleforceloop: test-improvedgals-large-sendtoself-floatonly

test-improvedgals-large-sendtoself-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-large-sendtoself-singleforceloop: test-improvedgals-large-sendtoself

test-improvedgals-bonds: DFLAGS+=-DBONDS
test-improvedgals-bonds: test-improvedgals

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

# **TEST Local calculation method: sendtoself**
test-improvedgals-bonds-sendtoself: DFLAGS+=-DSEND_TO_SELF
test-improvedgals-bonds-sendtoself: test-improvedgals-bonds

test-improvedgals-bonds-sendtoself-betterverlet: DFLAGS+=-DBETTER_VERLET
test-improvedgals-bonds-sendtoself-betterverlet: test-improvedgals-bonds-sendtoself

test-improvedgals-bonds-sendtoself-betterverlet-dtchange: DFLAGS+=-DSMALL_DT_EARLY
test-improvedgals-bonds-sendtoself-betterverlet-dtchange: test-improvedgals-bonds-sendtoself-betterverlet

test-improvedgals-bonds-sendtoself-betterverlet-dtchange-doublesqrt: DFLAGS+=-DOUBLE_SQRT
test-improvedgals-bonds-sendtoself-betterverlet-dtchange-doublesqrt: test-improvedgals-bonds-sendtoself-betterverlet-dtchange

test-improvedgals-bonds-sendtoself-betterverlet-dtchange-doublesqrt-msgmgmt: DFLAGS+=-DMESSAGE_MANAGEMENT
test-improvedgals-bonds-sendtoself-betterverlet-dtchange-doublesqrt-msgmgmt: test-improvedgals-bonds-sendtoself-betterverlet-dtchange-doublesqrt

test-improvedgals-bonds-sendtoself-betterverlet-dtchange-doublesqrt-msgmgmt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-improvedgals-bonds-sendtoself-betterverlet-dtchange-doublesqrt-msgmgmt-floatonly: test-improvedgals-bonds-sendtoself-betterverlet-dtchange-doublesqrt-msgmgmt

test-improvedgals-bonds-sendtoself-betterverlet-dtchange-doublesqrt-msgmgmt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-bonds-sendtoself-betterverlet-dtchange-doublesqrt-msgmgmt-floatonly-singleforceloop: test-improvedgals-bonds-sendtoself-betterverlet-dtchange-doublesqrt-msgmgmt-floatonly

test-improvedgals-bonds-sendtoself-betterverlet-dtchange-doublesqrt-msgmgmt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-bonds-sendtoself-betterverlet-dtchange-doublesqrt-msgmgmt-singleforceloop: test-improvedgals-bonds-sendtoself-betterverlet-dtchange-doublesqrt-msgmgmt

test-improvedgals-bonds-sendtoself-betterverlet-dtchange-doublesqrt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-improvedgals-bonds-sendtoself-betterverlet-dtchange-doublesqrt-floatonly: test-improvedgals-bonds-sendtoself-betterverlet-dtchange-doublesqrt

test-improvedgals-bonds-sendtoself-betterverlet-dtchange-doublesqrt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-bonds-sendtoself-betterverlet-dtchange-doublesqrt-floatonly-singleforceloop: test-improvedgals-bonds-sendtoself-betterverlet-dtchange-doublesqrt-floatonly

test-improvedgals-bonds-sendtoself-betterverlet-dtchange-doublesqrt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-bonds-sendtoself-betterverlet-dtchange-doublesqrt-singleforceloop: test-improvedgals-bonds-sendtoself-betterverlet-dtchange-doublesqrt

test-improvedgals-bonds-sendtoself-betterverlet-dtchange-msgmgmt: DFLAGS+=-DMESSAGE_MANAGEMENT
test-improvedgals-bonds-sendtoself-betterverlet-dtchange-msgmgmt: test-improvedgals-bonds-sendtoself-betterverlet-dtchange

test-improvedgals-bonds-sendtoself-betterverlet-dtchange-msgmgmt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-improvedgals-bonds-sendtoself-betterverlet-dtchange-msgmgmt-floatonly: test-improvedgals-bonds-sendtoself-betterverlet-dtchange-msgmgmt

test-improvedgals-bonds-sendtoself-betterverlet-dtchange-msgmgmt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-bonds-sendtoself-betterverlet-dtchange-msgmgmt-floatonly-singleforceloop: test-improvedgals-bonds-sendtoself-betterverlet-dtchange-msgmgmt-floatonly

test-improvedgals-bonds-sendtoself-betterverlet-dtchange-msgmgmt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-bonds-sendtoself-betterverlet-dtchange-msgmgmt-singleforceloop: test-improvedgals-bonds-sendtoself-betterverlet-dtchange-msgmgmt

test-improvedgals-bonds-sendtoself-betterverlet-dtchange-floatonly: DFLAGS+=-DFLOAT_ONLY
test-improvedgals-bonds-sendtoself-betterverlet-dtchange-floatonly: test-improvedgals-bonds-sendtoself-betterverlet-dtchange

test-improvedgals-bonds-sendtoself-betterverlet-dtchange-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-bonds-sendtoself-betterverlet-dtchange-floatonly-singleforceloop: test-improvedgals-bonds-sendtoself-betterverlet-dtchange-floatonly

test-improvedgals-bonds-sendtoself-betterverlet-dtchange-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-bonds-sendtoself-betterverlet-dtchange-singleforceloop: test-improvedgals-bonds-sendtoself-betterverlet-dtchange

test-improvedgals-bonds-sendtoself-betterverlet-doublesqrt: DFLAGS+=-DOUBLE_SQRT
test-improvedgals-bonds-sendtoself-betterverlet-doublesqrt: test-improvedgals-bonds-sendtoself-betterverlet

test-improvedgals-bonds-sendtoself-betterverlet-doublesqrt-msgmgmt: DFLAGS+=-DMESSAGE_MANAGEMENT
test-improvedgals-bonds-sendtoself-betterverlet-doublesqrt-msgmgmt: test-improvedgals-bonds-sendtoself-betterverlet-doublesqrt

test-improvedgals-bonds-sendtoself-betterverlet-doublesqrt-msgmgmt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-improvedgals-bonds-sendtoself-betterverlet-doublesqrt-msgmgmt-floatonly: test-improvedgals-bonds-sendtoself-betterverlet-doublesqrt-msgmgmt

test-improvedgals-bonds-sendtoself-betterverlet-doublesqrt-msgmgmt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-bonds-sendtoself-betterverlet-doublesqrt-msgmgmt-floatonly-singleforceloop: test-improvedgals-bonds-sendtoself-betterverlet-doublesqrt-msgmgmt-floatonly

test-improvedgals-bonds-sendtoself-betterverlet-doublesqrt-msgmgmt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-bonds-sendtoself-betterverlet-doublesqrt-msgmgmt-singleforceloop: test-improvedgals-bonds-sendtoself-betterverlet-doublesqrt-msgmgmt

test-improvedgals-bonds-sendtoself-betterverlet-doublesqrt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-improvedgals-bonds-sendtoself-betterverlet-doublesqrt-floatonly: test-improvedgals-bonds-sendtoself-betterverlet-doublesqrt

test-improvedgals-bonds-sendtoself-betterverlet-doublesqrt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-bonds-sendtoself-betterverlet-doublesqrt-floatonly-singleforceloop: test-improvedgals-bonds-sendtoself-betterverlet-doublesqrt-floatonly

test-improvedgals-bonds-sendtoself-betterverlet-doublesqrt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-bonds-sendtoself-betterverlet-doublesqrt-singleforceloop: test-improvedgals-bonds-sendtoself-betterverlet-doublesqrt

test-improvedgals-bonds-sendtoself-betterverlet-msgmgmt: DFLAGS+=-DMESSAGE_MANAGEMENT
test-improvedgals-bonds-sendtoself-betterverlet-msgmgmt: test-improvedgals-bonds-sendtoself-betterverlet

test-improvedgals-bonds-sendtoself-betterverlet-msgmgmt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-improvedgals-bonds-sendtoself-betterverlet-msgmgmt-floatonly: test-improvedgals-bonds-sendtoself-betterverlet-msgmgmt

test-improvedgals-bonds-sendtoself-betterverlet-msgmgmt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-bonds-sendtoself-betterverlet-msgmgmt-floatonly-singleforceloop: test-improvedgals-bonds-sendtoself-betterverlet-msgmgmt-floatonly

test-improvedgals-bonds-sendtoself-betterverlet-msgmgmt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-bonds-sendtoself-betterverlet-msgmgmt-singleforceloop: test-improvedgals-bonds-sendtoself-betterverlet-msgmgmt

test-improvedgals-bonds-sendtoself-betterverlet-floatonly: DFLAGS+=-DFLOAT_ONLY
test-improvedgals-bonds-sendtoself-betterverlet-floatonly: test-improvedgals-bonds-sendtoself-betterverlet

test-improvedgals-bonds-sendtoself-betterverlet-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-bonds-sendtoself-betterverlet-floatonly-singleforceloop: test-improvedgals-bonds-sendtoself-betterverlet-floatonly

test-improvedgals-bonds-sendtoself-betterverlet-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-bonds-sendtoself-betterverlet-singleforceloop: test-improvedgals-bonds-sendtoself-betterverlet

test-improvedgals-bonds-sendtoself-dtchange: DFLAGS+=-DSMALL_DT_EARLY
test-improvedgals-bonds-sendtoself-dtchange: test-improvedgals-bonds-sendtoself

test-improvedgals-bonds-sendtoself-dtchange-doublesqrt: DFLAGS+=-DOUBLE_SQRT
test-improvedgals-bonds-sendtoself-dtchange-doublesqrt: test-improvedgals-bonds-sendtoself-dtchange

test-improvedgals-bonds-sendtoself-dtchange-doublesqrt-msgmgmt: DFLAGS+=-DMESSAGE_MANAGEMENT
test-improvedgals-bonds-sendtoself-dtchange-doublesqrt-msgmgmt: test-improvedgals-bonds-sendtoself-dtchange-doublesqrt

test-improvedgals-bonds-sendtoself-dtchange-doublesqrt-msgmgmt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-improvedgals-bonds-sendtoself-dtchange-doublesqrt-msgmgmt-floatonly: test-improvedgals-bonds-sendtoself-dtchange-doublesqrt-msgmgmt

test-improvedgals-bonds-sendtoself-dtchange-doublesqrt-msgmgmt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-bonds-sendtoself-dtchange-doublesqrt-msgmgmt-floatonly-singleforceloop: test-improvedgals-bonds-sendtoself-dtchange-doublesqrt-msgmgmt-floatonly

test-improvedgals-bonds-sendtoself-dtchange-doublesqrt-msgmgmt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-bonds-sendtoself-dtchange-doublesqrt-msgmgmt-singleforceloop: test-improvedgals-bonds-sendtoself-dtchange-doublesqrt-msgmgmt

test-improvedgals-bonds-sendtoself-dtchange-doublesqrt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-improvedgals-bonds-sendtoself-dtchange-doublesqrt-floatonly: test-improvedgals-bonds-sendtoself-dtchange-doublesqrt

test-improvedgals-bonds-sendtoself-dtchange-doublesqrt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-bonds-sendtoself-dtchange-doublesqrt-floatonly-singleforceloop: test-improvedgals-bonds-sendtoself-dtchange-doublesqrt-floatonly

test-improvedgals-bonds-sendtoself-dtchange-doublesqrt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-bonds-sendtoself-dtchange-doublesqrt-singleforceloop: test-improvedgals-bonds-sendtoself-dtchange-doublesqrt

test-improvedgals-bonds-sendtoself-dtchange-msgmgmt: DFLAGS+=-DMESSAGE_MANAGEMENT
test-improvedgals-bonds-sendtoself-dtchange-msgmgmt: test-improvedgals-bonds-sendtoself-dtchange

test-improvedgals-bonds-sendtoself-dtchange-msgmgmt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-improvedgals-bonds-sendtoself-dtchange-msgmgmt-floatonly: test-improvedgals-bonds-sendtoself-dtchange-msgmgmt

test-improvedgals-bonds-sendtoself-dtchange-msgmgmt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-bonds-sendtoself-dtchange-msgmgmt-floatonly-singleforceloop: test-improvedgals-bonds-sendtoself-dtchange-msgmgmt-floatonly

test-improvedgals-bonds-sendtoself-dtchange-msgmgmt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-bonds-sendtoself-dtchange-msgmgmt-singleforceloop: test-improvedgals-bonds-sendtoself-dtchange-msgmgmt

test-improvedgals-bonds-sendtoself-dtchange-floatonly: DFLAGS+=-DFLOAT_ONLY
test-improvedgals-bonds-sendtoself-dtchange-floatonly: test-improvedgals-bonds-sendtoself-dtchange

test-improvedgals-bonds-sendtoself-dtchange-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-bonds-sendtoself-dtchange-floatonly-singleforceloop: test-improvedgals-bonds-sendtoself-dtchange-floatonly

test-improvedgals-bonds-sendtoself-dtchange-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-bonds-sendtoself-dtchange-singleforceloop: test-improvedgals-bonds-sendtoself-dtchange

test-improvedgals-bonds-sendtoself-doublesqrt: DFLAGS+=-DOUBLE_SQRT
test-improvedgals-bonds-sendtoself-doublesqrt: test-improvedgals-bonds-sendtoself

test-improvedgals-bonds-sendtoself-doublesqrt-msgmgmt: DFLAGS+=-DMESSAGE_MANAGEMENT
test-improvedgals-bonds-sendtoself-doublesqrt-msgmgmt: test-improvedgals-bonds-sendtoself-doublesqrt

test-improvedgals-bonds-sendtoself-doublesqrt-msgmgmt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-improvedgals-bonds-sendtoself-doublesqrt-msgmgmt-floatonly: test-improvedgals-bonds-sendtoself-doublesqrt-msgmgmt

test-improvedgals-bonds-sendtoself-doublesqrt-msgmgmt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-bonds-sendtoself-doublesqrt-msgmgmt-floatonly-singleforceloop: test-improvedgals-bonds-sendtoself-doublesqrt-msgmgmt-floatonly

test-improvedgals-bonds-sendtoself-doublesqrt-msgmgmt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-bonds-sendtoself-doublesqrt-msgmgmt-singleforceloop: test-improvedgals-bonds-sendtoself-doublesqrt-msgmgmt

test-improvedgals-bonds-sendtoself-doublesqrt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-improvedgals-bonds-sendtoself-doublesqrt-floatonly: test-improvedgals-bonds-sendtoself-doublesqrt

test-improvedgals-bonds-sendtoself-doublesqrt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-bonds-sendtoself-doublesqrt-floatonly-singleforceloop: test-improvedgals-bonds-sendtoself-doublesqrt-floatonly

test-improvedgals-bonds-sendtoself-doublesqrt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-bonds-sendtoself-doublesqrt-singleforceloop: test-improvedgals-bonds-sendtoself-doublesqrt

test-improvedgals-bonds-sendtoself-msgmgmt: DFLAGS+=-DMESSAGE_MANAGEMENT
test-improvedgals-bonds-sendtoself-msgmgmt: test-improvedgals-bonds-sendtoself

test-improvedgals-bonds-sendtoself-msgmgmt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-improvedgals-bonds-sendtoself-msgmgmt-floatonly: test-improvedgals-bonds-sendtoself-msgmgmt

test-improvedgals-bonds-sendtoself-msgmgmt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-bonds-sendtoself-msgmgmt-floatonly-singleforceloop: test-improvedgals-bonds-sendtoself-msgmgmt-floatonly

test-improvedgals-bonds-sendtoself-msgmgmt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-bonds-sendtoself-msgmgmt-singleforceloop: test-improvedgals-bonds-sendtoself-msgmgmt

test-improvedgals-bonds-sendtoself-floatonly: DFLAGS+=-DFLOAT_ONLY
test-improvedgals-bonds-sendtoself-floatonly: test-improvedgals-bonds-sendtoself

test-improvedgals-bonds-sendtoself-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-bonds-sendtoself-floatonly-singleforceloop: test-improvedgals-bonds-sendtoself-floatonly

test-improvedgals-bonds-sendtoself-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-improvedgals-bonds-sendtoself-singleforceloop: test-improvedgals-bonds-sendtoself


# ************** TEST Simulator: serial**************

test-serial: OBJS=SERIAL_OBJS
test-serial: DFLAGS+=-DTESTING -DSERIAL
test-serial: test

# **TEST Local calculation method: onebyone**
test-serial-onebyone: DFLAGS+=-DONE_BY_ONE
test-serial-onebyone: test-serial

test-serial-onebyone-betterverlet: DFLAGS+=-DBETTER_VERLET
test-serial-onebyone-betterverlet: test-serial-onebyone

test-serial-onebyone-betterverlet-dtchange: DFLAGS+=-DSMALL_DT_EARLY
test-serial-onebyone-betterverlet-dtchange: test-serial-onebyone-betterverlet

test-serial-onebyone-betterverlet-dtchange-doublesqrt: DFLAGS+=-DOUBLE_SQRT
test-serial-onebyone-betterverlet-dtchange-doublesqrt: test-serial-onebyone-betterverlet-dtchange

test-serial-onebyone-betterverlet-dtchange-doublesqrt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-serial-onebyone-betterverlet-dtchange-doublesqrt-floatonly: test-serial-onebyone-betterverlet-dtchange-doublesqrt

test-serial-onebyone-betterverlet-dtchange-doublesqrt-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-serial-onebyone-betterverlet-dtchange-doublesqrt-floatonly-reducedlocalcalcs: test-serial-onebyone-betterverlet-dtchange-doublesqrt-floatonly

test-serial-onebyone-betterverlet-dtchange-doublesqrt-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-serial-onebyone-betterverlet-dtchange-doublesqrt-floatonly-reducedlocalcalcs-singleforceloop: test-serial-onebyone-betterverlet-dtchange-doublesqrt-floatonly-reducedlocalcalcs

test-serial-onebyone-betterverlet-dtchange-doublesqrt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-serial-onebyone-betterverlet-dtchange-doublesqrt-floatonly-singleforceloop: test-serial-onebyone-betterverlet-dtchange-doublesqrt-floatonly

test-serial-onebyone-betterverlet-dtchange-doublesqrt-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-serial-onebyone-betterverlet-dtchange-doublesqrt-reducedlocalcalcs: test-serial-onebyone-betterverlet-dtchange-doublesqrt

test-serial-onebyone-betterverlet-dtchange-doublesqrt-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-serial-onebyone-betterverlet-dtchange-doublesqrt-reducedlocalcalcs-singleforceloop: test-serial-onebyone-betterverlet-dtchange-doublesqrt-reducedlocalcalcs

test-serial-onebyone-betterverlet-dtchange-doublesqrt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-serial-onebyone-betterverlet-dtchange-doublesqrt-singleforceloop: test-serial-onebyone-betterverlet-dtchange-doublesqrt

test-serial-onebyone-betterverlet-dtchange-floatonly: DFLAGS+=-DFLOAT_ONLY
test-serial-onebyone-betterverlet-dtchange-floatonly: test-serial-onebyone-betterverlet-dtchange

test-serial-onebyone-betterverlet-dtchange-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-serial-onebyone-betterverlet-dtchange-floatonly-reducedlocalcalcs: test-serial-onebyone-betterverlet-dtchange-floatonly

test-serial-onebyone-betterverlet-dtchange-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-serial-onebyone-betterverlet-dtchange-floatonly-reducedlocalcalcs-singleforceloop: test-serial-onebyone-betterverlet-dtchange-floatonly-reducedlocalcalcs

test-serial-onebyone-betterverlet-dtchange-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-serial-onebyone-betterverlet-dtchange-floatonly-singleforceloop: test-serial-onebyone-betterverlet-dtchange-floatonly

test-serial-onebyone-betterverlet-dtchange-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-serial-onebyone-betterverlet-dtchange-reducedlocalcalcs: test-serial-onebyone-betterverlet-dtchange

test-serial-onebyone-betterverlet-dtchange-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-serial-onebyone-betterverlet-dtchange-reducedlocalcalcs-singleforceloop: test-serial-onebyone-betterverlet-dtchange-reducedlocalcalcs

test-serial-onebyone-betterverlet-dtchange-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-serial-onebyone-betterverlet-dtchange-singleforceloop: test-serial-onebyone-betterverlet-dtchange

test-serial-onebyone-betterverlet-doublesqrt: DFLAGS+=-DOUBLE_SQRT
test-serial-onebyone-betterverlet-doublesqrt: test-serial-onebyone-betterverlet

test-serial-onebyone-betterverlet-doublesqrt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-serial-onebyone-betterverlet-doublesqrt-floatonly: test-serial-onebyone-betterverlet-doublesqrt

test-serial-onebyone-betterverlet-doublesqrt-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-serial-onebyone-betterverlet-doublesqrt-floatonly-reducedlocalcalcs: test-serial-onebyone-betterverlet-doublesqrt-floatonly

test-serial-onebyone-betterverlet-doublesqrt-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-serial-onebyone-betterverlet-doublesqrt-floatonly-reducedlocalcalcs-singleforceloop: test-serial-onebyone-betterverlet-doublesqrt-floatonly-reducedlocalcalcs

test-serial-onebyone-betterverlet-doublesqrt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-serial-onebyone-betterverlet-doublesqrt-floatonly-singleforceloop: test-serial-onebyone-betterverlet-doublesqrt-floatonly

test-serial-onebyone-betterverlet-doublesqrt-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-serial-onebyone-betterverlet-doublesqrt-reducedlocalcalcs: test-serial-onebyone-betterverlet-doublesqrt

test-serial-onebyone-betterverlet-doublesqrt-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-serial-onebyone-betterverlet-doublesqrt-reducedlocalcalcs-singleforceloop: test-serial-onebyone-betterverlet-doublesqrt-reducedlocalcalcs

test-serial-onebyone-betterverlet-doublesqrt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-serial-onebyone-betterverlet-doublesqrt-singleforceloop: test-serial-onebyone-betterverlet-doublesqrt

test-serial-onebyone-betterverlet-floatonly: DFLAGS+=-DFLOAT_ONLY
test-serial-onebyone-betterverlet-floatonly: test-serial-onebyone-betterverlet

test-serial-onebyone-betterverlet-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-serial-onebyone-betterverlet-floatonly-reducedlocalcalcs: test-serial-onebyone-betterverlet-floatonly

test-serial-onebyone-betterverlet-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-serial-onebyone-betterverlet-floatonly-reducedlocalcalcs-singleforceloop: test-serial-onebyone-betterverlet-floatonly-reducedlocalcalcs

test-serial-onebyone-betterverlet-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-serial-onebyone-betterverlet-floatonly-singleforceloop: test-serial-onebyone-betterverlet-floatonly

test-serial-onebyone-betterverlet-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-serial-onebyone-betterverlet-reducedlocalcalcs: test-serial-onebyone-betterverlet

test-serial-onebyone-betterverlet-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-serial-onebyone-betterverlet-reducedlocalcalcs-singleforceloop: test-serial-onebyone-betterverlet-reducedlocalcalcs

test-serial-onebyone-betterverlet-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-serial-onebyone-betterverlet-singleforceloop: test-serial-onebyone-betterverlet

test-serial-onebyone-dtchange: DFLAGS+=-DSMALL_DT_EARLY
test-serial-onebyone-dtchange: test-serial-onebyone

test-serial-onebyone-dtchange-doublesqrt: DFLAGS+=-DOUBLE_SQRT
test-serial-onebyone-dtchange-doublesqrt: test-serial-onebyone-dtchange

test-serial-onebyone-dtchange-doublesqrt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-serial-onebyone-dtchange-doublesqrt-floatonly: test-serial-onebyone-dtchange-doublesqrt

test-serial-onebyone-dtchange-doublesqrt-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-serial-onebyone-dtchange-doublesqrt-floatonly-reducedlocalcalcs: test-serial-onebyone-dtchange-doublesqrt-floatonly

test-serial-onebyone-dtchange-doublesqrt-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-serial-onebyone-dtchange-doublesqrt-floatonly-reducedlocalcalcs-singleforceloop: test-serial-onebyone-dtchange-doublesqrt-floatonly-reducedlocalcalcs

test-serial-onebyone-dtchange-doublesqrt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-serial-onebyone-dtchange-doublesqrt-floatonly-singleforceloop: test-serial-onebyone-dtchange-doublesqrt-floatonly

test-serial-onebyone-dtchange-doublesqrt-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-serial-onebyone-dtchange-doublesqrt-reducedlocalcalcs: test-serial-onebyone-dtchange-doublesqrt

test-serial-onebyone-dtchange-doublesqrt-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-serial-onebyone-dtchange-doublesqrt-reducedlocalcalcs-singleforceloop: test-serial-onebyone-dtchange-doublesqrt-reducedlocalcalcs

test-serial-onebyone-dtchange-doublesqrt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-serial-onebyone-dtchange-doublesqrt-singleforceloop: test-serial-onebyone-dtchange-doublesqrt

test-serial-onebyone-dtchange-floatonly: DFLAGS+=-DFLOAT_ONLY
test-serial-onebyone-dtchange-floatonly: test-serial-onebyone-dtchange

test-serial-onebyone-dtchange-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-serial-onebyone-dtchange-floatonly-reducedlocalcalcs: test-serial-onebyone-dtchange-floatonly

test-serial-onebyone-dtchange-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-serial-onebyone-dtchange-floatonly-reducedlocalcalcs-singleforceloop: test-serial-onebyone-dtchange-floatonly-reducedlocalcalcs

test-serial-onebyone-dtchange-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-serial-onebyone-dtchange-floatonly-singleforceloop: test-serial-onebyone-dtchange-floatonly

test-serial-onebyone-dtchange-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-serial-onebyone-dtchange-reducedlocalcalcs: test-serial-onebyone-dtchange

test-serial-onebyone-dtchange-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-serial-onebyone-dtchange-reducedlocalcalcs-singleforceloop: test-serial-onebyone-dtchange-reducedlocalcalcs

test-serial-onebyone-dtchange-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-serial-onebyone-dtchange-singleforceloop: test-serial-onebyone-dtchange

test-serial-onebyone-doublesqrt: DFLAGS+=-DOUBLE_SQRT
test-serial-onebyone-doublesqrt: test-serial-onebyone

test-serial-onebyone-doublesqrt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-serial-onebyone-doublesqrt-floatonly: test-serial-onebyone-doublesqrt

test-serial-onebyone-doublesqrt-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-serial-onebyone-doublesqrt-floatonly-reducedlocalcalcs: test-serial-onebyone-doublesqrt-floatonly

test-serial-onebyone-doublesqrt-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-serial-onebyone-doublesqrt-floatonly-reducedlocalcalcs-singleforceloop: test-serial-onebyone-doublesqrt-floatonly-reducedlocalcalcs

test-serial-onebyone-doublesqrt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-serial-onebyone-doublesqrt-floatonly-singleforceloop: test-serial-onebyone-doublesqrt-floatonly

test-serial-onebyone-doublesqrt-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-serial-onebyone-doublesqrt-reducedlocalcalcs: test-serial-onebyone-doublesqrt

test-serial-onebyone-doublesqrt-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-serial-onebyone-doublesqrt-reducedlocalcalcs-singleforceloop: test-serial-onebyone-doublesqrt-reducedlocalcalcs

test-serial-onebyone-doublesqrt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-serial-onebyone-doublesqrt-singleforceloop: test-serial-onebyone-doublesqrt

test-serial-onebyone-floatonly: DFLAGS+=-DFLOAT_ONLY
test-serial-onebyone-floatonly: test-serial-onebyone

test-serial-onebyone-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-serial-onebyone-floatonly-reducedlocalcalcs: test-serial-onebyone-floatonly

test-serial-onebyone-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-serial-onebyone-floatonly-reducedlocalcalcs-singleforceloop: test-serial-onebyone-floatonly-reducedlocalcalcs

test-serial-onebyone-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-serial-onebyone-floatonly-singleforceloop: test-serial-onebyone-floatonly

test-serial-onebyone-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-serial-onebyone-reducedlocalcalcs: test-serial-onebyone

test-serial-onebyone-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-serial-onebyone-reducedlocalcalcs-singleforceloop: test-serial-onebyone-reducedlocalcalcs

test-serial-onebyone-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-serial-onebyone-singleforceloop: test-serial-onebyone

# **TEST Local calculation method: sendtoself**
test-serial-sendtoself: DFLAGS+=-DSEND_TO_SELF
test-serial-sendtoself: test-serial

test-serial-sendtoself-betterverlet: DFLAGS+=-DBETTER_VERLET
test-serial-sendtoself-betterverlet: test-serial-sendtoself

test-serial-sendtoself-betterverlet-dtchange: DFLAGS+=-DSMALL_DT_EARLY
test-serial-sendtoself-betterverlet-dtchange: test-serial-sendtoself-betterverlet

test-serial-sendtoself-betterverlet-dtchange-doublesqrt: DFLAGS+=-DOUBLE_SQRT
test-serial-sendtoself-betterverlet-dtchange-doublesqrt: test-serial-sendtoself-betterverlet-dtchange

test-serial-sendtoself-betterverlet-dtchange-doublesqrt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-serial-sendtoself-betterverlet-dtchange-doublesqrt-floatonly: test-serial-sendtoself-betterverlet-dtchange-doublesqrt

test-serial-sendtoself-betterverlet-dtchange-doublesqrt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-serial-sendtoself-betterverlet-dtchange-doublesqrt-floatonly-singleforceloop: test-serial-sendtoself-betterverlet-dtchange-doublesqrt-floatonly

test-serial-sendtoself-betterverlet-dtchange-doublesqrt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-serial-sendtoself-betterverlet-dtchange-doublesqrt-singleforceloop: test-serial-sendtoself-betterverlet-dtchange-doublesqrt

test-serial-sendtoself-betterverlet-dtchange-floatonly: DFLAGS+=-DFLOAT_ONLY
test-serial-sendtoself-betterverlet-dtchange-floatonly: test-serial-sendtoself-betterverlet-dtchange

test-serial-sendtoself-betterverlet-dtchange-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-serial-sendtoself-betterverlet-dtchange-floatonly-singleforceloop: test-serial-sendtoself-betterverlet-dtchange-floatonly

test-serial-sendtoself-betterverlet-dtchange-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-serial-sendtoself-betterverlet-dtchange-singleforceloop: test-serial-sendtoself-betterverlet-dtchange

test-serial-sendtoself-betterverlet-doublesqrt: DFLAGS+=-DOUBLE_SQRT
test-serial-sendtoself-betterverlet-doublesqrt: test-serial-sendtoself-betterverlet

test-serial-sendtoself-betterverlet-doublesqrt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-serial-sendtoself-betterverlet-doublesqrt-floatonly: test-serial-sendtoself-betterverlet-doublesqrt

test-serial-sendtoself-betterverlet-doublesqrt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-serial-sendtoself-betterverlet-doublesqrt-floatonly-singleforceloop: test-serial-sendtoself-betterverlet-doublesqrt-floatonly

test-serial-sendtoself-betterverlet-doublesqrt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-serial-sendtoself-betterverlet-doublesqrt-singleforceloop: test-serial-sendtoself-betterverlet-doublesqrt

test-serial-sendtoself-betterverlet-floatonly: DFLAGS+=-DFLOAT_ONLY
test-serial-sendtoself-betterverlet-floatonly: test-serial-sendtoself-betterverlet

test-serial-sendtoself-betterverlet-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-serial-sendtoself-betterverlet-floatonly-singleforceloop: test-serial-sendtoself-betterverlet-floatonly

test-serial-sendtoself-betterverlet-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-serial-sendtoself-betterverlet-singleforceloop: test-serial-sendtoself-betterverlet

test-serial-sendtoself-dtchange: DFLAGS+=-DSMALL_DT_EARLY
test-serial-sendtoself-dtchange: test-serial-sendtoself

test-serial-sendtoself-dtchange-doublesqrt: DFLAGS+=-DOUBLE_SQRT
test-serial-sendtoself-dtchange-doublesqrt: test-serial-sendtoself-dtchange

test-serial-sendtoself-dtchange-doublesqrt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-serial-sendtoself-dtchange-doublesqrt-floatonly: test-serial-sendtoself-dtchange-doublesqrt

test-serial-sendtoself-dtchange-doublesqrt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-serial-sendtoself-dtchange-doublesqrt-floatonly-singleforceloop: test-serial-sendtoself-dtchange-doublesqrt-floatonly

test-serial-sendtoself-dtchange-doublesqrt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-serial-sendtoself-dtchange-doublesqrt-singleforceloop: test-serial-sendtoself-dtchange-doublesqrt

test-serial-sendtoself-dtchange-floatonly: DFLAGS+=-DFLOAT_ONLY
test-serial-sendtoself-dtchange-floatonly: test-serial-sendtoself-dtchange

test-serial-sendtoself-dtchange-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-serial-sendtoself-dtchange-floatonly-singleforceloop: test-serial-sendtoself-dtchange-floatonly

test-serial-sendtoself-dtchange-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-serial-sendtoself-dtchange-singleforceloop: test-serial-sendtoself-dtchange

test-serial-sendtoself-doublesqrt: DFLAGS+=-DOUBLE_SQRT
test-serial-sendtoself-doublesqrt: test-serial-sendtoself

test-serial-sendtoself-doublesqrt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-serial-sendtoself-doublesqrt-floatonly: test-serial-sendtoself-doublesqrt

test-serial-sendtoself-doublesqrt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-serial-sendtoself-doublesqrt-floatonly-singleforceloop: test-serial-sendtoself-doublesqrt-floatonly

test-serial-sendtoself-doublesqrt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-serial-sendtoself-doublesqrt-singleforceloop: test-serial-sendtoself-doublesqrt

test-serial-sendtoself-floatonly: DFLAGS+=-DFLOAT_ONLY
test-serial-sendtoself-floatonly: test-serial-sendtoself

test-serial-sendtoself-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-serial-sendtoself-floatonly-singleforceloop: test-serial-sendtoself-floatonly

test-serial-sendtoself-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-serial-sendtoself-singleforceloop: test-serial-sendtoself

test-serial-large: DFLAGS+=-DLARGE_TEST
test-serial-large: test-serial

# **TEST Local calculation method: onebyone**
test-serial-large-onebyone: DFLAGS+=-DONE_BY_ONE
test-serial-large-onebyone: test-serial-large

test-serial-large-onebyone-betterverlet: DFLAGS+=-DBETTER_VERLET
test-serial-large-onebyone-betterverlet: test-serial-large-onebyone

test-serial-large-onebyone-betterverlet-dtchange: DFLAGS+=-DSMALL_DT_EARLY
test-serial-large-onebyone-betterverlet-dtchange: test-serial-large-onebyone-betterverlet

test-serial-large-onebyone-betterverlet-dtchange-doublesqrt: DFLAGS+=-DOUBLE_SQRT
test-serial-large-onebyone-betterverlet-dtchange-doublesqrt: test-serial-large-onebyone-betterverlet-dtchange

test-serial-large-onebyone-betterverlet-dtchange-doublesqrt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-serial-large-onebyone-betterverlet-dtchange-doublesqrt-floatonly: test-serial-large-onebyone-betterverlet-dtchange-doublesqrt

test-serial-large-onebyone-betterverlet-dtchange-doublesqrt-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-serial-large-onebyone-betterverlet-dtchange-doublesqrt-floatonly-reducedlocalcalcs: test-serial-large-onebyone-betterverlet-dtchange-doublesqrt-floatonly

test-serial-large-onebyone-betterverlet-dtchange-doublesqrt-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-serial-large-onebyone-betterverlet-dtchange-doublesqrt-floatonly-reducedlocalcalcs-singleforceloop: test-serial-large-onebyone-betterverlet-dtchange-doublesqrt-floatonly-reducedlocalcalcs

test-serial-large-onebyone-betterverlet-dtchange-doublesqrt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-serial-large-onebyone-betterverlet-dtchange-doublesqrt-floatonly-singleforceloop: test-serial-large-onebyone-betterverlet-dtchange-doublesqrt-floatonly

test-serial-large-onebyone-betterverlet-dtchange-doublesqrt-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-serial-large-onebyone-betterverlet-dtchange-doublesqrt-reducedlocalcalcs: test-serial-large-onebyone-betterverlet-dtchange-doublesqrt

test-serial-large-onebyone-betterverlet-dtchange-doublesqrt-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-serial-large-onebyone-betterverlet-dtchange-doublesqrt-reducedlocalcalcs-singleforceloop: test-serial-large-onebyone-betterverlet-dtchange-doublesqrt-reducedlocalcalcs

test-serial-large-onebyone-betterverlet-dtchange-doublesqrt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-serial-large-onebyone-betterverlet-dtchange-doublesqrt-singleforceloop: test-serial-large-onebyone-betterverlet-dtchange-doublesqrt

test-serial-large-onebyone-betterverlet-dtchange-floatonly: DFLAGS+=-DFLOAT_ONLY
test-serial-large-onebyone-betterverlet-dtchange-floatonly: test-serial-large-onebyone-betterverlet-dtchange

test-serial-large-onebyone-betterverlet-dtchange-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-serial-large-onebyone-betterverlet-dtchange-floatonly-reducedlocalcalcs: test-serial-large-onebyone-betterverlet-dtchange-floatonly

test-serial-large-onebyone-betterverlet-dtchange-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-serial-large-onebyone-betterverlet-dtchange-floatonly-reducedlocalcalcs-singleforceloop: test-serial-large-onebyone-betterverlet-dtchange-floatonly-reducedlocalcalcs

test-serial-large-onebyone-betterverlet-dtchange-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-serial-large-onebyone-betterverlet-dtchange-floatonly-singleforceloop: test-serial-large-onebyone-betterverlet-dtchange-floatonly

test-serial-large-onebyone-betterverlet-dtchange-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-serial-large-onebyone-betterverlet-dtchange-reducedlocalcalcs: test-serial-large-onebyone-betterverlet-dtchange

test-serial-large-onebyone-betterverlet-dtchange-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-serial-large-onebyone-betterverlet-dtchange-reducedlocalcalcs-singleforceloop: test-serial-large-onebyone-betterverlet-dtchange-reducedlocalcalcs

test-serial-large-onebyone-betterverlet-dtchange-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-serial-large-onebyone-betterverlet-dtchange-singleforceloop: test-serial-large-onebyone-betterverlet-dtchange

test-serial-large-onebyone-betterverlet-doublesqrt: DFLAGS+=-DOUBLE_SQRT
test-serial-large-onebyone-betterverlet-doublesqrt: test-serial-large-onebyone-betterverlet

test-serial-large-onebyone-betterverlet-doublesqrt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-serial-large-onebyone-betterverlet-doublesqrt-floatonly: test-serial-large-onebyone-betterverlet-doublesqrt

test-serial-large-onebyone-betterverlet-doublesqrt-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-serial-large-onebyone-betterverlet-doublesqrt-floatonly-reducedlocalcalcs: test-serial-large-onebyone-betterverlet-doublesqrt-floatonly

test-serial-large-onebyone-betterverlet-doublesqrt-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-serial-large-onebyone-betterverlet-doublesqrt-floatonly-reducedlocalcalcs-singleforceloop: test-serial-large-onebyone-betterverlet-doublesqrt-floatonly-reducedlocalcalcs

test-serial-large-onebyone-betterverlet-doublesqrt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-serial-large-onebyone-betterverlet-doublesqrt-floatonly-singleforceloop: test-serial-large-onebyone-betterverlet-doublesqrt-floatonly

test-serial-large-onebyone-betterverlet-doublesqrt-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-serial-large-onebyone-betterverlet-doublesqrt-reducedlocalcalcs: test-serial-large-onebyone-betterverlet-doublesqrt

test-serial-large-onebyone-betterverlet-doublesqrt-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-serial-large-onebyone-betterverlet-doublesqrt-reducedlocalcalcs-singleforceloop: test-serial-large-onebyone-betterverlet-doublesqrt-reducedlocalcalcs

test-serial-large-onebyone-betterverlet-doublesqrt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-serial-large-onebyone-betterverlet-doublesqrt-singleforceloop: test-serial-large-onebyone-betterverlet-doublesqrt

test-serial-large-onebyone-betterverlet-floatonly: DFLAGS+=-DFLOAT_ONLY
test-serial-large-onebyone-betterverlet-floatonly: test-serial-large-onebyone-betterverlet

test-serial-large-onebyone-betterverlet-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-serial-large-onebyone-betterverlet-floatonly-reducedlocalcalcs: test-serial-large-onebyone-betterverlet-floatonly

test-serial-large-onebyone-betterverlet-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-serial-large-onebyone-betterverlet-floatonly-reducedlocalcalcs-singleforceloop: test-serial-large-onebyone-betterverlet-floatonly-reducedlocalcalcs

test-serial-large-onebyone-betterverlet-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-serial-large-onebyone-betterverlet-floatonly-singleforceloop: test-serial-large-onebyone-betterverlet-floatonly

test-serial-large-onebyone-betterverlet-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-serial-large-onebyone-betterverlet-reducedlocalcalcs: test-serial-large-onebyone-betterverlet

test-serial-large-onebyone-betterverlet-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-serial-large-onebyone-betterverlet-reducedlocalcalcs-singleforceloop: test-serial-large-onebyone-betterverlet-reducedlocalcalcs

test-serial-large-onebyone-betterverlet-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-serial-large-onebyone-betterverlet-singleforceloop: test-serial-large-onebyone-betterverlet

test-serial-large-onebyone-dtchange: DFLAGS+=-DSMALL_DT_EARLY
test-serial-large-onebyone-dtchange: test-serial-large-onebyone

test-serial-large-onebyone-dtchange-doublesqrt: DFLAGS+=-DOUBLE_SQRT
test-serial-large-onebyone-dtchange-doublesqrt: test-serial-large-onebyone-dtchange

test-serial-large-onebyone-dtchange-doublesqrt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-serial-large-onebyone-dtchange-doublesqrt-floatonly: test-serial-large-onebyone-dtchange-doublesqrt

test-serial-large-onebyone-dtchange-doublesqrt-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-serial-large-onebyone-dtchange-doublesqrt-floatonly-reducedlocalcalcs: test-serial-large-onebyone-dtchange-doublesqrt-floatonly

test-serial-large-onebyone-dtchange-doublesqrt-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-serial-large-onebyone-dtchange-doublesqrt-floatonly-reducedlocalcalcs-singleforceloop: test-serial-large-onebyone-dtchange-doublesqrt-floatonly-reducedlocalcalcs

test-serial-large-onebyone-dtchange-doublesqrt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-serial-large-onebyone-dtchange-doublesqrt-floatonly-singleforceloop: test-serial-large-onebyone-dtchange-doublesqrt-floatonly

test-serial-large-onebyone-dtchange-doublesqrt-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-serial-large-onebyone-dtchange-doublesqrt-reducedlocalcalcs: test-serial-large-onebyone-dtchange-doublesqrt

test-serial-large-onebyone-dtchange-doublesqrt-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-serial-large-onebyone-dtchange-doublesqrt-reducedlocalcalcs-singleforceloop: test-serial-large-onebyone-dtchange-doublesqrt-reducedlocalcalcs

test-serial-large-onebyone-dtchange-doublesqrt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-serial-large-onebyone-dtchange-doublesqrt-singleforceloop: test-serial-large-onebyone-dtchange-doublesqrt

test-serial-large-onebyone-dtchange-floatonly: DFLAGS+=-DFLOAT_ONLY
test-serial-large-onebyone-dtchange-floatonly: test-serial-large-onebyone-dtchange

test-serial-large-onebyone-dtchange-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-serial-large-onebyone-dtchange-floatonly-reducedlocalcalcs: test-serial-large-onebyone-dtchange-floatonly

test-serial-large-onebyone-dtchange-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-serial-large-onebyone-dtchange-floatonly-reducedlocalcalcs-singleforceloop: test-serial-large-onebyone-dtchange-floatonly-reducedlocalcalcs

test-serial-large-onebyone-dtchange-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-serial-large-onebyone-dtchange-floatonly-singleforceloop: test-serial-large-onebyone-dtchange-floatonly

test-serial-large-onebyone-dtchange-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-serial-large-onebyone-dtchange-reducedlocalcalcs: test-serial-large-onebyone-dtchange

test-serial-large-onebyone-dtchange-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-serial-large-onebyone-dtchange-reducedlocalcalcs-singleforceloop: test-serial-large-onebyone-dtchange-reducedlocalcalcs

test-serial-large-onebyone-dtchange-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-serial-large-onebyone-dtchange-singleforceloop: test-serial-large-onebyone-dtchange

test-serial-large-onebyone-doublesqrt: DFLAGS+=-DOUBLE_SQRT
test-serial-large-onebyone-doublesqrt: test-serial-large-onebyone

test-serial-large-onebyone-doublesqrt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-serial-large-onebyone-doublesqrt-floatonly: test-serial-large-onebyone-doublesqrt

test-serial-large-onebyone-doublesqrt-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-serial-large-onebyone-doublesqrt-floatonly-reducedlocalcalcs: test-serial-large-onebyone-doublesqrt-floatonly

test-serial-large-onebyone-doublesqrt-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-serial-large-onebyone-doublesqrt-floatonly-reducedlocalcalcs-singleforceloop: test-serial-large-onebyone-doublesqrt-floatonly-reducedlocalcalcs

test-serial-large-onebyone-doublesqrt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-serial-large-onebyone-doublesqrt-floatonly-singleforceloop: test-serial-large-onebyone-doublesqrt-floatonly

test-serial-large-onebyone-doublesqrt-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-serial-large-onebyone-doublesqrt-reducedlocalcalcs: test-serial-large-onebyone-doublesqrt

test-serial-large-onebyone-doublesqrt-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-serial-large-onebyone-doublesqrt-reducedlocalcalcs-singleforceloop: test-serial-large-onebyone-doublesqrt-reducedlocalcalcs

test-serial-large-onebyone-doublesqrt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-serial-large-onebyone-doublesqrt-singleforceloop: test-serial-large-onebyone-doublesqrt

test-serial-large-onebyone-floatonly: DFLAGS+=-DFLOAT_ONLY
test-serial-large-onebyone-floatonly: test-serial-large-onebyone

test-serial-large-onebyone-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-serial-large-onebyone-floatonly-reducedlocalcalcs: test-serial-large-onebyone-floatonly

test-serial-large-onebyone-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-serial-large-onebyone-floatonly-reducedlocalcalcs-singleforceloop: test-serial-large-onebyone-floatonly-reducedlocalcalcs

test-serial-large-onebyone-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-serial-large-onebyone-floatonly-singleforceloop: test-serial-large-onebyone-floatonly

test-serial-large-onebyone-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-serial-large-onebyone-reducedlocalcalcs: test-serial-large-onebyone

test-serial-large-onebyone-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-serial-large-onebyone-reducedlocalcalcs-singleforceloop: test-serial-large-onebyone-reducedlocalcalcs

test-serial-large-onebyone-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-serial-large-onebyone-singleforceloop: test-serial-large-onebyone

# **TEST Local calculation method: sendtoself**
test-serial-large-sendtoself: DFLAGS+=-DSEND_TO_SELF
test-serial-large-sendtoself: test-serial-large

test-serial-large-sendtoself-betterverlet: DFLAGS+=-DBETTER_VERLET
test-serial-large-sendtoself-betterverlet: test-serial-large-sendtoself

test-serial-large-sendtoself-betterverlet-dtchange: DFLAGS+=-DSMALL_DT_EARLY
test-serial-large-sendtoself-betterverlet-dtchange: test-serial-large-sendtoself-betterverlet

test-serial-large-sendtoself-betterverlet-dtchange-doublesqrt: DFLAGS+=-DOUBLE_SQRT
test-serial-large-sendtoself-betterverlet-dtchange-doublesqrt: test-serial-large-sendtoself-betterverlet-dtchange

test-serial-large-sendtoself-betterverlet-dtchange-doublesqrt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-serial-large-sendtoself-betterverlet-dtchange-doublesqrt-floatonly: test-serial-large-sendtoself-betterverlet-dtchange-doublesqrt

test-serial-large-sendtoself-betterverlet-dtchange-doublesqrt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-serial-large-sendtoself-betterverlet-dtchange-doublesqrt-floatonly-singleforceloop: test-serial-large-sendtoself-betterverlet-dtchange-doublesqrt-floatonly

test-serial-large-sendtoself-betterverlet-dtchange-doublesqrt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-serial-large-sendtoself-betterverlet-dtchange-doublesqrt-singleforceloop: test-serial-large-sendtoself-betterverlet-dtchange-doublesqrt

test-serial-large-sendtoself-betterverlet-dtchange-floatonly: DFLAGS+=-DFLOAT_ONLY
test-serial-large-sendtoself-betterverlet-dtchange-floatonly: test-serial-large-sendtoself-betterverlet-dtchange

test-serial-large-sendtoself-betterverlet-dtchange-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-serial-large-sendtoself-betterverlet-dtchange-floatonly-singleforceloop: test-serial-large-sendtoself-betterverlet-dtchange-floatonly

test-serial-large-sendtoself-betterverlet-dtchange-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-serial-large-sendtoself-betterverlet-dtchange-singleforceloop: test-serial-large-sendtoself-betterverlet-dtchange

test-serial-large-sendtoself-betterverlet-doublesqrt: DFLAGS+=-DOUBLE_SQRT
test-serial-large-sendtoself-betterverlet-doublesqrt: test-serial-large-sendtoself-betterverlet

test-serial-large-sendtoself-betterverlet-doublesqrt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-serial-large-sendtoself-betterverlet-doublesqrt-floatonly: test-serial-large-sendtoself-betterverlet-doublesqrt

test-serial-large-sendtoself-betterverlet-doublesqrt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-serial-large-sendtoself-betterverlet-doublesqrt-floatonly-singleforceloop: test-serial-large-sendtoself-betterverlet-doublesqrt-floatonly

test-serial-large-sendtoself-betterverlet-doublesqrt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-serial-large-sendtoself-betterverlet-doublesqrt-singleforceloop: test-serial-large-sendtoself-betterverlet-doublesqrt

test-serial-large-sendtoself-betterverlet-floatonly: DFLAGS+=-DFLOAT_ONLY
test-serial-large-sendtoself-betterverlet-floatonly: test-serial-large-sendtoself-betterverlet

test-serial-large-sendtoself-betterverlet-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-serial-large-sendtoself-betterverlet-floatonly-singleforceloop: test-serial-large-sendtoself-betterverlet-floatonly

test-serial-large-sendtoself-betterverlet-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-serial-large-sendtoself-betterverlet-singleforceloop: test-serial-large-sendtoself-betterverlet

test-serial-large-sendtoself-dtchange: DFLAGS+=-DSMALL_DT_EARLY
test-serial-large-sendtoself-dtchange: test-serial-large-sendtoself

test-serial-large-sendtoself-dtchange-doublesqrt: DFLAGS+=-DOUBLE_SQRT
test-serial-large-sendtoself-dtchange-doublesqrt: test-serial-large-sendtoself-dtchange

test-serial-large-sendtoself-dtchange-doublesqrt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-serial-large-sendtoself-dtchange-doublesqrt-floatonly: test-serial-large-sendtoself-dtchange-doublesqrt

test-serial-large-sendtoself-dtchange-doublesqrt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-serial-large-sendtoself-dtchange-doublesqrt-floatonly-singleforceloop: test-serial-large-sendtoself-dtchange-doublesqrt-floatonly

test-serial-large-sendtoself-dtchange-doublesqrt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-serial-large-sendtoself-dtchange-doublesqrt-singleforceloop: test-serial-large-sendtoself-dtchange-doublesqrt

test-serial-large-sendtoself-dtchange-floatonly: DFLAGS+=-DFLOAT_ONLY
test-serial-large-sendtoself-dtchange-floatonly: test-serial-large-sendtoself-dtchange

test-serial-large-sendtoself-dtchange-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-serial-large-sendtoself-dtchange-floatonly-singleforceloop: test-serial-large-sendtoself-dtchange-floatonly

test-serial-large-sendtoself-dtchange-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-serial-large-sendtoself-dtchange-singleforceloop: test-serial-large-sendtoself-dtchange

test-serial-large-sendtoself-doublesqrt: DFLAGS+=-DOUBLE_SQRT
test-serial-large-sendtoself-doublesqrt: test-serial-large-sendtoself

test-serial-large-sendtoself-doublesqrt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-serial-large-sendtoself-doublesqrt-floatonly: test-serial-large-sendtoself-doublesqrt

test-serial-large-sendtoself-doublesqrt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-serial-large-sendtoself-doublesqrt-floatonly-singleforceloop: test-serial-large-sendtoself-doublesqrt-floatonly

test-serial-large-sendtoself-doublesqrt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-serial-large-sendtoself-doublesqrt-singleforceloop: test-serial-large-sendtoself-doublesqrt

test-serial-large-sendtoself-floatonly: DFLAGS+=-DFLOAT_ONLY
test-serial-large-sendtoself-floatonly: test-serial-large-sendtoself

test-serial-large-sendtoself-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-serial-large-sendtoself-floatonly-singleforceloop: test-serial-large-sendtoself-floatonly

test-serial-large-sendtoself-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-serial-large-sendtoself-singleforceloop: test-serial-large-sendtoself

test-serial-bonds: DFLAGS+=-DBONDS
test-serial-bonds: test-serial

# **TEST Local calculation method: onebyone**
test-serial-bonds-onebyone: DFLAGS+=-DONE_BY_ONE
test-serial-bonds-onebyone: test-serial-bonds

test-serial-bonds-onebyone-betterverlet: DFLAGS+=-DBETTER_VERLET
test-serial-bonds-onebyone-betterverlet: test-serial-bonds-onebyone

test-serial-bonds-onebyone-betterverlet-dtchange: DFLAGS+=-DSMALL_DT_EARLY
test-serial-bonds-onebyone-betterverlet-dtchange: test-serial-bonds-onebyone-betterverlet

test-serial-bonds-onebyone-betterverlet-dtchange-doublesqrt: DFLAGS+=-DOUBLE_SQRT
test-serial-bonds-onebyone-betterverlet-dtchange-doublesqrt: test-serial-bonds-onebyone-betterverlet-dtchange

test-serial-bonds-onebyone-betterverlet-dtchange-doublesqrt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-serial-bonds-onebyone-betterverlet-dtchange-doublesqrt-floatonly: test-serial-bonds-onebyone-betterverlet-dtchange-doublesqrt

test-serial-bonds-onebyone-betterverlet-dtchange-doublesqrt-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-serial-bonds-onebyone-betterverlet-dtchange-doublesqrt-floatonly-reducedlocalcalcs: test-serial-bonds-onebyone-betterverlet-dtchange-doublesqrt-floatonly

test-serial-bonds-onebyone-betterverlet-dtchange-doublesqrt-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-serial-bonds-onebyone-betterverlet-dtchange-doublesqrt-floatonly-reducedlocalcalcs-singleforceloop: test-serial-bonds-onebyone-betterverlet-dtchange-doublesqrt-floatonly-reducedlocalcalcs

test-serial-bonds-onebyone-betterverlet-dtchange-doublesqrt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-serial-bonds-onebyone-betterverlet-dtchange-doublesqrt-floatonly-singleforceloop: test-serial-bonds-onebyone-betterverlet-dtchange-doublesqrt-floatonly

test-serial-bonds-onebyone-betterverlet-dtchange-doublesqrt-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-serial-bonds-onebyone-betterverlet-dtchange-doublesqrt-reducedlocalcalcs: test-serial-bonds-onebyone-betterverlet-dtchange-doublesqrt

test-serial-bonds-onebyone-betterverlet-dtchange-doublesqrt-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-serial-bonds-onebyone-betterverlet-dtchange-doublesqrt-reducedlocalcalcs-singleforceloop: test-serial-bonds-onebyone-betterverlet-dtchange-doublesqrt-reducedlocalcalcs

test-serial-bonds-onebyone-betterverlet-dtchange-doublesqrt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-serial-bonds-onebyone-betterverlet-dtchange-doublesqrt-singleforceloop: test-serial-bonds-onebyone-betterverlet-dtchange-doublesqrt

test-serial-bonds-onebyone-betterverlet-dtchange-floatonly: DFLAGS+=-DFLOAT_ONLY
test-serial-bonds-onebyone-betterverlet-dtchange-floatonly: test-serial-bonds-onebyone-betterverlet-dtchange

test-serial-bonds-onebyone-betterverlet-dtchange-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-serial-bonds-onebyone-betterverlet-dtchange-floatonly-reducedlocalcalcs: test-serial-bonds-onebyone-betterverlet-dtchange-floatonly

test-serial-bonds-onebyone-betterverlet-dtchange-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-serial-bonds-onebyone-betterverlet-dtchange-floatonly-reducedlocalcalcs-singleforceloop: test-serial-bonds-onebyone-betterverlet-dtchange-floatonly-reducedlocalcalcs

test-serial-bonds-onebyone-betterverlet-dtchange-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-serial-bonds-onebyone-betterverlet-dtchange-floatonly-singleforceloop: test-serial-bonds-onebyone-betterverlet-dtchange-floatonly

test-serial-bonds-onebyone-betterverlet-dtchange-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-serial-bonds-onebyone-betterverlet-dtchange-reducedlocalcalcs: test-serial-bonds-onebyone-betterverlet-dtchange

test-serial-bonds-onebyone-betterverlet-dtchange-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-serial-bonds-onebyone-betterverlet-dtchange-reducedlocalcalcs-singleforceloop: test-serial-bonds-onebyone-betterverlet-dtchange-reducedlocalcalcs

test-serial-bonds-onebyone-betterverlet-dtchange-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-serial-bonds-onebyone-betterverlet-dtchange-singleforceloop: test-serial-bonds-onebyone-betterverlet-dtchange

test-serial-bonds-onebyone-betterverlet-doublesqrt: DFLAGS+=-DOUBLE_SQRT
test-serial-bonds-onebyone-betterverlet-doublesqrt: test-serial-bonds-onebyone-betterverlet

test-serial-bonds-onebyone-betterverlet-doublesqrt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-serial-bonds-onebyone-betterverlet-doublesqrt-floatonly: test-serial-bonds-onebyone-betterverlet-doublesqrt

test-serial-bonds-onebyone-betterverlet-doublesqrt-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-serial-bonds-onebyone-betterverlet-doublesqrt-floatonly-reducedlocalcalcs: test-serial-bonds-onebyone-betterverlet-doublesqrt-floatonly

test-serial-bonds-onebyone-betterverlet-doublesqrt-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-serial-bonds-onebyone-betterverlet-doublesqrt-floatonly-reducedlocalcalcs-singleforceloop: test-serial-bonds-onebyone-betterverlet-doublesqrt-floatonly-reducedlocalcalcs

test-serial-bonds-onebyone-betterverlet-doublesqrt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-serial-bonds-onebyone-betterverlet-doublesqrt-floatonly-singleforceloop: test-serial-bonds-onebyone-betterverlet-doublesqrt-floatonly

test-serial-bonds-onebyone-betterverlet-doublesqrt-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-serial-bonds-onebyone-betterverlet-doublesqrt-reducedlocalcalcs: test-serial-bonds-onebyone-betterverlet-doublesqrt

test-serial-bonds-onebyone-betterverlet-doublesqrt-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-serial-bonds-onebyone-betterverlet-doublesqrt-reducedlocalcalcs-singleforceloop: test-serial-bonds-onebyone-betterverlet-doublesqrt-reducedlocalcalcs

test-serial-bonds-onebyone-betterverlet-doublesqrt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-serial-bonds-onebyone-betterverlet-doublesqrt-singleforceloop: test-serial-bonds-onebyone-betterverlet-doublesqrt

test-serial-bonds-onebyone-betterverlet-floatonly: DFLAGS+=-DFLOAT_ONLY
test-serial-bonds-onebyone-betterverlet-floatonly: test-serial-bonds-onebyone-betterverlet

test-serial-bonds-onebyone-betterverlet-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-serial-bonds-onebyone-betterverlet-floatonly-reducedlocalcalcs: test-serial-bonds-onebyone-betterverlet-floatonly

test-serial-bonds-onebyone-betterverlet-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-serial-bonds-onebyone-betterverlet-floatonly-reducedlocalcalcs-singleforceloop: test-serial-bonds-onebyone-betterverlet-floatonly-reducedlocalcalcs

test-serial-bonds-onebyone-betterverlet-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-serial-bonds-onebyone-betterverlet-floatonly-singleforceloop: test-serial-bonds-onebyone-betterverlet-floatonly

test-serial-bonds-onebyone-betterverlet-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-serial-bonds-onebyone-betterverlet-reducedlocalcalcs: test-serial-bonds-onebyone-betterverlet

test-serial-bonds-onebyone-betterverlet-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-serial-bonds-onebyone-betterverlet-reducedlocalcalcs-singleforceloop: test-serial-bonds-onebyone-betterverlet-reducedlocalcalcs

test-serial-bonds-onebyone-betterverlet-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-serial-bonds-onebyone-betterverlet-singleforceloop: test-serial-bonds-onebyone-betterverlet

test-serial-bonds-onebyone-dtchange: DFLAGS+=-DSMALL_DT_EARLY
test-serial-bonds-onebyone-dtchange: test-serial-bonds-onebyone

test-serial-bonds-onebyone-dtchange-doublesqrt: DFLAGS+=-DOUBLE_SQRT
test-serial-bonds-onebyone-dtchange-doublesqrt: test-serial-bonds-onebyone-dtchange

test-serial-bonds-onebyone-dtchange-doublesqrt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-serial-bonds-onebyone-dtchange-doublesqrt-floatonly: test-serial-bonds-onebyone-dtchange-doublesqrt

test-serial-bonds-onebyone-dtchange-doublesqrt-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-serial-bonds-onebyone-dtchange-doublesqrt-floatonly-reducedlocalcalcs: test-serial-bonds-onebyone-dtchange-doublesqrt-floatonly

test-serial-bonds-onebyone-dtchange-doublesqrt-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-serial-bonds-onebyone-dtchange-doublesqrt-floatonly-reducedlocalcalcs-singleforceloop: test-serial-bonds-onebyone-dtchange-doublesqrt-floatonly-reducedlocalcalcs

test-serial-bonds-onebyone-dtchange-doublesqrt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-serial-bonds-onebyone-dtchange-doublesqrt-floatonly-singleforceloop: test-serial-bonds-onebyone-dtchange-doublesqrt-floatonly

test-serial-bonds-onebyone-dtchange-doublesqrt-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-serial-bonds-onebyone-dtchange-doublesqrt-reducedlocalcalcs: test-serial-bonds-onebyone-dtchange-doublesqrt

test-serial-bonds-onebyone-dtchange-doublesqrt-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-serial-bonds-onebyone-dtchange-doublesqrt-reducedlocalcalcs-singleforceloop: test-serial-bonds-onebyone-dtchange-doublesqrt-reducedlocalcalcs

test-serial-bonds-onebyone-dtchange-doublesqrt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-serial-bonds-onebyone-dtchange-doublesqrt-singleforceloop: test-serial-bonds-onebyone-dtchange-doublesqrt

test-serial-bonds-onebyone-dtchange-floatonly: DFLAGS+=-DFLOAT_ONLY
test-serial-bonds-onebyone-dtchange-floatonly: test-serial-bonds-onebyone-dtchange

test-serial-bonds-onebyone-dtchange-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-serial-bonds-onebyone-dtchange-floatonly-reducedlocalcalcs: test-serial-bonds-onebyone-dtchange-floatonly

test-serial-bonds-onebyone-dtchange-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-serial-bonds-onebyone-dtchange-floatonly-reducedlocalcalcs-singleforceloop: test-serial-bonds-onebyone-dtchange-floatonly-reducedlocalcalcs

test-serial-bonds-onebyone-dtchange-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-serial-bonds-onebyone-dtchange-floatonly-singleforceloop: test-serial-bonds-onebyone-dtchange-floatonly

test-serial-bonds-onebyone-dtchange-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-serial-bonds-onebyone-dtchange-reducedlocalcalcs: test-serial-bonds-onebyone-dtchange

test-serial-bonds-onebyone-dtchange-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-serial-bonds-onebyone-dtchange-reducedlocalcalcs-singleforceloop: test-serial-bonds-onebyone-dtchange-reducedlocalcalcs

test-serial-bonds-onebyone-dtchange-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-serial-bonds-onebyone-dtchange-singleforceloop: test-serial-bonds-onebyone-dtchange

test-serial-bonds-onebyone-doublesqrt: DFLAGS+=-DOUBLE_SQRT
test-serial-bonds-onebyone-doublesqrt: test-serial-bonds-onebyone

test-serial-bonds-onebyone-doublesqrt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-serial-bonds-onebyone-doublesqrt-floatonly: test-serial-bonds-onebyone-doublesqrt

test-serial-bonds-onebyone-doublesqrt-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-serial-bonds-onebyone-doublesqrt-floatonly-reducedlocalcalcs: test-serial-bonds-onebyone-doublesqrt-floatonly

test-serial-bonds-onebyone-doublesqrt-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-serial-bonds-onebyone-doublesqrt-floatonly-reducedlocalcalcs-singleforceloop: test-serial-bonds-onebyone-doublesqrt-floatonly-reducedlocalcalcs

test-serial-bonds-onebyone-doublesqrt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-serial-bonds-onebyone-doublesqrt-floatonly-singleforceloop: test-serial-bonds-onebyone-doublesqrt-floatonly

test-serial-bonds-onebyone-doublesqrt-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-serial-bonds-onebyone-doublesqrt-reducedlocalcalcs: test-serial-bonds-onebyone-doublesqrt

test-serial-bonds-onebyone-doublesqrt-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-serial-bonds-onebyone-doublesqrt-reducedlocalcalcs-singleforceloop: test-serial-bonds-onebyone-doublesqrt-reducedlocalcalcs

test-serial-bonds-onebyone-doublesqrt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-serial-bonds-onebyone-doublesqrt-singleforceloop: test-serial-bonds-onebyone-doublesqrt

test-serial-bonds-onebyone-floatonly: DFLAGS+=-DFLOAT_ONLY
test-serial-bonds-onebyone-floatonly: test-serial-bonds-onebyone

test-serial-bonds-onebyone-floatonly-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-serial-bonds-onebyone-floatonly-reducedlocalcalcs: test-serial-bonds-onebyone-floatonly

test-serial-bonds-onebyone-floatonly-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-serial-bonds-onebyone-floatonly-reducedlocalcalcs-singleforceloop: test-serial-bonds-onebyone-floatonly-reducedlocalcalcs

test-serial-bonds-onebyone-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-serial-bonds-onebyone-floatonly-singleforceloop: test-serial-bonds-onebyone-floatonly

test-serial-bonds-onebyone-reducedlocalcalcs: DFLAGS+=-DREDUCED_LOCAL_CALCS
test-serial-bonds-onebyone-reducedlocalcalcs: test-serial-bonds-onebyone

test-serial-bonds-onebyone-reducedlocalcalcs-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-serial-bonds-onebyone-reducedlocalcalcs-singleforceloop: test-serial-bonds-onebyone-reducedlocalcalcs

test-serial-bonds-onebyone-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-serial-bonds-onebyone-singleforceloop: test-serial-bonds-onebyone

# **TEST Local calculation method: sendtoself**
test-serial-bonds-sendtoself: DFLAGS+=-DSEND_TO_SELF
test-serial-bonds-sendtoself: test-serial-bonds

test-serial-bonds-sendtoself-betterverlet: DFLAGS+=-DBETTER_VERLET
test-serial-bonds-sendtoself-betterverlet: test-serial-bonds-sendtoself

test-serial-bonds-sendtoself-betterverlet-dtchange: DFLAGS+=-DSMALL_DT_EARLY
test-serial-bonds-sendtoself-betterverlet-dtchange: test-serial-bonds-sendtoself-betterverlet

test-serial-bonds-sendtoself-betterverlet-dtchange-doublesqrt: DFLAGS+=-DOUBLE_SQRT
test-serial-bonds-sendtoself-betterverlet-dtchange-doublesqrt: test-serial-bonds-sendtoself-betterverlet-dtchange

test-serial-bonds-sendtoself-betterverlet-dtchange-doublesqrt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-serial-bonds-sendtoself-betterverlet-dtchange-doublesqrt-floatonly: test-serial-bonds-sendtoself-betterverlet-dtchange-doublesqrt

test-serial-bonds-sendtoself-betterverlet-dtchange-doublesqrt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-serial-bonds-sendtoself-betterverlet-dtchange-doublesqrt-floatonly-singleforceloop: test-serial-bonds-sendtoself-betterverlet-dtchange-doublesqrt-floatonly

test-serial-bonds-sendtoself-betterverlet-dtchange-doublesqrt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-serial-bonds-sendtoself-betterverlet-dtchange-doublesqrt-singleforceloop: test-serial-bonds-sendtoself-betterverlet-dtchange-doublesqrt

test-serial-bonds-sendtoself-betterverlet-dtchange-floatonly: DFLAGS+=-DFLOAT_ONLY
test-serial-bonds-sendtoself-betterverlet-dtchange-floatonly: test-serial-bonds-sendtoself-betterverlet-dtchange

test-serial-bonds-sendtoself-betterverlet-dtchange-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-serial-bonds-sendtoself-betterverlet-dtchange-floatonly-singleforceloop: test-serial-bonds-sendtoself-betterverlet-dtchange-floatonly

test-serial-bonds-sendtoself-betterverlet-dtchange-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-serial-bonds-sendtoself-betterverlet-dtchange-singleforceloop: test-serial-bonds-sendtoself-betterverlet-dtchange

test-serial-bonds-sendtoself-betterverlet-doublesqrt: DFLAGS+=-DOUBLE_SQRT
test-serial-bonds-sendtoself-betterverlet-doublesqrt: test-serial-bonds-sendtoself-betterverlet

test-serial-bonds-sendtoself-betterverlet-doublesqrt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-serial-bonds-sendtoself-betterverlet-doublesqrt-floatonly: test-serial-bonds-sendtoself-betterverlet-doublesqrt

test-serial-bonds-sendtoself-betterverlet-doublesqrt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-serial-bonds-sendtoself-betterverlet-doublesqrt-floatonly-singleforceloop: test-serial-bonds-sendtoself-betterverlet-doublesqrt-floatonly

test-serial-bonds-sendtoself-betterverlet-doublesqrt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-serial-bonds-sendtoself-betterverlet-doublesqrt-singleforceloop: test-serial-bonds-sendtoself-betterverlet-doublesqrt

test-serial-bonds-sendtoself-betterverlet-floatonly: DFLAGS+=-DFLOAT_ONLY
test-serial-bonds-sendtoself-betterverlet-floatonly: test-serial-bonds-sendtoself-betterverlet

test-serial-bonds-sendtoself-betterverlet-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-serial-bonds-sendtoself-betterverlet-floatonly-singleforceloop: test-serial-bonds-sendtoself-betterverlet-floatonly

test-serial-bonds-sendtoself-betterverlet-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-serial-bonds-sendtoself-betterverlet-singleforceloop: test-serial-bonds-sendtoself-betterverlet

test-serial-bonds-sendtoself-dtchange: DFLAGS+=-DSMALL_DT_EARLY
test-serial-bonds-sendtoself-dtchange: test-serial-bonds-sendtoself

test-serial-bonds-sendtoself-dtchange-doublesqrt: DFLAGS+=-DOUBLE_SQRT
test-serial-bonds-sendtoself-dtchange-doublesqrt: test-serial-bonds-sendtoself-dtchange

test-serial-bonds-sendtoself-dtchange-doublesqrt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-serial-bonds-sendtoself-dtchange-doublesqrt-floatonly: test-serial-bonds-sendtoself-dtchange-doublesqrt

test-serial-bonds-sendtoself-dtchange-doublesqrt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-serial-bonds-sendtoself-dtchange-doublesqrt-floatonly-singleforceloop: test-serial-bonds-sendtoself-dtchange-doublesqrt-floatonly

test-serial-bonds-sendtoself-dtchange-doublesqrt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-serial-bonds-sendtoself-dtchange-doublesqrt-singleforceloop: test-serial-bonds-sendtoself-dtchange-doublesqrt

test-serial-bonds-sendtoself-dtchange-floatonly: DFLAGS+=-DFLOAT_ONLY
test-serial-bonds-sendtoself-dtchange-floatonly: test-serial-bonds-sendtoself-dtchange

test-serial-bonds-sendtoself-dtchange-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-serial-bonds-sendtoself-dtchange-floatonly-singleforceloop: test-serial-bonds-sendtoself-dtchange-floatonly

test-serial-bonds-sendtoself-dtchange-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-serial-bonds-sendtoself-dtchange-singleforceloop: test-serial-bonds-sendtoself-dtchange

test-serial-bonds-sendtoself-doublesqrt: DFLAGS+=-DOUBLE_SQRT
test-serial-bonds-sendtoself-doublesqrt: test-serial-bonds-sendtoself

test-serial-bonds-sendtoself-doublesqrt-floatonly: DFLAGS+=-DFLOAT_ONLY
test-serial-bonds-sendtoself-doublesqrt-floatonly: test-serial-bonds-sendtoself-doublesqrt

test-serial-bonds-sendtoself-doublesqrt-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-serial-bonds-sendtoself-doublesqrt-floatonly-singleforceloop: test-serial-bonds-sendtoself-doublesqrt-floatonly

test-serial-bonds-sendtoself-doublesqrt-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-serial-bonds-sendtoself-doublesqrt-singleforceloop: test-serial-bonds-sendtoself-doublesqrt

test-serial-bonds-sendtoself-floatonly: DFLAGS+=-DFLOAT_ONLY
test-serial-bonds-sendtoself-floatonly: test-serial-bonds-sendtoself

test-serial-bonds-sendtoself-floatonly-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-serial-bonds-sendtoself-floatonly-singleforceloop: test-serial-bonds-sendtoself-floatonly

test-serial-bonds-sendtoself-singleforceloop: DFLAGS+=-DSINGLE_FORCE_LOOP
test-serial-bonds-sendtoself-singleforceloop: test-serial-bonds-sendtoself

