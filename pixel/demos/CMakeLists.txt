add_executable(main main.cpp)
add_executable(sprite sprite.cpp)
add_executable(map map.cpp)
add_executable(lua lua.cpp)

target_link_libraries(main pixel)
target_link_libraries(sprite pixel)
target_link_libraries(map pixel)
target_link_libraries(lua pixel)
