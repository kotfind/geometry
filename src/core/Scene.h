#pragma once

#include "Object.h"

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsSceneWheelEvent>
#include <QList>

class Function;
class Engine;
class Generator;
class GeometryGenerator;

class Scene : public QGraphicsScene {
    Q_OBJECT

    public:
        Scene(Engine* engine, QObject* parent = nullptr);

        // Same as clear but items are not deleted
        void detachAll();

    private:
        GeometryGenerator* getFreeOrRestrictedGeneratorAt(const QPointF& pos) const;
        GeometryGenerator* getDependantGeneratorAt(const QPointF& pos) const;
        GeometryGenerator* getGeneratorAt(const QPointF& pos, int objectType = ~0, bool allowHidden = false) const;
        GeometryGenerator* getGeneratorAtHelper(
            const QPointF& pos,
            bool allowFree,
            bool allowRestricted,
            bool allowDependant,
            int objectType,
            bool allowHidden
        ) const;

        void updateCursor(QGraphicsSceneMouseEvent* e);

        GeometryGenerator* currentFreeGenerator = nullptr;

        Engine* engine;

    signals:
        void cursorChanged(const QCursor& cursor);

    protected:
        void mousePressEvent(QGraphicsSceneMouseEvent*) override;
        void mouseMoveEvent(QGraphicsSceneMouseEvent*) override;
        void mouseReleaseEvent(QGraphicsSceneMouseEvent*) override;

        void wheelEvent(QGraphicsSceneWheelEvent*) override;
};
