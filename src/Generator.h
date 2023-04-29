#pragma once

#include <QPointF>
#include <memory>
#include <QList>
#include <QHash>
#include <QJsonObject>

#include "Calculator.h"
#include "Object.h"

class Function;
class Geometry;

class Generator {
    public:
        virtual ~Generator() {}

        const Object* getObject() const;

        bool isFree() const;
        bool isDependant() const;

        void recalc();

        QJsonObject toJson(const QHash<Generator*, int>& ids) const;
        static Generator* fromJson(const QJsonObject& json, const QList<Generator*>& gens);

    protected:
        // Constructs free Generator.
        // Is called from Geometry::make_gen.
        Generator(std::unique_ptr<Object> obj);

        // Constructs dependant Generator.
        // Is called from Geometry::make_gen.
        Generator(Function* func, const QList<Generator*>& args, int funcResNum = 0);

        virtual void beginResetObject() {}
        virtual void endResetObject() {}

        std::unique_ptr<Object> obj;

        Geometry* geom;

    private:
        void recalcSelf();

        virtual bool checkObjectType() const = 0;

        std::unique_ptr<Calculator> calc;

        QList<Generator*> dependant;

    friend Geometry;
};
