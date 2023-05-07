#include "SectionMaster.h"

#include "Section.h"

#include <stdexcept>
#include <cassert>

SectionMaster::SectionMaster()
{}

SectionMaster::~SectionMaster() {
    for (auto* sec : sections) {
        delete sec;
    }
}

const QList<const Section*>& SectionMaster::getSections() const {
    return sections;
}

const Section* SectionMaster::getSection(const QString& name) const {
    for (auto* sec : sections) {
        if (sec->getName() == name) {
            return sec;
        }
    }
    throw std::logic_error("This section does not exist.");
}

const Function* SectionMaster::get(const QString& fullName) const {
    auto parts = fullName.split('/');
    assert(parts.size() == 2);

    return getSection(parts[0])->get(parts[1]);
}
