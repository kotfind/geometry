#pragma once

#include "GeometryGenerator.h"
#include "EditMode.h"
#include "Transformation.h"

#include <QRectF>
#include <QList>
#include <QPointF>
#include <memory>
#include <QObject>

class QJsonObject;
class QString;
class QGraphicsScene;
class RealGenerator;
enum class Type : unsigned int;

class Geometry : public QObject {
    Q_OBJECT

    public:
        Geometry(QObject* parent = nullptr);
        ~Geometry();

        template<typename GenT, typename... Args>
        std::enable_if_t<std::is_base_of_v<Generator, GenT>, GenT*>
        makeGenerator(Args&&... args) {
            setChanged();

            auto* gen = new GenT(std::forward<Args>(args)...);

            gens << gen;
            gen->geom = this;
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
        void move(const QPointF& delta);
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

        EditMode getEditMode() const;
        void setEditMode(EditMode);

        Function* getActiveFunction() const;
        void setActiveFunction(Function*, QGraphicsScene*);

        Type getNextFuncArgType() const;
        void selectFuncArg(Generator*, QGraphicsScene*);
        void clearFuncArgs(QGraphicsScene*);

        QList<RealGenerator*> getRealGenerators() const;
        QList<GeometryGenerator*> getGeomeryGenerators() const;

        const Transformation& getTransformation() const;

    private:
        QList<Generator*> getGeneratorRecalcOrder();

        void recalcAll();
        void recalcAllItems();

        void checkSelectedFuncArgs(QGraphicsScene*);
        void createGeneratorFromSelectedFuncArgs(QGraphicsScene*);
        void processRealFuncArg(QGraphicsScene*);

        QRectF sceneRect = QRect(-0.5, -0.5, 1, 1);

        QList<Generator*> gens;

        bool changed = false;

        EditMode editMode = EditMode::MOVE;

        Function* activeFunction = nullptr;
        QList<Generator*> selectedFuncArgs;

        Transformation transformation;

    signals:
        void generatorChanged(Generator*);
        void generatorMade(Generator*);
        void generatorRemoved(Generator*);
        void resetCompleted();

        void selectedCountChanged(int n);
};
