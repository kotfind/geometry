#pragma once

#include "AbstractPoint.h"

#include <QPointF>
#include <QJsonObject>

class AbstractTransformation {
    public:
        virtual ~AbstractTransformation() {}

        virtual void scroll(const QPointF& delta) = 0;
        virtual void move(const AbstractPoint* from, const AbstractPoint* to) = 0;
        virtual void zoom(double, const QPointF& zoomCenter) = 0;

        virtual void clear() = 0;

        virtual void transform(AbstractPoint*) const = 0;
        virtual void untransform(AbstractPoint*) const = 0;

        virtual QJsonObject toJson() const = 0;
        virtual void fromJson(const QJsonObject& json) = 0;
};
