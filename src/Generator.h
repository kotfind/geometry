#pragma once

#include <QList>

class Object;
class GeometryItem;
class Geometry;

class Generator {
    public:
        virtual ~Generator();

        virtual bool isFree() const = 0;
        bool isDependant() const { return !isFree(); }

        void recalc();
        virtual void recalcSelf() = 0;

        GeometryItem* getGeometryItem() const { return item; }

        int getObjectType() const;

        void addDependant(Generator*);
        void removeDependant(Generator*);

        const Object* getObject() const { return object; }

        virtual void remove();

    protected:
        Generator(Geometry* geom);

        Object* object = nullptr;
        GeometryItem* item;

        Geometry* geom;

    private:
        void recalcDependant() const;

        // List of generators that depends on current
        QList<Generator*> dependant;
};
