#pragma once

#include "EditMode.h"

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QList>

class Function;
class Generator;

class Scene : public QGraphicsScene {
    public:
        Scene(QObject* parent = nullptr);

    private:
        EditMode mode = EditMode::MOVE;
        Function* func = nullptr;
        QList<Generator*> selectedFuncArgs;

    protected:
        void mousePressEvent(QGraphicsSceneMouseEvent*) override;

    public slots:
        void setMode(EditMode);
        void setFunction(Function*);
};
