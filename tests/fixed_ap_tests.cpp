// A test file for the fixed point arithmetic stuff
#include "fixed_ap.h"
#include <stdio.h>
#include <math.h>

typedef int test_res; // 0 - PASS, 1 - FAIL

int main() {

  int t = 0;
  
  fixap<uint16_t, 8> a1(0.4);  
  printf("<16,8> 0.4 = %.4f\n", (float)a1);

  fixap<uint32_t, 30> a2(0.4);  
  printf("<32,30> 0.4 = %.4f\n", (float)a2);

  fixap<uint16_t, 13> a3(0.00021);
  printf("<16,13> 0.00021 = %.4f\n", (float)a3);

  fixap<uint16_t, 13> a4(0.03);
  fixap<uint16_t, 13> b4(0.7);
  printf("<16,13>0.03 * <16,13>0.7 = %.4f\n", (float)(a4 * b4)); 

  fixap<uint16_t, 13> a5(0.7);
  fixap<uint16_t, 13> b5(0.35);
  printf("<16,13>0.7 * <16,13>0.35 = %.4f\n", (float)(a5 / b5)); 
  

	printf("\n-----------------------------\n");
	if(t == 1) {
           printf("Overall: fail.\n");
	   return -1;
	}
  printf("Overall: pass.\n");
	return 0;
}

