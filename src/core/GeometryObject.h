#pragma once

#include "Object.h"

class QRectF;
class QPainter;
class QPainterPath;
class AbstractTransformation;
class AbstractPoint;
class QColor;

class GeometryObject : public Object {
    public:
        virtual ~GeometryObject() {}

        bool isGeometry() const override { return true; }

        virtual bool isPoint() const { return false; }

        virtual void paint(QPainter* painter, double scale, const QColor& color) const = 0;
        virtual QRectF boundingRect(double scale) const = 0;
        virtual QPainterPath shape(double scale) const = 0;

        virtual void transform(const AbstractTransformation*) = 0;

        virtual AbstractPoint* calcNearestPoint(const AbstractPoint* pos) const = 0;

        virtual double pointToPosValue(const AbstractPoint* pos) const = 0;
        virtual AbstractPoint* posValueToPoint(double val) const = 0;
        
    protected:
        GeometryObject() {}
};
