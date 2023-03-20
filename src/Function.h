#pragma once

#include "ArgumentInfo.h"

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
            const QString& description,
            const QList<ArgumentInfo>& argsInfo,
            int maxReturnSize,
            const std::function<QList<Object*>(const QList<const Object*>&)>& func
        );

        QList<Object*> operator()(const QList<const Object*>& objs) const;

        const QString& getName() const { return name; }
        const QString& getDescription() const { return description; }
        int countArgs() const { return argsInfo.size(); }
        const ArgumentInfo& getArgInfo(int i) const { return argsInfo[i]; }
        int getMaxReturnSize() const { return maxReturnSize; }

        static Function* get(const QString& name);
        static const QHash<QString, Function*>& getAll() { return funcs; }

    private:
        static QHash<QString, Function*> funcs;

        QString name;
        QString description;
        QList<ArgumentInfo> argsInfo;
        int maxReturnSize;
        std::function<QList<Object*>(const QList<const Object*>&)> func;
};
