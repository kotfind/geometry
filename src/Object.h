#pragma once

class Object {
    public:
        virtual int getType() const = 0;

    protected:
        Object() {}
};
