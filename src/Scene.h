#pragma once

#include <QGraphicsScene>

#include "EditMode.h"

class Function;

class Scene : public QGraphicsScene {
    public:
        Scene(QObject* parent = nullptr);

    private:
        EditMode mode = EditMode::MOVE;
        Function* function = nullptr;

    public slots:
        void setMode(EditMode);
        void setFunction(Function*);
};
