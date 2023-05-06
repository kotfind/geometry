#include "Function.h"

#include "Object.h"
#include "Section.h"

#include <exception>
#include <QIcon>

Function::Function(
    const QString& name,
    const QString& iconName,
    const QString& description,
    const QList<ArgumentInfo>& argsInfo,
    int maxReturnSize,
    const std::function<QList<Object*>(const QList<const Object*>&)>& func
) : name(name),
    description(description),
    argInfo(argsInfo),
    maxReturnSize(maxReturnSize),
    func(func)
{
    Q_INIT_RESOURCE(imgs);
    icon = QIcon(iconName);
}

QList<Object*> Function::operator()(const QList<const Object*>& objs) const {
    if (objs.size() != countArgs())
        throw std::invalid_argument("Invalid number of objects");

    for (int i = 0; i < objs.size(); ++i) {
        if (!objs[i]->is(getArgInfo(i).getType())) {
            throw std::invalid_argument("Invalid type of object");
        }
    }

    return func(objs);
}

const QString& Function::getSelfName() const {
    return name;
}

QString Function::getFullName() const {
    return parent->getName() + "/" + getSelfName();
}

const QIcon& Function::getIcon() const {
    return icon;
}

const QString& Function::getDescription() const {
    return description;
}

int Function::countArgs() const {
    return argInfo.size();
}

const ArgumentInfo& Function::getArgInfo(int i) const {
    return argInfo[i];
}

int Function::getMaxReturnSize() const {
    return maxReturnSize;
}

