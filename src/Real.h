#pragma once

#include "Object.h"

class Real : public Object {
    public:
        Type getType() const { return Type::Real; }

        Real();
        Real(double value);

        Object* clone() const override;

        double value;
};
