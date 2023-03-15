#pragma once

#include <QList>

class Function;
class Object;
class GeometryItem;
class Geometry;

class Generator {
    public:
        // Creates non-free generator
        Generator(Geometry* geom, Function* func, const QList<Generator*>& args, int funcResNum = 0);

        // Creates free generator
        Generator(Geometry* geom, Object* obj);

        ~Generator();

        void recalc();

        // Returns true if func is nullptr
        // i.e. object does not depend on others
        bool isFree() const;

        GeometryItem* getGeometryItem() const { return item; }

        void move(double x, double y);

        int getObjectType() const;

        const QList<Generator*>& getArgs() const { return args; }

    private:
        void init();

        void recalcDependant() const;

        // untrasformed object
        // for free generators only
        Object* origObject = nullptr;

        Object* object = nullptr;
        Function* func = nullptr;
        QList<Generator*> args;
        int funcResNum = 0;
        GeometryItem* item;

        // List of generators that depends on current
        QList<Generator*> dependant;

        Geometry* geom;

    friend GeometryItem;
};
