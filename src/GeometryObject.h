#pragma once

#include "Object.h"

class QRectF;
class QPainter;

class GeometryObject : public Object {
    public:
        virtual void paint(QPainter* painter, bool selected) const = 0;
        virtual QRectF boundingRect() const = 0;
        
    protected:
        GeometryObject() : Object() {}
};
