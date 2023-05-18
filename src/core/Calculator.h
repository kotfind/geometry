#pragma once

#include <QPointF>
#include <QJsonObject>
#include <QHash>
#include <QList>

class Object;
class Generator;
class Geometry;

class Calculator {
    public:
        enum class Type {
            FREE,
            DEPENDANT,
            RESTRICTED,
        };

        virtual ~Calculator() {}

        virtual Object* calc() const = 0;

        virtual Type getType() const = 0;

        virtual QList<Generator*> getArgs() const = 0;

        virtual void setPos(const QPointF& pos);

        virtual QJsonObject toJson(const QHash<Generator*, int>& ids, bool isReal) const = 0;
        static Calculator* fromJson(
            const Geometry* geom,
            const QJsonObject& json,
            const QList<Generator*>& gens,
            bool isReal
        );

    protected:
        Calculator(const Geometry* geom);

        const Geometry* geom;
};
