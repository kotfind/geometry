#pragma once

#include "Object.h"
#include "AbstractGeometry.h"

#include <QRectF>
#include <QList>
#include <QPointF>
#include <memory>
#include <QObject>

class Generator;
class QJsonObject;
class QString;
class QGraphicsScene;
class RealGenerator;
class GeometryGenerator;
class EditMode;
class Function;

class Engine : public QObject {
    Q_OBJECT

    public:
        Engine(const QList<AbstractGeometry*>& geoms, QObject* parent = nullptr);
        ~Engine();

        template<typename GenT, typename... Args>
        std::enable_if_t<std::is_base_of_v<Generator, GenT>, GenT*>
        makeGenerator(Args&&... args) {
            setChanged();

            auto* gen = new GenT(std::forward<Args>(args)...);

            gens << gen;
            gen->engine = this;
            gen->recalc();

            emit generatorMade(gen);

            return gen;
        }

        template<typename... Args>
        auto makeGeometryGenerator(Args&&... args) {
            return makeGenerator<GeometryGenerator>(std::forward<Args>(args)...);
        }

        template<typename... Args>
        auto makeRealGenerator(Args&&... args) {
            return makeGenerator<RealGenerator>(std::forward<Args>(args)...);
        }

        const QRectF& getSceneRect() const { return sceneRect; }

        void scroll(const QPointF& delta);
        void move(const AbstractPoint* from, const AbstractPoint* to);
        void zoom(double v, const QPointF& zoomCenter);

        QJsonObject toJson() const;

        void save(const QString& fileName) const;

        void fromJson(const QJsonObject&);

        void load(const QString& file);

        void clear();

        void populateScene(QGraphicsScene*);

        void setChanged(bool v = true);
        bool isChanged() const { return changed; }

        void removeGenerator(Generator*);

        const EditMode* getEditMode() const;
        void setEditMode(const EditMode*);

        const Function* getActiveFunction() const;
        void setActiveFunction(const Function*, QGraphicsScene*);

        int getNextFuncArgType() const;
        void selectFuncArg(Generator*, QGraphicsScene*);
        void clearFuncArgs(QGraphicsScene*);

        QList<RealGenerator*> getRealGenerators() const;
        QList<GeometryGenerator*> getGeomeryGenerators() const;

        const AbstractGeometry* getActiveGeometry() const;
        void setActiveGeometry(const QString& name);
        void setActiveGeometry(const AbstractGeometry* geom);

        QList<const AbstractGeometry*> getAllGeometries() const;

        AbstractPoint* makeUntransformedPoint(const QPointF& pos);

    private:
        QList<Generator*> getGeneratorRecalcOrder();

        void recalcAll();
        void recalcAllItems();

        void checkSelectedFuncArgs(QGraphicsScene*);
        void createGeneratorFromSelectedFuncArgs(QGraphicsScene*);
        void processRealFuncArg(QGraphicsScene*);

        QRectF sceneRect = QRect(-1, -1, 2, 2);

        QList<Generator*> gens;

        bool changed = false;

        const EditMode* editMode = nullptr;
        const Function* activeFunction = nullptr;
        QList<Generator*> selectedFuncArgs;

        QList<AbstractGeometry*> geoms;
        AbstractGeometry* geom;

    signals:
        void generatorChanged(Generator*);
        void generatorMade(Generator*);
        void generatorRemoved(Generator*);
        void resetCompleted();

        void selectedCountChanged(int n);
};
