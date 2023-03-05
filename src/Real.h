#pragma once

#include "Object.h"

class Real : public Object {
    public:
        int getType() const { return type; }
        static const int type = 1;

        Real() : Real(0) {}
        Real(double value) : Object(), value(value) {}

        double value;
};
