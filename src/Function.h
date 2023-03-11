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

        QList<Object*> operator()(const QList<Object*>& objs) const;

        int countArgs() const { return typeHints.size(); }
        const QList<int>& getTypeHints() const { return typeHints; }

        static Function* get(const QString& name);
        static const QHash<QString, Function*>& getAll() { return funcs; }

    private:
        static QHash<QString, Function*> funcs;

        QList<int> typeHints;
        std::function<QList<Object*>(const QList<Object*>&)> func;
};
