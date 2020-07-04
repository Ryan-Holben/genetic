#pragma once

#include <core/random.h>
#include <core/types.h>

namespace evo {

// clang-format off
namespace constants {

    namespace flags {
        constexpr bool PARENTS_DIE_EACH_GENERATION = false;
    }

    namespace population {
        constexpr size_t MAX_NUM_AGENTS = 50;
        constexpr size_t MAX_NUM_CHILDREN = 8;
    }

    namespace mutation {
        namespace chance {
            static core::Dice RemoveNeuron(0.10);
            static core::Dice NewNeuron(0.30);
            static core::Dice NewLayer(0.10);
        }
    }

} // namespace constants
//clang-format on

} // namespace evo