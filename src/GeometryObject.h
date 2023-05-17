#pragma once

#include "Object.h"

#include <QPointF>
#include <QColor>

class QRectF;
class QPainter;
class QPainterPath;
class Transformation;

class GeometryObject : public Object {
    public:
        virtual ~GeometryObject() {}

        bool isGeometry() const override { return true; }

        virtual bool isPoint() const { return false; }

        virtual void paint(QPainter* painter, const QColor& color) const = 0;
        virtual QRectF boundingRect() const = 0;
        virtual QPainterPath shape() const = 0;

        virtual GeometryObject* transformed(const Transformation&) const = 0;

        virtual QPointF calcNearestPoint(const QPointF& pos) const = 0;

        virtual double pointToPosValue(const QPointF& pos) const = 0;
        virtual QPointF posValueToPoint(double val) const = 0;
        
    protected:
        GeometryObject() {}
};
