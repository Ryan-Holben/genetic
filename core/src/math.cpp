#include <core/math.h>

#include <core/platform.h>

namespace core {

// Square L2 norm between two equal-length vectors
Number L2_SQUARED(const Tuple& x, const Tuple& y) {
    ASSERT_WITH_MSG(x.size() == y.size(),
                    "Size mismatch: x.size() = " << x.size() << ", y.size() = " << y.size());

    Number ret = 0.0;
    for (size_t i = 0; i < x.size(); i++) {
        const Number diff = x[i] - y[i];
        ret += diff * diff;
    }

    return ret;
}

} // namespace core