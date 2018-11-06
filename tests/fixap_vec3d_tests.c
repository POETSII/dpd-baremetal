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


// vector scalar multiplication 
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

// vector - scalar addition 
test_res test_scalar_add(vec_t a, fix16_t b, vec_t expected) {
   printf("testing vector_scalar_add <%.6f,%.6f,%.6f> + %.6f... ", fix2float(a.x), fix2float(a.y), fix2float(a.z), fix2float(b)); 

   vec_t c = vec3d_add_scalar(a,b);

   printf("<%.6f,%.6f,%.6f>", fix2float(c.x),fix2float(c.y),fix2float(c.z));
   if( (c.x != expected.x) || (c.y != expected.y) || (c.z != expected.z)) { 
      printf("\t\tfail\n");
      return Fail;
   }
   printf("\t\tpass\n");
   return Pass;
}

// vector - scalar subtraction 
test_res test_scalar_sub(vec_t a, fix16_t b, vec_t expected) {
   printf("testing vector_scalar_sub <%.6f,%.6f,%.6f> - %.6f... ", fix2float(a.x), fix2float(a.y), fix2float(a.z), fix2float(b)); 

   vec_t c = vec3d_sub_scalar(a,b);

   printf("<%.6f,%.6f,%.6f>", fix2float(c.x),fix2float(c.y),fix2float(c.z));
   if( (c.x != expected.x) || (c.y != expected.y) || (c.z != expected.z)) { 
      printf("\t\tfail\n");
      return Fail;
   }
   printf("\t\tpass\n");
   return Pass;
}

// vector - vector addition 
test_res test_vec_addition(vec_t a, vec_t b, vec_t expected) {
   printf("testing vector_vector_add <%.6f,%.6f,%.6f> + <%.6f,%.6f,%.6f>... ", fix2float(a.x), fix2float(a.y), fix2float(a.z), fix2float(b.x), fix2float(b.y), fix2float(b.z)); 

   vec_t c = vec3d_add(a,b);

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

  // scalar - vector tests
  vec_t a_smul = { float2fix(2.3), float2fix(1.2), float2fix(0.7) };
  vec_t e_smul = { float2fix(1.150024), float2fix(0.599976), float2fix(0.349976) };
	t |= test_scalar_mul(a_smul, float2fix(0.5), e_smul);

  vec_t a_sadd = { float2fix(2.3), float2fix(1.2), float2fix(0.7) };
  vec_t e_sadd = { float2fix(2.619995), float2fix(1.519897), float2fix(1.019897) };
	t |= test_scalar_add(a_sadd, float2fix(0.32), e_sadd);

  vec_t a_ssub = { float2fix(2.3), float2fix(1.2), float2fix(0.7) };
  vec_t e_ssub = { float2fix(1.900024), float2fix(0.799927), float2fix(0.299927) };
	t |= test_scalar_sub(a_ssub, float2fix(0.4), e_ssub);

  // vector - vector addition
  vec_t a2 = { float2fix(0.5), float2fix(1.1), float2fix(0.7) };
  vec_t b2 = { float2fix(1.25), float2fix(1.2), float2fix(0.4) };
  vec_t e2 = { float2fix(1.75), float2fix(2.299927), float2fix(1.099976) };
	t |= test_vec_addition(a2, b2, e2);

	printf("\n-----------------------------\n");
	if(t == Fail) {
           printf("Overall: fail.\n");
	   return -1;
	}
        printf("Overall: pass.\n");
	return 0;
}
