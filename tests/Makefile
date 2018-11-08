# Makefile for the testing of various components of the DPD simulation

DPD_CFLAGS= -w -std=c++11 

DPD_INC=../inc
DPD_SRC=../src

TEST_OBJS=fixap.o fixap_vec3d.o

.PHONY: all

all: fixap_tests fixap_vec3d_tests $(TEST_OBJS) tests

tests: fixap_tests fixap_vec3d_tests
	./fixap_tests
	./fixap_vec3d_tests

%.o: $(DPD_INC)/%.h $(DPD_SRC)/%.c
	g++ $(DPD_CFLAGS) -I $(DPD_INC) -c $(DPD_SRC)/$*.c -o $@ -lm

fixap_tests: fixap_tests.c $(DPD_INC)/fixap.h $(DPD_SRC)/fixap.c $(TEST_OBJS)
	g++ $(DPD_CFLAGS) -I $(DPD_INC) fixap_tests.c $(TEST_OBJS) -o fixap_tests -lm

fixap_vec3d_tests: fixap_vec3d_tests.c $(DPD_INC)/fixap_vec3d.h $(DPD_SRC)/fixap_vec3d.c $(TEST_OBJS)
	g++ $(DPD_CFLAGS) -I $(DPD_INC) fixap_vec3d_tests.c $(TEST_OBJS) -o fixap_vec3d_tests -lm

clean:
	rm -f fixap_tests fixap_vec3d_tests
	rm -f *.o
