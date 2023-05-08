#include "FreeCalculator.h"

FreeCalculator::FreeCalculator(std::unique_ptr<Point> pt)
  : pt(std::move(pt))
{}

Object* FreeCalculator::calc() const {
    return pt->clone();
}

bool FreeCalculator::isFree() const {
    return true;
}

void FreeCalculator::setPos(const QPointF& pos) {
    pt->setPos(pos);
}
