#pragma once

#include "EditMode.h"
#include "Type.h"

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QList>

class Function;
class Geometry;
class Generator;

class Scene : public QGraphicsScene {
    Q_OBJECT

    public:
        Scene(QObject* parent = nullptr);

        ~Scene();

        Geometry* getGeometry() { return geom; }

    private:
        Generator* getFreeGeneratorAt(const QPointF& pos) const;
        Generator* getGeneratorAt(const QPointF& pos, Type type = Type::Any) const;

        void updateCursor(QGraphicsSceneMouseEvent* e);

        EditMode mode = EditMode::MOVE;
        Function* func = nullptr;
        QList<Generator*> selectedFuncArgs;

        Generator* currentFreeGenerator = nullptr;

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
