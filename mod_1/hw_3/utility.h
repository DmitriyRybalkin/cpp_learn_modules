
#pragma once

#include <utility>

namespace utility {

int fact(int val) {
    if(val == 0)
        return 1;

    return val * fact(val - 1);
}

} // namespace utility