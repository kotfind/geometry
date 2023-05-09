#include "FreeCalculator.h"

#include "Real.h"

FreeCalculator::FreeCalculator(std::unique_ptr<Object> obj)
  : obj(std::move(obj))
{}

Object* FreeCalculator::calc() const {
    return obj->clone();
}

Calculator::Type FreeCalculator::getType() const {
    return Type::FREE;
}

void FreeCalculator::setPos(const QPointF& pos) {
    assert(obj->is(Object::Type::Point));
    static_cast<Point*>(obj.get())->setPos(pos);
}

void FreeCalculator::setValue(double v) {
    assert(obj->is(Object::Type::Real));
    static_cast<Real*>(obj.get())->value = v;
}
