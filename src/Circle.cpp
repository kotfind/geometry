#include "Circle.h"

Circle::Circle() : Circle(Point(), 1) {}
Circle::Circle(const Point& o, double r) : GeometryObject(), o(o), r(r) {}
