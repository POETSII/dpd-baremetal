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
      
        fixap<C, F> half() const {
              return fixap<C,F>(_value >> 1);
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

        // uint32_t
        fixap<uint32_t,F> operator *(fixap<uint32_t,F> const& a){
             return fixap<uint32_t, F>(((int64_t)this->_value * (int64_t)a._value)/(1<<F)); 
        }

        // int8_t
        fixap<int8_t,F> operator *(fixap<int8_t,F> const& a){
             return fixap<int8_t, F>(((int16_t)this->_value * (int16_t)a._value)/(1<<F)); 
        }

        // int16_t
        fixap<int16_t,F> operator *(fixap<int16_t,F> const& a){
             return fixap<int16_t, F>(((int32_t)this->_value * (int32_t)a._value)/(1<<F)); 
        }

        // int32_t
        fixap<int32_t,F> operator *(fixap<int32_t,F> const& a){
             return fixap<int32_t, F>(((int64_t)this->_value * (int64_t)a._value)/(1<<F)); 
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
        
        // int8_t
        fixap<int8_t,F> operator /(fixap<int8_t,F> const& a){
             return fixap<int8_t, F>(((int16_t)this->_value *(1<<F))/(int16_t)a._value); 
        }

        // int16_t
        fixap<int16_t,F> operator /(fixap<int16_t,F> const& a){
             return fixap<int16_t, F>(((int32_t)this->_value *(1<<F))/(int32_t)a._value); 
        }
        
        // int32_t
        fixap<int32_t,F> operator /(fixap<int32_t,F> const& a){
             return fixap<int32_t, F>(((int64_t)this->_value *(1<<F))/(int64_t)a._value); 
        }

        // addition
        fixap<C,F> operator +(fixap<uint8_t,F> const& a){ return fixap<C,F>(_value + a._value); }
        fixap<C,F> operator +(fixap<uint16_t,F> const& a){ return fixap<C,F>(_value + a._value); }
        fixap<C,F> operator +(fixap<uint32_t,F> const& a){ return fixap<C,F>(_value + a._value); }
        fixap<C,F> operator +(fixap<uint64_t,F> const& a){ return fixap<C,F>(_value + a._value); }

        fixap<C,F> operator +(fixap<int8_t,F> const& a){ return fixap<C,F>(_value + a._value); }
        fixap<C,F> operator +(fixap<int16_t,F> const& a){ return fixap<C,F>(_value + a._value); }
        fixap<C,F> operator +(fixap<int32_t,F> const& a){ return fixap<C,F>(_value + a._value); }
        fixap<C,F> operator +(fixap<int64_t,F> const& a){ return fixap<C,F>(_value + a._value); }

        // subtraction
        fixap<C,F> operator -(fixap<uint8_t,F> const& a){ return fixap<C,F>(_value - a._value); }
        fixap<C,F> operator -(fixap<uint16_t,F> const& a){ return fixap<C,F>(_value - a._value); }
        fixap<C,F> operator -(fixap<uint32_t,F> const& a){ return fixap<C,F>(_value - a._value); }
        fixap<C,F> operator -(fixap<uint64_t,F> const& a){ return fixap<C,F>(_value - a._value); }

        fixap<C,F> operator -(fixap<int8_t,F> const& a){ return fixap<C,F>(_value - a._value); }
        fixap<C,F> operator -(fixap<int16_t,F> const& a){ return fixap<C,F>(_value - a._value); }
        fixap<C,F> operator -(fixap<int32_t,F> const& a){ return fixap<C,F>(_value - a._value); }
        fixap<C,F> operator -(fixap<int64_t,F> const& a){ return fixap<C,F>(_value - a._value); }


        // inverse_sqrt -- newton raphson method
        // adapted from https://stackoverflow.com/questions/6286450/inverse-sqrt-for-fixed-point
        fixap<C, F> inv_sqrt(uint16_t iters) {
           fixap<C,F> three(3.0); 
 
           // initial guess taken from https://sites.math.washington.edu/~morrow/336_12/papers/ben.pdf
           uint32_t t = _value >> 1;
           fixap<C,F> y = 0x5f3759df - t; // magic number

           for(uint32_t i=0; i<iters; i++) {
              y = y * (three - (*this * y * y)).half(); 
           }
           return y;
        }

        // sqrt - recipriocal of the inverse square root
        fixap<C,F> sqrt(uint32_t iters) {
            return fixap<C,F>(1.0)/this->inv_sqrt(iters);
        }

};

#endif /* _FIX_AP_H */
