//
//

#ifndef PIXEL_ATTRIBUTE_H
#define PIXEL_ATTRIBUTE_H

#include <unordered_map>
#include <string>

namespace pixel::graphics
{

struct Attribute
{
    unsigned int index;
    int size;
    int location;
    unsigned int type;
    std::string name;

    std::string debugPrint() const;
};

using AttributeMap = std::unordered_map<std::string, Attribute>;

};

#endif //PIXEL_ATTRIBUTE_H
