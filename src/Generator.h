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
class SectionMaster;
class GeometryGenerator;

class Generator {
    public:
        virtual ~Generator() {}

        const Object* getObject() const;

        bool isFree() const;
        bool isDependant() const;
        bool isRestricted() const;

        virtual bool isReal() const = 0;
        bool isGeometry() const;

        // Returns args for Dependant,
        // restrictor for Restricted
        // and nothing for Free
        QList<Generator*> getArgs() const;

        void recalc();

        Geometry* getGeometry() const;

        QJsonObject toJson(const QHash<Generator*, int>& ids) const;
        static Generator* fromJson(
            const QJsonObject& json,
            const QList<Generator*>& gens,
            const SectionMaster* sectionMaster
        );

    protected:
        // Constructs Free Generator.
        // Is called from Geometry::makeGenerator.
        Generator(std::unique_ptr<Object> obj);

        // Constructs Dependant Generator.
        // Is called from Geometry::makeGenerator.
        Generator(
            const Function* func,
            const QList<Generator*>& args,
            int funcResNum = 0
        );

        // Constructs Restricted Generator.
        // Is called from Geometry::makeGenerator.
        Generator(GeometryGenerator* restrictor, double posValue = 0);

        virtual void onChanged();

        std::unique_ptr<Object> obj;

        std::unique_ptr<Calculator> calc;

        Geometry* geom;

    private:
        // Basic Generator c'tor.
        // Is used by other c'tors and fromJson function
        Generator(std::unique_ptr<Calculator> calc);

        void recalcSelf();

        virtual bool checkObjectType() const = 0;

        QList<Generator*> dependant;

    friend Geometry;
};
