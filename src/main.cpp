#include "core/run.h"

#include "euclidian/EGeometry.h"

int main(int argc, char** argv) {
    return run(
        argc,
        argv,
        std::make_unique<EGeometry>()
    );
}
