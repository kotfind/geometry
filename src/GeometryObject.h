#pragma once

#include "Object.h"

#include <QPointF>

class QRectF;
class QPainter;
class QPainterPath;

class GeometryObject : public Object {
    public:
        virtual void paint(QPainter* painter) const = 0;
        virtual QRectF boundingRect() const = 0;
        virtual QPainterPath shape() const = 0;

        // Throws std::logic_error by default
        virtual void move(const QPointF& delta);
        
    protected:
        GeometryObject() : Object() {}
};
