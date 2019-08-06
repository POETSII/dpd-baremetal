# Makefile for the testing of various components of the DPD simulation

CPPFLAGS += -I ../inc $(DFLAGS)

DPD_CFLAGS= -w -std=c++11

DPD_INC=../inc
DPD_SRC=../src

TEST_OBJS= vec3d_double.o

.PHONY: all

all: fixed_ap_tests force_tests $(TEST_OBJS) tests dpd_tests

tests: fixed_ap_tests force_tests dpd_tests
	./fixed_ap_tests
	./force_tests

%.o: $(DPD_INC)/%.h $(DPD_SRC)/%.cpp
	g++ $(DPD_CFLAGS) -I $(DPD_INC) -c $(DPD_SRC)/$*.cpp -o $@ -lm

fixed_ap_tests: fixed_ap_tests.cpp $(DPD_INC)/fixed_ap.h $(TEST_OBJS)
	g++ $(DPD_CFLAGS) -I $(DPD_INC) fixed_ap_tests.cpp $(TEST_OBJS) -o fixed_ap_tests -lm

force_tests: force_tests.cpp $(DPD_INC)/fixed_ap.h $(TEST_OBJS)
	g++ $(DPD_CFLAGS) -I $(DPD_INC) force_tests.cpp $(TEST_OBJS) -o force_tests -lm

dpd_tests: dpd_tests.cpp $(DPD_INC)/dpd.h $(TEST_OBJS)
	g++ $(DPD_CFLAGS) -I $(DPD_INC) dpd_tests.cpp $(TEST_OBJS) -o dpd_tests -lm

test_accelerator: DFLAGS=-DTEST_ACCELERATOR_DT10
test_accelerator: ../src/accelerator.cpp ../src/utils.cpp

clean:
	rm -f fixed_ap_tests force_tests
	rm -f test_accelerator
	rm -f *.o
