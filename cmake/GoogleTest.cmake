include(ExternalProject)

include(ExternalProject)
ExternalProject_Add(googletest
        GIT_REPOSITORY    https://github.com/google/googletest.git
        GIT_TAG           master
        SOURCE_DIR        ${PROJECT_BINARY_DIR}/googletest-src
        CONFIGURE_COMMAND ""
        BUILD_COMMAND     ""
        INSTALL_COMMAND   ""
        TEST_COMMAND      ""
        #SOURCE_SUBDIR     .
        #BUILD "${CMAKE_COMMAND} -G ${CMAKE_GENERATOR} ."
        )
