#pragma once

#include "Type.h"

class Object {
    public:
        virtual ~Object() {}

        virtual Type getType() const = 0;

        bool is(Type) const;

        virtual Object* clone() const = 0;

    protected:
        Object() {}
};
