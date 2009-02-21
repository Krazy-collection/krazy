project(cppchecks)
cmake_minimum_required(VERSION 2.6)

include(CheckCXXCompilerFlag)

set(QT_MIN_VERSION "4.5.0")
find_package(Qt4 REQUIRED)

set(CMAKE_INSTALL_RPATH ${CMAKE_PREFIX_PATH}/lib)
check_cxx_compiler_flag(-fvisibility=hidden __KRAZY_HAVE_GCC_VISIBILITY)
set( __KRAZY_HAVE_GCC_VISIBILITY ${__KDE_HAVE_GCC_VISIBILITY} CACHE BOOL "GCC support for hidden visibility")

set(CPP_PREPROCESSOR_SOURCE_DIR ${CMAKE_SOURCE_DIR}/cplusplus/preprocessor)
set(CPP_PARSER_SOURCE_DIR ${CMAKE_SOURCE_DIR}/cplusplus/parser)
set(CPP_TOOLS_SOURCE_DIR ${CMAKE_SOURCE_DIR}/cplusplus/tools)

configure_file(krazymacros.h.cmake ${CMAKE_CURRENT_BINARY_DIR}/krazymacros.h)

add_definitions(-DCPLUSPLUS_WITH_NAMESPACE=1)

# This is the cplusplus parser from QtCreator which can be found at
add_subdirectory(cplusplus)
add_subdirectory(checkutil)
add_subdirectory(checks)
add_subdirectory(gui)
#add_subdirectory(test)
