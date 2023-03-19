#include "FreeGenerator.h"

#include "Point.h"
#include "GeometryItem.h"
#include "Geometry.h"
#include "getOrThrow.h"

#include <QPointF>
#include <QJsonObject>
#include <QJsonArray>

FreeGenerator::FreeGenerator(Point* pt)
  : Generator(),
    origPoint(pt)
{
    object = new Point(*pt);
}

FreeGenerator::~FreeGenerator() {
    delete origPoint;
}

void FreeGenerator::move(const QPointF& delta) {
    geom->setChanged();

    item->beginResetObject();
    origPoint->move(delta);
    item->endResetObject();

    recalc();
}

void FreeGenerator::recalcSelf() {
    point()->setPos(geom->transform(origPoint->getPos()));
}

const Point* FreeGenerator::point() const {
    return static_cast<Point*>(object);
}

Point* FreeGenerator::point() {
    return static_cast<Point*>(object);
}

QJsonObject FreeGenerator::toJson(const QHash<Generator*, int>& ids) const {
    auto json = Generator::toJson(ids);
    json["object"] = point()->toJson();
    return json;
}

void FreeGenerator::load(Geometry* geom, const QJsonArray& jsonGens, QList<Generator*>& gens, int i) {
    const auto& json = jsonGens[i];
    auto* pt = Point::fromJson(getOrThrow(json["object"]).toObject());
    gens[i] = new FreeGenerator(pt);
    gens[i]->setGeometry(geom);
}
