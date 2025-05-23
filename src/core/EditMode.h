#pragma once

#include <QString>
#include <QIcon>
#include <QHash>

class Section;

class EditMode {
    public:
        enum class Type {
            MOVE,
            CREATE_POINT,
            REMOVE,
            FUNCTION,
            HIDE,
            count
        };

        EditMode(
            Type type,
            const QString& name,
            const QIcon& icon,
            const QString& description
        );

        const QString& getName() const;
        const QIcon& getIcon() const;
        const QString& getDescription() const;
        Type getType() const;

        static EditMode* get(Type);

        static void init();
        static void cleanup();

    private:
        const QString name;
        const QIcon icon;
        const QString description;
        const Type type;

        static EditMode* modes[static_cast<int>(Type::count)];

    friend Section;
};
