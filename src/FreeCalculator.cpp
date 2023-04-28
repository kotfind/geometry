#include "FreeCalculator.h"

FreeCalculator::FreeCalculator(std::unique_ptr<Object> obj)
  : obj(std::move(obj))
{}

Object* FreeCalculator::calc() const {
    return obj->clone();
}

bool FreeCalculator::isFree() const {
    return true;
}
