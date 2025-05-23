add_library(qf_core STATIC
    "${CMAKE_CURRENT_LIST_DIR}/cmdline.cc"
    "${CMAKE_CURRENT_LIST_DIR}/cmdline.hh"
    "${CMAKE_CURRENT_LIST_DIR}/config.cc"
    "${CMAKE_CURRENT_LIST_DIR}/config.hh"
    "${CMAKE_CURRENT_LIST_DIR}/const.hh"
    "${CMAKE_CURRENT_LIST_DIR}/crc64.cc"
    "${CMAKE_CURRENT_LIST_DIR}/crc64.hh"
    "${CMAKE_CURRENT_LIST_DIR}/cvar.cc"
    "${CMAKE_CURRENT_LIST_DIR}/cvar.hh"
    "${CMAKE_CURRENT_LIST_DIR}/epoch.cc"
    "${CMAKE_CURRENT_LIST_DIR}/epoch.hh"
    "${CMAKE_CURRENT_LIST_DIR}/pch.hh"
    "${CMAKE_CURRENT_LIST_DIR}/setup.cc"
    "${CMAKE_CURRENT_LIST_DIR}/setup.hh"
    "${CMAKE_CURRENT_LIST_DIR}/strtools.cc"
    "${CMAKE_CURRENT_LIST_DIR}/strtools.hh"
    "${CMAKE_CURRENT_LIST_DIR}/version.hh")
target_compile_features(qf_core PUBLIC cxx_std_20)
target_include_directories(qf_core PUBLIC "${PROJECT_SOURCE_DIR}")
target_precompile_headers(qf_core PUBLIC "${CMAKE_CURRENT_LIST_DIR}/pch.hh")
target_link_libraries(qf_core PUBLIC eigen parson spdlog)

if(WIN32)
    target_compile_definitions(qf_core PUBLIC _CRT_SECURE_NO_WARNINGS)
    target_compile_definitions(qf_core PUBLIC _USE_MATH_DEFINES)
    target_compile_definitions(qf_core PUBLIC NOMINMAX)
endif()

if(MSVC)
    target_compile_options(qf_core PUBLIC /utf-8)
endif()

configure_file("${CMAKE_CURRENT_LIST_DIR}/version.hh.in" "${CMAKE_CURRENT_LIST_DIR}/version.hh")
