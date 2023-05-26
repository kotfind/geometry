#pragma once

#include "core/AbstractTransformation.h"

#include <complex>

namespace hyperbolic::impl {
    class Transformation : public AbstractTransformation {
        public:
            void scroll(const QPointF& delta) override;
            void move(const AbstractPoint* from, const AbstractPoint* to) override;
            void zoom(double, const QPointF& zoomCenter) override;

            void clear() override;

            void transform(AbstractPoint*) const override;
            void untransform(AbstractPoint*) const override;

            QJsonObject toJson() const override;
            void fromJson(const QJsonObject& json) override;
    };
}
