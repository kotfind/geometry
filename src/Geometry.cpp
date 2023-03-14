#include "Geometry.h"

#include "Generator.h"

Geometry::Geometry() {
}

Geometry::~Geometry() {
    for (auto* gen : generators) {
         delete gen;
    }
}

void Geometry::addGenerator(Generator* gen) {
    generators << gen;
}
