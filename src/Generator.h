#pragma once

#include <QPointF>
#include <memory>
#include <QList>

class Object;
class Function;
class Calculator;
class Geometry;

class Generator {
    public:
        const Object* getObject() const;

        bool isFree() const;
        bool isDependant() const;

        void recalc();

        void remove(); // TODO: Move to Geometry class ?

    protected:
        virtual void beginResetObject() {}
        virtual void endResetObject() {}

        std::unique_ptr<Object> obj;

        Geometry* geom;

    private:
        // Constructs free Generator.
        // Is called from Geometry::make_gen.
        Generator(Object*);

        // Constructs dependant Generator.
        // Is called from Geometry::make_gen.
        Generator(Function* func, const QList<Generator*>& args, int funcResNum = 0);

        virtual bool checkObjectType() const = 0;

        std::unique_ptr<Calculator> calc;

        QList<Generator*> dependant;

    friend Geometry;
};
