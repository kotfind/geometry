#pragma once

#include "Generator.h"

class Real;

class RealGenerator : public Generator {
    using Generator::Generator; // Inherit constructors

    private:
        bool checkObjectType() const override;
};
