include(hunter_config)

hunter_config(glfw
  VERSION ${HUNTER_glfw_VERSION}
  CMAKE_ARGS BUILD_SHARED_LIBS=OFF
)
