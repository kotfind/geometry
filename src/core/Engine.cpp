#include "Engine.h"

#include "Object.h"
#include "Generator.h"
#include "GeometryItem.h"
#include "GeometryGenerator.h"
#include "DependantCalculator.h"
#include "RealGenerator.h"
#include "Real.h"
#include "EditMode.h"
#include "Function.h"
#include "VariableDialog.h"
#include "AbstractTransformation.h"

#include "util/getOrThrow.h"
#include "util/IOError.h"

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
#include <QSet>
#include <functional>
#include <QMessageBox>

Engine::Engine(const QList<AbstractGeometry*>& geoms, QObject* parent)
  : QObject(parent),
    geoms(geoms),
    geom(geoms[0])
{}

Engine::~Engine() {
    clear();
}

// Topsort algorithm.
QList<Generator*> Engine::getGeneratorRecalcOrder() {
    QSet<Generator*> used;

    QList<Generator*> ans;
    ans.reserve(gens.size());

    // helper function
    std::function<void(Generator*)> dfs = [this, &dfs, &ans, &used](Generator* u) {
        used << u;

        for (auto* v : u->getArgs()) {
            if (!used.contains(v)) {
                dfs(v);
            }
        }

        ans << u;
    };

    for (auto* gen : gens) {
        if (!used.contains(gen)) {
            dfs(gen);
        }
    }

    return ans;
}

void Engine::recalcAll() {
    for (auto* gen : getGeneratorRecalcOrder()) {
        gen->recalcSelf();
    }
}

void Engine::recalcAllItems() {
    for (auto* gen : getGeomeryGenerators()) {
        gen->getGeometryItem()->update();
    }
}

void Engine::scroll(const QPointF& delta) {
    setChanged();

    getActiveGeometry()->getTransformation()->scroll(delta);

    recalcAllItems();
}

void Engine::move(const AbstractPoint* from, const AbstractPoint* to) {
    setChanged();

    getActiveGeometry()->getTransformation()->move(from, to);

    recalcAllItems();
}

void Engine::zoom(double v, const QPointF& zoomCenter) {
    setChanged();

    getActiveGeometry()->getTransformation()->zoom(v, zoomCenter);

    recalcAllItems();
}

QJsonObject Engine::toJson() const {
    QJsonObject json;

    json["transformation"] = getActiveGeometry()->getTransformation()->toJson();
    json["geometryName"] = getActiveGeometry()->getName();

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

void Engine::save(const QString& fileName) const {
    QFile file(fileName);

    if (!file.open(QIODevice::WriteOnly))
        throw IOError("Couldn't open file");

    file.write(QJsonDocument(toJson()).toJson());
}

// Topsort algorithm.
static QList<int> getGeneratorLoadOrder(const QJsonArray& jsonGens) {
    int n = jsonGens.size();

    QList<int> used(n, 0);

    QList<int> ans;
    ans.reserve(n);

    std::function<void(int)> dfs = [&dfs, &jsonGens, &ans, &used](int u) {
        used[u] = 1;

        const auto& json = jsonGens[u];
        const auto& jsonCalc = json["calc"];

        auto type = getOrThrow(jsonCalc["type"]).toString();
        bool isFree         = type == "FREE";
        bool isDependant    = type == "DEPENDANT";
        bool isRestricted   = type == "RESTRICTED";
        assert(isFree || isDependant || isRestricted);

        QJsonArray jsonArgs;

        if (isDependant) {
            jsonArgs = getOrThrow(jsonCalc["args"]).toArray();
        } else if (isRestricted) {
            jsonArgs = { getOrThrow(jsonCalc["restrictor"]) };
        } else /* isFree */ {
            jsonArgs = {};
        }

        for (const auto& jsonArg : jsonArgs) {
            int v = jsonArg.toInt();
            if (!used[v]) {
                dfs(v);
            }
        }

        ans << u;
    };

    for (int i = 0; i < n; ++i) {
        if (!used[i]) {
            dfs(i);
        }
    }

    return ans;
}

void Engine::fromJson(const QJsonObject& json) {
    setActiveGeometry(getOrThrow(json["geometryName"]).toString());

    getActiveGeometry()->getTransformation()->fromJson(
        getOrThrow(json["transformation"]).toObject()
    );

    const auto& jsonGens = getOrThrow(json["gens"]).toArray();
    const auto order = getGeneratorLoadOrder(jsonGens);

    gens.resize(jsonGens.size(), nullptr);
    for (int i : order) {
        gens[i] = Generator::fromJson(
            getActiveGeometry(),
            jsonGens[i].toObject(),
            gens
        );
        gens[i]->engine = this;
    }

    recalcAll();
}

void Engine::load(const QString& fileName) {
    clear();

    QFile file(fileName);

    if (!file.open(QIODevice::ReadOnly))
        throw IOError("Couldn't open file");

    fromJson(QJsonDocument::fromJson(file.readAll()).object());

    emit resetCompleted();
}

void Engine::clear() {
    getActiveGeometry()->getTransformation()->clear();

    while (!gens.isEmpty()) {
        removeGenerator(gens.first());
    }

    setChanged(false);

    emit resetCompleted();
}

void Engine::populateScene(QGraphicsScene* scene) {
    for (auto* gen : getGeomeryGenerators()) {
        scene->addItem(
            gen->getGeometryItem()
        );
    }
}

void Engine::setChanged(bool v) {
    changed = v;
}

void Engine::removeGenerator(Generator* gen) {
    setChanged();

    emit generatorRemoved(gen);

    int i = gens.indexOf(gen);
    if (i == -1) return;
    gens.remove(i);

    while (!gen->dependant.empty()) {
        removeGenerator(gen->dependant.first());
    }

    for (auto* arg : gen->getArgs()) {
        int i = arg->dependant.indexOf(gen);
        assert(i != -1);
        arg->dependant.remove(i);
    }

    delete gen;
}

QList<RealGenerator*> Engine::getRealGenerators() const {
    QList<RealGenerator*> ans;

    for (auto* gen : gens) {
        if (gen->isReal()) {
            ans << static_cast<RealGenerator*>(gen);
        }
    }

    return ans;
}

QList<GeometryGenerator*> Engine::getGeomeryGenerators() const {
    QList<GeometryGenerator*> ans;

    for (auto* gen : gens) {
        if (gen->isGeometry()) {
            ans << static_cast<GeometryGenerator*>(gen);
        }
    }

    return ans;
}

const EditMode* Engine::getEditMode() const {
    return editMode;
}

void Engine::setEditMode(const EditMode* mode) {
    editMode = mode;
}

const Function* Engine::getActiveFunction() const {
    assert(editMode->getType() == EditMode::Type::FUNCTION);
    return activeFunction;
}

void Engine::setActiveFunction(const Function* func, QGraphicsScene* scene) {
    assert(editMode->getType() == EditMode::Type::FUNCTION);
    activeFunction = func;
    clearFuncArgs(scene);
}

int Engine::getNextFuncArgType() const {
    return getActiveFunction()->getArgInfo(selectedFuncArgs.size()).getType();
}

void Engine::selectFuncArg(Generator* gen, QGraphicsScene* scene) {
    selectedFuncArgs << gen;
    emit selectedCountChanged(selectedFuncArgs.size());

    checkSelectedFuncArgs(scene);
}

void Engine::clearFuncArgs(QGraphicsScene* scene) {
    selectedFuncArgs.clear();
    emit selectedCountChanged(selectedFuncArgs.size());

    checkSelectedFuncArgs(scene);
}

void Engine::checkSelectedFuncArgs(QGraphicsScene* scene) {
    if (selectedFuncArgs.size() == getActiveFunction()->countArgs()) {
        createGeneratorFromSelectedFuncArgs(scene);
    }

    if (getNextFuncArgType() == Real::Type) {
        processRealFuncArg(scene);
    }
}

void Engine::createGeneratorFromSelectedFuncArgs(QGraphicsScene* scene) {
    auto* func = getActiveFunction();
    for (int funcResNum = 0; funcResNum < func->getMaxReturnSize(); ++funcResNum) {
        if (func->returnsReal()) {
            auto* gen = makeRealGenerator(
                tr("new"),
                getActiveGeometry(),
                func,
                selectedFuncArgs,
                funcResNum
            );
        } else {
            auto* gen = makeGeometryGenerator(
                getActiveGeometry(),
                func,
                selectedFuncArgs,
                funcResNum
            );
            auto* item = gen->getGeometryItem();
            scene->addItem(item);
        }
    }

    clearFuncArgs(scene);
}

void Engine::processRealFuncArg(QGraphicsScene* scene) {
    if (getRealGenerators().empty()) {
        QMessageBox::critical(
            nullptr,
            tr("Error: No Variables"),
            tr("This function argument is a variable. "
                "Please create a variable to use it.")
        );
        clearFuncArgs(scene);
        return;
    }


    auto* var = VariableDialog::getVariable(this);
    if (!var) {
        clearFuncArgs(scene);
        return;
    }

    selectFuncArg(var, scene);
}

const AbstractGeometry* Engine::getActiveGeometry() const {
    return geom;
}

void Engine::setActiveGeometry(const QString& name) {
    for (auto* g : geoms) {
        if (g->getName() == name) {
            geom = g;
            return;
        }
    }

    throw std::runtime_error("Geometry " + name.toStdString() + " was not loaded.");
}

void Engine::setActiveGeometry(const AbstractGeometry* g) {
    auto i = geoms.indexOf(g);
    assert(i != -1);
    geom = geoms[i];
}

QList<const AbstractGeometry*> Engine::getAllGeometries() const {
    QList<const AbstractGeometry*> ans;
    for (auto* g : geoms) {
        ans << g;
    }
    return ans;
}

AbstractPoint* Engine::makeUntransformedPoint(const QPointF& pos) {
    auto pt = getActiveGeometry()->makePoint(pos);
    if (!pt) return nullptr;

    pt->untransform(
        getActiveGeometry()->getTransformation()
    );

    return pt;
}
