#include "Calculator.h"

#include "Real.h"
#include "FreeCalculator.h"
#include "DependantCalculator.h"
#include "RestrictedCalculator.h"
#include "GeometryGenerator.h"
#include "Function.h"
#include "getOrThrow.h"
#include "SectionMaster.h"

#include <stdexcept>
#include <QJsonArray>

void Calculator::setPos(const QPointF& pos) {
    throw std::logic_error("This type of Calculator cannot be moved");
}

Calculator* Calculator::fromJson(
    const QJsonObject& json,
    const QList<Generator*>& gens,
    const SectionMaster* sectionMaster,
    bool isReal
) {
    auto type = getOrThrow(json["type"]).toString();
    bool isFree         = type == "FREE";
    bool isDependant    = type == "DEPENDANT";
    bool isRestricted   = type == "RESTRICTED";
    assert(isFree || isDependant || isRestricted);

    if (isFree) {
        return FreeCalculator::fromJson(json, isReal);
    } else if (isRestricted) {
        return RestrictedCalculator::fromJson(json, gens);
    } else /* isDependant */ {
        return DependantCalculator::fromJson(json, gens, sectionMaster);
    }
}
