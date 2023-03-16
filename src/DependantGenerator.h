#pragma once

#include "Generator.h"

#include <QList>

class Function;

class DependantGenerator : public Generator {
    public:
        DependantGenerator(Geometry* geom, Function* func, const QList<Generator*>& args, int funcResNum = 0);

        bool isFree() const override { return false; }

        void recalcSelf() override;

        const QList<Generator*>& getArgs() const { return args; }

        void remove() override;

    private:
        Function* func = nullptr;
        QList<Generator*> args;
        int funcResNum = 0;
};
