//! A 3D vector class that is double precision for comparing the accuracy of different fixed point implementations 
#ifndef __VEC_DOUBLE_3D_H
#define __VEC_DOUBLE_3D_H

#include <math.h>
#include <sstream>
#include <string>
//#include <cmath.h>

class vec3d_double {
   public:
       vec3d_double(double x, double y, double z); /**< constructor */
       vec3d_double(); /**< default constructor */
       ~vec3d_double(); /**< destructor */
       vec3d_double(const vec3d_double &in); /**< copy constructor */

       // various getters and setters
       void clear(); /**< clears the current vector values (sets them all to zero) */
       double x(); /**< returns the x value of this vector */
       double y(); /**< returns the x value of this vector */
       double z(); /**< returns the x value of this vector */
       void x(double in); /**< sets the value of x */
       void y(double in); /**< sets the value of y */
       void z(double in); /**< sets the value of z */
       void set(double x, double y, double z); /**< sets the vector value */

       //         operations 
       // ----------------------------
       // multiplication
       vec3d_double operator*(vec3d_double const& a); /**< multiple a vector to this */
       vec3d_double operator*(double const& a); /**< multiple a scalar to this */
       //vec3d_double operator*(double a, vec3d_double &b); /**< computers vector = vector * double */
       // addition
       vec3d_double operator+(vec3d_double const& a); /**< add a vector to this */
       vec3d_double operator+(double const& a); /**< add a scalar to this */
       // subtraction 
       vec3d_double operator-(vec3d_double const& a); /**< subtract a vector from this */
       vec3d_double operator-(double const& a); /**< subtract a scalar to this */
       // scalar division
       vec3d_double operator/(double const& a); /**< divide the vector by a scalar value */
       // toroidal subtraction
       vec3d_double toroidal_subtraction(vec3d_double a, double N, double R_C);

       // dot product
       double dot(vec3d_double a); /**< computes the dot product between this and vector a */

       // cross product
       vec3d_double cross(vec3d_double a); /**< computes the cross product between this and vector a */

       // magnitude
       double mag(); /**< calculates the magnitude of this vector */

       // modulo add
       vec3d_double modulo_add(vec3d_double a, double N);

       // distance
       double dist(vec3d_double a); /**< calculates the euclidean distance */
       double toroidal_dist(vec3d_double a, double N); /**< calculates the euclidean distance */

       // format string
       std::string str();

   private:
       double _x;
       double _y;
       double _z;
};

#endif /* __VEC_DOUBLE_3D_H */
