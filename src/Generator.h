#pragma once

#include "NotImplementedError.h" // XXX

#include <QList>
#include <QHash>
#include <QJsonObject> // XXX
#include <QJsonArray> // XXX

class Object;
class Geometry;
class QJsonObject;
class QJsonArray;

class Generator {
    public:
        Generator(Geometry* geom = nullptr);
        virtual ~Generator() {}

        virtual void remove();

        const Object* getObject() const { return object(); }

        virtual bool isFree() const = 0;
        virtual bool hasGeometry() const = 0;

        void recalc();

        void addDependant(Generator*);
        void removeDependant(Generator*);

        virtual void beginResetObject() {}
        virtual void endResetObject() {}

        QJsonObject toJson(const QHash<Generator*, int>& ids) const { throw NotImplementedError(); }
        static void load(Geometry* geom, const QJsonArray& jsonGens, QList<Generator*>& gens, int i) { throw NotImplementedError(); }

    protected:
        virtual Object*& object() = 0;
        virtual const Object* object() const = 0;

        virtual void recalcSelf() = 0;

    private:
        void recalcDependant() const;

        Geometry* geom;

        QList<Generator*> dependant;
};
