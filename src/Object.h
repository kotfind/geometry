#pragma once

#include <QStringList>

class QString;

class Object {
    public:
        virtual ~Object() {}

        virtual int getType() const = 0;

        virtual bool isGeometry() const = 0;
        bool isReal() const;

        virtual Object* clone() const = 0;

        bool is(int) const;
};

QString typeName(int);
QStringList complexTypeName(int);
