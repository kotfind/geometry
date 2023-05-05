#include "Transformation.h"

#include "getOrThrow.h"

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

QJsonObject Transformation::toJson() const {
    QJsonObject json;

    QJsonObject centerJson;
    centerJson["x"] = center.x();
    centerJson["y"] = center.y();
    json["center"] = centerJson;

    json["scale"] = scale;

    return json;
}

Transformation Transformation::fromJson(const QJsonObject& json) {
    Transformation t;

    auto centerJson = getOrThrow(json["center"]).toObject();
    t.center = {
        getOrThrow(centerJson["x"]).toDouble(),
        getOrThrow(centerJson["y"]).toDouble()
    };

    t.scale = getOrThrow(json["scale"]).toDouble();

    return t;
}
