#project(pixel_entrypoints)
#
#file(GLOB entrypoint_sources ${CMAKE_CURRENT_LIST_DIR}/*.cpp)
#
#foreach(fname ${entrypoint_sources})
#    get_filename_component(target_name ${fname} NAME_WE)
#    message("pixel: Generating entrypoint ${target_name}")
#    add_executable(${target_name} ${fname})
#    target_link_libraries(${target_name} pixel)
#endforeach(fname ${entrypoint_sources})
