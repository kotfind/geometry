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

    private:
        std::unique_ptr<ETransformation> transformation;
        std::unique_ptr<SectionMaster> sectionMaster;
};
