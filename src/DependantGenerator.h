#pragma once

#include "Generator.h"

#include <QList>
#include <QHash>

class Function;
class QJsonObject;

class DependantGenerator : public Generator {
    public:
        DependantGenerator(Geometry* geom, Function* func, const QList<Generator*>& args, int funcResNum = 0);

        bool isFree() const override { return false; }

        void recalcSelf() override;

        const QList<Generator*>& getArgs() const { return args; }

        void remove() override;

        QJsonObject toJson(const QHash<Generator*, int>& ids) const override;

    private:
        Function* func;
        QList<Generator*> args;
        int funcResNum = 0;
};
