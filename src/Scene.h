#pragma once

#include "Type.h"

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QList>

class Function;
class Geometry;
class Generator;
class GeometryGenerator;

class Scene : public QGraphicsScene {
    Q_OBJECT

    public:
        Scene(Geometry* geom, QObject* parent = nullptr);

    private:
        GeometryGenerator* getFreeGeneratorAt(const QPointF& pos) const;
        GeometryGenerator* getGeneratorAt(const QPointF& pos, Type type = Type::Any) const;

        void updateCursor(QGraphicsSceneMouseEvent* e);

        GeometryGenerator* currentFreeGenerator = nullptr;

        Geometry* geom;

    signals:
        void cursorChanged(const QCursor& cursor);

    protected:
        void mousePressEvent(QGraphicsSceneMouseEvent*) override;
        void mouseMoveEvent(QGraphicsSceneMouseEvent*) override;
        void mouseReleaseEvent(QGraphicsSceneMouseEvent*) override;
};
