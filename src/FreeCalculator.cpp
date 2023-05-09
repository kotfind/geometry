#include "FreeCalculator.h"

FreeCalculator::FreeCalculator(std::unique_ptr<Point> pt)
  : pt(std::move(pt))
{}

Object* FreeCalculator::calc() const {
    return pt->clone();
}

Calculator::Type FreeCalculator::getType() const {
    return Type::FREE;
}

void FreeCalculator::setPos(const QPointF& pos) {
    pt->setPos(pos);
}
