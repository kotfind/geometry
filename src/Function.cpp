#include "Function.h"

#include "Object.h"

#include <exception>

QHash<QString, Function*> Function::funcs;

Function::Function(
    const QString& name,
    const QList<int>& typeHints,
    const std::function<QList<Object*>(const QList<Object*>&)>& func
) : typeHints(typeHints),
    func(func)
{
    funcs[name] = this;
}

QList<Object*> Function::operator()(const QList<Object*>& objs) {
    if (objs.size() != typeHints.size())
        throw std::invalid_argument("Invalid number of objects");

    for (int i = 0; i < objs.size(); ++i) {
        if (objs[i]->getType() != typeHints[i]) {
            throw std::invalid_argument("Invalid type of object");
        }
    }

    return func(objs);
}
