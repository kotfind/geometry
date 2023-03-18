#include "Geometry.h"

#include "Generator.h"
#include "DependantGenerator.h"
#include "GeometryItem.h"
#include "getOrThrow.h"
#include "IOError.h"

#include <QHash>
#include <cassert>
#include <QDebug>
#include <QJsonObject>
#include <QJsonArray>
#include <QString>
#include <QFile>
#include <QIODevice>
#include <stdexcept>
#include <QJsonDocument>
#include <QGraphicsScene>

Geometry::Geometry() {
}

Geometry::~Geometry() {
    clear();
}

bool Geometry::hasGenerators() const {
    return !gens.isEmpty();
}

void Geometry::addGenerator(Generator* gen) {
    gens << gen;
    gen->setGeometry(this);
}

void Geometry::removeGenerator(Generator* gen) {
    auto i = gens.indexOf(gen);
    assert(i != -1);

    std::swap(gens[i], gens.back());
    gens.pop_back();
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
    for (auto* u : gens) {
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

QJsonObject Geometry::toJson() const {
    QJsonObject json;

    QJsonObject shiftJson;
    shiftJson["x"] = shift.x();
    shiftJson["y"] = shift.y();
    json["shift"] = shiftJson;

    QHash<Generator*, int> ids;
    for (int i = 0; i < gens.size(); ++i) {
        ids[gens[i]] = i;
    }

    QJsonArray gensJson;
    for (auto* gen : gens) {
        gensJson << gen->toJson(ids);
    }
    json["gens"] = gensJson;

    return json;
}

void Geometry::save(const QString& fileName) const {
    QFile file(fileName);

    if (!file.open(QIODevice::WriteOnly))
        throw IOError("Couldn't open file");

    file.write(QJsonDocument(toJson()).toJson());
}

void Geometry::fromJson(const QJsonObject& json) {
    const auto& shiftJson = getOrThrow(json["shift"]).toObject();
    shift.setX(getOrThrow(shiftJson["x"]).toInt());
    shift.setY(getOrThrow(shiftJson["y"]).toInt());

    const auto& jsonGens = getOrThrow(json["gens"]).toArray();
    gens.fill(nullptr, jsonGens.size());

    for (int i = 0; i < gens.size(); ++i) {
        if (!gens[i]) {
            Generator::load(this, jsonGens, gens, i);
        }
    }
}

void Geometry::load(const QString& fileName) {
    clear();

    QFile file(fileName);

    if (!file.open(QIODevice::ReadOnly))
        throw IOError("Couldn't open file");

    fromJson(QJsonDocument::fromJson(file.readAll()).object());
}

void Geometry::clear() {
    shift = QPointF(0, 0);
    while (!gens.isEmpty()) {
        auto* gen = gens.first();
        gen->remove();
    }
}

void Geometry::populateScene(QGraphicsScene* scene) {
    for (auto* gen : gens) {
        scene->addItem(gen->getGeometryItem());
    }
}
