#pragma once

#include <QGraphicsView>

class QResizeEvent;

class View : public QGraphicsView {
    public:
        View(QWidget* parent = nullptr);

    protected:
        void resizeEvent(QResizeEvent*) override;
};
