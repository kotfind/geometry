#include "RealGenerator.h"

#include "Real.h"
#include "FreeCalculator.h"

bool RealGenerator::isReal() const {
    return true;
}

bool RealGenerator::checkObjectType() const {
    return obj->isReal();
}

QString RealGenerator::getName() const {
    return name;
}

void RealGenerator::setName(const QString& n) {
    name = n;
}

void RealGenerator::setValue(double v) {
    assert(isFree());
    static_cast<FreeCalculator*>(calc.get())->setValue(v);
    recalc();
}

double RealGenerator::getValue() const {
    return static_cast<Real*>(obj.get())->value;
}
