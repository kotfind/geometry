#pragma once

#include "GeometryObject.h"

#include <QPointF>
#include <QJsonObject>

class AbstractPoint : public GeometryObject {
    public:
        virtual ~AbstractPoint() {}

        bool isPoint() const override { return true; }

        void transform(const AbstractTransformation*) override;
        void untransform(const AbstractTransformation*);

        AbstractPoint* calcNearestPoint(const AbstractPoint* pos) const override;

        double pointToPosValue(const AbstractPoint* pos) const override;
        AbstractPoint* posValueToPoint(double val) const override;

        virtual QPointF getPos() const = 0;
        virtual void setPos(const QPointF&) = 0;

        virtual QJsonObject toJson() const = 0;
        virtual void fromJson(const QJsonObject& json) = 0;

    protected:
        AbstractPoint() {}
};
