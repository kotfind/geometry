#pragma once

#include <QList>

class Function;
class Object;
class GeometryItem;

class Generator {
    public:
        // Creates non-free generator
        Generator(Function* func, const QList<Generator*>& args, int funcResNum = 0);

        // Creates free generator
        Generator(Object* obj);

        ~Generator();

        void recalc();

        // Returns true if func is nullptr
        // i.e. object does not depend on others
        bool isFree() const;

        GeometryItem* getGeometryItem() const { return item; }

    private:
        void initItem();

        Object* object = nullptr;
        Function* func = nullptr;
        QList<Generator*> args;
        int funcResNum = 0;
        GeometryItem* item;

    friend GeometryItem;
};
