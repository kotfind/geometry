#include "ETransformation.h"

#include "getOrThrow.h"

#include <math.h>

void ETransformation::scroll(const QPointF& delta) {
    center += delta * scrollSpeed / scale;
}

void ETransformation::move(const QPointF& delta) {
    center += delta / scale;
}

void ETransformation::zoom(double v, const QPointF& zoomCenter) {
    double s1 = scale;
    double s2 = s1 * std::pow(2., v * zoomSpeed);
    center += zoomCenter * (1 / s2 - 1 / s1);
    scale = s2;
}

void ETransformation::clear() {
    scale = 1;
    center = {0, 0};
}

QJsonObject ETransformation::toJson() const {
    QJsonObject json;

    QJsonObject centerJson;
    centerJson["x"] = center.x();
    centerJson["y"] = center.y();
    json["center"] = centerJson;

    json["scale"] = scale;

    return json;
}

void ETransformation::fromJson(const QJsonObject& json) {
    auto centerJson = getOrThrow(json["center"]).toObject();
    center = {
        getOrThrow(centerJson["x"]).toDouble(),
        getOrThrow(centerJson["y"]).toDouble()
    };

    scale = getOrThrow(json["scale"]).toDouble();
}

QPointF ETransformation::transform(const QPointF& p) const {
    return (p + center) * scale;
}

QPointF ETransformation::untransform(const QPointF& p) const {
    return p / scale - center;
}
