#include "Object.h"

bool Object::is(int t) const {
    return static_cast<bool>(getType() & t);
}

bool Object::isReal() const {
    return !isGeometry();
}
