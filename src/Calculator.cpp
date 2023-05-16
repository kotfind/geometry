#include "Calculator.h"

#include "Real.h"
#include "Point.h"
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

QJsonObject Calculator::toJson(const QHash<Generator*, int>& ids, bool isReal) const {
    QJsonObject json;

    QString type;
    if (getType() == Type::FREE)        type = "FREE";
    if (getType() == Type::DEPENDANT)   type = "DEPENDANT";
    if (getType() == Type::RESTRICTED)  type = "RESTRICTED";
    json["type"] = type;

    switch (getType()) {
        case Type::FREE: {
            // TODO: Move to FreeCalculator::toJson
            auto* freeCalc = static_cast<const FreeCalculator*>(this);
            json["object"] = isReal
                ? static_cast<Real*>(freeCalc->getObject())->toJson()
                : static_cast<Point*>(freeCalc->getObject())->toJson();
        } break;

        case Type::RESTRICTED: {
            // TODO: Move to RestrictedCalculator::toJson
            auto* restCalc = static_cast<const RestrictedCalculator*>(this);

            json["restrictor"] = ids[restCalc->getRestrictor()];
            json["posValue"] = restCalc->getPosValue();
        } break;

        case Type::DEPENDANT: {
            // TODO: Move to DependantCalculator::toJson
            auto* depCalc = static_cast<const DependantCalculator*>(this);

            json["funcName"] = depCalc->getFunc()->getFullName();
            json["funcResNum"] = depCalc->getFuncResNum();
            QJsonArray jsonArgs;
            for (auto arg : depCalc->getArgs()) {
                jsonArgs << ids[arg];
            }
            json["args"] = jsonArgs;
        } break;
    }

    return json;
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
        // TODO: Move to FreeCalculator::fromJson
        auto jsonObject = getOrThrow(json["object"]).toObject();
        auto obj = isReal
            ? std::unique_ptr<Object>(Real::fromJson(jsonObject))
            : std::unique_ptr<Object>(Point::fromJson(jsonObject));
        return new FreeCalculator(std::move(obj));
    } else if (isRestricted) {
        // TODO: Move to RestrictedCalculator::fromJson
        auto* restrictor = gens[getOrThrow(json["restrictor"]).toInt()];
        auto posValue = getOrThrow(json["posValue"]).toDouble();

        assert(restrictor->isGeometry());
        return new RestrictedCalculator(
            static_cast<GeometryGenerator*>(restrictor),
            posValue
        );
    } else {
        // TODO: Move to DependantCalculator::fromJson
        const auto& funcName = getOrThrow(json["funcName"]).toString();
        const auto* func = sectionMaster->get(funcName);

        QList<Generator*> args;
        const auto& jsonArgs = getOrThrow(json["args"]).toArray();
        for (const auto& arg : jsonArgs) {
            int id = arg.toInt();
            args << gens[id];
        }

        auto funcResNum = getOrThrow(json["funcResNum"]).toInt();

        return new DependantCalculator(func, args, funcResNum);
    }
}
