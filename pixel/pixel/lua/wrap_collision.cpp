#include <pixel/collision/collision.h>
#include "binding.h"

namespace pixel::binding
{

using namespace std;
using namespace pixel::collision;

void bind_collision_map(sol::state& lua, sol::table& binding, const string& type_name)
{
    auto collison_map = binding.new_usertype<CollisionMap>(
        type_name,
        "new", sol::constructors<CollisionMap(uint, uint)>(),
        "set", &CollisionMap::set,
        "collide_row", &CollisionMap::collide_row,
        "collide_column", &CollisionMap::collide_column
    );
}

}
