#include "AbstractGeometry.h"

QStringList AbstractGeometry::complexTypeName(int type) const {
    QStringList ans;
    for (int i = 0; i < sizeof(int); ++i) {
        auto t = 1 << i;
        if (type & t) {
            ans << typeName(t);
        }
    }
    return ans;
}
