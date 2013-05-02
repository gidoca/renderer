cmake_minimum_required(VERSION 2.8.0)

include_directories(${Qt5Widgets_INCLUDE_DIRS} ${GSL_INCLUDE_DIR} ${OPENEXR_INCLUDE_PATHS} ${Boost_INCLUDE_DIRS} ${CMAKE_CURRENT_BINARY_DIR})

set(CMAKE_AUTOMOC TRUE)
add_definitions(${Qt5Widgets_DEFINITIONS} -DQT_NO_KEYWORDS)

set(renderingproject_HEADERS win.h renderer.h printfinished.h)
set(renderingproject_SRCS camera.cpp plane.cpp sphere.cpp intersectable.cpp
  sceneast.cpp sceneparser.cpp scenedumper.cpp
  triangle.cpp intersectableinstance.cpp
  darkmatter.cpp diffusematerial.cpp phongmaterial.cpp texturematerial.cpp transparentmaterial.cpp
  conelight.cpp intersectablelist.cpp
  pointlight.cpp objreader.cpp mirrormaterial.cpp
  axisalignedbox.cpp
  sampler.cpp jitteredsampler.cpp arealight.cpp
  simpleintegrator.cpp unidipathtracingintegrator.cpp bidipathtracingintegrator.cpp
  quad.cpp csgobject.cpp csgintersection.cpp bvh.cpp
  renderer.cpp perpixelrenderer.cpp metropolisrenderer.cpp metropolisfltrenderer.cpp
  tonemapper.cpp
  symmetricfilter.cpp
  win.cpp energyredistributionrenderer.cpp metropolissample.cpp
  mathhelper.cpp
  printfinished.cpp
  environmentmap.cpp
)

add_executable(renderingproject main.cpp ${renderingproject_SRCS} ${renderingproject_HEADERS_MOC})

target_link_libraries(renderingproject ${Qt5Widgets_LIBRARIES} ${GSL_LIBRARIES} ${OPENEXR_LIBRARIES} ${Boost_LIBRARIES} ${OpenCV_LIBS})

#-Wno-unitialized suppresses warnings from boost
set(COMMON_CXX_FLAGS "-Wextra -Wall -Wno-unused-parameter -Wno-uninitialized -std=c++0x ${Qt5Widgets_EXECUTABLE_COMPILE_FLAGS}")

set(CMAKE_CXX_FLAGS_RELEASE "${COMMON_CXX_FLAGS} -fopenmp -DNDEBUG -DUSE_OPENMP -march=native -O3 -fno-strict-aliasing ")
set(CMAKE_CXX_FLAGS_DEBUG "${COMMON_CXX_FLAGS} -g -Wno-unknown-pragmas")

