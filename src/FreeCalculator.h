#pragma once

#include "Calculator.h"
#include "Object.h"
#include "Point.h"

#include <memory>
#include <QPointF>

class FreeCalculator : public Calculator {
    public:
        FreeCalculator(std::unique_ptr<Object> pt);

        Object* calc() const override;

        Type getType() const override;

        void setPos(const QPointF& pos) override;

        void setValue(double);

        Object* getObject() const; // TODO: delete me

    private:
        std::unique_ptr<Object> obj;
};
