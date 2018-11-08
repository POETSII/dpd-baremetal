// A test file for the fixed point arithmetic stuff
#include "fixed_ap.h"
#include <stdio.h>
#include <math.h>

typedef int test_res; // 0 - PASS, 1 - FAIL

//// fixed point addition
//test_res test_add(fix16_t a, fix16_t b, fix16_t expected) {
//   printf("testing fix_add %.6f + %.6f = ... ", fix2float(a), fix2float(b)); 
//
//   fix16_t c = a + b;
//
//   printf("%.6f", fix2float(c));
//   if(c != expected) { 
//      printf("\t\tfail\n");
//      return 1;
//   }
//   printf("\t\tpass\n");
//   return 0;
//}
//
//// fixed point subtraction
//test_res test_sub(fix16_t a, fix16_t b, fix16_t expected) {
//   printf("testing fix_sub %.6f - %.6f = ... ", fix2float(a), fix2float(b)); 
//
//   fix16_t c = a - b;
//
//   printf("%.6f", fix2float(c));
//   if(c != expected) { 
//      printf("\t\tfail\n");
//      return 1;
//   }
//   printf("\t\tpass\n");
//   return 0;
//}
//
//// fixed point multiplication
//test_res test_mul(fix16_t a, fix16_t b, fix16_t expected) {
//   printf("testing fix_mul %.6f * %.6f = ... ", fix2float(a), fix2float(b)); 
//
//   fix16_t c = fixap_mul(a,b);
//
//   printf("%.6f", fix2float(c));
//   if(c != expected) { 
//      printf("\t\tfail\n");
//      return 1;
//   }
//   printf("\t\tpass\n");
//   return 0;
//}
//
//// fixed point division 
//test_res test_div(fix16_t a, fix16_t b, fix16_t expected) {
//   printf("testing fix_div %.6f / %.6f = ... ", fix2float(a), fix2float(b)); 
//
//   fix16_t c = fixap_div(a,b);
//
//   printf("%.6f", fix2float(c));
//   if(c != expected) { 
//      printf("\t\tfail\n");
//      return 1;
//   }
//   printf("\t\tpass\n");
//   return 0;
//}
//
//test_res test_sqrt(fix16_t a, fix16_t expected) {
//   printf("testing sqrt %.6f = ... ", fix2float(a)); 
//
//   fix16_t c = fixap_sqrt(a);
//
//   printf("%.6f", fix2float(c));
//   if(c != expected) { 
//      printf("\t\tfail\n");
//      return 1;
//   }
//   printf("\t\tpass\n");
//   return 0;
//}

int main() {

  int t = 0;
  
  fixap<uint16_t, 8> a1(0.4);  
  printf("<16,8> 0.4 = %.4f\n", (float)a1);

  fixap<uint32_t, 30> a2(0.4);  
  printf("<32,30> 0.4 = %.4f\n", (float)a2);

  fixap<uint16_t, 13> a3(0.00021);
  printf("<16,13> 0.00021 = %.4f\n", (float)a3);

	printf("\n-----------------------------\n");
	if(t == 1) {
           printf("Overall: fail.\n");
	   return -1;
	}
  printf("Overall: pass.\n");
	return 0;
}

