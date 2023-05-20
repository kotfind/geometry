#pragma once

#include <QPointF>
#include <QStringList>

class AbstractPoint;
class AbstractTransformation;
class SectionMaster;
class QString;
class QGraphicsItem;

class AbstractGeometry {
    public:
        virtual ~AbstractGeometry() {}

        virtual AbstractPoint* makePoint(const QPointF& pos = QPointF()) const = 0;

        virtual const SectionMaster* getSectionMaster() const = 0;
        virtual AbstractTransformation* getTransformation() const = 0;

        virtual QString typeName(int) const = 0;
        QStringList complexTypeName(int) const;

        virtual QGraphicsItem* getGraphicsItem() const;
};
