#pragma once

#include "Calculator.h"

#include <QPointF>

class GeometryGenerator;

class RestrictedCalculator : public Calculator {
    public:
        RestrictedCalculator(
            GeometryGenerator* restrictorGen,
            const QPointF& mousePos
        );

        Object* calc() const override;

        Type getType() const override;

        void setPos(const QPointF& pos);

    private:
        GeometryGenerator* restrictorGen;
        QPointF mousePos;
};
