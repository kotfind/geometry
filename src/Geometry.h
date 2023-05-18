#pragma once

#include <QPointF>

class AbstractPoint;
class Transformation;
class SectionMaster;

class Geometry {
    public:
        virtual ~Geometry() {}

        virtual AbstractPoint* makePoint(const QPointF& pos = QPointF()) const = 0;

        virtual const SectionMaster* getSectionMaster() const = 0;
        virtual Transformation* getTransformation() const = 0;
};
