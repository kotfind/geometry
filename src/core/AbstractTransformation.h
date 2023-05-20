#pragma once

#include <QPointF>
#include <QJsonObject>

class AbstractTransformation {
    public:
        virtual ~AbstractTransformation() {}

        virtual void scroll(const QPointF& delta) = 0;
        virtual void move(const QPointF& delta) = 0;
        virtual void zoom(double, const QPointF& zoomCenter) = 0;

        virtual void clear() = 0;

        virtual QPointF transform(const QPointF&) const = 0;
        virtual QPointF untransform(const QPointF&) const = 0;

        virtual QJsonObject toJson() const = 0;
        virtual void fromJson(const QJsonObject& json) = 0;
};
