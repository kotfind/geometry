#include "Transformation.h"

#include "Point.h"

#include "util/getOrThrow.h"
#include "util/cramer.h"

#include <math.h>
#include <complex>

using namespace std::complex_literals;

namespace hyperbolic::impl {
    void Transformation::scroll(const QPointF& delta) {
        // XXX: remeber about metrix
        z0 += std::complex<double>(delta.x(), delta.y()) * scrollSpeed;
    }

    void Transformation::move(const AbstractPoint* from, const AbstractPoint* to) {
        // Solving equation:
        // q = exp(1i * phi) * (z - z0) / (1 - conj(z0) * z)
        //   where
        //     z0 is variable,
        //     z is from,
        //     q is transformed(to).
        //
        // Getting equation:
        // z0 - q * exp(-1i * phi) * z * conj(z0) = z - q * exp(-1i * phi)
        //
        // Let m = z - q * exp(-i * phi),
        //     n = q * exp(-i * phi) * z,
        //
        // Getting equation:
        // z - n * conj(z0) = m
        //
        // Getting equation system (in Re(z0) ans Im(z0))

        auto z = static_cast<const Point*>(from)->getComplex();
        auto to_ = std::unique_ptr<Point>(
            static_cast<Point*>(to->clone())
        );
        transform(to_.get());
        auto q = to_->getComplex();

        auto m = z - q * std::exp(-1i * phi);
        auto n = q * std::exp(-1i * phi) * z;

        auto cramerAns = cramer({
            //     a                   b               free
            {1 - std::real(n),  -std::imag(n),      std::real(m)},
            {-std::imag(n),     1 + std::real(n),   std::imag(m)},
        });
        assert(!cramerAns.isEmpty());

        z0 = std::complex<double>(cramerAns[0], cramerAns[1]);
    }

    void Transformation::zoom(double v, const QPointF& zoomCenter) {
        phi += v * rotationSpeed;
    }

    void Transformation::clear() {
        phi = 0;
        z0 = 0;
    }

    QJsonObject Transformation::toJson() const {
        // FIXME
    }

    void Transformation::fromJson(const QJsonObject& json) {
        // FIXME
    }

    void Transformation::transform(AbstractPoint* pt) const {
        auto& p = *static_cast<Point*>(pt);
        auto z = p.getComplex();
        p.setComplex(
            std::exp(phi * 1i) * (z - z0) / (1. - std::conj(z0) * z)
        );
    }

    void Transformation::untransform(AbstractPoint* pt) const {
        auto& p = *static_cast<Point*>(pt);
        auto e = std::exp(-1i * phi);
        auto z = p.getComplex();
        p.setComplex(
            (z * e + z0) / (z * std::conj(z0) * e + 1.)
        );
    }
}
