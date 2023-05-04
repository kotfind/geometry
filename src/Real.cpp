#include "Real.h"

#include "getOrThrow.h"

#include <QJsonObject>

Real::Real() : Real(0) {}
Real::Real(double value) : Object(), value(value) {}

Object* Real::clone() const {
    return new Real(*this);
}

QJsonObject Real::toJson() const {
    QJsonObject json;
    json["value"] = value;
    return json;
}

Real* Real::fromJson(const QJsonObject& json) {
    return new Real(getOrThrow(json["value"]).toDouble());
}
