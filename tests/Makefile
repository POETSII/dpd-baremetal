# Makefile for the testing of various components of the DPD simulation

DPD_CFLAGS= -w -std=c++11 

DPD_INC=../inc
DPD_SRC=../src

TEST_OBJS=fixap.o fixap_vec3d.o 

.PHONY: all

all: fixap_tests fixap_vec3d_tests $(TEST_OBJS) tests force_tests

tests: fixap_tests fixap_vec3d_tests
	./fixap_tests
	./fixap_vec3d_tests

%.o: $(DPD_INC)/%.h $(DPD_SRC)/%.cpp
	g++ $(DPD_CFLAGS) -I $(DPD_INC) -c $(DPD_SRC)/$*.cpp -o $@ -lm

fixap_tests: fixap_tests.cpp $(DPD_INC)/fixap.h $(DPD_SRC)/fixap.cpp $(TEST_OBJS)
	g++ $(DPD_CFLAGS) -I $(DPD_INC) fixap_tests.cpp $(TEST_OBJS) -o fixap_tests -lm

fixap_vec3d_tests: fixap_vec3d_tests.cpp $(DPD_INC)/fixap_vec3d.h $(DPD_SRC)/fixap_vec3d.cpp $(TEST_OBJS)
	g++ $(DPD_CFLAGS) -I $(DPD_INC) fixap_vec3d_tests.cpp $(TEST_OBJS) -o fixap_vec3d_tests -lm

force_tests: force_tests.cpp $(DPD_INC)/fixap_vec3d.h $(DPD_SRC)/fixap_vec3d.cpp $(TEST_OBJS)
	g++ $(DPD_CFLAGS) -I $(DPD_INC) force_tests.cpp $(TEST_OBJS) -o force_tests -lm

template: fixed_ap_tests.cpp $(DPD_INC)/fixed_ap.h 
	g++ $(DPD_CFLAGS) -I $(DPD_INC) fixed_ap_tests.cpp -o template -lm

clean:
	rm -f fixap_tests fixap_vec3d_tests force_tests
	rm -f template
	rm -f *.o
