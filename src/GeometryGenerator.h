#pragma once

#include "Generator.h"
#include "GeometryItem.h"

#include <memory>

class GeometryObject;
class Geometry;

class GeometryGenerator : public Generator {
    public:
        GeometryItem* getGeometryItem() const;

        void move(const QPointF& delta);

    protected:
        template<typename... Args>
        GeometryGenerator(Args&&... args)
          : Generator(std::forward<Args>(args)...),
            item(std::make_unique<GeometryItem>(this))
        {}

        void beginResetObject() override;
        void endResetObject() override;

    private:
        bool checkObjectType() const override;

        std::unique_ptr<GeometryItem> item;

    friend Geometry;
};
