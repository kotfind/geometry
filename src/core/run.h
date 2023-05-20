#pragma once

#include "AbstractGeometry.h"

#include <memory>

int run(int argc, char** argv, std::unique_ptr<AbstractGeometry> geom);
