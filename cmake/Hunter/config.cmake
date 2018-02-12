include(hunter_config)

hunter_config(Boost
        VERSION ${HUNTER_Boost_VERSION}
        CMAKE_ARGS BUILD_SHARED_LIBS=OFF
        )

hunter_config(glfw
        VERSION ${HUNTER_glfw_VERSION}
        CMAKE_ARGS BUILD_SHARED_LIBS=OFF
        )
