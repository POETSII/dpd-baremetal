// implementation file for the vec3d_double class
#include "vec3d_double.h"

// constructor
vec3d_double::vec3d_double(double x, double y, double z){
   _x = x; 
   _y = y; 
   _z = z;
}

// default constructor
vec3d_double::vec3d_double() {
  _x = 0.0;
  _y = 0.0;
  _z = 0.0;
}

// destructor
vec3d_double::~vec3d_double() {

}

// copy constructor
vec3d_double::vec3d_double(const vec3d_double &in) {
   _x = in._x;
   _y = in._y;
   _z = in._z;
}


// clears the vector
void vec3d_double::clear() {
   _x = 0.0;
   _y = 0.0;
   _z = 0.0;
}

// getters
double vec3d_double::x() { return _x; }
double vec3d_double::y() { return _y; }
double vec3d_double::z() { return _z; }

// setters
void vec3d_double::x(double in) { _x = in; }
void vec3d_double::y(double in) { _y = in; }
void vec3d_double::z(double in) { _z = in; }

// setter
void vec3d_double::set(double x, double y, double z) {
    _x = x;
    _y = y;
    _z = z;
}

// operators
//----------------------
// vector * vector I don't think this is correct?
vec3d_double vec3d_double::operator*(vec3d_double const& a) {
   vec3d_double c;
   double x =  this->_x * a._x;
   double y =  this->_y * a._y;
   double z =  this->_z * a._z;
   c.set(x,y,z);
   return c;
}

// vector * scalar
vec3d_double vec3d_double::operator*(double const& a) {
   vec3d_double c;
   double x = this->_x * a;
   double y = this->_y * a;
   double z = this->_z * a;
   c.set(x,y,z);
   return c;
}

// vector + vector
vec3d_double vec3d_double::operator+(vec3d_double const& a) {
   vec3d_double c;
   double x =  this->_x  + a._x;
   double y =  this->_y  + a._y;
   double z =  this->_z  + a._z;
   c.set(x,y,z);
   return c;
}

// vector + scalar
vec3d_double vec3d_double::operator+(double const& a) {
   vec3d_double c;
   double x = this->_x + a;
   double y = this->_y + a;
   double z = this->_z + a;
   c.set(x,y,z);
   return c;
}

// vector - vector
vec3d_double vec3d_double::operator-(vec3d_double const& a) {
   vec3d_double c;
   double x = this->_x - a._x;
   double y = this->_y - a._y;
   double z = this->_z - a._z;
   c.set(x,y,z);
   return c;
}

// vector - scalar
vec3d_double vec3d_double::operator-(double const& a) {
   vec3d_double c;
   double x = this->_x - a;
   double y = this->_y - a;
   double z = this->_z - a;
   c.set(x,y,z);
   return c;
}

// vector / scalar
vec3d_double vec3d_double::operator/(double const& a) {
   vec3d_double c;
   double x = this->_x / a;
   double y = this->_y / a;
   double z = this->_z / a;
   c.set(x,y,z);
   return c;
}

// dot product
double vec3d_double::dot(vec3d_double a) {
    return (_x * a.x()) + (_y * a.y()) + (_z * a.z());
}

// modulo add (based on the universe size)
vec3d_double vec3d_double::modulo_add(vec3d_double a, double N) {
    // add to the vector mod the universe size
    double x = fmod(_x + a.x(), N); 
    double y = fmod(_y + a.y(), N); 
    double z = fmod(_z + a.z(), N); 

    return vec3d_double(x,y,z);

}

// toroidal distance
// returns this - a ( over a toroidal space NxNxN, where the cutoff is R_C)
vec3d_double vec3d_double::toroidal_subtraction(vec3d_double a, double N, double R_C) {

  double diff_x = _x - a.x();
  double diff_y = _y - a.y();
  double diff_z = _z - a.z();

  //if(diff_x > N/2){
  //   diff_x = N - diff_x;     
  //} else if (diff_x < -1.0*(N/2)) {
  //   diff_x = N + diff_x;
  //} 

  //if(diff_y > N/2){
  //  diff_y = N - diff_y;
  //} else if (diff_y < -1.0*(N/2)) {
  //   diff_y = N + diff_y;
  //} 

  //if(diff_z > N/2){
  //  diff_z = N - diff_z;
  //} else if (diff_z < -1.0*(N/2)) {
  //   diff_z = N + diff_z;
  //} 

  if(diff_x > R_C) {
     diff_x = N - diff_x;
  } else if (diff_x < -1.0*R_C) {
     diff_x = N + diff_x;
  }

  if(diff_y > R_C) {
    diff_y = N - diff_y;
  } else if (diff_y < -1.0*R_C) {
    diff_y = N + diff_y;
  }

  if(diff_z > R_C) {
    diff_z = N - diff_z;
  } else if (diff_z < -1.0*R_C) {
    diff_z = N + diff_z;
  }
 
  return vec3d_double(diff_x, diff_y, diff_z);  
}

// cross product
vec3d_double vec3d_double::cross(vec3d_double a){
   vec3d_double c;
   double x = (_y*a.z()) - (_z*a.y()); 
   double y = (_z*a.x()) - (_x*a.z()); 
   double z = (_x*a.y()) - (_y*a.x()); 
   c.set(x,y,z);
   return c;
}

// mag
double vec3d_double::mag(){
   return sqrt(_x*_x + _y*_y + _z*_z);
} 

// dist
double vec3d_double::dist(vec3d_double a) {
   vec3d_double c = *this - a; 
   return c.mag();
}

// calculate the toroidal distance
double vec3d_double::toroidal_dist(vec3d_double a, double N) {

    double dx = fabs(a.x() - _x);
    double dy = fabs(a.y() - _y);
    double dz = fabs(a.z() - _z);

    if(dx > (N/2)) {
        dx = N - dx; 
    }  

    if(dy > (N/2)) {
        dy = N - dy; 
    }  

    if(dz > (N/2)) {
        dz = N - dz; 
    }  

    return sqrt(dx*dx + dy*dy + dz*dz);
}

// returns a formatted string
std::string vec3d_double::str(){
   std::stringstream ss;
   ss << "<"<<x()<<", "<<y()<<", "<<z()<<">";
   return ss.str();
}
