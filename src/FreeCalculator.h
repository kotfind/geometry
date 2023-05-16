#pragma once

#include "Calculator.h"
#include "Object.h"
#include "Point.h"

#include <memory>
#include <QPointF>
#include <QJsonObject>

class FreeCalculator : public Calculator {
    public:
        FreeCalculator(std::unique_ptr<Object> pt);

        Object* calc() const override;

        Type getType() const override;

        void setPos(const QPointF& pos) override;

        void setValue(double);

        Object* getObject() const; // TODO: delete me

        QJsonObject toJson(const QHash<Generator*, int>& ids, bool isReal) const override;

    private:
        // Is called from Calculator::fromJson(...)
        static FreeCalculator* fromJson(const QJsonObject& json, bool isReal);

        std::unique_ptr<Object> obj;

    friend Calculator;
};
