#pragma once

#include <QRectF>
#include <QList>
#include <QPointF>

class Generator;
class QJsonObject;
class QString;
class QGraphicsScene;

class Geometry {
    public:
        Geometry();
        ~Geometry();

        void addGenerator(Generator*);
        void removeGenerator(Generator*);

        const QRectF& getSceneRect() const { return sceneRect; }

        void move(const QPointF& delta);

        QPointF transform(const QPointF& pt);

        QJsonObject toJson() const;

        void save(const QString& fileName) const;

        void fromJson(const QJsonObject&);

        void load(const QString& file);

        void clear();

        void populateScene(QGraphicsScene*);

        void setChanged(bool v = true);
        bool isChanged() const { return changed; }

    private:
        void recalcAll();

        QRectF sceneRect = QRect(-0.5, -0.5, 1, 1);

        QList<Generator*> gens;

        QPointF shift = QPointF(0, 0);

        bool changed = false;
};
