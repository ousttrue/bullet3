set(TARGET_NAME RenderingExamples)
add_library(
  ${TARGET_NAME}
  CoordinateSystemDemo.cpp
  CoordinateSystemDemo.h
  RaytracerSetup.cpp
  RaytracerSetup.h
  RenderInstancingDemo.cpp
  RenderInstancingDemo.h
  TimeSeriesExample.cpp
  TinyRendererSetup.cpp
  TimeSeriesCanvas.cpp
  TimeSeriesCanvas.h
  TimeSeriesFontData.cpp
  TimeSeriesFontData.h
  DynamicTexturedCubeDemo.cpp
  DynamicTexturedCubeDemo.h
  TinyVRGui.cpp
  TinyVRGui.h)
target_link_libraries(${TARGET_NAME} PRIVATE BulletDynamics Bullet3Common
                                             CommonInterfaces TinyRenderer)
