#pragma once

#include "Generator.h"

class Point;
class QPointF;
class QJsonObject;

class FreeGenerator : public Generator {
    public:
        FreeGenerator(Point* obj);
        ~FreeGenerator();

        bool isFree() const override { return true; }

        void recalcSelf() override;

        void move(const QPointF& delta);

        QJsonObject toJson(const QHash<Generator*, int>& ids) const override;

        static void load(Geometry* geom, const QJsonArray& jsonGens, QList<Generator*>& gens, int i);

    private:
        const Point* point() const;
        Point* point();

        // untrasformed object
        Point* origPoint = nullptr;
};
