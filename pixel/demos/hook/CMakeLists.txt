project(hook)

list(APPEND hook_sources
    hook.cpp
    src/game.h src/game.cpp
    src/stage.h
    src/hook_stage.cpp src/hook_stage.h
    src/triangle_batch.cpp src/triangle_batch.h
    )

add_executable(hook ${hook_sources})

target_link_libraries(hook pixel)
