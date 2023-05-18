#pragma once

#include "Transformation.h"
#include "SectionMaster.h"

#include <memory>
#include <QPointF>

class AbstractPoint;

class Geometry {
    public:
        Geometry();

        AbstractPoint* makePoint(const QPointF& pos = QPointF());

        const SectionMaster* getSectionMaster() const;
        Transformation* getTransformation() const;

    private:
        std::unique_ptr<Transformation> transformation;
        std::unique_ptr<SectionMaster> sectionMaster;
};
