#pragma once

#include <QGraphicsItem>

class GeometryObject;
class QRectF;
class QPainter;
class QStyleOptionGraphicsItem;

// Is a proxy between GeometryObject and QGraphicsItem
class GeometryItem : public QGraphicsItem {
    public:
        GeometryItem();

        void setObject(GeometryObject* object);

        void paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*) override;
        QRectF boundingRect() const override;

    private:
        GeometryObject* object = nullptr;
};
