#include "Point.h"

#include "global.h"

Point::Point() : Point(0, 0) {}
Point::Point(double x, double y) : GeometryObject(), x(x), y(y) {}

bool operator==(const Point& p1, const Point& p2) {
    return eq(p1.x, p2.x) && eq(p1.y, p2.y);
}
