#include "getOrThrow.h"

#include "IOError.h"

#include <QJsonValue>

QJsonValue& getOrThrow(QJsonValue& t) {
    if (t.isUndefined())
        throw IOError("Couldn't parse");
    return t;
}

const QJsonValue& getOrThrow(const QJsonValue& t) {
    if (t.isUndefined())
        throw IOError("Couldn't parse");
    return t;
}
