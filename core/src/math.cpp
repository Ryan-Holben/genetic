#include <core/math.h>

#include <assert.h>

namespace core {

// Square L2 norm between two equal-length vectors
Number L2_SQUARED(const Tuple& x, const Tuple& y) {
    assert(x.size() == y.size());

    Number ret = 0.0;
    for (size_t i = 0; i < x.size(); i++) {
        const Number diff = x[i] - y[i];
        ret += diff * diff;
    }

    return ret;
}

} // namespace core