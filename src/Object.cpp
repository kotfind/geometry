#include "Object.h"

#include "TR.h"

#include "Real.h" // TODO: delete me
#include "Point.h" // TODO: delete me
#include "Line.h" // TODO: delete me
#include "Circle.h" // TODO: delete me

#include <QString>
#include <type_traits>
#include <stdexcept>

// -------------------- Object --------------------

bool Object::is(int t) const {
    return static_cast<bool>(getType() & t);
}

bool Object::isReal() const {
    return !isGeometry();
}

// -------------------- Object::Type --------------------

QString typeName(int t) {
    switch (t) {
        case Real::Type: return TR("Real");
        case Point::Type: return TR("Point");
        case Line::Type: return TR("Line");
        case Circle::Type: return TR("Circle");
        default: throw std::invalid_argument("Wrong or complex type");
    }
}

QStringList complexTypeName(int type) {
    QStringList ans;
    for (int i = 0; i < sizeof(int); ++i) {
        auto t = 1 << i;
        if (type & t) {
            ans << typeName(t);
        }
    }
    return ans;
}
