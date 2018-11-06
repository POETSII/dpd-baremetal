// A test file for the fixed point arithmetic stuff
#include "fixap.h"
#include <stdio.h>
#include <math.h>

typedef enum _test_res { Pass=0, Fail=1 } test_res;

// conversion test functions
fix16_t float2fix(float a){
    return round(a * FPScaleF);
}

float fix2float(fix16_t a){
   return ((float)a/FPScaleF);
}


// fixed point addition
test_res test_add(fix16_t a, fix16_t b, fix16_t expected) {
   printf("testing fix_add %.6f + %.6f = ... ", fix2float(a), fix2float(b)); 

   fix16_t c = a + b;

   printf("%.6f", fix2float(c));
   if(c != expected) { 
      printf("\t\tfail\n");
      return Fail;
   }
   printf("\t\tpass\n");
   return Pass;
}

// fixed point subtraction
test_res test_sub(fix16_t a, fix16_t b, fix16_t expected) {
   printf("testing fix_sub %.6f - %.6f = ... ", fix2float(a), fix2float(b)); 

   fix16_t c = a - b;

   printf("%.6f", fix2float(c));
   if(c != expected) { 
      printf("\t\tfail\n");
      return Fail;
   }
   printf("\t\tpass\n");
   return Pass;
}

// fixed point multiplication
test_res test_mul(fix16_t a, fix16_t b, fix16_t expected) {
   printf("testing fix_mul %.6f * %.6f = ... ", fix2float(a), fix2float(b)); 

   fix16_t c = fixap_mul(a,b);

   printf("%.6f", fix2float(c));
   if(c != expected) { 
      printf("\t\tfail\n");
      return Fail;
   }
   printf("\t\tpass\n");
   return Pass;
}

// fixed point division 
test_res test_div(fix16_t a, fix16_t b, fix16_t expected) {
   printf("testing fix_div %.6f / %.6f = ... ", fix2float(a), fix2float(b)); 

   fix16_t c = fixap_div(a,b);

   printf("%.6f", fix2float(c));
   if(c != expected) { 
      printf("\t\tfail\n");
      return Fail;
   }
   printf("\t\tpass\n");
   return Pass;
}

test_res test_sqrt(fix16_t a, fix16_t expected) {
   printf("testing sqrt %.6f = ... ", fix2float(a)); 

   fix16_t c = fixap_sqrt(a);

   printf("%.6f", fix2float(c));
   if(c != expected) { 
      printf("\t\tfail\n");
      return Fail;
   }
   printf("\t\tpass\n");
   return Pass;
}

int main() {

  test_res t = Pass;
 
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
	if(t == Fail) {
           printf("Overall: fail.\n");
	   return -1;
	}
        printf("Overall: pass.\n");
	return 0;
}
