add_library(qf_shared STATIC
    "${CMAKE_CURRENT_LIST_DIR}/class.cc"
    "${CMAKE_CURRENT_LIST_DIR}/class.hh"
    "${CMAKE_CURRENT_LIST_DIR}/effect.cc"
    "${CMAKE_CURRENT_LIST_DIR}/effect.hh"
    "${CMAKE_CURRENT_LIST_DIR}/game.cc"
    "${CMAKE_CURRENT_LIST_DIR}/game.hh"
    "${CMAKE_CURRENT_LIST_DIR}/game_info.cc"
    "${CMAKE_CURRENT_LIST_DIR}/game_info.hh"
    "${CMAKE_CURRENT_LIST_DIR}/globals.cc"
    "${CMAKE_CURRENT_LIST_DIR}/globals.hh"
    "${CMAKE_CURRENT_LIST_DIR}/health.cc"
    "${CMAKE_CURRENT_LIST_DIR}/health.hh"
    "${CMAKE_CURRENT_LIST_DIR}/pch.hh"
    "${CMAKE_CURRENT_LIST_DIR}/team.cc"
    "${CMAKE_CURRENT_LIST_DIR}/team.hh")
target_compile_features(qf_shared PUBLIC cxx_std_20)
target_include_directories(qf_shared PUBLIC "${PROJECT_SOURCE_DIR}")
target_include_directories(qf_shared PRIVATE "${PROJECT_SOURCE_DIR}/game")
target_precompile_headers(qf_shared PUBLIC "${CMAKE_CURRENT_LIST_DIR}/pch.hh")
target_link_libraries(qf_shared PUBLIC qf_core)
target_link_libraries(qf_shared PUBLIC entt)
