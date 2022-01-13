set(TARGET_NAME Bullet3Common)
add_library(
  ${TARGET_NAME}
  b3AlignedAllocator.cpp
  b3Vector3.cpp
  b3Logging.cpp
  b3AlignedAllocator.h
  b3AlignedObjectArray.h
  b3CommandLineArgs.h
  b3HashMap.h
  b3Logging.h
  b3Matrix3x3.h
  b3MinMax.h
  b3PoolAllocator.h
  b3QuadWord.h
  b3Quaternion.h
  b3Random.h
  b3Scalar.h
  b3StackAlloc.h
  b3Transform.h
  b3TransformUtil.h
  b3Vector3.h
  shared/b3Float4.h
  shared/b3Int2.h
  shared/b3Int4.h
  shared/b3Mat3x3.h
  shared/b3PlatformDefinitions.h
  shared/b3Quat.h)
target_include_directories(${TARGET_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/..)
target_compile_options(${TARGET_NAME} PRIVATE /wd4267)
set_target_properties(${TARGET_NAME} PROPERTIES VERSION ${BULLET_VERSION})
set_target_properties(${TARGET_NAME} PROPERTIES SOVERSION ${BULLET_VERSION})
