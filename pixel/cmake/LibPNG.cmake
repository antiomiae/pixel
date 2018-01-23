include(ExternalProject)

ExternalProject_Add(png-external
    GIT_REPOSITORY https://github.com/glennrp/libpng.git
    GIT_TAG v1.6.34
    GIT_SHALLOW 1
    PREFIX ""
    SOURCE_DIR ${PROJECT_BINARY_DIR}/libpng-src
    SOURCE_SUBDIR .
    INSTALL_COMMAND ""
)

ExternalProject_Get_Property(png-external SOURCE_DIR)
ExternalProject_Get_Property(png-external BINARY_DIR)

ExternalProject_Add_StepTargets(png-external build)

ExternalProject_Add_Step(png-external copy_pnglibconf
  COMMAND "${CMAKE_COMMAND}" -E copy_if_different "${BINARY_DIR}/pnglibconf.h" "${SOURCE_DIR}/pnglibconf.h"
  DEPENDS  png-external-build
  )

set(EXTERNAL_PNG_INCLUDE_DIR ${SOURCE_DIR})

set(PNG_STATIC OFF CACHE BOOL "" FORCE)
set(PNG_SHARED ON CACHE BOOL "" FORCE)
set(PNG_FRAMEWORK OFF CACHE BOOL "" FORCE)
