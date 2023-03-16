#include "Generator.h"

#include "Object.h"
#include "GeometryItem.h"
#include "Geometry.h"

#include <QJsonObject>

Generator::Generator(Geometry* geom) : geom(geom) {
    geom->addGenerator(this);
    item = new GeometryItem(this);
}

Generator::~Generator() {
    delete object;
}

void Generator::recalc() {
    item->beginResetObject();
    recalcSelf();
    recalcDependant();
    item->endResetObject();
}

void Generator::recalcDependant() const {
    for (auto* dep : dependant) {
        dep->recalc();
    }
}

int Generator::getObjectType() const {
    return object ? object->getType() : -1;
}

void Generator::addDependant(Generator* g) {
    dependant << g;
}

void Generator::removeDependant(Generator* g) {
    auto i = dependant.indexOf(g);
    assert(i != -1);

    std::swap(dependant[i], dependant.back());
    dependant.pop_back();
}

void Generator::remove() {
    for (auto* dep : dependant) {
        dep->remove();
    }
    geom->removeGenerator(this);
    item->remove();
    delete this;
}

QJsonObject Generator::toJson() const {
    QJsonObject json;
    json["isFree"] = isFree();
    return json;
}
