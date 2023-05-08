#pragma once

#include "Generator.h"
#include "GeometryItem.h"

#include <memory>

class GeometryObject;
class Geometry;

class GeometryGenerator : public Generator {
    public:
        bool isReal() const override;

        GeometryItem* getGeometryItem() const;

        const GeometryObject* getGeometryObject() const;

        void setPos(const QPointF&);

    protected:
        template<typename... Args>
        GeometryGenerator(Args&&... args)
          : Generator(std::forward<Args>(args)...),
            item(std::make_unique<GeometryItem>(this))
        {}

        void onChanged() override;

    private:
        bool checkObjectType() const override;

        std::unique_ptr<GeometryItem> item;

    friend Geometry;
    friend Generator;
};
