#include "Line.h"


Line::Line() : Line(0, 1, 0) {}
Line::Line(double a, double b, double c) : GeometryObject(), a(a), b(b), c(c) {}
