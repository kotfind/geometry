#include "Object.h"

bool Object::is(Type t) const {
    return static_cast<bool>(getType() & t);
}
