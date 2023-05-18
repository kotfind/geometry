#pragma once

#include "GeometryObject.h"

#include <QPointF>
#include <QJsonObject>

class AbstractPoint : public GeometryObject {
    public:
        virtual ~AbstractPoint() {}

        bool isPoint() const override { return true; }

        virtual QPointF getPos() const = 0;
        virtual void setPos(const QPointF&) = 0;

        virtual QJsonObject toJson() const = 0;
        virtual void fromJson(const QJsonObject& json) = 0;

    protected:
        AbstractPoint() {}
};
