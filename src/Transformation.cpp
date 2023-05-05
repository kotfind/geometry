#include "Transformation.h"

QPointF Transformation::getCenter() const {
    return center;
}

void Transformation::setCenter(const QPointF& c) {
    center = c;
}

void Transformation::move(const QPointF& delta) {
    center += delta;
}

double Transformation::getScale() const {
    return scale;
}

void Transformation::setScale(double s) {
    scale = s;
}

