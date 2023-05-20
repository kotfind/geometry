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

QRectF getCircleRect(const QPointF& center, double radius) {
    return QRectF(
        center.x() - radius,
        center.y() - radius,
        radius * 2,
        radius * 2
    );
}

double cross(const QPointF& p1, const QPointF& p2) {
    return p1.x() * p2.y() - p2.x() * p1.y();
}

void solveSqEq(
    double a,
    double b,
    double c,
    int& n,
    double& x1,
    double& x2
) {
    double D = b * b - 4 * a * c;

    if (le(D, 0)) {
        n = 0;
        return;
    }

    if (eq(D, 0)) {
        n = 1;
        x1 = - b / (2 * a);
        return;
    }

    n = 2;
    double sqD = sqrt(D);
    x1 = (-b - sqD) / (2 * a);
    x2 = (-b + sqD) / (2 * a);
}
