#include "core/run.h"

#include "euclidian/Geometry.h"
#include "hyperbolic/Geometry.h"

int main(int argc, char** argv) {
    euclidian::Geometry eGeom;
    hyperbolic::Geometry hGeom;

    return run(argc, argv, { &eGeom, &hGeom });
}
