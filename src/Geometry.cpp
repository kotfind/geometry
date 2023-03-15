#include "Geometry.h"

#include "Generator.h"

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
    for (auto& v : u->getArgs()) {
        if (!recalced[v]) {
            recalcGen(recalced, v);
        }
    }
    u->recalc();
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
