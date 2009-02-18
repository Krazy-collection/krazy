project(cppchecks)
cmake_minimum_required(VERSION 2.6)

include(CheckCXXCompilerFlag)

find_package(Qt4 REQUIRED)

set(CMAKE_INSTALL_RPATH ${CMAKE_PREFIX_PATH}/lib)
check_cxx_compiler_flag(-fvisibility=hidden __KRAZY_HAVE_GCC_VISIBILITY)
set( __KRAZY_HAVE_GCC_VISIBILITY ${__KDE_HAVE_GCC_VISIBILITY} CACHE BOOL "GCC support for hidden visibility")

configure_file(krazymacros.h.cmake ${CMAKE_CURRENT_BINARY_DIR}/krazymacros.h)

add_definitions(-DCPLUSPLUS_WITH_NAMESPACE=1)

include_directories(
    ${CMAKE_CURRENT_BINARY_DIR}
    ${QT_INCLUDES}
    cplusplus/lib
    cplusplus/parser
    cplusplus/preprocessor
    checkutil
)

# This is the cplusplus parser from QtCreator which can be found at
add_subdirectory(cplusplus)
add_subdirectory(checkutil)
add_subdirectory(dpointercheck)
