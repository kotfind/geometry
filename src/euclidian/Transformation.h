#pragma once

#include "Point.h"

#include "core/AbstractTransformation.h"

namespace euclidian::impl {
    class Transformation : public AbstractTransformation {
        public:
            void scroll(const QPointF& delta) override;
            void move(const QPointF& delta) override;
            void zoom(double, const QPointF& zoomCenter) override;

            void clear() override;

            void transform(AbstractPoint*) const override;
            void untransform(AbstractPoint*) const override;

            QJsonObject toJson() const override;
            void fromJson(const QJsonObject& json) override;

        private:
            Point center{0, 0};
            double scale = 1;

            static constexpr double scrollSpeed = 0.01;
            static constexpr double zoomSpeed = 0.01;
    };
}
