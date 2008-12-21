project(cppchecks)
cmake_minimum_required(VERSION 2.6)
find_package(Qt4 REQUIRED)
find_package(KDE4 REQUIRED)
find_package(KDevPlatform REQUIRED)

set(CMAKE_INSTALL_RPATH ${CMAKE_PREFIX_PATH}/lib)

include_directories(
    ${CMAKE_CURRENT_BINARY_DIR}
    ${QT_INCLUDES}
    ${KDE4_INCLUDE_DIR}
    ${KDE4_INCLUDE_DIR}/KDE
    ${KDevPlatform_INCLUDE_DIR}
    include
    shared
)

add_subdirectory(shared)
add_subdirectory(dpointercheck)
