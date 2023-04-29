#include "Calculator.h"

bool Calculator::isDependant() const {
    return !isFree();
}
