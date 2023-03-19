#pragma once

#include <functional>
#include <QList>
#include <QHash>
#include <QString>

class Object;
enum class Type : unsigned int;

class Function {
    public:
        Function(
            const QString& name,
            const QList<Type>& typeHints,
            int maxReturnSize,
            const std::function<QList<Object*>(const QList<const Object*>&)>& func
        );

        QList<Object*> operator()(const QList<const Object*>& objs) const;

        const QString& getName() const { return name; }
        int countArgs() const { return typeHints.size(); }
        const QList<Type>& getTypeHints() const { return typeHints; }
        int getMaxReturnSize() const { return maxReturnSize; }

        static Function* get(const QString& name);
        static const QHash<QString, Function*>& getAll() { return funcs; }

    private:
        static QHash<QString, Function*> funcs;

        QString name;
        QList<Type> typeHints;
        int maxReturnSize;
        std::function<QList<Object*>(const QList<const Object*>&)> func;
};
