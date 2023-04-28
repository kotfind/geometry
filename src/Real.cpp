#include "Real.h"

Real::Real() : Real(0) {}
Real::Real(double value) : Object(), value(value) {}

Object* Real::clone() const {
    return new Real(*this);
}
