//! A 3D vector class for computing forces etc.. in the simple DPD particle simulator
#ifndef __VECTOR_3D_H
#define __VECTOR_3D_H

#include <math.h>
// #include "fixed_ap.h"
#include "stdint.h"
#include "utils.hpp"

template<class S>
class Vector3D {
   public:
       Vector3D(S x, S y, S z); /**< constructor */
       Vector3D(); /**< default constructor */
       ~Vector3D(); /**< destructor */
       Vector3D(const Vector3D<S> &in); /**< copy constructor */

       // various getters and setters
       void clear(); /**< clears the current vector values (sets them all to zero) */
       S x() const volatile; /**< returns the x value of this vector */
       S y() const volatile; /**< returns the x value of this vector */
       S z() const volatile; /**< returns the x value of this vector */
       void x(S in) volatile; /**< sets the value of x */
       void y(S in) volatile; /**< sets the value of y */
       void z(S in) volatile; /**< sets the value of z */
       void set(const S x, const S y, const S z) volatile; /**< sets the vector value */

       //         operations
       // ----------------------------
       // multiplication
       Vector3D<S> operator*(Vector3D<S> const& a); /**< multiple a vector to this makes no sense -- remove*/
       Vector3D<S> operator*(S const& a); /**< multiple a scalar to this */

       // addition
       Vector3D<S> operator+(Vector3D<S> const& a); /**< add a vector to this */
       Vector3D<S> operator+(S const& a); /**< add a scalar to this */
       // subtraction
       Vector3D<S> operator-(Vector3D<S> const& a); /**< subtract a vector from this */
       Vector3D<S> operator-(S const& a); /**< subtract a scalar to this */
       // scalar division
       Vector3D<S> operator/(S const& a); /**< divide the vector by a scalar value */

       // dot product
       S dot(Vector3D<S> a); /**< computes the dot product between this and vector a */

       // cross product
       Vector3D<S> cross(Vector3D<S> a); /**< computes the cross product between this and vector a */

       // magnitude
       S mag(); /**< calculates the magnitude of this vector */

       // modulo add
       Vector3D<float> modulo_add(Vector3D<S> a, float N);

       // distance
       S dist(Vector3D<S> a); /**< calculates the euclidean distance */

       // Euclidian distance squared (less time in sqrt hopefully)
       S sq_dist(Vector3D<S> a);
#ifndef FLOAT_ONLY
       Vector3D<int32_t> floatToFixed();
       Vector3D<float> fixedToFloat();
#endif

   private:
       S _x;
       S _y;
       S _z;
};

#include "../src/Vector3D.cpp"

#endif /* __VECTOR_3D_H */
