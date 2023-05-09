#pragma once

#include "Object.h"

#include <QPointF>

class QRectF;
class QPainter;
class QPainterPath;
class Transformation;

class GeometryObject : public Object {
    public:
        virtual void paint(QPainter* painter) const = 0;
        virtual QRectF boundingRect() const = 0;
        virtual QPainterPath shape() const = 0;

        virtual GeometryObject* transformed(const Transformation&) const = 0;

        virtual QPointF calcNearestPoint(const QPointF& pos) const = 0;
        
    protected:
        GeometryObject() : Object() {}
};
