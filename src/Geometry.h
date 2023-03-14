#pragma once

#include <QRectF>
#include <QList>

class Generator;

class Geometry {
    public:
        Geometry();
        ~Geometry();

        void addGenerator(Generator*);
        const QRectF& getSceneRect() const { return sceneRect; }

    private:
        QRectF sceneRect = QRect(-0.5, -0.5, 1, 1);
        QList<Generator*> generators;
};
