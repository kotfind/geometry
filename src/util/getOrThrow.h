#pragma once

class QJsonValue;

QJsonValue& getOrThrow(QJsonValue&);

const QJsonValue& getOrThrow(const QJsonValue&);
