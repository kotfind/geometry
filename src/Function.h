#pragma once

#include "ArgumentInfo.h"
#include "Section.h"

#include <functional>
#include <QList>
#include <QHash>
#include <QString>
#include <QIcon>

class Object;
enum class Type : unsigned int;

class Function {
    public:
        Function(
            const QString& name,
            const QString& icon,
            const QString& description,
            const QList<ArgumentInfo>& argsInfo,
            int maxReturnSize,
            const std::function<QList<Object*>(const QList<const Object*>&)>& func
        );

        QList<Object*> operator()(const QList<const Object*>& objs) const;

        const QString& getFullName() const { return fullName; }
        const QIcon& getIcon() const { return icon; }
        const QString& getDescription() const { return description; }
        int countArgs() const { return argsInfo.size(); }
        const ArgumentInfo& getArgInfo(int i) const { return argsInfo[i]; }
        int getMaxReturnSize() const { return maxReturnSize; }

        const QString& getSelfName() const { return selfName; }
        Section* getSection() const { return section; }

        static Function* get(const QString& name) { return funcs[name]; }
        static const QHash<QString, Function*>& getAll() { return funcs; }

    private:
        static QHash<QString, Function*> funcs;

        QString fullName;
        QIcon icon;
        QString description;
        QList<ArgumentInfo> argsInfo;
        int maxReturnSize;
        std::function<QList<Object*>(const QList<const Object*>&)> func;

        QString selfName;
        Section* section;
};
