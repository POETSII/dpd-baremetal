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

// vector - scalar subtraction 
test_res test_scalar_div(vec_t a, fix16_t b, vec_t expected) {
   printf("testing vector_scalar_div <%.6f,%.6f,%.6f> / %.6f... ", fix2float(a.x), fix2float(a.y), fix2float(a.z), fix2float(b)); 

   vec_t c = vec3d_div_scalar(a,b);

   printf("<%.6f,%.6f,%.6f>", fix2float(c.x),fix2float(c.y),fix2float(c.z));
   if( (c.x != expected.x) || (c.y != expected.y) || (c.z != expected.z)) { 
      printf("\t\tfail\n");
      return Fail;
   }
   printf("\t\tpass\n");
   return Pass;
}

// vector - vector subtraction 
test_res test_vec_subtraction(vec_t a, vec_t b, vec_t expected) {
   printf("testing vector_vector_sub <%.6f,%.6f,%.6f> - <%.6f,%.6f,%.6f>... ", fix2float(a.x), fix2float(a.y), fix2float(a.z), fix2float(b.x), fix2float(b.y), fix2float(b.z)); 

   vec_t c = vec3d_sub(a,b);

   printf("<%.6f,%.6f,%.6f>", fix2float(c.x),fix2float(c.y),fix2float(c.z));
   if( (c.x != expected.x) || (c.y != expected.y) || (c.z != expected.z)) { 
      printf("\t\tfail\n");
      return Fail;
   }
   printf("\t\tpass\n");
   return Pass;
}

// vector - vector distance 
test_res test_vec_distance(vec_t a, vec_t b, fix16_t expected) {
   printf("testing vector_vector_distance <%.6f,%.6f,%.6f>, <%.6f,%.6f,%.6f>... ", fix2float(a.x), fix2float(a.y), fix2float(a.z), fix2float(b.x), fix2float(b.y), fix2float(b.z)); 

   fix16_t c = vec3d_dist(a,b);

   printf("%.6f", fix2float(c));
   if( c != expected ) { 
      printf("\t\tfail\n");
      return Fail;
   }
   printf("\t\tpass\n");
   return Pass;
}

// vector - vector dot product 
test_res test_vec_dot(vec_t a, vec_t b, fix16_t expected) {
   printf("testing vector_vector_dot product <%.6f,%.6f,%.6f> . <%.6f,%.6f,%.6f>... ", fix2float(a.x), fix2float(a.y), fix2float(a.z), fix2float(b.x), fix2float(b.y), fix2float(b.z)); 

   fix16_t c = vec3d_dot(a,b);

   printf("%.6f", fix2float(c));
   if( c != expected ) { 
      printf("\t\tfail\n");
      return Fail;
   }
   printf("\t\tpass\n");
   return Pass;
}

// vector - vector cross product 
test_res test_vec_cross(vec_t a, vec_t b, vec_t expected) {
   printf("testing vector_vector_cross <%.6f,%.6f,%.6f> X <%.6f,%.6f,%.6f>... ", fix2float(a.x), fix2float(a.y), fix2float(a.z), fix2float(b.x), fix2float(b.y), fix2float(b.z)); 

   vec_t c = vec3d_cross(a,b);

   printf("<%.6f,%.6f,%.6f>", fix2float(c.x),fix2float(c.y),fix2float(c.z));
   if( (c.x != expected.x) || (c.y != expected.y) || (c.z != expected.z)) { 
      printf("\t\tfail\n");
      return Fail;
   }
   printf("\t\tpass\n");
   return Pass;
}


// vector magnitude 
test_res test_mag(vec_t a, fix16_t expected) {
   printf("testing vector_magnitude <%.6f,%.6f,%.6f> ... ", fix2float(a.x), fix2float(a.y), fix2float(a.z)); 

   fix16_t c = vec3d_mag(a);

   printf("%.6f", fix2float(c));
   if( c != expected ) { 
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
  // scalar multiplication
  vec_t a_smul = { float2fix(2.3), float2fix(1.2), float2fix(0.7) };
  vec_t e_smul = { float2fix(1.150024), float2fix(0.599976), float2fix(0.349976) };
	t |= test_scalar_mul(a_smul, float2fix(0.5), e_smul);

  // scalar addition
  vec_t a_sadd = { float2fix(2.3), float2fix(1.2), float2fix(0.7) };
  vec_t e_sadd = { float2fix(2.619995), float2fix(1.519897), float2fix(1.019897) };
	t |= test_scalar_add(a_sadd, float2fix(0.32), e_sadd);

  // scalar subtraction
  vec_t a_ssub = { float2fix(2.3), float2fix(1.2), float2fix(0.7) };
  vec_t e_ssub = { float2fix(1.900024), float2fix(0.799927), float2fix(0.299927) };
	t |= test_scalar_sub(a_ssub, float2fix(0.4), e_ssub);

  // scalar division 
  vec_t a_sdiv = { float2fix(2.3), float2fix(1.2), float2fix(0.7) };
  vec_t e_sdiv = { float2fix(1.642822), float2fix(0.857056), float2fix(0.499878) };
	t |= test_scalar_div(a_ssub, float2fix(1.4), e_sdiv);

  // vector - vector addition
  vec_t a2 = { float2fix(0.5), float2fix(1.1), float2fix(0.7) };
  vec_t b2 = { float2fix(1.25), float2fix(1.2), float2fix(0.4) };
  vec_t e2 = { float2fix(1.75), float2fix(2.299927), float2fix(1.099976) };
	t |= test_vec_addition(a2, b2, e2);

  // vector - vector subtraction 
  vec_t a_sub2 = { float2fix(0.5), float2fix(1.1), float2fix(0.7) };
  vec_t b_sub2 = { float2fix(1.25), float2fix(1.2), float2fix(0.4) };
  vec_t e_sub2 = { float2fix(-0.75), float2fix(-0.099976), float2fix(0.299927) };
	t |= test_vec_subtraction(a_sub2, b_sub2, e_sub2);

  // vector - vector distance 
  vec_t a_dist = { float2fix(0.5), float2fix(1.1), float2fix(0.7) };
  vec_t b_dist = { float2fix(1.25), float2fix(1.2), float2fix(0.4) };
  fix16_t e_dist = float2fix(0.813721);
	t |= test_vec_distance(a_dist, b_dist, e_dist);

  // vector - vector dot product 
  vec_t a_dot = { float2fix(0.5), float2fix(1.1), float2fix(0.7) };
  vec_t b_dot = { float2fix(1.25), float2fix(1.2), float2fix(0.4) };
  fix16_t e_dot = float2fix(2.224731);
	t |= test_vec_dot(a_dot, b_dot, e_dot);

  // vector - vector cross product 
  vec_t a_cross = { float2fix(0.1), float2fix(1.2), float2fix(0.2) };
  vec_t b_cross = { float2fix(0.8), float2fix(0.64), float2fix(0.23) };
  vec_t e_cross = { float2fix(0.147949), float2fix(0.136963), float2fix(-0.895996) };
	t |= test_vec_cross(a_cross, b_cross, e_cross);

  // vector magnitude
  vec_t a_mag = { float2fix(1.8), float2fix(0.73), float2fix(0.38) };
  fix16_t e_mag = float2fix(1.979126);
  t |= test_mag(a_mag, e_mag);  

	printf("\n-----------------------------\n");
	if(t == Fail) {
           printf("Overall: fail.\n");
	   return -1;
	}
        printf("Overall: pass.\n");
	return 0;
}
