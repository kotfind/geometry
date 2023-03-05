#pragma once

#include "Object.h"

class GeometryObject : public Object {
    public:
        virtual void paint() const = 0;

    protected:
        GeometryObject() : Object() {}
};
