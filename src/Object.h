#pragma once

#include <QStringList>

class QString;

class Object {
    public:
        enum class Type : unsigned int {
            None    = 0,
            Any     = static_cast<unsigned int>(-1),
            Real    = 1<<0,
            Point   = 1<<1,
            Line    = 1<<2,
            Circle  = 1<<3,
            Segment = 1<<4,
        };

        virtual ~Object() {}

        virtual Type getType() const = 0;

        bool is(Type) const;

        virtual Object* clone() const = 0;

    protected:
        Object() {}
};

Object::Type operator|(Object::Type lhs, Object::Type rhs);
Object::Type operator&(Object::Type lhs, Object::Type rhs);

QString typeName(Object::Type);
QStringList complexTypeName(Object::Type);
