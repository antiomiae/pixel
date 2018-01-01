//
//

#ifndef MAIN_SYMBOL_MAP_CPP_H
#define MAIN_SYMBOL_MAP_CPP_H

#include <vector>
#include <string>
#include <unordered_map>
#include <cstdint>

namespace pixel::util {

using SymbolMap = std::unordered_map<uint64_t, std::vector<std::string> >;

const SymbolMap& symbolMap();

};


#endif //MAIN_SYMBOL_MAP_CPP_H
