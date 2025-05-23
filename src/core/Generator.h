#pragma once

#include <QPointF>
#include <memory>
#include <QList>
#include <QHash>
#include <QJsonObject>

#include "Calculator.h"
#include "Object.h"

class Function;
class Engine;
class SectionMaster;
class GeometryGenerator;
class AbstractGeometry;

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

        Engine* getEngine() const;

        virtual QJsonObject toJson(const QHash<Generator*, int>& ids) const;
        static Generator* fromJson(
            const AbstractGeometry* geom,
            const QJsonObject& json,
            const QList<Generator*>& gens
        );

    protected:
        // Constructs Free Generator.
        // Is called from Engine::makeGenerator.
        Generator(const AbstractGeometry* geom, std::unique_ptr<Object> obj);

        // Constructs Dependant Generator.
        // Is called from Engine::makeGenerator.
        Generator(
            const AbstractGeometry* geom,
            const Function* func,
            const QList<Generator*>& args,
            int funcResNum = 0
        );

        // Constructs Restricted Generator.
        // Is called from Engine::makeGenerator.
        Generator(const AbstractGeometry* geom, GeometryGenerator* restrictor, double posValue = 0);

        // Basic Generator c'tor.
        // Is used by other c'tors and fromJson function
        Generator(std::unique_ptr<Calculator> calc);

        virtual void onChanged();

        std::unique_ptr<Object> obj;

        std::unique_ptr<Calculator> calc;

        Engine* engine = nullptr;

    private:
        void recalcSelf();

        virtual bool checkObjectType() const = 0;

        QList<Generator*> dependant;

    friend Engine;
};
