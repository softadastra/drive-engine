cmake_minimum_required(VERSION 3.20)

if(CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
    project(drive_engine
        VERSION 0.1.0
        DESCRIPTION "Drive orchestration engine"
        LANGUAGES CXX
    )
endif()

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

# ========================
# Options
# ========================
option(DRIVE_ENGINE_BUILD_TESTS "Build drive-engine tests" OFF)
option(DRIVE_WARNINGS_AS_ERRORS "Treat warnings as errors" OFF)

# ========================
# Dependencies
# ========================
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/.vix/vix_deps.cmake")
    include("${CMAKE_CURRENT_SOURCE_DIR}/.vix/vix_deps.cmake")
endif()

# ========================
# Sources
# ========================
file(GLOB_RECURSE DRIVE_ENGINE_SOURCES
    CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
)

# ========================
# Library
# ========================
add_library(drive_engine STATIC ${DRIVE_ENGINE_SOURCES})
add_library(drive::engine ALIAS drive_engine)

target_compile_features(drive_engine PUBLIC cxx_std_20)

target_include_directories(drive_engine
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

# ========================
# Product links
# ========================
target_link_libraries(drive_engine
    PUBLIC
        drive::watcher
        drive::adapter
)

# ========================
# Compiler options
# ========================
if(MSVC)
    target_compile_options(drive_engine PRIVATE /W4 /permissive-)
    if(DRIVE_WARNINGS_AS_ERRORS)
        target_compile_options(drive_engine PRIVATE /WX)
    endif()
else()
    target_compile_options(drive_engine PRIVATE -Wall -Wextra -Wpedantic)
    if(DRIVE_WARNINGS_AS_ERRORS)
        target_compile_options(drive_engine PRIVATE -Werror)
    endif()
endif()

# ========================
# Target properties
# ========================
set_target_properties(drive_engine PROPERTIES
    OUTPUT_NAME drive_engine
    VERSION ${PROJECT_VERSION}
    SOVERSION 0
    EXPORT_NAME engine
)

# ========================
# Install
# ========================
install(
    TARGETS drive_engine
    EXPORT driveEngineTargets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(
    DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/include/"
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(
    EXPORT driveEngineTargets
    FILE driveEngineTargets.cmake
    NAMESPACE drive::
    DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/drive_engine"
)

write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/drive_engineConfigVersion.cmake"
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
)

configure_package_config_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake/drive_engineConfig.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/drive_engineConfig.cmake"
    INSTALL_DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/drive_engine"
)

install(
    FILES
        "${CMAKE_CURRENT_BINARY_DIR}/drive_engineConfig.cmake"
        "${CMAKE_CURRENT_BINARY_DIR}/drive_engineConfigVersion.cmake"
    DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/drive_engine"
)

# ========================
# Tests
# ========================
if(DRIVE_ENGINE_BUILD_TESTS)
    include(CTest)
    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/tests/CMakeLists.txt")
        add_subdirectory(tests)
    endif()
endif()

# ========================
# Summary
# ========================
message(STATUS "------------------------------------------------------")
message(STATUS "drive::engine configured (${PROJECT_VERSION})")
message(STATUS "Include dir:      ${CMAKE_CURRENT_SOURCE_DIR}/include")
message(STATUS "Build tests:      ${DRIVE_ENGINE_BUILD_TESTS}")
message(STATUS "Warnings=error:   ${DRIVE_WARNINGS_AS_ERRORS}")
message(STATUS "Build type:       ${CMAKE_BUILD_TYPE}")
message(STATUS "Install prefix:   ${CMAKE_INSTALL_PREFIX}")
message(STATUS "------------------------------------------------------")
