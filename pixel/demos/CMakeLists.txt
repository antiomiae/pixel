add_executable(main main.cpp)
add_executable(sprite sprite.cpp)

target_link_libraries(main pixel)
target_link_libraries(sprite pixel)
