#include "childGenerators.h"

RealFreeGenerator::RealFreeGenerator(Real* obj, Geometry* geom)
  : RealGenerator(obj, geom),
    FreeGenerator(geom)
{
}

GeometryFreeGenerator::GeometryFreeGenerator(GeometryObject* obj, Geometry* geom)
  : GeometryGenerator(obj, geom),
    FreeGenerator(geom)
{
}

RealDependantGenerator::RealDependantGenerator(
    Function* func,
    const QList<Generator*>& args,
    int funcResNum,
    Geometry* geom
) : RealGenerator(nullptr, geom),
    DependantGenerator(func, args, funcResNum, geom)
{
    recalc();
}

GeometryDependantGenerator::GeometryDependantGenerator(
    Function* func,
    const QList<Generator*>& args,
    int funcResNum,
    Geometry* geom
) : GeometryGenerator(nullptr, geom),
    DependantGenerator(func, args, funcResNum, geom)
{
    recalc();
}
