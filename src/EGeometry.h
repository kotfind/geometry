#pragma once

#include "Geometry.h"

#include "ETransformation.h"
#include "SectionMaster.h"

#include <memory>
#include <QPointF>

class EGeometry : public Geometry {
    public:
        EGeometry();

        AbstractPoint* makePoint(const QPointF& pos = QPointF()) const override;

        const SectionMaster* getSectionMaster() const override;
        Transformation* getTransformation() const override;

        QString typeName(int) const override;

    private:
        // Is called from c'tor
        // Is defined in EFunctionList.cpp
        SectionMaster* makeSectionMaster() const;

        std::unique_ptr<ETransformation> transformation;
        std::unique_ptr<SectionMaster> sectionMaster;
};
