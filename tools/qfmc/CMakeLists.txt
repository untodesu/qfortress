add_executable(qfmc
    "${CMAKE_CURRENT_LIST_DIR}/main.cc"
    "${CMAKE_CURRENT_LIST_DIR}/mapfile.cc"
    "${CMAKE_CURRENT_LIST_DIR}/mapfile.hh"
    "${CMAKE_CURRENT_LIST_DIR}/pch.hh")
target_compile_features(qfmc PUBLIC cxx_std_20)
target_include_directories(qfmc PUBLIC "${PROJECT_SOURCE_DIR}")
target_include_directories(qfmc PRIVATE "${PROJECT_SOURCE_DIR}/tools")
target_precompile_headers(qfmc PUBLIC "${CMAKE_CURRENT_LIST_DIR}/pch.hh")
target_link_libraries(qfmc PUBLIC qf_core qf_shared)
