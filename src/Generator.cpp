#include "Generator.h"

#include "FreeCalculator.h"
#include "DependantCalculator.h"
#include "RestrictedCalculator.h"
#include "Object.h"
#include "Geometry.h"
#include "Point.h"
#include "Function.h"
#include "getOrThrow.h"
#include "GeometryGenerator.h"
#include "RealGenerator.h"
#include "Real.h"
#include "SectionMaster.h"

#include <stdexcept>
#include <QJsonArray>

Generator::Generator(std::unique_ptr<Object> obj)
  : calc(
        std::make_unique<FreeCalculator>(
            std::unique_ptr<Point>(
                static_cast<Point*>(obj->clone())
            )
        )
    ),
    obj(std::unique_ptr<Object>(obj->clone()))
{}

Generator::Generator(
    const Function* func,
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
    for (auto* gen : args) {
        gen->dependant << this;
    }
}

Generator::Generator(GeometryGenerator* restrictor, const QPointF& mousePos)
  : calc(
        std::make_unique<RestrictedCalculator>(
            restrictor,
            mousePos
        )
    )
{
    restrictor->dependant << this;
}

const Object* Generator::getObject() const {
    return obj.get();
}

bool Generator::isFree() const {
    return calc->getType() == Calculator::Type::FREE;
}

bool Generator::isDependant() const {
    return calc->getType() == Calculator::Type::DEPENDANT;
}

bool Generator::isRestricted() const {
    return calc->getType() == Calculator::Type::RESTRICTED;
}

bool Generator::isGeometry() const {
    return !isReal();
}

void Generator::recalc() {
    recalcSelf();

    for (auto* dep : dependant) {
        dep->recalc();
    }
}

void Generator::recalcSelf() {
    obj.reset(calc->calc());
    if (obj && !checkObjectType()) throw std::runtime_error("Wrong object type");

    onChanged();
}

QList<Generator*> Generator::getArgs() const {
    if (isRestricted()) {
        return { static_cast<RestrictedCalculator*>(calc.get())->getRestrictor() };
    } else if (isDependant()) {
        return static_cast<DependantCalculator*>(calc.get())->getArgs();
    } else /* isFree() */ {
        return {};
    }
}

Geometry* Generator::getGeometry() const {
    return geom;
}

QJsonObject Generator::toJson(const QHash<Generator*, int>& ids) const {
    QJsonObject json;

    json["isReal"] = isReal();

    QString type;
    if (isFree())       type = "FREE";
    if (isDependant())  type = "DEPENDANT";
    if (isRestricted()) type = "RESTRICTED";
    json["type"] = type;

    if (isReal()) {
        json["name"] = static_cast<const RealGenerator*>(this)->getName();
    }

    if (isFree()) {
        json["object"] = isReal()
            ? static_cast<Real*>(obj.get())->toJson()
            : static_cast<Point*>(obj.get())->toJson();
    } else if (isRestricted()) {
        auto* restCalc = static_cast<RestrictedCalculator*>(calc.get());

        json["restrictor"] = ids[restCalc->getRestrictor()];

        auto mousePos = restCalc->getMousePos();
        QJsonObject mousePosJson;
        mousePosJson["x"] = mousePos.x();
        mousePosJson["y"] = mousePos.y();
        json["mousePos"] = mousePosJson;
    } else {
        auto* depCalc = static_cast<DependantCalculator*>(calc.get());

        json["funcName"] = depCalc->getFunc()->getFullName();
        json["funcResNum"] = depCalc->getFuncResNum();
        QJsonArray jsonArgs;
        for (auto arg : depCalc->getArgs()) {
            jsonArgs << ids[arg];
        }
        json["args"] = jsonArgs;
    }

    return json;
}

Generator* Generator::fromJson(
    const QJsonObject& json,
    const QList<Generator*>& gens,
    const SectionMaster* sectionMaster
) {
    auto isReal = getOrThrow(json["isReal"]).toBool();

    auto type = getOrThrow(json["type"]).toString();
    bool isFree         = type == "FREE";
    bool isDependant    = type == "DEPENDANT";
    bool isRestricted   = type == "RESTRICTED";

    Generator* gen;

    QString name;
    if (isReal) {
        name = getOrThrow(json["name"]).toString();
    }

    if (isFree) {
        auto jsonObject = getOrThrow(json["object"]).toObject();
        if (isReal) {
            auto obj = std::unique_ptr<Real>(
                Real::fromJson(jsonObject)
            );
            gen = new RealGenerator(name, std::move(obj));
        } else {
            auto obj = std::unique_ptr<Point>(
                Point::fromJson(jsonObject)
            );
            gen = new GeometryGenerator(std::move(obj));
        }
    } else if (isRestricted) {
        auto* restrictor = gens[getOrThrow(json["restrictor"]).toInt()];

        auto mousePosJson = getOrThrow(json["mousePos"]).toObject();
        auto mousePos = QPointF(
            getOrThrow(mousePosJson["x"]).toDouble(),
            getOrThrow(mousePosJson["y"]).toDouble()
        );

        assert(restrictor->isGeometry());
        gen = new GeometryGenerator(
            static_cast<GeometryGenerator*>(restrictor),
            mousePos
        );
    } else {
        const auto& funcName = getOrThrow(json["funcName"]).toString();
        const auto* func = sectionMaster->get(funcName);

        QList<Generator*> args;
        const auto& jsonArgs = getOrThrow(json["args"]).toArray();
        for (const auto& arg : jsonArgs) {
            int id = arg.toInt();
            args << gens[id];
        }

        auto funcResNum = getOrThrow(json["funcResNum"]).toInt();

        if (isReal) {
            gen = new RealGenerator(name, func, args, funcResNum);
        } else {
            gen = new GeometryGenerator(func, args, funcResNum);
        }
    }

    return gen;
}

void Generator::onChanged() {
    geom->generatorChanged(this);
}
