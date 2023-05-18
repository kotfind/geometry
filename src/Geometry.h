#pragma once

#include <QPointF>
#include <QStringList>

class AbstractPoint;
class Transformation;
class SectionMaster;
class QString;

class Geometry {
    public:
        virtual ~Geometry() {}

        virtual AbstractPoint* makePoint(const QPointF& pos = QPointF()) const = 0;

        virtual const SectionMaster* getSectionMaster() const = 0;
        virtual Transformation* getTransformation() const = 0;

        virtual QString typeName(int) const = 0;
        QStringList complexTypeName(int) const;
};
