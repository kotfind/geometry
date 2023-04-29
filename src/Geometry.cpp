#include "Geometry.h"

#include "Generator.h"
#include "GeometryItem.h"
#include "getOrThrow.h"
#include "IOError.h"
#include "GeometryGenerator.h"
#include "DependantCalculator.h"

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
#include <algorithm>

Geometry::Geometry() {
}

Geometry::~Geometry() {
    clear();
}

/*
static void recalcGen(QHash<Generator*, int>& recalced, Generator* u) {
    if (u->isDependant()) {
        for (auto& v : u->getArgs()) {
            if (!recalced[v]) {
                recalcGen(recalced, v);
            }
        }
    }
    u->recalcSelf();
    recalced[u] = true;
}
*/

void Geometry::recalcAll() {
    // FIXME
    for (auto* gen : geomGens) {
        gen->recalc();
    }
    /*
    QHash<Generator*, int> recalced;
    for (auto* u : gens) {
        if (!recalced[u]) {
            recalcGen(recalced, u);
        }
    }
    */
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
    for (int i = 0; i < geomGens.size(); ++i) {
        ids[geomGens[i]] = i;
    }

    QJsonArray gensJson;
    for (auto* gen : geomGens) {
        gensJson << gen->toJson(ids);
    }
    json["geomGens"] = gensJson;

    return json;
}

void Geometry::save(const QString& fileName) const {
    QFile file(fileName);

    if (!file.open(QIODevice::WriteOnly))
        throw IOError("Couldn't open file");

    file.write(QJsonDocument(toJson()).toJson());
}

// Should be called from getGeneratorLoadOrder only.
static void getGeneratorLoadOrderDFS(const QJsonArray& jsonGens, QList<int>& ans, QList<int>& used, int u) {
    used[u] = 1;

    const auto& json = jsonGens[u];
    auto isFree = getOrThrow(json["isFree"]).toBool();

    if (!isFree) {
        const auto& jsonArgs = getOrThrow(json["args"]).toArray();
        for (const auto& jsonArg : jsonArgs) {
            int v = jsonArg.toInt();
            if (!used[v]) {
                getGeneratorLoadOrderDFS(jsonGens, ans, used, v);
            }
        }
    }

    ans << u;
}

// Topsort algorithm.
static QList<int> getGeneratorLoadOrder(const QJsonArray& jsonGens) {
    int n = jsonGens.size();

    QList<int> used(n, 0);

    QList<int> ans;
    ans.reserve(n);

    for (int i = 0; i < n; ++i) {
        if (!used[i]) {
            getGeneratorLoadOrderDFS(jsonGens, ans, used, i);
        }
    }

    return ans;
}

void Geometry::fromJson(const QJsonObject& json) {
    const auto& shiftJson = getOrThrow(json["shift"]).toObject();
    shift.setX(getOrThrow(shiftJson["x"]).toDouble());
    shift.setY(getOrThrow(shiftJson["y"]).toDouble());

    const auto& jsonGens = getOrThrow(json["geomGens"]).toArray();

    QList<Generator*> gens(jsonGens.size(), nullptr);
    const auto order = getGeneratorLoadOrder(jsonGens);
    for (int i : order) {
        gens[i] = Generator::fromJson(jsonGens[i].toObject(), gens);
        gens[i]->geom = this;
    }

    geomGens.clear();
    geomGens.reserve(gens.size());
    // TODO: differ between geometry and real gens
    for (auto* gen : gens) {
        geomGens << static_cast<GeometryGenerator*>(gen);
    }

    recalcAll();
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
    while (!geomGens.isEmpty()) {
        removeGenerator(geomGens.first());
    }
}

void Geometry::populateScene(QGraphicsScene* scene) {
    for (auto* gen : geomGens) {
        scene->addItem(
            static_cast<GeometryGenerator*>(gen)->getGeometryItem()
        );
    }
}

void Geometry::setChanged(bool v) {
    changed = v;
}

void Geometry::removeGenerator(Generator* gen) {
    int i = geomGens.indexOf(gen);
    if (i == -1) return;
    geomGens.remove(i);

    for (auto* dep : gen->dependant) {
        removeGenerator(dep);
    }

    if (gen->isDependant()) {
        const auto& args = static_cast<DependantCalculator*>(
            gen->calc.get()
        )->getArgs();

        for (auto* arg : args) {
            int i = arg->dependant.indexOf(gen);
            assert(i != -1);
            arg->dependant.remove(i);
        }
    }

    delete gen;
}
