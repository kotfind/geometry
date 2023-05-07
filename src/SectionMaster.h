#pragma once

#include <QList>

class Section;
class Function;

class SectionMaster {
    public:
        SectionMaster();
        ~SectionMaster();

        template <typename... Args>
        Section* makeSection(Args&&... args) {
            auto* sec = new Section(std::forward<Args>(args)...);
            sections << sec;
            return sec;
        }

        const QList<const Section*>& getSections() const;

        const Section* getSection(const QString& name) const;
        const Function* get(const QString& fullName) const;

    private:
        QList<const Section*> sections;
};
