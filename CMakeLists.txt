project(renderingproject)
cmake_minimum_required(VERSION 2.6)
find_package(Qt4 REQUIRED)

include_directories(${QT_INCLUDES} ${CMAKE_CURRENT_BINARY_DIR})

include(${QT_USE_FILE})

set(renderingproject_SRCS main.cpp camera.cpp ray.cpp plane.cpp)
qt4_automoc(${renderingproject_SRCS})
add_executable(renderingproject ${renderingproject_SRCS})
target_link_libraries(renderingproject ${QT_LIBRARIES})

