// A test file for the fixed point arithmetic stuff
#include "fixap.h"
#include <stdio.h>
#include <math.h>

typedef int test_res; // 0 - PASS, 1 - FAIL

// fixed point addition
test_res test_add(fix16_t a, fix16_t b, fix16_t expected) {
   printf("testing fix_add %.6f + %.6f = ... ", fix2float(a), fix2float(b)); 

   fix16_t c = a + b;

   printf("%.6f", fix2float(c));
   if(c != expected) { 
      printf("\t\tfail\n");
      return 1;
   }
   printf("\t\tpass\n");
   return 0;
}

// fixed point subtraction
test_res test_sub(fix16_t a, fix16_t b, fix16_t expected) {
   printf("testing fix_sub %.6f - %.6f = ... ", fix2float(a), fix2float(b)); 

   fix16_t c = a - b;

   printf("%.6f", fix2float(c));
   if(c != expected) { 
      printf("\t\tfail\n");
      return 1;
   }
   printf("\t\tpass\n");
   return 0;
}

// fixed point multiplication
test_res test_mul(fix16_t a, fix16_t b, fix16_t expected) {
   printf("testing fix_mul %.6f * %.6f = ... ", fix2float(a), fix2float(b)); 

   fix16_t c = fixap_mul(a,b);

   printf("%.6f", fix2float(c));
   if(c != expected) { 
      printf("\t\tfail\n");
      return 1;
   }
   printf("\t\tpass\n");
   return 0;
}

// fixed point division 
test_res test_div(fix16_t a, fix16_t b, fix16_t expected) {
   printf("testing fix_div %.6f / %.6f = ... ", fix2float(a), fix2float(b)); 

   fix16_t c = fixap_div(a,b);

   printf("%.6f", fix2float(c));
   if(c != expected) { 
      printf("\t\tfail\n");
      return 1;
   }
   printf("\t\tpass\n");
   return 0;
}

test_res test_sqrt(fix16_t a, fix16_t expected) {
   printf("testing sqrt %.6f = ... ", fix2float(a)); 

   fix16_t c = fixap_sqrt(a);

   printf("%.6f", fix2float(c));
   if(c != expected) { 
      printf("\t\tfail\n");
      return 1;
   }
   printf("\t\tpass\n");
   return 0;
}

const fix16_t f_static = float2fix_const(1.5); // 1.5

int main() {

  int t = 0;
  printf("fixed const test 1.5 = %.4f\n", fix2float(f_static));
 
  // addition tests
	t |= test_add(float2fix(0.2), float2fix(1.5), float2fix(1.7));
	t |= test_add(float2fix(-0.5), float2fix(1.8), float2fix(1.3));

  // subtraction tests
	t |= test_sub(float2fix(2.5), float2fix(0.9), float2fix(1.6));
	t |= test_sub(float2fix(1.5), float2fix(2.8), float2fix(-1.3));

  // multiplication tests
	t |= test_mul(float2fix(1.1), float2fix(0.75), float2fix(0.824951));
	t |= test_mul(float2fix(-0.1), float2fix(1.75), float2fix(-0.174927));

  // division tests
  t |= test_div(float2fix(2.2), float2fix(1.1), float2fix(2.0));

  // sqrt test
  t |= test_sqrt(float2fix(0.16), float2fix(0.400024));
  t |= test_sqrt(float2fix(1.5), float2fix(1.2247));

	printf("\n-----------------------------\n");
	if(t == 1) {
           printf("Overall: fail.\n");
	   return -1;
	}
        printf("Overall: pass.\n");
	return 0;
}
