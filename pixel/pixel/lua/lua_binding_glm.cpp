#include <pixel/pixel.h>
#include "lua_binding.h"


namespace pixel
{

using namespace std;
using namespace pixel::graphics;


void bind_glm(sol::state& lua, sol::table& binding, const string& module_name)
{
    sol::table _glm = binding[module_name] = lua.create_table();

    _glm.new_usertype<glm::vec2>(
        "vec2",
        "new", sol::constructors<glm::vec2(), glm::vec2(float, float), glm::vec2(glm::ivec2)>(),
        "comp", [](glm::vec2& self, unsigned i) -> float { return self[i]; }
    );

    _glm.new_usertype<glm::ivec2>(
        "ivec2",
        "new", sol::constructors<glm::ivec2(), glm::ivec2(int, int)>(),
        "comp", [](glm::ivec2& self, unsigned i) -> int { return self[i]; }
    );

    _glm.new_usertype<glm::uvec2>(
        "uvec2",
        "new", sol::constructors<glm::uvec2(), glm::uvec2(unsigned, unsigned)>(),
        "comp", [](glm::uvec2& self, unsigned i) -> unsigned { return self[i]; },
        "__index", [](glm::uvec2& self, unsigned i) -> unsigned { return self[i]; }

    );

    _glm.new_usertype<glm::vec3>(
        "vec3",
        "new", sol::constructors<glm::vec3(), glm::vec3(float, float, float)>()
    );

    _glm.new_usertype<glm::ivec3>(
        "ivec3",
        "new", sol::constructors<glm::ivec3(), glm::ivec3(int, int, int)>()
    );

    _glm.new_usertype<glm::uvec3>(
        "uvec3",
        "new", sol::constructors<glm::uvec3(), glm::uvec3(unsigned, unsigned, unsigned)>()
    );

    _glm.new_usertype<glm::vec4>(
        "vec4",
        "new", sol::constructors<glm::vec4(void), glm::vec4(float, float, float, float)>()
    );

    _glm.new_usertype<glm::ivec4>(
        "ivec4",
        "new", sol::constructors<glm::ivec4(), glm::ivec4(int, int, int, int)>()
    );

    _glm.new_usertype<glm::uvec4>(
        "uvec4",
        "new", sol::constructors<glm::uvec4(), glm::uvec4(unsigned, unsigned, unsigned, unsigned)>()
    );

    _glm.new_usertype<glm::mat2>(
        "mat2",
        "new", sol::constructors<glm::mat2(), glm::mat2(
            float, float,
            float, float
        )>()
    );

    _glm.new_usertype<glm::mat3>(
        "mat3",
        "new", sol::constructors<glm::mat3(), glm::mat3(
            float, float, float,
            float, float, float,
            float, float, float
        )>()
    );

    _glm.new_usertype<glm::mat4>(
        "mat4",
        "new", sol::constructors<glm::mat4(), glm::mat4(
            float, float, float, float,
            float, float, float, float,
            float, float, float, float,
            float, float, float, float
        )>()
    );
}

}
