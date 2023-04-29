#pragma once

#include "GeometryGenerator.h"

#include <QRectF>
#include <QList>
#include <QPointF>

class QJsonObject;
class QString;
class QGraphicsScene;

class Geometry {
    public:
        Geometry();
        ~Geometry();

        template<typename... Args>
        GeometryGenerator* makeGeometryGenerator(Args&&... args) {
            auto* gen = new GeometryGenerator(std::forward<Args>(args)...);
            gen->recalc();
            gen->geom = this;
            geomGens << gen;
            return gen;
        }

        const QRectF& getSceneRect() const { return sceneRect; }

        void move(const QPointF& delta);

        QPointF transform(const QPointF& pt);

        QJsonObject toJson() const;

        void save(const QString& fileName) const;

        void fromJson(const QJsonObject&);

        void load(const QString& file);

        void clear();

        void populateScene(QGraphicsScene*);

        void setChanged(bool v = true);
        bool isChanged() const { return changed; }

        void removeGenerator(Generator*);

    private:
        QList<Generator*> getGeneratorRecalcOrder();

        void recalcAll();

        QRectF sceneRect = QRect(-0.5, -0.5, 1, 1);

        QList<GeometryGenerator*> geomGens;

        QPointF shift = QPointF(0, 0);

        bool changed = false;
};
