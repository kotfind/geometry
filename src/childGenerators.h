#pragma once

#include "RealGenerator.h"
#include "GeometryGenerator.h"
#include "FreeGenerator.h"
#include "DependantGenerator.h"

class RealFreeGenerator : public RealGenerator, public FreeGenerator {
    public:
        RealFreeGenerator(Real* obj, Geometry* geom = nullptr);
};

class GeometryFreeGenerator : public GeometryGenerator, public FreeGenerator {
    public:
        GeometryFreeGenerator(GeometryObject* obj, Geometry* geom = nullptr);
};

class RealDependantGenerator : public RealGenerator, public DependantGenerator {
    public:
        RealDependantGenerator(
            Function* func,
            const QList<Generator*>& args,
            int funcResNum = 0,
            Geometry* geom = nullptr
        );
};

class GeometryDependantGenerator : public GeometryGenerator, public DependantGenerator {
    public:
        GeometryDependantGenerator(
            Function* func,
            const QList<Generator*>& args,
            int funcResNum = 0,
            Geometry* geom = nullptr
        );
};
