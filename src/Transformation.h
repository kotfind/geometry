#pragma once

#include <QPointF>

class Transformation {
    public:
        Transformation();

        QPointF getCenter() const;
        void setCenter(const QPointF&);
        void scroll(const QPointF& delta);
        void move(const QPointF& delta);

        double getScale() const;
        void setScale(double);
        void zoom(double, const QPointF& zoomCenter);
        
    private:
        QPointF center{0, 0};
        double scale = 1;

        static constexpr double scrollSpeed = 0.01;
        static constexpr double zoomSpeed = 0.01;
};
