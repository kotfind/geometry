#include "Point.h"

#include "global.h"

bool operator==(const Point& p1, const Point& p2) {
    return eq(p1.x, p2.x) && eq(p1.y, p2.y);
}
