#include "Function.h"

#include "Object.h"

#include <exception>

QHash<QString, Function*> Function::funcs;

Function::Function(
    const QString& name,
    const QList<int>& typeHints,
    int maxReturnSize,
    const std::function<QList<Object*>(const QList<const Object*>&)>& func
) : typeHints(typeHints),
    maxReturnSize(maxReturnSize),
    func(func)
{
    funcs[name] = this;
}

QList<Object*> Function::operator()(const QList<const Object*>& objs) const {
    if (objs.size() != countArgs())
        throw std::invalid_argument("Invalid number of objects");

    for (int i = 0; i < objs.size(); ++i) {
        if (objs[i]->getType() != typeHints[i]) {
            throw std::invalid_argument("Invalid type of object");
        }
    }

    return func(objs);
}

Function* Function::get(const QString& name) {
    return funcs[name];
}
