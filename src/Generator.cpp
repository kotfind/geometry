#include "Generator.h"

#include "Object.h"
#include "Geometry.h"

Generator::Generator(Geometry* geom) {
    if (geom) {
        geom->addGenerator(this);
    }
}

void Generator::remove() {
    while (!dependant.isEmpty()) {
        // Next line will remove dependant[0] from dependant array,
        // so dependant will eventually became empty
        dependant[0]->remove();
    }
    geom->removeGenerator(this);
    delete this;
}

void Generator::recalc() {
    beginResetObject();
    recalcSelf();
    endResetObject();

    recalcDependant();
}

void Generator::addDependant(Generator* gen) {
    dependant << gen;
}
 
void Generator::removeDependant(Generator* gen) {
    geom->setChanged();

    auto i = dependant.indexOf(gen);
    assert(i != -1);

    std::swap(dependant[i], dependant.back());
    dependant.pop_back();
}

void Generator::recalcDependant() const {
    for (auto* dep : dependant) {
        dep->recalc();
    }
}
