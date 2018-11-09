# Makefile for the testing of various components of the DPD simulation

DPD_CFLAGS= -w -std=c++11 

DPD_INC=../inc
DPD_SRC=../src

TEST_OBJS= 

.PHONY: all

all: fixed_ap_tests $(TEST_OBJS) tests

tests: fixed_ap_tests 
	./fixed_ap_tests

%.o: $(DPD_INC)/%.h $(DPD_SRC)/%.cpp
	g++ $(DPD_CFLAGS) -I $(DPD_INC) -c $(DPD_SRC)/$*.cpp -o $@ -lm

fixed_ap_tests: fixed_ap_tests.cpp $(DPD_INC)/fixed_ap.h 
	g++ $(DPD_CFLAGS) -I $(DPD_INC) fixed_ap_tests.cpp -o fixed_ap_tests -lm

clean:
	rm -f fixed_ap_tests
	rm -f *.o
