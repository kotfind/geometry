#pragma once

#include "Calculator.h"
#include "Object.h"
#include "Point.h"

#include <memory>
#include <QPointF>
#include <QJsonObject>

class FreeCalculator : public Calculator {
    public:
        FreeCalculator(const Geometry* geom, std::unique_ptr<Object> pt);

        Object* calc() const override;

        Type getType() const override;

        QList<Generator*> getArgs() const override;

        void setPos(const QPointF& pos) override;

        void setValue(double);

        QJsonObject toJson(const QHash<Generator*, int>& ids, bool isReal) const override;

    private:
        // Is called from Calculator::fromJson(...)
        static FreeCalculator* fromJson(
            const Geometry* geom,
            const QJsonObject& json,
            bool isReal
        );

        std::unique_ptr<Object> obj;

    friend Calculator;
};
