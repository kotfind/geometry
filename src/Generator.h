#pragma once

#include <QList>

class Function;
class Object;

class Generator {
    public:
        Generator(Function* func, const QList<Generator*>& args, int funcResNum = 0);
        ~Generator();

        void recalc();

    private:
        Object* object = nullptr;
        Function* func;
        QList<Generator*> args;
        int funcResNum;
};
