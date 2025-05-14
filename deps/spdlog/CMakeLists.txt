add_library(spdlog STATIC
    "${CMAKE_CURRENT_LIST_DIR}/src/async.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/src/bundled_fmtlib_format.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/src/cfg.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/src/color_sinks.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/src/file_sinks.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/src/spdlog.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/src/stdout_sinks.cpp")
target_compile_features(spdlog PUBLIC cxx_std_20)
target_compile_definitions(spdlog PUBLIC SPDLOG_COMPILED_LIB)
target_compile_definitions(spdlog PUBLIC SPDLOG_USE_STD_FORMAT)
target_include_directories(spdlog PUBLIC "${CMAKE_CURRENT_LIST_DIR}/include")

find_package(Threads REQUIRED)
target_link_libraries(spdlog PUBLIC Threads::Threads)

if(MSVC)
    # There seems to be a compile-time check merged into the
    # master fmtlib branch that has been pulled into spdlog as well
    # that seems to fix an MSVC bug (https://github.com/fmtlib/fmt/pull/2297)
    # FIXME: should I bundle fmtlib as a separate dependency?
    target_compile_options(spdlog PUBLIC /utf-8)
endif()
