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
            auto* func = new Function(this, std::forward<Args>(args)...);
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
        const QList<const Function*>& getFunctions() const;
        const QList<const EditMode*>& getModes() const;

        const Function* get(const QString& name) const;

    private:
        Section(const QString& name);

        const QString name;
        QList<const Function*> functions;
        QList<const EditMode*> modes;

    friend SectionMaster;
};
