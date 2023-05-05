#include "Transformation.h"

#include <math.h>

Transformation::Transformation()
{}

QPointF Transformation::getCenter() const {
    return center;
}

void Transformation::setCenter(const QPointF& c) {
    center = c;
}

void Transformation::scroll(const QPointF& delta) {
    center += delta * scrollSpeed / scale;
}

void Transformation::move(const QPointF& delta) {
    center += delta / scale;
}

double Transformation::getScale() const {
    return scale;
}

void Transformation::setScale(double s) {
    scale = s;
}

void Transformation::zoom(double v) {
    scale *= std::pow(2., v * zoomSpeed);
}
