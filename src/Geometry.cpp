#include "Geometry.h"

#include "Object.h"
#include "Generator.h"
#include "GeometryItem.h"
#include "getOrThrow.h"
#include "IOError.h"
#include "GeometryGenerator.h"
#include "DependantCalculator.h"
#include "RealGenerator.h"
#include "Real.h"
#include "EditMode.h"
#include "Function.h"
#include "VariableDialog.h"
#include "functionList.h"

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

Geometry::Geometry(QObject* parent)
  : QObject(parent),
    sectionMaster(functionList::makeSectionMaster())
{}

Geometry::~Geometry() {
    clear();
}

// Topsort algorithm.
QList<Generator*> Geometry::getGeneratorRecalcOrder() {
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

void Geometry::recalcAll() {
    for (auto* gen : getGeneratorRecalcOrder()) {
        gen->recalcSelf();
    }
}

void Geometry::recalcAllItems() {
    for (auto* gen : getGeomeryGenerators()) {
        gen->getGeometryItem()->update();
    }
}

void Geometry::scroll(const QPointF& delta) {
    setChanged();

    transformation.scroll(delta);

    recalcAllItems();
}

void Geometry::move(const QPointF& delta) {
    setChanged();

    transformation.move(delta);

    recalcAllItems();
}

void Geometry::zoom(double v, const QPointF& zoomCenter) {
    setChanged();

    transformation.zoom(v, zoomCenter);

    recalcAllItems();
}

QJsonObject Geometry::toJson() const {
    QJsonObject json;

    json["transformation"] = transformation.toJson();

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

// Topsort algorithm.
static QList<int> getGeneratorLoadOrder(const QJsonArray& jsonGens) {
    // FIXME
    /*
    int n = jsonGens.size();

    QList<int> used(n, 0);

    QList<int> ans;
    ans.reserve(n);

    std::function<void(int)> dfs = [&dfs, &jsonGens, &ans, &used](int u) {
        used[u] = 1;

        const auto& json = jsonGens[u];
        auto isFree = getOrThrow(json["isFree"]).toBool();

        if (!isFree) {
            const auto& jsonArgs = getOrThrow(json["args"]).toArray();
            for (const auto& jsonArg : jsonArgs) {
                int v = jsonArg.toInt();
                if (!used[v]) {
                    dfs(v);
                }
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
    */
}

void Geometry::fromJson(const QJsonObject& json) {
    transformation = Transformation::fromJson(
        getOrThrow(json["transformation"]).toObject()
    );

    const auto& jsonGens = getOrThrow(json["gens"]).toArray();
    const auto order = getGeneratorLoadOrder(jsonGens);

    gens.resize(jsonGens.size(), nullptr);
    for (int i : order) {
        gens[i] = Generator::fromJson(jsonGens[i].toObject(), gens, sectionMaster.get());
        gens[i]->geom = this;
    }

    recalcAll();
}

void Geometry::load(const QString& fileName) {
    clear();

    QFile file(fileName);

    if (!file.open(QIODevice::ReadOnly))
        throw IOError("Couldn't open file");

    fromJson(QJsonDocument::fromJson(file.readAll()).object());

    emit resetCompleted();
}

void Geometry::clear() {
    transformation.clear();

    while (!gens.isEmpty()) {
        removeGenerator(gens.first());
    }

    setChanged(false);

    emit resetCompleted();
}

void Geometry::populateScene(QGraphicsScene* scene) {
    for (auto* gen : getGeomeryGenerators()) {
        scene->addItem(
            gen->getGeometryItem()
        );
    }
}

void Geometry::setChanged(bool v) {
    changed = v;
}

void Geometry::removeGenerator(Generator* gen) {
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

QList<RealGenerator*> Geometry::getRealGenerators() const {
    QList<RealGenerator*> ans;

    for (auto* gen : gens) {
        if (gen->isReal()) {
            ans << static_cast<RealGenerator*>(gen);
        }
    }

    return ans;
}

QList<GeometryGenerator*> Geometry::getGeomeryGenerators() const {
    QList<GeometryGenerator*> ans;

    for (auto* gen : gens) {
        if (gen->isGeometry()) {
            ans << static_cast<GeometryGenerator*>(gen);
        }
    }

    return ans;
}

const Transformation& Geometry::getTransformation() const {
    return transformation;
}

const EditMode* Geometry::getEditMode() const {
    return editMode;
}

void Geometry::setEditMode(const EditMode* mode) {
    editMode = mode;
}

const Function* Geometry::getActiveFunction() const {
    assert(editMode->getType() == EditMode::Type::FUNCTION);
    return activeFunction;
}

void Geometry::setActiveFunction(const Function* func, QGraphicsScene* scene) {
    assert(editMode->getType() == EditMode::Type::FUNCTION);
    activeFunction = func;
    clearFuncArgs(scene);
}

Object::Type Geometry::getNextFuncArgType() const {
    return getActiveFunction()->getArgInfo(selectedFuncArgs.size()).getType();
}

void Geometry::selectFuncArg(Generator* gen, QGraphicsScene* scene) {
    selectedFuncArgs << gen;
    emit selectedCountChanged(selectedFuncArgs.size());

    checkSelectedFuncArgs(scene);
}

void Geometry::clearFuncArgs(QGraphicsScene* scene) {
    selectedFuncArgs.clear();
    emit selectedCountChanged(selectedFuncArgs.size());

    checkSelectedFuncArgs(scene);
}

void Geometry::checkSelectedFuncArgs(QGraphicsScene* scene) {
    if (selectedFuncArgs.size() == getActiveFunction()->countArgs()) {
        createGeneratorFromSelectedFuncArgs(scene);
    }

    if (getNextFuncArgType() == Object::Type::Real) {
        processRealFuncArg(scene);
    }
}

void Geometry::createGeneratorFromSelectedFuncArgs(QGraphicsScene* scene) {
    auto* func = getActiveFunction();
    for (int funcResNum = 0; funcResNum < func->getMaxReturnSize(); ++funcResNum) {
        auto* gen = makeGeometryGenerator(
            func,
            selectedFuncArgs,
            funcResNum
        );
        auto* item = gen->getGeometryItem();
        scene->addItem(item);
    }

    clearFuncArgs(scene);
}

void Geometry::processRealFuncArg(QGraphicsScene* scene) {
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

const SectionMaster* Geometry::getSectionMaster() const {
    return sectionMaster.get();
}
