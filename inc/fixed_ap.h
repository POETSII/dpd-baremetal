// fixed point precision operators for the DPD simulation
//
// format 1 sign 2 Integer bits 13 fractional bits
#ifndef _FIX_AP_H
#define _FIX_AP_H

#include "stdint.h"
#include "stdio.h"
#include "math.h"

// C - the container type uint8_t, uint16_t, uint32_t, uint64_t
// F - the number of fractional bits
template <class C, unsigned F>
class fixap
{
    private:
        C _value;

    public:

        // constructor with default const parameter
        constexpr explicit fixap(float x) : _value(round(x * (1 << F))) { }
        constexpr explicit fixap(double x) : _value(round(x * (1 << F))) { }

        // constructors
        fixap(uint8_t x)  : _value(x) {}
        fixap(uint16_t x) : _value(x) {}
        fixap(uint32_t x) : _value(x) {}
        fixap(uint64_t x) : _value(x) {}
        fixap(int8_t x)  : _value(x) {}
        fixap(int16_t x) : _value(x) {}
        fixap(int32_t x) : _value(x) {}
        fixap(int64_t x) : _value(x) {}
      
        // sets the fixed point variable from a floating point value (at compile time)
        constexpr void set(float x){
           _value = round(x * (1 << F));
        }

        // get
        C get(){ return _value; }

        // overloaded float operator to return a single precision floating point value of the
        // fixed point number 
        operator float() const {
           return ((float)_value/(1<<F));
        }
        
        float get_float() const {
            return ((float)_value/(1<<F));
        }

        // multiplication
        // uint8_t
        fixap<uint8_t,F> operator *(fixap<uint8_t,F> const& a){
             return fixap<uint8_t, F>(((int16_t)this->_value * (int16_t)a._value)/(1<<F)); 
        }

        // uint16_t
        fixap<uint16_t,F> operator *(fixap<uint16_t,F> const& a){
             return fixap<uint16_t, F>(((int32_t)this->_value * (int32_t)a._value)/(1<<F)); 
        }

        // uint8_t
        fixap<uint32_t,F> operator *(fixap<uint32_t,F> const& a){
             return fixap<uint32_t, F>(((int64_t)this->_value * (int64_t)a._value)/(1<<F)); 
        }


        // division
        // uint8_t
        fixap<uint8_t,F> operator /(fixap<uint8_t,F> const& a){
             return fixap<uint8_t, F>(((int16_t)this->_value *(1<<F))/(int16_t)a._value); 
        }

        // uint16_t
        fixap<uint16_t,F> operator /(fixap<uint16_t,F> const& a){
             return fixap<uint16_t, F>(((int32_t)this->_value *(1<<F))/(int32_t)a._value); 
        }
        
        // uint32_t
        fixap<uint32_t,F> operator /(fixap<uint32_t,F> const& a){
             return fixap<uint32_t, F>(((int64_t)this->_value *(1<<F))/(int64_t)a._value); 
        }
        

};

#endif /* _FIX_AP_H */
