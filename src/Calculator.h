#pragma once

class Object;

class Calculator {
    public:
        virtual Object* calc(const Object* obj) const = 0;
        virtual bool isFree() const = 0;
        bool isDependant() const;
};
