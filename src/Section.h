#pragma once

#include "Function.h"
#include "EditMode.h"

#include <QString>
#include <QList>

class SectionMaster;

class Section {
    public:
        ~Section();

        template <typename... Args>
        const Function* makeFunction(Args&&... args) {
            auto* func = new Function(std::forward<Args>(args)...);
            func->parent = this;
            functions << func;
            return func;
        }

        template <typename... Args>
        const EditMode* makeEditMode(Args&&... args) {
            auto* mode = new EditMode(std::forward<Args>(args)...);
            modes << mode;
            return mode;
        }

        const QString& getName() const;
        const QList<Function*>& getFunctions() const;
        const QList<EditMode*>& getModes() const;

        Function* get(const QString& name) const;

    private:
        Section(const QString& name);

        QString name;
        QList<Function*> functions;
        QList<EditMode*> modes;

    friend SectionMaster;
};
