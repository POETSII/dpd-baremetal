// A test file for the fixed point arithmetic stuff
#include "fixed_ap.h"
#include "vec3d.h"
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

  printf("testing the square roots\n");
  fixap<uint16_t, 14> a7(1.5);
  printf("<16,14> inv_sqrt(1.5) = %.4f\n", (float)a7.inv_sqrt(20)); 

  fixap<uint16_t, 13> a6(1.5);
  printf("<16,13> inv_sqrt(1.5) = %.4f\n", (float)a6.inv_sqrt(20)); 

  fixap<uint16_t, 12> a8(1.5);
  printf("<16,12> inv_sqrt(1.5) = %.4f\n", (float)a8.inv_sqrt(20)); 

  fixap<uint16_t, 11> a9(1.5);
  printf("<16,11> inv_sqrt(1.5) = %.4f\n", (float)a9.inv_sqrt(20)); 

  fixap<uint8_t, 6> a10(1.5);
  printf("<8,6> inv_sqrt(1.5) = %.4f\n", (float)a10.inv_sqrt(20)); 

  fixap<uint32_t, 30> a11(1.5);
  printf("<32,30> inv_sqrt(1.5) = %.4f\n", (float)a11.inv_sqrt(20)); 

  fixap<uint32_t, 30> a11_a(1.5546);
  printf("<32,13> inv_sqrt(1.5546) = %.4f\n", (float)a11_a.inv_sqrt(25)); 

  fixap<uint16_t, 14> a12(1.5);
  printf("<16,14> sqrt(1.5) = %.4f\n", (float)a12.sqrt(20)); 

  // half test
  fixap<uint32_t, 30> ht1(2.4);
  printf("<32,30> 2.4/2 = %.4f\n", (float)ht1.half());

  // -------------------------------
  //  vector testing
  // -------------------------------

  vec3d<int16_t, 13> vec_a1(0.67, 0.56, 0.89);
  vec3d<int16_t, 13> vec_b1(1.2, 0.68, 0.92);

  // vector - vector addition
  vec3d<int16_t, 13> vec_out1 = vec_a1 + vec_b1;
  printf("%s + %s = %s\n", vec_a1.str().c_str(), vec_b1.str().c_str(),vec_out1.str().c_str());

  vec3d<int16_t, 13> vec_out2 = vec_a1 + 0.65;
  printf("%s + 0.65 = %s\n", vec_a1.str().c_str(), vec_out2.str().c_str());

  vec3d<int16_t, 13> vec_out3 = vec_a1 + fixap<int16_t, 13>(0.65);
  printf("%s + 0.65 = %s\n", vec_a1.str().c_str(), vec_out3.str().c_str());

  // vector - vector subtraction
  vec3d<int16_t, 13> vec_out4 = vec_a1 - vec_b1;
  printf("%s - %s = %s\n", vec_a1.str().c_str(), vec_b1.str().c_str(),vec_out4.str().c_str());

  vec3d<int16_t, 13> vec_out5 = vec_a1 - 0.65;
  printf("%s - 0.65 = %s\n", vec_a1.str().c_str(), vec_out5.str().c_str());

  vec3d<int16_t, 13> vec_out6 = vec_a1 - fixap<int16_t, 13>(0.65);
  printf("%s - 0.65 = %s\n", vec_a1.str().c_str(), vec_out6.str().c_str());

  // scalar mult and div
  vec3d<int16_t, 13> vec_out7 = vec_a1 * 0.05;
  printf("%s * 0.05 = %s\n", vec_a1.str().c_str(), vec_out7.str().c_str());

  vec3d<int16_t, 13> vec_out8 = vec_a1 / 1.2;
  printf("%s / 1.2 = %s\n", vec_a1.str().c_str(), vec_out8.str().c_str());

  // magnitude
  printf("mag(%s) = %.4f\n", vec_a1.str().c_str(), (float)vec_a1.convert_u16().mag());

  // dot product
  fixap<int16_t, 13> vec_out9 = vec_a1.dot(vec_b1);
  printf("%s . %s = %.4f\n", vec_a1.str().c_str(), vec_b1.str().c_str(), (float)vec_out9);

  // distance 
  fixap<int16_t, 13> vec_out10 = vec_a1.dist(vec_b1);
  printf("distance(%s <-> %s) = %.4f\n", vec_a1.str().c_str(), vec_b1.str().c_str(), (float)vec_out10);

  printf("\n-----------------------------\n");
  if(t == 1) {
    printf("Overall: fail.\n");
    return -1;
  }
  printf("Overall: pass.\n");
  return 0;
}

