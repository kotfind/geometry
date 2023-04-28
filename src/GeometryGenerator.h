#pragma once

#include "Generator.h"

class GeometryObject;
class GeometryItem;

class GeometryGenerator : public Generator {
    using Generator::Generator; // Inherit constructors

    public:
        GeometryItem* getGeometryItem() const;

        void move(const QPointF& delta);

    protected:
        void beginResetObject() override;
        void endResetObject() override;

    private:
        bool checkObjectType() const override;

        GeometryItem* item = nullptr;
};
