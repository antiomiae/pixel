#ifndef MAIN_COLLECTIONS_H
#define MAIN_COLLECTIONS_H

#include <vector>
#include <map>
#include <algorithm>

namespace pixel::collections {

template <typename M>
std::vector<typename M::mapped_type> entries(const M &col) {
    std::vector<typename M::mapped_type> out(col.size());

    std::transform(std::begin(col), std::end(col), std::begin(out), [](auto pair) -> typename M::mapped_type { return pair.second; });

    return out;
};

}

#endif //MAIN_COLLECTIONS_H
