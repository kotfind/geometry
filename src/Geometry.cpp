#include "Geometry.h"

#include "Generator.h"
#include "DependantGenerator.h"

#include <QHash>

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

static void recalcGen(QHash<Generator*, int/* bool */>& recalced, Generator* u) {
    if (u->isDependant()) {
        auto* U = static_cast<DependantGenerator*>(u);
        for (auto& v : U->getArgs()) {
            if (!recalced[v]) {
                recalcGen(recalced, v);
            }
        }
    }
    u->recalcSelf();
    recalced[u] = true;
}

void Geometry::recalcAll() {
    QHash<Generator*, int> recalced;
    for (auto* u : generators) {
        if (!recalced[u]) {
            recalcGen(recalced, u);
        }
    }
}

void Geometry::move(const QPointF& delta) {
    shift += delta;
    recalcAll();
}

QPointF Geometry::transform(const QPointF& pt) {
    return pt + shift;
}
