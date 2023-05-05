#pragma once

#include "GeometryObject.h"

#include <QGraphicsItem>
#include <memory>

class QRectF;
class QPainter;
class QStyleOptionGraphicsItem;
class GeometryGenerator;

// Is a proxy between GeometryObject and QGraphicsItem
class GeometryItem : public QGraphicsItem {
    public:
        GeometryItem(GeometryGenerator* gen);

        GeometryGenerator* getGeometryGenerator() const { return gen; }

        void paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*) override;
        QRectF boundingRect() const override;
        QPainterPath shape() const override;

        void remove();

        void update();

    private:
        void updateBoundingRect();

        GeometryGenerator* gen;

        // Transformed object
        std::unique_ptr<GeometryObject> obj = nullptr;
};
