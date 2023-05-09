#pragma once

class Object;

class Calculator {
    public:
        enum class Type {
            FREE,
            DEPENDANT,
            RESTRICTED,
        };

        virtual ~Calculator() {}

        virtual Object* calc() const = 0;

        virtual Type getType() const = 0;
};
