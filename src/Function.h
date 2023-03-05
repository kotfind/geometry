#pragma once

#include <functional>
#include <QList>
#include <QHash>
#include <QString>

class Object;

class Function {
    public:
        Function(
            const QString& name,
            const QList<int>& typeHints,
            const std::function<QList<Object*>(const QList<Object*>&)>& func
        );

        QList<Object*> operator()(const QList<Object*>& objs);

    private:
        static QHash<QString, Function*> funcs;

        QList<int> typeHints;
        std::function<QList<Object*>(const QList<Object*>&)> func;
};
