## Copyright 2022 Intel Corporation
## SPDX-License-Identifier: Apache-2.0

## Core device target ##

project_add_library(SHARED)

project_sources(PRIVATE
  OSPRayDevice.cpp
  OSPRayGlobalState.cpp
  OSPRayLibrary.cpp
  Object.cpp
  array/Array.cpp
  array/Array1D.cpp
  array/Array2D.cpp
  array/Array3D.cpp
  array/ObjectArray.cpp
  camera/Camera.cpp
  camera/Orthographic.cpp
  camera/Perspective.cpp
  frame/Frame.cpp
  renderer/AO.cpp
  renderer/Debug.cpp
  renderer/Pathtracer.cpp
  renderer/Renderer.cpp
  renderer/SciVis.cpp
  scene/Group.cpp
  scene/Instance.cpp
  scene/World.cpp
  geometry/Cone.cpp
  geometry/Curve.cpp
  geometry/Cylinder.cpp
  geometry/Geometry.cpp
  geometry/Isosurface.cpp
  geometry/Quad.cpp
  geometry/Sphere.cpp
  geometry/Triangle.cpp
  light/Directional.cpp
  light/HDRI.cpp
  light/Light.cpp
  light/Point.cpp
  light/Quad.cpp
  light/Spot.cpp
  material/Material.cpp
  material/Matte.cpp
  material/PBM.cpp
  sampler/Image1D.cpp
  sampler/Image2D.cpp
  sampler/Sampler.cpp
  spatial_field/AMRField.cpp
  spatial_field/SpatialField.cpp
  spatial_field/StructuredRegularField.cpp
  spatial_field/UnstructuredField.cpp
  surface/Surface.cpp
  volume/TransferFunction1D.cpp
  volume/Volume.cpp
)

file(GLOB OSPRAY_DEVICE_JSONS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}/code_gen "*.json")

anari_generate_queries(
  DEVICE_TARGET ${PROJECT_NAME}
  CPP_NAMESPACE anari_ospray
  JSON_ROOT_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/code_gen
  JSON_DEFINITIONS_FILE ${CMAKE_CURRENT_SOURCE_DIR}/code_gen/ospray_device.json
  JSON_EXTENSION_FILES ${OSPRAY_DEVICE_JSONS}
)

include(GenerateExportHeader)
generate_export_header(${PROJECT_NAME}
  EXPORT_MACRO_NAME "ANARI_OSPRAY_DEVICE_INTERFACE"
)

project_include_directories(
PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
)

project_link_libraries(PUBLIC anari::helium ospray::ospray)

if(WIN32)
  project_compile_definitions(PRIVATE _USE_MATH_DEFINES)
endif()

## Installation ##

install(TARGETS ${PROJECT_NAME}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

install(
FILES
  ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}_export.h
  ${CMAKE_CURRENT_LIST_DIR}/include/anari/ext/ospray/anariNewOSPRayDevice.h
DESTINATION
  ${CMAKE_INSTALL_INCLUDEDIR}/anari/ext/ospray
)
