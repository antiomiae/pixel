#include <pixel/graphics/common.h>
#include "binding.h"


namespace pixel::binding
{

using namespace std;

void bind_opengl(sol::state& lua, sol::table& binding, const string& module_name)
{
    sol::table gl = binding[module_name] = lua.create_table();

    gl.set_function("glClear", &glClear);

    gl["GL_COLOR_BUFFER_BIT"] = GL_COLOR_BUFFER_BIT;
    gl["GL_DEPTH_BUFFER_BIT"] = GL_DEPTH_BUFFER_BIT;
}


}

