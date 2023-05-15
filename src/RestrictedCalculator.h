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

    private:
        GeometryGenerator* restrictor;
        double posValue;
};
