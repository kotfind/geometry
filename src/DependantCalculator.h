#pragma once

#include <QList>

#include "Calculator.h"

class Function;
class Generator;

class DependantCalculator : public Calculator {
    public:
        DependantCalculator(
            const Function* func,
            const QList<Generator*>& args,
            int funcResNum = 0
        );

        Object* calc(const Object*) const override;
        bool isFree() const override;

        const QList<Generator*> getArgs() const;
        const Function* getFunc() const;
        int getFuncResNum() const;

    private:
        const Function* func;
        const QList<Generator*> args;
        const int funcResNum;
};
