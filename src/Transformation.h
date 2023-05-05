#pragma once

#include <QPointF>

class Transformation {
    public:
        Transformation();

        QPointF getCenter() const;
        void setCenter(const QPointF&);
        void move(const QPointF& delta);

        double getScale() const;
        void setScale(double);
        
    private:
        QPointF center{0, 0};
        double scale = 1;
};
