project(renderingproject)
cmake_minimum_required(VERSION 2.6)
find_package(Qt4 REQUIRED)

include_directories(${QT_INCLUDES} ${CMAKE_CURRENT_BINARY_DIR})

include(${QT_USE_FILE})

set(renderingproject_SRCS main.cpp camera.cpp plane.cpp sphere.cpp
  triangle.cpp intersectableinstance.cpp
  darkmatter.cpp diffusematerial.cpp phongmaterial.cpp participatingmaterial.cpp
  directionallight.cpp conelight.cpp intersectablelist.cpp binarymaterial.cpp
  pointlight.cpp objreader.cpp mirrormaterial.cpp
  axisalignedbox.cpp bsp.cpp integrator.cpp
  sampler.cpp jitteredsampler.cpp arealight.cpp
  hdrloader.cpp environmentmap.cpp simpleintegrator.cpp pathtracingintegrator.cpp unidipathtracingintegrator.cpp bidipathtracingintegrator.cpp
  quad.cpp csgobject.cpp csgintersection.cpp)
qt4_automoc(${renderingproject_SRCS})
add_executable(renderingproject ${renderingproject_SRCS})
#add_executable(stst stst.cpp sampler.cpp jitteredsampler.cpp)
target_link_libraries(renderingproject ${QT_LIBRARIES})
#target_link_libraries(stst ${QT_LIBRARIES})

set(CMAKE_CXX_FLAGS "-Wall -O2 -fopenmp -DNDEBUG")
#set(CMAKE_CXX_FLAGS "-Wall -pg")
#set(CMAKE_CXX_FLAGS "-Wall -g")
