#pragma once

#include "Generator.h"

#include <QList>

class Function;

class DependantGenerator : public virtual Generator {
    public:
        // Call recalc() manually in child constructor
        DependantGenerator(
            Function* func,
            const QList<Generator*>& args,
            int funcResNum = 0,
            Geometry* geom = nullptr
        );

        void remove() override;

        bool isFree() const override { return false; }

    protected:
        void recalcSelf() override;

    private:
        Function* func;
        QList<Generator*> args;
        int funcResNum = 0;
};
