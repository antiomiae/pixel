//
//

#include <pixel/util/symbol_map.h>
#include "shader.h"
#include "attribute.h"
#include <sstream>

using namespace std;
using namespace pixel::graphics;


std::string Attribute::debugPrint() const
{
    const pixel::util::SymbolMap& symbols = pixel::util::symbolMap();

    stringstream out;

    out << "Attribute(" << endl;
    out << "  name = " << this->name << endl;
    out << "  location = " << this->location << endl;
    out << "  index = " << this->index << endl;
    out << "  size = " << this->size << endl;
    out << "  type = " << symbols.at(this->type)[0] << endl;
    out << ")" << endl;

    return out.str();
}
