#include "RealGenerator.h"

#include "Real.h"

bool RealGenerator::isReal() const {
    return true;
}

bool RealGenerator::checkObjectType() const {
    return obj->is(Type::Real);
}

QString RealGenerator::getName() const {
    return name;
}

void RealGenerator::setName(const QString& n) {
    name = n;
}

void RealGenerator::setValue(double v) {
    obj = std::make_unique<Real>(v);
}

double RealGenerator::getValue() const {
    return static_cast<Real*>(obj.get())->value;
}
