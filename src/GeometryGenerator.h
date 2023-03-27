#pragma once

#include "Generator.h"
#include "GeometryObject.h"

class GeometryItem;

class GeometryGenerator : public virtual Generator {
    public:
        GeometryGenerator(GeometryObject* obj, Geometry* geom = nullptr);

        bool hasGeometry() const override { return true; }

        void beginResetObject() override;
        void endResetObject() override;

        GeometryItem* getGeometryItem() const { return item; }

    protected:
        Object*& object() override { return reinterpret_cast<Object*&>(obj); }
        const Object* object() const override { return static_cast<const Object*>(obj); }

    private:
        GeometryObject* obj;
        GeometryItem* item;
};
