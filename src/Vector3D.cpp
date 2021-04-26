// implementation file for the Vector3D class
#include "Vector3D.hpp"

#ifndef _VECTOR_3D_IMPL
#define _VECTOR_3D_IMPL

#ifndef FLOAT_ONLY
const float FIXED = (1<<20);
const float FLOAT = 1/FIXED;
#endif

// constructor
template<class S>
Vector3D<S>::Vector3D(S x, S y, S z){
   _x = x;
   _y = y;
   _z = z;
}

// default constructor
template<class S>
Vector3D<S>::Vector3D() {
}

// destructor
template<class S>
Vector3D<S>::~Vector3D() {

}

// copy constructor
template<class S>
Vector3D<S>::Vector3D(const Vector3D<S> &in) {
   _x = in._x;
   _y = in._y;
   _z = in._z;
}

// clears the vector
template<class S>
void Vector3D<S>::clear() {
   _x = 0.0;
   _y = 0.0;
   _z = 0.0;
}

// getters
template<class S>
S Vector3D<S>::x() const volatile { return _x; }
template<class S>
S Vector3D<S>::y() const volatile { return _y; }
template<class S>
S Vector3D<S>::z() const volatile { return _z; }

// setters
template<class S>
void Vector3D<S>::x(S in) volatile { _x = in; }
template<class S>
void Vector3D<S>::y(S in) volatile { _y = in; }
template<class S>
void Vector3D<S>::z(S in) volatile { _z = in; }

// setter
template<class S>
void Vector3D<S>::set(const S x, const S y, const S z) volatile {
    _x = x;
    _y = y;
    _z = z;
}

// operators
//----------------------
// vector * vector I don't think this is correct?
template<class S>
Vector3D<S> Vector3D<S>::operator*(Vector3D<S> const& a) {
   Vector3D<S> c;
   S x =  this->_x * a._x;
   S y =  this->_y * a._y;
   S z =  this->_z * a._z;
   c.set(x,y,z);
   return c;
}

// vector * scalar
template<class S>
Vector3D<S> Vector3D<S>::operator*(S const& a) {
   Vector3D c;
   S x = this->_x * a;
   S y = this->_y * a;
   S z = this->_z * a;
   c.set(x,y,z);
   return c;
}

// vector + vector
template<class S>
Vector3D<S> Vector3D<S>::operator+(Vector3D<S> const& a) {
   Vector3D<S> c;
   S x =  this->_x  + a._x;
   S y =  this->_y  + a._y;
   S z =  this->_z  + a._z;
   c.set(x,y,z);
   return c;
}

// vector + scalar
template<class S>
Vector3D<S> Vector3D<S>::operator+(S const& a) {
   Vector3D<S> c;
   S x = this->_x + a;
   S y = this->_y + a;
   S z = this->_z + a;
   c.set(x,y,z);
   return c;
}

// vector - vector
template<class S>
Vector3D<S> Vector3D<S>::operator-(Vector3D<S> const& a) {
   Vector3D<S> c;
   S x = this->_x - a._x;
   S y = this->_y - a._y;
   S z = this->_z - a._z;
   c.set(x,y,z);
   return c;
}

// vector - scalar
template<class S>
Vector3D<S> Vector3D<S>::operator-(S const& a) {
   Vector3D<S> c;
   S x = this->_x - a;
   S y = this->_y - a;
   S z = this->_z - a;
   c.set(x,y,z);
   return c;
}

// vector / scalar
template<class S>
Vector3D<S> Vector3D<S>::operator/(S const& a) {
   Vector3D<S> c;
   S x = this->_x / a;
   S y = this->_y / a;
   S z = this->_z / a;
   c.set(x,y,z);
   return c;
}

// dot product
template<class S>
S Vector3D<S>::dot(Vector3D<S> a) {
    return (_x * a.x()) + (_y * a.y()) + (_z * a.z());
}

// modulo add (based on the universe size)
template<class S>
Vector3D<float> Vector3D<S>::modulo_add(Vector3D<S> a, float N) {
    // add to the vector mod the universe size
    float x = fmod(_x + (float)a.x(), N);
    float y = fmod(_y + (float)a.y(), N);
    float z = fmod(_z + (float)a.z(), N);

    return Vector3D<float>(x,y,z);

}

// cross product
template<class S>
Vector3D<S> Vector3D<S>::cross(Vector3D<S> a){
   Vector3D<S> c;
   S x = (_y*a.z()) - (_z*a.y());
   S y = (_z*a.x()) - (_x*a.z());
   S z = (_x*a.y()) - (_y*a.x());
   c.set(x,y,z);
   return c;
}

// mag
template<class S>
S Vector3D<S>::mag(){
   #ifndef TINSEL
   return sqrt(_x*_x + _y*_y + _z*_z);
   #else
   return newt_sqrt(_x*_x + _y*_y + _z*_z);
   #endif
}

// dist
template<class S>
S Vector3D<S>::dist(Vector3D<S> a) {
   Vector3D<S> c = *this - a;
   return c.mag();
}

// Euclidian distance squared (less time in sqrt hopefully)
template<class S>
S Vector3D<S>::sq_dist(Vector3D<S> a) {
   Vector3D<S> c = *this - a;

   return (c.x()*c.x() + c.y()*c.y() + c.z()*c.z());
}

#ifndef FLOAT_ONLY
//Floating-point Vector to fixed-point Vector
template<class S>
Vector3D<int32_t> Vector3D<S>::floatToFixed() {
  Vector3D<int32_t> c;
  int32_t x = int32_t(this->_x * FIXED);
  int32_t y = int32_t(this->_y * FIXED);
  int32_t z = int32_t(this->_z * FIXED);
  c.set(x, y, z);
  return c;
}

//Fixed-point Vector to floating-point Vector
template<class S>
Vector3D<float> Vector3D<S>::fixedToFloat() {
  Vector3D<float> c;
  float x = (float)(this->_x) * FLOAT;
  float y = (float)(this->_y) * FLOAT;
  float z = (float)(this->_z) * FLOAT;
  c.set(x, y, z);
  return c;
}
#endif

#endif /* _VECTOR_3D_IMPL */
