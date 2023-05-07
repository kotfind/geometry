#pragma once

#include "ArgumentInfo.h"

#include <functional>
#include <QList>
#include <QHash>
#include <QString>
#include <QIcon>

class Section;
class Object;
enum class Type : unsigned int;

class Function {
    public:
        QList<Object*> operator()(const QList<const Object*>& objs) const;

        const QString& getSelfName() const;
        QString getFullName() const;
        const QIcon& getIcon() const;
        const QString& getDescription() const;
        int countArgs() const;
        const ArgumentInfo& getArgInfo(int i) const;
        int getMaxReturnSize() const;

    private:
        Function(
            const QString& name,
            const QIcon& icon,
            const QString& description,
            const QList<ArgumentInfo>& argsInfo,
            int maxReturnSize,
            const std::function<QList<Object*>(const QList<const Object*>&)>& func
        );

        Section* parent = nullptr;
        QString name;
        QIcon icon;
        QString description;
        QList<ArgumentInfo> argInfo;
        int maxReturnSize;
        std::function<QList<Object*>(const QList<const Object*>&)> func;

    friend Section;
};
