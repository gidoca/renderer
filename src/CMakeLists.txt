cmake_minimum_required(VERSION 2.8.0)
find_package(Qt4 REQUIRED)

include_directories(${QT_INCLUDES} ${GSL_INCLUDE_DIR} ${OPENEXR_INCLUDE_PATHS} ${TCLAP_INCLUDEDIR} ${CMAKE_CURRENT_BINARY_DIR})

include(${QT_USE_FILE})

#set(CMAKE_AUTOMOC TRUE)

set(renderingsproject_HEADERS win.h)
set(renderingproject_SRCS main.cpp camera.cpp plane.cpp sphere.cpp
  triangle.cpp intersectableinstance.cpp
  darkmatter.cpp diffusematerial.cpp phongmaterial.cpp
  conelight.cpp intersectablelist.cpp binarymaterial.cpp
  pointlight.cpp objreader.cpp mirrormaterial.cpp
  axisalignedbox.cpp
  sampler.cpp jitteredsampler.cpp arealight.cpp
  simpleintegrator.cpp unidipathtracingintegrator.cpp bidipathtracingintegrator.cpp
  quad.cpp csgobject.cpp csgintersection.cpp bvh.cpp
  renderer.cpp perpixelrenderer.cpp metropolisrenderer.cpp
  tonemapper.cpp film.cpp
  win.cpp)
qt4_automoc(${renderingproject_SRCS})
add_executable(renderingproject ${renderingproject_SRCS})
#add_executable(stst stst.cpp sampler.cpp jitteredsampler.cpp)
target_link_libraries(renderingproject ${QT_LIBRARIES} ${GSL_LIBRARIES} ${OPENEXR_LIBRARIES})
#target_link_libraries(stst ${QT_LIBRARIES})

set(COMMON_CXX_FLAGS "-Wextra -Wall -pedantic -std=c++0x ")

set(CMAKE_CXX_FLAGS "${COMMON_CXX_FLAGS} -fopenmp -DNDEBUG -march=native")
#set(CMAKE_CXX_FLAGS "${COMMON_CXX_FLAGS} -pg")
#set(CMAKE_CXX_FLAGS "${COMMON_CXX_FLAGS} -g")
