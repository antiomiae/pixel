#include <pixel/collision/collision.h>
#include "binding.h"

namespace pixel::binding
{

using namespace std;
using namespace pixel::collision;

/*
 * CollisionMap() = default;
    CollisionMap(uint width, uint height);

    void set(int x, int y, bool solid);

    bool collide_row(uint row, uint start_col, uint end_col);
    bool collide_column(uint col, uint a, uint b);
 */

void bind_collision_map(sol::state& lua, sol::table& binding, const string& type_name)
{
    auto collison_map = binding.new_usertype<CollisionMap>(
        type_name,
        "new", sol::constructors<CollisionMap(), CollisionMap(uint, uint)>(),
        "set", &CollisionMap::set,
        "collide_row", &CollisionMap::collide_row,
        "collide_column", &CollisionMap::collide_column
    );
}

}
