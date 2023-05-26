#include "Transformation.h"

#include "Point.h"

#include "util/getOrThrow.h"
#include "util/cramer.h"

#include <math.h>
#include <complex>

using namespace std::complex_literals;

namespace hyperbolic::impl {
    void Transformation::scroll(const QPointF& delta) {
        // FIXME
    }

    void Transformation::move(const AbstractPoint* from, const AbstractPoint* to) {
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

    void Transformation::transform(AbstractPoint* pt) const {
        // FIXME
    }

    void Transformation::untransform(AbstractPoint* pt) const {
        // FIXME
    }
}
