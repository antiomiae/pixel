#include <pixel/math.h>
#include "binding.h"


namespace pixel::binding
{

using namespace std;

void bind_glm(sol::state& lua, sol::table& binding, const string& module_name)
{
    sol::table _glm = binding[module_name] = lua.create_table();

    _glm.new_usertype<glm::vec2>(
        "vec2",
        "new", sol::constructors<glm::vec2(), glm::vec2(float, float), glm::vec2(glm::ivec2)>(),
        "comp", [](glm::vec2& self, unsigned i) -> float { return self[i]; },
        "x", &glm::vec2::x,
        "y", &glm::vec2::y
    );

    _glm.new_usertype<glm::ivec2>(
        "ivec2",
        "new", sol::constructors<glm::ivec2(), glm::ivec2(int, int)>(),
        "comp", [](glm::ivec2& self, unsigned i) -> int { return self[i]; },
        "x", &glm::ivec2::x,
        "y", &glm::ivec2::y
    );

    _glm.new_usertype<glm::uvec2>(
        "uvec2",
        "new", sol::constructors<glm::uvec2(), glm::uvec2(unsigned, unsigned)>(),
        "comp", [](glm::uvec2& self, unsigned i) -> unsigned { return self[i]; },
        "__index", [](glm::uvec2& self, unsigned i) -> unsigned { return self[i]; },
        "x", &glm::uvec2::x,
        "y", &glm::uvec2::y
    );

    _glm.new_usertype<glm::vec3>(
        "vec3",
        "new", sol::constructors<glm::vec3(), glm::vec3(float, float, float)>(),
        "x", &glm::vec3::x,
        "y", &glm::vec3::y,
        "z", &glm::vec3::z
    );

    _glm.new_usertype<glm::ivec3>(
        "ivec3",
        "new", sol::constructors<glm::ivec3(), glm::ivec3(int, int, int)>(),
        "x", &glm::ivec3::x,
        "y", &glm::ivec3::y,
        "z", &glm::ivec3::z
    );

    _glm.new_usertype<glm::uvec3>(
        "uvec3",
        "new", sol::constructors<glm::uvec3(), glm::uvec3(unsigned, unsigned, unsigned)>(),
        "x", &glm::uvec3::x,
        "y", &glm::uvec3::y,
        "z", &glm::uvec3::z
    );

    _glm.new_usertype<glm::vec4>(
        "vec4",
        "new", sol::constructors<glm::vec4(void), glm::vec4(float, float, float, float)>(),
        "x", &glm::vec4::x,
        "y", &glm::vec4::y,
        "z", &glm::vec4::z,
        "w", &glm::vec4::w
    );

    _glm.new_usertype<glm::ivec4>(
        "ivec4",
        "new", sol::constructors<glm::ivec4(), glm::ivec4(int, int, int, int)>(),
        "x", &glm::ivec4::x,
        "y", &glm::ivec4::y,
        "z", &glm::ivec4::z,
        "w", &glm::ivec4::w
    );

    _glm.new_usertype<glm::uvec4>(
        "uvec4",
        "new", sol::constructors<glm::uvec4(), glm::uvec4(unsigned, unsigned, unsigned, unsigned)>(),
        "x", &glm::uvec4::x,
        "y", &glm::uvec4::y,
        "z", &glm::uvec4::z,
        "w", &glm::uvec4::w
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
