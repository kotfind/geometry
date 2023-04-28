#include "RealGenerator.h"

#include "Object.h"

bool RealGenerator::checkObjectType() const {
    return obj->is(Type::Real);
}
