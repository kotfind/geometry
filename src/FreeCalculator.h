#pragma once

#include "Calculator.h"
#include "Object.h"
#include "Point.h"

#include <memory>
#include <QPointF>

class FreeCalculator : public Calculator {
    public:
        FreeCalculator(std::unique_ptr<Point> pt);

        Object* calc() const override;

        Type getType() const override;

        void setPos(const QPointF& pos);

    private:
        std::unique_ptr<Point> pt;
};
