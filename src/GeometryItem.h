#pragma once

#include <QGraphicsItem>

class GeometryObject;
class QRectF;
class QPainter;
class QStyleOptionGraphicsItem;
class GeometryGenerator;

// Is a proxy between GeometryObject and QGraphicsItem
class GeometryItem : public QGraphicsItem {
    public:
        GeometryItem(GeometryGenerator* gen);

        GeometryGenerator* getGeometryGenerator() const { return gen; }

        void beginResetObject();
        void endResetObject();

        void paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*) override;
        QRectF boundingRect() const override;
        QPainterPath shape() const override;

        void remove();

    private:
        void updateBoundingRect();

        const GeometryObject* object() const;

        GeometryGenerator* gen = nullptr;
};
