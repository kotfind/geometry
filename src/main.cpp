#include "core/run.h"

#include "euclidian/Geometry.h"
#include "hyperbolic/Geometry.h"

int main(int argc, char** argv) {
    hyperbolic::Geometry hGeom;
    euclidian::Geometry eGeom;

    return run(argc, argv, { &hGeom, &eGeom });
}
