#include "math.h"

#include <math.h>

bool eq(double a, double b) {
    return abs(a - b) < eps;
}

bool le(double a, double b) {
    return a < b - eps;
}

bool gr(double a, double b) {
    return a > b + eps;
}

bool leq(double a, double b) {
    return a < b + eps;
}

bool geq(double a, double b) {
    return a > b - eps;
}

double sq(double x) {
    return x*x;
}

double len(const QPointF& p) {
    return hypot(p.x(), p.y());
}

double dist(const QPointF& p1, const QPointF& p2) {
    return len(p1 - p2);
}
