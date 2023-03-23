#include "Function.h"

#include "Object.h"

#include <exception>

QHash<QString, Function*> Function::funcs;

Function::Function(
    const QString& fullName,
    const QString& description,
    const QList<ArgumentInfo>& argsInfo,
    int maxReturnSize,
    const std::function<QList<Object*>(const QList<const Object*>&)>& func
) : fullName(fullName),
    description(description),
    argsInfo(argsInfo),
    maxReturnSize(maxReturnSize),
    func(func)
{
    funcs[fullName] = this;

    selfName = fullName.section('/', -1);
    auto path = fullName.section('/', 0, -2);
    (section = Section::get(path))->addFunction(this);
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
