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

        // isZero returns true if this fixap number is zero
        bool isZero() { return (this->_value == 0); }

        // smallest returns the smallest value this number can represent
        fixap<C,F> smallest() { C t = 1; fixap<C,F> tmp(t); return t; }

        // constructors
        fixap() : _value(0) {}
        fixap(uint8_t x)  : _value(x) {}
        fixap(uint16_t x) : _value(x) {}
        fixap(uint32_t x) : _value(x) {}
        fixap(uint64_t x) : _value(x) {}
        fixap(int8_t x)  : _value(x) {}
        fixap(int16_t x) : _value(x) {}
        fixap(int32_t x) : _value(x) {}
        fixap(int64_t x) : _value(x) {}
      
        // double assignment operator
        fixap<C,F>& operator =(const double x) {
           _value = round(x * (1 << F));
        }

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
        
        // comparisons
        bool operator < (fixap<C,F> const& a) 
        { return this->_value < a._value; }

        bool operator > (fixap<C,F> const& a) 
        { return this->_value > a._value; }

        bool operator == (fixap<C,F> const& a) 
        { return this->_value == a._value; }

        bool operator >= (fixap<C,F> const& a) 
        { return this->_value >= a._value; }

        bool operator <= (fixap<C,F> const& a) 
        { return this->_value <= a._value; }

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
             fixap<uint8_t,F> t = a;
             if(t.isZero())
                 t = t.smallest(); 
             return fixap<uint8_t, F>(((int16_t)this->_value *(1<<F))/(int16_t)t._value); 
        }

        // uint16_t
        fixap<uint16_t,F> operator /(fixap<uint16_t,F> const& a){
             fixap<uint16_t,F> t = a;
             if(t.isZero())
                 t = t.smallest(); 
             return fixap<uint16_t, F>(((int32_t)this->_value *(1<<F))/(int32_t)t._value); 
        }
        
        // uint32_t
        fixap<uint32_t,F> operator /(fixap<uint32_t,F> const& a){
             fixap<uint32_t,F> t = a;
             if(t.isZero())
                 t = t.smallest(); 
             return fixap<uint32_t, F>(((int64_t)this->_value *(1<<F))/(int64_t)t._value); 
        }
        
        // int8_t
        fixap<int8_t,F> operator /(fixap<int8_t,F> const& a){
             fixap<int8_t,F> t = a;
             if(t.isZero())
                 t = t.smallest(); 
             return fixap<int8_t, F>(((int16_t)this->_value *(1<<F))/(int16_t)t._value); 
        }

        // int16_t
        fixap<int16_t,F> operator /(fixap<int16_t,F> const& a){
             fixap<int16_t,F> t = a;
             if(t.isZero())
                 t = t.smallest(); 
             return fixap<int16_t, F>(((int32_t)this->_value *(1<<F))/(int32_t)t._value); 
        }
        
        // int32_t
        fixap<int32_t,F> operator /(fixap<int32_t,F> const& a){
             fixap<int32_t,F> t = a;
             if(t.isZero())
                 t = t.smallest(); 
             return fixap<int32_t, F>(((int64_t)this->_value *(1<<F))/(int64_t)t._value); 
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

// conversion functions
template<unsigned F>
fixap<uint64_t,F> to_unsigned(fixap<int64_t, F> x) { return fixap<uint64_t,F>((uint64_t)x.get()); }
template<unsigned F>
fixap<uint32_t,F> to_unsigned(fixap<int32_t, F> x) { return fixap<uint32_t,F>((uint32_t)x.get()); }
template<unsigned F>
fixap<uint16_t,F> to_unsigned(fixap<int16_t, F> x) { return fixap<uint16_t,F>((uint16_t)x.get()); }
template<unsigned F>
fixap<uint8_t,F> to_unsigned(fixap<int8_t, F> x) { return fixap<uint8_t,F>((uint8_t)x.get()); }

template<unsigned F>
fixap<int64_t,F> to_signed(fixap<uint64_t, F> x) { return fixap<int64_t,F>((int64_t)x.get()); }
template<unsigned F>
fixap<int32_t,F> to_signed(fixap<uint32_t, F> x) { return fixap<int32_t,F>((int32_t)x.get()); }
template<unsigned F>
fixap<int16_t,F> to_signed(fixap<uint16_t, F> x) { return fixap<int16_t,F>((int16_t)x.get()); }
template<unsigned F>
fixap<int8_t,F> to_signed(fixap<uint8_t, F> x) { return fixap<int8_t,F>((int8_t)x.get()); }


// sqrt functions
template<unsigned F>
fixap<int64_t, F> sqrt(fixap<int64_t, F> in) {
    fixap<uint64_t, F> t = to_unsigned(in);
    return to_signed(t.sqrt(100));
}

template<unsigned F>
fixap<int32_t, F> sqrt(fixap<int32_t, F> in) {
    fixap<uint32_t, F> t = to_unsigned(in);
    return to_signed(t.sqrt(100));
}

template<unsigned F>
fixap<int16_t, F> sqrt(fixap<int16_t, F> in) {
    fixap<uint16_t, F> t = to_unsigned(in);
    return to_signed(t.sqrt(100));
}

template<unsigned F>
fixap<int8_t, F> sqrt(fixap<int8_t, F> in) {
    fixap<uint8_t, F> t = to_unsigned(in);
    return to_signed(t.sqrt(100));
}

//template<class C, unsigned F>
//fixap<C,F> sqrt(fixap<C,F> in){
//    return in.sqrt(40); // newton raphson 30 iterations default
//}

#endif /* _FIX_AP_H */
