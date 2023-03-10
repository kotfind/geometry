#pragma once

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>

#include "EditMode.h"

class Function;

class Scene : public QGraphicsScene {
    public:
        Scene(QObject* parent = nullptr);

    private:
        EditMode mode = EditMode::MOVE;
        Function* function = nullptr;

    protected:
        void mousePressEvent(QGraphicsSceneMouseEvent*) override;

    public slots:
        void setMode(EditMode);
        void setFunction(Function*);
};
