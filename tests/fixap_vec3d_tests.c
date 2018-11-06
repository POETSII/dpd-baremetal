// A test file for the fixed point 3 dimensional vector 
#include "fixap.h"
#include "fixap_vec3d.h"
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
test_res test_scalar_mul(vec_t a, fix16_t b, vec_t expected) {
   printf("testing vector_scalar_mul <%.6f,%.6f,%.6f> * %.6f... ", fix2float(a.x), fix2float(a.y), fix2float(a.z), fix2float(b)); 

   vec_t c = vec3d_mul_scalar(a,b);

   printf("<%.6f,%.6f,%.6f>", fix2float(c.x),fix2float(c.y),fix2float(c.z));
   if( (c.x != expected.x) || (c.y != expected.y) || (c.z != expected.z)) { 
      printf("\t\tfail\n");
      return Fail;
   }
   printf("\t\tpass\n");
   return Pass;
}

int main() {

  test_res t = Pass;

  //vec_t a, b;
  //vec_t expected;

  // scalar vector mul tests
  vec_t a = { float2fix(2.3), float2fix(1.2), float2fix(0.7) };
  vec_t expected = { float2fix(1.150024), float2fix(0.599976), float2fix(0.349976) };
	t |= test_scalar_mul(a, float2fix(0.5), expected);



	printf("\n-----------------------------\n");
	if(t == Fail) {
           printf("Overall: fail.\n");
	   return -1;
	}
        printf("Overall: pass.\n");
	return 0;
}
