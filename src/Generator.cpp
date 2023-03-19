#include "Generator.h"

#include "Object.h"
#include "GeometryItem.h"
#include "Geometry.h"
#include "FreeGenerator.h"
#include "DependantGenerator.h"
#include "getOrThrow.h"

#include <QJsonObject>
#include <QJsonArray>

Generator::Generator() {
    item = new GeometryItem(this);
}

Generator::~Generator() {
    delete object;
}

void Generator::setGeometry(Geometry* g) {
    geom = g;
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

void Generator::addDependant(Generator* g) {
    dependant << g;
}

void Generator::removeDependant(Generator* g) {
    geom->setChanged();

    auto i = dependant.indexOf(g);
    assert(i != -1);

    std::swap(dependant[i], dependant.back());
    dependant.pop_back();
}

void Generator::remove() {
    while (!dependant.empty()) {
        // Next line will remove dependant[0] from dependant array,
        // so dependant will eventually became empty
        dependant[0]->remove();
    }
    geom->removeGenerator(this);
    item->remove();
    delete this;
}

QJsonObject Generator::toJson(const QHash<Generator*, int>& ids) const {
    QJsonObject json;
    json["isFree"] = isFree();
    return json;
}

void Generator::load(Geometry* geom, const QJsonArray& jsonGens, QList<Generator*>& gens, int i) {
    auto isFree = getOrThrow(jsonGens[i]["isFree"]).toBool();
    if (isFree) {
        FreeGenerator::load(geom, jsonGens, gens, i);
    } else {
        DependantGenerator::load(geom, jsonGens, gens, i);
    }
}
