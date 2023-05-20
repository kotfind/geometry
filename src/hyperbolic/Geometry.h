#pragma once

#include "Transformation.h"

#include "core/AbstractGeometry.h"
#include "core/SectionMaster.h"

#include <memory>

class QGraphicsEllipseItem;

namespace hyperbolic {
    class Geometry : public AbstractGeometry {
        public:
            Geometry();

            AbstractPoint* makePoint(const QPointF& pos = QPointF()) const override;

            const SectionMaster* getSectionMaster() const override;
            AbstractTransformation* getTransformation() const override;

            QString typeName(int) const override;

            QGraphicsItem* getGraphicsItem() const override;

        private:
            // Is called from c'tor
            // Is defined in functionList.cpp
            SectionMaster* makeSectionMaster() const;

            std::unique_ptr<impl::Transformation> transformation;
            std::unique_ptr<SectionMaster> sectionMaster;

            QGraphicsEllipseItem* absoluteCircleItem;
    };
}
