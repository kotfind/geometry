#pragma once

#include "Calculator.h"
#include "Object.h"

#include <memory>

class FreeCalculator : public Calculator {
    public:
        FreeCalculator(std::unique_ptr<Object>);

        Object* calc() const override;
        bool isFree() const override;

    private:
        std::unique_ptr<Object> obj;
};
