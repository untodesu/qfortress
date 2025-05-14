add_executable(qf_client
    "${CMAKE_CURRENT_LIST_DIR}/main.cc"
    "${CMAKE_CURRENT_LIST_DIR}/pch.hh")
target_compile_features(qf_client PUBLIC cxx_std_20)
target_include_directories(qf_client PUBLIC "${PROJECT_SOURCE_DIR}")
target_include_directories(qf_client PRIVATE "${PROJECT_SOURCE_DIR}/game")
target_precompile_headers(qf_client PUBLIC "${CMAKE_CURRENT_LIST_DIR}/pch.hh")
target_link_libraries(qf_client PUBLIC qf_core qf_shared)
