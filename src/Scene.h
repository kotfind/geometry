#pragma once

#include "EditMode.h"
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
        Scene(QObject* parent = nullptr);

        ~Scene();

        Geometry* getGeometry() { return geom; }

    private:
        GeometryGenerator* getFreeGeneratorAt(const QPointF& pos) const;
        GeometryGenerator* getGeneratorAt(const QPointF& pos, Type type = Type::Any) const;

        void updateCursor(QGraphicsSceneMouseEvent* e);

        EditMode mode = EditMode::MOVE;
        Function* func = nullptr;
        QList<Generator*> selectedFuncArgs;

        GeometryGenerator* currentFreeGenerator = nullptr;

        Geometry* geom;

    signals:
        void cursorChanged(const QCursor& cursor);
        void selectedCountChanged(int n);

    protected:
        void mousePressEvent(QGraphicsSceneMouseEvent*) override;
        void mouseMoveEvent(QGraphicsSceneMouseEvent*) override;
        void mouseReleaseEvent(QGraphicsSceneMouseEvent*) override;

    public slots:
        void setMode(EditMode);
        void setFunction(Function*);
};
