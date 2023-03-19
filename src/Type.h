#pragma once

enum class Type : unsigned int {
    None    = 0,
    Any     = static_cast<unsigned int>(-1),
    Real    = 1<<0,
    Point   = 1<<1,
    Line    = 1<<2,
    Circle  = 1<<3,
};

Type operator|(Type lhs, Type rhs);

Type operator&(Type lhs, Type rhs);
