#pragma once

#include "Generator.h"

class Point;
class QPointF;
class QJsonObject;

class FreeGenerator : public Generator {
    public:
        FreeGenerator(Geometry* geom, Point* obj);
        ~FreeGenerator();

        bool isFree() const override { return true; }

        void recalcSelf() override;

        void move(const QPointF& delta);

        QJsonObject toJson() const override;

    private:
        const Point* point() const;
        Point* point();

        // untrasformed object
        Point* origPoint = nullptr;
};
