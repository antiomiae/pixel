project(golf)

list(APPEND golf_sources
    golf.cpp
    src/golf.h src/golf.cpp
    )

add_executable(golf ${golf_sources})
target_link_libraries(golf pixel)
