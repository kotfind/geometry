#pragma once

#include <QRectF>
#include <QList>
#include <QPointF>

class Generator;

class Geometry {
    public:
        Geometry();
        ~Geometry();

        void addGenerator(Generator*);

        const QRectF& getSceneRect() const { return sceneRect; }

        void move(const QPointF& delta);

        QPointF transform(const QPointF& pt);

    private:
        void recalcAll();

        QRectF sceneRect = QRect(-0.5, -0.5, 1, 1);

        QList<Generator*> generators;

        QPointF shift = QPointF(0, 0);
};
