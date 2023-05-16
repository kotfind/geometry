#pragma once

#include "Calculator.h"

#include <QPointF>

class GeometryGenerator;

class RestrictedCalculator : public Calculator {
    public:
        RestrictedCalculator(
            GeometryGenerator* restrictor,
            double posValue
        );

        Object* calc() const override;

        Type getType() const override;

        void setPos(const QPointF& pos) override;

        GeometryGenerator* getRestrictor() const;
        double getPosValue() const;

        QJsonObject toJson(const QHash<Generator*, int>& ids, bool isReal) const override;

    private:
        // Is called from Calculator::fromJson(...)
        static RestrictedCalculator* fromJson(
            const QJsonObject& json,
            const QList<Generator*>& gens
        );

        GeometryGenerator* restrictor;
        double posValue;

    friend Calculator;
};
