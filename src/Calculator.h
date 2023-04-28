#pragma once

class Object;

class Calculator {
    public:
        virtual Object* calc() const = 0;
        virtual bool isFree() const = 0;
        bool isDependant() const;
};
