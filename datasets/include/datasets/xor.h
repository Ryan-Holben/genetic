#pragma once

#include <core/core.h>

core::Dataset GetXorDataset() {
    core::Dataset d{{{0, 0}, {0}}, {{1, 0}, {1}}, {{0, 1}, {1}}, {{1, 1}, {1}}};

    return d;
}
