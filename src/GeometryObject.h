#pragma once

#include "Object.h"

class QRectF;
class QPainter;
class QPainterPath;

class GeometryObject : public Object {
    public:
        virtual void paint(QPainter* painter) const = 0;
        virtual QRectF boundingRect() const = 0;
        virtual QPainterPath shape() const = 0;
        
    protected:
        GeometryObject() : Object() {}
};
