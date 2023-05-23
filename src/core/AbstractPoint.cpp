#include "AbstractPoint.h"

#include "AbstractTransformation.h"

void AbstractPoint::transform(const AbstractTransformation* t) {
    t->transform(this);
}

void AbstractPoint::untransform(const AbstractTransformation* t) {
    t->untransform(this);
}

AbstractPoint* AbstractPoint::calcNearestPoint(const AbstractPoint* pos) const {
    throw std::logic_error("Cannot call calcNearestPoint for Point.");
}

double AbstractPoint::pointToPosValue(const AbstractPoint* pos) const {
    throw std::logic_error("Cannot call pointToPosValue for Point.");
}

AbstractPoint* AbstractPoint::posValueToPoint(double val) const {
    throw std::logic_error("Cannot call posValueToPoint for Point.");
}
