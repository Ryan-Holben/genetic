#pragma once

#include <vector>

namespace core {

using Number = float;
using Tuple = std::vector<Number>;
using Datum = std::pair<Tuple, Tuple>;
using Dataset = std::vector<Datum>;

} // namespace core