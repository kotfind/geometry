#include "Function.h"

#include "Object.h"

#include <exception>

QHash<QString, Function*> Function::funcs;
QList<QString> Function::sections;

Function::Function(
    const QString& sectionName,
    const QString& name,
    const QString& description,
    const QList<ArgumentInfo>& argsInfo,
    int maxReturnSize,
    const std::function<QList<Object*>(const QList<const Object*>&)>& func
) : sectionName(sectionName),
    name(name),
    description(description),
    argsInfo(argsInfo),
    maxReturnSize(maxReturnSize),
    func(func)
{
    funcs[name] = this;
    if (sections.indexOf(sectionName) == -1) {
        sections << sectionName;
    }
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
