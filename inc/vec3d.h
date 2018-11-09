// a vector class that build on the fixed point class defined in fixed_ap.h
#ifndef _VEC_FIXED_H
#define _VEC_FIXED_H
#include <stdio.h>
#include <sstream>
#include <string>

// C - the container type uint8_t, uint16_t, uint32_t, uint64_t
// F - the number of fractional bits
template <class C, unsigned F>
class vec3d {
   private:
      fixap<C,F> _x;
      fixap<C,F> _y;
      fixap<C,F> _z;
  public:
     constexpr explicit vec3d(float x, float y, float z) : _x(fixap<C,F>(x)), _y(fixap<C,F>(y)), _z(fixap<C,F>(z)) {} 
     constexpr explicit vec3d(double x, double y, double z) : _x(fixap<C,F>(x)), _y(fixap<C,F>(y)), _z(fixap<C,F>(z)) {} 
     vec3d<C,F>(fixap<C,F> x, fixap<C,F> y, fixap<C,F> z) : _x(x), _y(y), _z(z) {}
    
     // getters
     fixap<C,F> x() { return _x; }
     fixap<C,F> y() { return _y; }
     fixap<C,F> z() { return _z; }

     // converters
     vec3d<uint8_t,F>  convert_u8()  const { return vec3d<uint8_t,F>(_x.convert_u8(), _y.convert_u8(), _z.convert_u8()   ); }
     vec3d<uint16_t,F> convert_u16() const { return vec3d<uint16_t,F>(_x.convert_u16(), _y.convert_u16(), _z.convert_u16()); }
     vec3d<uint32_t,F> convert_u32() const { return vec3d<uint32_t,F>(_x.convert_u32(), _y.convert_u32(), _z.convert_u32()); }
     vec3d<uint64_t,F> convert_u64() const { return vec3d<uint64_t,F>(_x.convert_u64(), _y.convert_u64(), _z.convert_u64()); }

     vec3d<int8_t,F>  convert_s8()  const  { return vec3d<int8_t,F>(_x.convert_u8(), _y.convert_u8(), _z.convert_u8()   );  }
     vec3d<int16_t,F> convert_s16() const  { return vec3d<int16_t,F>(_x.convert_u16(), _y.convert_u16(), _z.convert_u16());  }
     vec3d<int32_t,F> convert_s32() const  { return vec3d<int32_t,F>(_x.convert_u32(), _y.convert_u32(), _z.convert_u32());  }
     vec3d<int64_t,F> convert_s64() const  { return vec3d<int64_t,F>(_x.convert_u64(), _y.convert_u64(), _z.convert_u64());  }
 
     // vector arithmetic operations
     // addition
     vec3d<C,F> operator+(vec3d<C,F> const& a) { return vec3d<C,F>(_x + a._x, _y + a._y, _z + a._z); }
     vec3d<C,F> operator+(float a) { return vec3d<C,F>(_x + fixap<C,F>(a), _y + fixap<C,F>(a), _z + fixap<C,F>(a)); }
     vec3d<C,F> operator+(double a) { return vec3d<C,F>(_x + fixap<C,F>(a), _y + fixap<C,F>(a), _z + fixap<C,F>(a)); }
     vec3d<C,F> operator+(fixap<C,F> a) { return vec3d<C,F>(_x + a, _y + a, _z + a); }
     
     // subtraction 
     vec3d<C,F> operator - (vec3d<C,F> const& a) { return vec3d<C,F>(_x - a._x, _y - a._y, _z - a._z); }
     vec3d<C,F> operator - (float a) { return vec3d<C,F>(_x - fixap<C,F>(a), _y - fixap<C,F>(a), _z - fixap<C,F>(a)); }
     vec3d<C,F> operator - (double a) { return vec3d<C,F>(_x - fixap<C,F>(a), _y - fixap<C,F>(a), _z - fixap<C,F>(a)); }
     vec3d<C,F> operator - (fixap<C,F> a) { return vec3d<C,F>(_x - a, _y - a, _z - a); }
     
     // scalar division
     vec3d<C,F> operator / (float a) { return vec3d<C,F>(_x/fixap<C,F>(a), _y/fixap<C,F>(a), _z/fixap<C,F>(a)); } 
     vec3d<C,F> operator / (double a) { return vec3d<C,F>(_x/fixap<C,F>(a), _y/fixap<C,F>(a), _z/fixap<C,F>(a)); } 
     vec3d<C,F> operator / (fixap<C,F> a) { return vec3d<C,F>(_x/a, _y/a, _z/a); } 

     // scalar multiplication
     vec3d<C,F> operator * (float a) { return vec3d<C,F>(_x*fixap<C,F>(a), _y*fixap<C,F>(a), _z*fixap<C,F>(a)); } 
     vec3d<C,F> operator * (double a) { return vec3d<C,F>(_x*fixap<C,F>(a), _y*fixap<C,F>(a), _z*fixap<C,F>(a)); } 
     vec3d<C,F> operator * (fixap<C,F> a) { return vec3d<C,F>(_x*a, _y*a, _z*a); } 

     // mag -- returns the magnitude of this vector
     fixap<C,F> mag() { 
           fixap<C,F> t = (this->_x*this->_x ) +(this->_y*this->_y) + (this->_z*this->_z); 
           return t.sqrt(25);
     }

     std::string str() {
        std::stringstream ss;
        ss << "<" << (float)_x << "," << (float)_y << "," << (float)_z <<">";
        return ss.str();
     } 

};

#endif /* _VEC_FIXED_H */
