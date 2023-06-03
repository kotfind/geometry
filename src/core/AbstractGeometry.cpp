#include "AbstractGeometry.h"

QStringList AbstractGeometry::complexTypeName(int type) const {
    QStringList ans;
    for (int i = 0; i < 8 * sizeof(int) - 1; ++i) {
        auto t = 1 << i;
        if (type & t) {
            ans << typeName(t);
        }
    }
    return ans;
}

QGraphicsItem* AbstractGeometry::getGraphicsItem() const {
    return nullptr;
}
