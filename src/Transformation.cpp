#include "Transformation.h"

#include "getOrThrow.h"

#include <math.h>

void Transformation::scroll(const QPointF& delta) {
    center += delta * scrollSpeed / scale;
}

void Transformation::move(const QPointF& delta) {
    center += delta / scale;
}

void Transformation::zoom(double v, const QPointF& zoomCenter) {
    double s1 = scale;
    double s2 = s1 * std::pow(2., v * zoomSpeed);
    center += zoomCenter * (1 / s2 - 1 / s1);
    scale = s2;
}

void Transformation::clear() {
    scale = 1;
    center = {0, 0};
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

void Transformation::fromJson(const QJsonObject& json) {
    auto centerJson = getOrThrow(json["center"]).toObject();
    center = {
        getOrThrow(centerJson["x"]).toDouble(),
        getOrThrow(centerJson["y"]).toDouble()
    };

    scale = getOrThrow(json["scale"]).toDouble();
}

QPointF Transformation::transform(const QPointF& p) const {
    return (p + center) * scale;
}

QPointF Transformation::untransform(const QPointF& p) const {
    return p / scale - center;
}
