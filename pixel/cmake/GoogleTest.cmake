include(ExternalProject)

include(ExternalProject)
ExternalProject_Add(googletest
        GIT_REPOSITORY    https://github.com/google/googletest.git
        GIT_TAG           master
        SOURCE_SUBDIR     .
        SOURCE_DIR        ${PROJECT_BINARY_DIR}/googletest-src
        #BUILD "${CMAKE_COMMAND} -G ${CMAKE_GENERATOR} ."
        )
