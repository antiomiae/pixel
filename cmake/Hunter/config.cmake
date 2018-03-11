include(hunter_config)

hunter_config(Boost
        VERSION ${HUNTER_Boost_VERSION}
        CMAKE_ARGS BUILD_SHARED_LIBS=OFF
        )

hunter_config(glfw
        VERSION ${HUNTER_glfw_VERSION}
        CMAKE_ARGS BUILD_SHARED_LIBS=OFF
        )

if(APPLE)
    hunter_config(Lua
            VERSION ${HUNTER_Lua_VERSION}
            CMAKE_ARGS -DCMAKE_VERBOSE_MAKEFILE:BOOL=ON -DLUA_USE_MACOSX:BOOL=OFF -DDLUA_USE_DLOPEN:BOOL=ON -DLUA_USE_LINUX:BOOL=ON
            )

endif(APPLE)
