#pragma once

#include <QJsonObject>
#include <QHash>
#include <QList>

class Object;
class Generator;
class AbstractGeometry;
class AbstractPoint;

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

        virtual void setPos(const AbstractPoint* pos);

        virtual QJsonObject toJson(const QHash<Generator*, int>& ids, bool isReal) const = 0;
        static Calculator* fromJson(
            const AbstractGeometry* geom,
            const QJsonObject& json,
            const QList<Generator*>& gens,
            bool isReal
        );

    protected:
        Calculator(const AbstractGeometry* geom);

        const AbstractGeometry* geom;
};
