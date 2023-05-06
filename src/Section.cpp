#include "Section.h"

Section* Section::master = nullptr;

Section::Section(const QString& name) : name(name) {
}

void Section::addSection(Section* s) {
    sections << s;
} 

void Section::addMode(EditMode* m) {
    modes << m;
}

void Section::addFunction(Function* f) {
    functions << f;
}

Section* Section::getMaster() {
    if (!master) {
        master = new Section("");
    }
    return master;
}

Section* Section::get(const QString& name) {
    auto path = name.split('/');
    auto* sec = getMaster();
    for (int i = 0; i < path.size(); ++i) {
        const auto& ss = sec->getSections();
        int j = 0;
        while (j < ss.size() && ss[j]->getName() != path[i]) {
            ++j;
        }
        if (j == ss.size()) {
            sec->addSection(new Section(path[i]));
            j = ss.size() - 1;
        }
        sec = ss[j];
    }
    return sec;
}
