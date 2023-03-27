#pragma once

#include "Generator.h"

class FreeGenerator : public virtual Generator {
    public:
        // Call object() = obj manually in child constructor
        FreeGenerator(Geometry* geom = nullptr);

        bool isFree() const override { return true; }

    protected:
        void recalcSelf() override;
};
