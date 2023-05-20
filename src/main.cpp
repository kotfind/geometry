#include "core/run.h"

#include "euclidian/Geometry.h"
#include "hyperbolic/Geometry.h"

int main(int argc, char** argv) {
    return run(
        argc,
        argv,
        std::make_unique<hyperbolic::Geometry>()
    );
}
