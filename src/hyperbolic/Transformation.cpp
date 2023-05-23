#include "Transformation.h"

#include "util/getOrThrow.h"

#include <math.h>

namespace hyperbolic::impl {
    void Transformation::scroll(const QPointF& delta) {
        // FIXME
    }

    void Transformation::move(const QPointF& delta) {
        // FIXME
    }

    void Transformation::zoom(double v, const QPointF& zoomCenter) {
        // FIXME
    }

    void Transformation::clear() {
        // FIXME
    }

    QJsonObject Transformation::toJson() const {
        // FIXME
    }

    void Transformation::fromJson(const QJsonObject& json) {
        // FIXME
    }

    void Transformation::transform(AbstractPoint* p) const {
        // FIXME
    }

    void Transformation::untransform(AbstractPoint* p) const {
        // FIXME
    }
}
