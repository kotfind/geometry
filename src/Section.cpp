#include "Section.h"

#include <stdexcept>

Section::Section(const QString& name)
  : name(name)
{}

Section::~Section() {
    for (auto* func : functions) {
        delete func;
    }
}

const QString& Section::getName() const {
    return name;
}

const QList<Function*>& Section::getFunctions() const {
    return functions;
}

const QList<EditMode*>& Section::getModes() const {
    return modes;
}

Function* Section::get(const QString& name) const {
    for (auto* func : functions) {
        if (func->getSelfName() == name) {
            return func;
        }
    }
    throw std::logic_error("This function does not exist.");
}
