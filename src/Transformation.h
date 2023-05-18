#pragma once

#include <QPointF>
#include <QJsonObject>

class Transformation {
    public:
        Transformation();

        void scroll(const QPointF& delta);
        void move(const QPointF& delta);
        void zoom(double, const QPointF& zoomCenter);

        void clear();

        QPointF transform(const QPointF&) const;
        QPointF untransform(const QPointF&) const;

        QJsonObject toJson() const;
        static Transformation fromJson(const QJsonObject& json);
        
    private:
        QPointF center{0, 0};
        double scale = 1;

        static constexpr double scrollSpeed = 0.01;
        static constexpr double zoomSpeed = 0.01;
};
