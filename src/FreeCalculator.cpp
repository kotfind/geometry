#include "FreeCalculator.h"

FreeCalculator::FreeCalculator()
{}

Object* FreeCalculator::calc(const Object* obj) const {
    return obj->clone();
}

bool FreeCalculator::isFree() const {
    return true;
}
