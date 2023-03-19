#include "Type.h"

#include <type_traits>

Type operator|(Type lhs, Type rhs) {
    using T = std::underlying_type_t<Type>;
    return static_cast<Type>(static_cast<T>(lhs) | static_cast<T>(rhs));
}

Type operator&(Type lhs, Type rhs) {
    using T = std::underlying_type_t<Type>;
    return static_cast<Type>(static_cast<T>(lhs) & static_cast<T>(rhs));
}
