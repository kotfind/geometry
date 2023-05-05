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

void Transformation::zoom(double v, const QPointF& zoomCenter) {
    double s1 = scale;
    double s2 = s1 * std::pow(2., v * zoomSpeed);
    center += zoomCenter * (1 / s2 - 1 / s1);
    scale = s2;
}
