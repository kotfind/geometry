#pragma once

#include <QGraphicsItem>

class GeometryObject;
class QRectF;
class QPainter;
class QStyleOptionGraphicsItem;
class Generator;

// Is a proxy between GeometryObject and QGraphicsItem
class GeometryItem : public QGraphicsItem {
    public:
        Generator* getGenerator() const { return gen; }

        void beginResetObject();
        void endResetObject();

        void paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*) override;
        QRectF boundingRect() const override;
        QPainterPath shape() const override;

    private:
        GeometryItem(Generator* gen);

        void updateBoundingRect();

        GeometryObject* getObject() const;

        Generator* gen = nullptr;

    friend Generator;
};
