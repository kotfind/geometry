#include "core/run.h"

#include "euclidian/Geometry.h"

int main(int argc, char** argv) {
    return run(
        argc,
        argv,
        std::make_unique<euclidian::Geometry>()
    );
}
