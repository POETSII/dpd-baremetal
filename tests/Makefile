# Makefile for the testing of various components of the DPD simulation

DPD_CFLAGS= -w  

DPD_INC=../inc
DPD_SRC=../src

TEST_OBJS=fixap.o

.PHONY: all

all: fixap_tests $(TEST_OBJS)

%.o: $(DPD_INC)/%.h $(DPD_SRC)/%.c
	gcc $(DPD_CFLAGS) -I $(DPD_INC) -c $(DPD_SRC)/$*.c -o $@ -lm

fixap_tests: fixap_tests.c $(DPD_INC)/fixap.h $(DPD_SRC)/fixap.c $(TEST_OBJS)
	gcc $(DPD_CFLAGS) -I $(DPD_INC) fixap_tests.c $(TEST_OBJS) -o fixap_tests -lm

clean:
	rm -f fixap_tests
	rm -f *.o
