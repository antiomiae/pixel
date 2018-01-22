include(ExternalProject)

ExternalProject_Add(glew-external
    URL    "https://sourceforge.net/projects/glew/files/glew/2.1.0/glew-2.1.0.tgz/download"
    SOURCE_DIR ${PROJECT_BINARY_DIR}/glew-src
    SOURCE_SUBDIR build/cmake
    INSTALL_COMMAND ""    
)
