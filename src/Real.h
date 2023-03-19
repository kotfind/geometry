#pragma once

#include "Object.h"

class Real : public Object {
    public:
        Type getType() const { return Type::Real; }

        Real() : Real(0) {}
        Real(double value) : Object(), value(value) {}

        double value;
};
