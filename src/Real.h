#pragma once

#include "Object.h"

class QJsonObject;

class Real : public Object {
    public:
        Type getType() const { return Type::Real; }

        Real();
        Real(double value);

        Object* clone() const override;

        QJsonObject toJson() const;
        static Real* fromJson(const QJsonObject& json);

        double value;
};
