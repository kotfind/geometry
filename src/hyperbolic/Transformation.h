#pragma once

#include "core/AbstractTransformation.h"

namespace hyperbolic::impl {
    class Transformation : public AbstractTransformation {
        public:
            void scroll(const QPointF& delta) override;
            void move(const QPointF& delta) override;
            void zoom(double, const QPointF& zoomCenter) override;

            void clear() override;

            QPointF transform(const QPointF&) const override;
            QPointF untransform(const QPointF&) const override;

            QJsonObject toJson() const override;
            void fromJson(const QJsonObject& json) override;
    };
}
