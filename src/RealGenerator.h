#pragma once

#include "Generator.h"

#include "Real.h"

class RealGenerator : public virtual Generator {
    public:
        RealGenerator(Real* obj, Geometry* geom = nullptr);

        bool hasGeometry() const override { return false; }

    protected:
        Object*& object() override { return reinterpret_cast<Object*&>(obj); }
        const Object* object() const override { return static_cast<const Object*>(obj); }

    private:
        Real* obj;
};
