#pragma once

#include "EditMode.h"

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QList>

class Function;
class Generator;

class Scene : public QGraphicsScene {
    Q_OBJECT

    public:
        Scene(QObject* parent = nullptr);

        ~Scene();

    private:
        Generator* getFreeGeneratorAt(const QPointF& pos) const;

        void updateCursor(QGraphicsSceneMouseEvent* e);

        EditMode mode = EditMode::MOVE;
        Function* func = nullptr;
        QList<Generator*> selectedFuncArgs;

        Generator* currentFreeGenerator = nullptr;

        QList<Generator*> generators; // put all generators here so that they could be properly deleted when Scene is deleted

    signals:
        void cursorChanged(const QCursor& cursor);

    protected:
        void mousePressEvent(QGraphicsSceneMouseEvent*) override;
        void mouseMoveEvent(QGraphicsSceneMouseEvent*) override;
        void mouseReleaseEvent(QGraphicsSceneMouseEvent*) override;

    public slots:
        void setMode(EditMode);
        void setFunction(Function*);
};
