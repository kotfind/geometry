#pragma once

#include "Calculator.h"
#include "Object.h"

#include <memory>

class FreeCalculator : public Calculator {
    public:
        FreeCalculator();

        Object* calc(const Object*) const override;
        bool isFree() const override;
};
