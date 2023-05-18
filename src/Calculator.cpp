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

Calculator::Calculator(const Geometry* geom)
  : geom(geom)
{}

void Calculator::setPos(const QPointF& pos) {
    throw std::logic_error("This type of Calculator cannot be moved");
}

Calculator* Calculator::fromJson(
    const Geometry* geom,
    const QJsonObject& json,
    const QList<Generator*>& gens,
    bool isReal
) {
    auto type = getOrThrow(json["type"]).toString();
    bool isFree         = type == "FREE";
    bool isDependant    = type == "DEPENDANT";
    bool isRestricted   = type == "RESTRICTED";
    assert(isFree || isDependant || isRestricted);

    if (isFree) {
        return FreeCalculator::fromJson(geom, json, isReal);
    } else if (isRestricted) {
        return RestrictedCalculator::fromJson(geom, json, gens);
    } else /* isDependant */ {
        return DependantCalculator::fromJson(geom, json, gens);
    }
}
