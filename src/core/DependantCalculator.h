#pragma once

#include <QList>

#include "Calculator.h"

class Function;
class Generator;

class DependantCalculator : public Calculator {
    public:
        DependantCalculator(
            const AbstractGeometry* geom,
            const Function* func,
            const QList<Generator*>& args,
            int funcResNum = 0
        );

        Object* calc() const override;

        Type getType() const override;

        QList<Generator*> getArgs() const override;

        QJsonObject toJson(const QHash<Generator*, int>& ids, bool isReal) const override;

    private:
        // Is called from Calculator::fromJson(...)
        static DependantCalculator* fromJson(
            const AbstractGeometry* geom,
            const QJsonObject& json,
            const QList<Generator*>& gens
        );

        const Function* func;
        const QList<Generator*> args;
        const int funcResNum;

    friend Calculator;
};
