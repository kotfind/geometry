#pragma once

#include "Object.h"

class QJsonObject;

class Real : public Object {
    public:
        Real();
        Real(double value);

        Type getType() const override { return Type::Real; }

        bool isGeometry() const override { return false; }

        Object* clone() const override;

        QJsonObject toJson() const;
        static Real* fromJson(const QJsonObject& json);

        double value;
};
