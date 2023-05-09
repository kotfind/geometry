#include "Calculator.h"

#include <stdexcept>

void Calculator::setPos(const QPointF& pos) {
    throw std::logic_error("This type of Calculator cannot be moved");
}
