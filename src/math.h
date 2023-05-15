#pragma once

#include <QList>
#include <QPointF>

constexpr double eps = 1e-9;

bool eq(double a, double b);
bool le(double a, double b);
bool gr(double a, double b);
bool leq(double a, double b);
bool geq(double a, double b);

double sq(double x);

double len(const QPointF& p);
