#pragma once

#include "GeometryObject.h"

#include <QPair> // TODO delete me
#include <QPointF> // TODO delete me

// Line with equation a*x + b*y + c = 0
class Line : public GeometryObject {
    public:
        int getType() const { return type; }
        static const int type = 3;

        Line();
        Line(double a, double b, double c);

        void paint(QPainter* painter) const override;
        QRectF boundingRect() const override;

        double a, b, c;

        const double drawWidth = 2;

    private:
        // XXX the size of line is quite random
        QPair<QPointF, QPointF> getTwoPoints() const;
};
