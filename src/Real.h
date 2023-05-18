#pragma once

#include "Object.h"

class QJsonObject;

class Real : public Object {
    public:
        Real();
        Real(double value);

        enum { Type = 1 << 0 };
        int getType() const override { return Type; }

        bool isGeometry() const override { return false; }

        Object* clone() const override;

        QJsonObject toJson() const;
        static Real* fromJson(const QJsonObject& json);

        double value;
};
