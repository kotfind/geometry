#pragma once

#include <QList>
#include <QPointF>
#include <QRectF>

constexpr double eps = 1e-9;

bool eq(double a, double b);
bool le(double a, double b);
bool gr(double a, double b);
bool leq(double a, double b);
bool geq(double a, double b);

double sq(double x);

double len(const QPointF& p);
double dist(const QPointF& p1, const QPointF& p2);

QRectF getCircleRect(const QPointF& center, double radius);

double cross(const QPointF& p1, const QPointF& p2);

// Solves square equation a * x^2 + b * x + c = 0.
// Returns number of arguments n,
// and roots x1, x2 (if exists)
void solveSqEq(
    double a,
    double b,
    double c,
    int& n,
    double& x1,
    double& x2
);
