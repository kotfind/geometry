#include "Type.h"

#include "global.h"

#include <QString>
#include <type_traits>
#include <stdexcept>

Type operator|(Type lhs, Type rhs) {
    using T = std::underlying_type_t<Type>;
    return static_cast<Type>(static_cast<T>(lhs) | static_cast<T>(rhs));
}

Type operator&(Type lhs, Type rhs) {
    using T = std::underlying_type_t<Type>;
    return static_cast<Type>(static_cast<T>(lhs) & static_cast<T>(rhs));
}

QString typeName(Type t) {
    switch (t) {
        case Type::Real: return TR("Real");
        case Type::Point: return TR("Point");
        case Type::Line: return TR("Line");
        case Type::Circle: return TR("Circle");
        default: throw std::invalid_argument("Wrong or complex type");
    }
}

QStringList complexTypeName(Type type) {
    QStringList ans;
    for (int i = 0; i < sizeof(type); ++i) {
        auto t = static_cast<Type>(1 << i);
        if (static_cast<int>(type & t)) {
            ans << typeName(t);
        }
    }
    return ans;
}
