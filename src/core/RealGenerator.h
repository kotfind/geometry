#pragma once

#include "Generator.h"

#include <QString>

class Real;
class Engine;

class RealGenerator : public Generator {
    public:
        bool isReal() const override;

        QString getName() const;
        void setName(const QString&);

        double getValue() const;
        void setValue(double);

    protected:
        template<typename... Args>
        RealGenerator(const QString& name, Args&&... args)
          : Generator(std::forward<Args>(args)...),
            name(name)
        {}

    private:
        bool checkObjectType() const override;

        QString name;

    friend Engine;
    friend Generator;
};
