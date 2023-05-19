#pragma once

#include "core/AbstractGeometry.h"

#include "Transformation.h"
#include "core/SectionMaster.h"

#include <memory>
#include <QPointF>

class Geometry : public AbstractGeometry {
    public:
        Geometry();

        AbstractPoint* makePoint(const QPointF& pos = QPointF()) const override;

        const SectionMaster* getSectionMaster() const override;
        AbstractTransformation* getTransformation() const override;

        QString typeName(int) const override;

    private:
        // Is called from c'tor
        // Is defined in EFunctionList.cpp
        SectionMaster* makeSectionMaster() const;

        std::unique_ptr<Transformation> transformation;
        std::unique_ptr<SectionMaster> sectionMaster;
};
