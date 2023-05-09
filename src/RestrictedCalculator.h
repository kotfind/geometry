#pragma once

#include "Calculator.h"

#include <QPointF>

class GeometryGenerator;

class RestrictedCalculator : public Calculator {
    public:
        RestrictedCalculator(
            GeometryGenerator* restrictor,
            const QPointF& mousePos
        );

        Object* calc() const override;

        Type getType() const override;

        void setPos(const QPointF& pos) override;

        GeometryGenerator* getRestrictor() const;

    private:
        GeometryGenerator* restrictor;
        QPointF mousePos;
};
