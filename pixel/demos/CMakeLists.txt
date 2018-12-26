project(pixel_demos)

# generate executable target for every cpp file in this directory
file(GLOB demo_sources ${CMAKE_CURRENT_LIST_DIR}/*.cpp)

foreach(fname ${demo_sources})
  get_filename_component(target_name ${fname} NAME_WE)
  message("pixel: Generating demo ${target_name}")
  add_executable(${target_name} ${fname})
  target_link_libraries(${target_name} pixel)
endforeach(fname ${demo_sources})


add_subdirectory(golf)
