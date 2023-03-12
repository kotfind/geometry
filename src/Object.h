#pragma once

class Object {
    public:
        virtual ~Object() {}

        virtual int getType() const = 0;

    protected:
        Object() {}
};
