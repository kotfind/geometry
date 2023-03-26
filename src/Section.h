#pragma once

#include <QList>
#include <QString>

class Function;
enum class EditMode;

class Section {
    public:
        Section(const QString& name);

        void addSection(Section*);
        void addMode(EditMode);
        void addFunction(Function*);

        const QString& getName() const { return name; }
        const QList<Section*>& getSections() const { return sections; }
        const QList<EditMode>& getModes() const { return modes; }
        const QList<Function*>& getFunctions() const { return functions; }

        static Section* getMaster();
        static Section* get(const QString& name);

    private:
        static Section* master;

        QString name;
        QList<Section*> sections;
        QList<EditMode> modes;
        QList<Function*> functions;
};
