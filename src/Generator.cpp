#include "Generator.h"

#include "FreeCalculator.h"
#include "DependantCalculator.h"
#include "Object.h"
#include "Geometry.h"

#include <stdexcept>

Generator::Generator(Object* obj)
  : calc(
        std::make_unique<FreeCalculator>(
            std::unique_ptr<Object>(obj->clone())
        )
    )
{
    recalc();
}

Generator::Generator(
    Function* func,
    const QList<Generator*>& args,
    int funcResNum
) : calc(
        std::make_unique<DependantCalculator>(
            func,
            args,
            funcResNum
        )
    )
{
    recalc();

    for (auto* gen : args) {
        gen->dependant << this;
    }
}

const Object* Generator::getObject() const {
    return obj.get();
}

bool Generator::isFree() const {
    return calc->isFree();
}

bool Generator::isDependant() const {
    return !isFree();
}

void Generator::recalc() {
    beginResetObject();

    obj.reset(calc->calc());
    if (!checkObjectType()) throw std::runtime_error("Wrong object type");

    for (auto* dep : dependant) {
        dep->recalc();
    }

    endResetObject();
}

void Generator::remove() {
    throw std::logic_error("NotImplemented");
}

