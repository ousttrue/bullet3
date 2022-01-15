set(TARGET_NAME LinearMath)
add_library(
  ${TARGET_NAME}
  btAlignedAllocator.cpp
  btConvexHull.cpp
  btConvexHullComputer.cpp
  btGeometryUtil.cpp
  btPolarDecomposition.cpp
  btQuickprof.cpp
  btReducedVector.cpp
  btSerializer.cpp
  btSerializer64.cpp
  btThreads.cpp
  btVector3.cpp
  TaskScheduler/btTaskScheduler.cpp
  TaskScheduler/btThreadSupportPosix.cpp
  TaskScheduler/btThreadSupportWin32.cpp
  btAabbUtil2.h
  btAlignedAllocator.h
  btAlignedObjectArray.h
  btConvexHull.h
  btConvexHullComputer.h
  btDefaultMotionState.h
  btGeometryUtil.h
  btGrahamScan2dConvexHull.h
  btHashMap.h
  btIDebugDraw.h
  btList.h
  btMatrix3x3.h
  btImplicitQRSVD.h
  btMinMax.h
  btModifiedGramSchmidt.h
  btMotionState.h
  btPolarDecomposition.h
  btPoolAllocator.h
  btQuadWord.h
  btQuaternion.h
  btQuickprof.h
  btReducedVector.h
  btRandom.h
  btScalar.h
  btSerializer.h
  btStackAlloc.h
  btThreads.h
  btTransform.h
  btTransformUtil.h
  btVector3.h
  TaskScheduler/btThreadSupportInterface.h)
target_compile_options(${TARGET_NAME} PRIVATE /wd4267)
target_compile_definitions(${TARGET_NAME} PUBLIC BT_NO_PROFILE)
target_include_directories(${TARGET_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/..)
set_target_properties(${TARGET_NAME} PROPERTIES VERSION ${BULLET_VERSION})
set_target_properties(${TARGET_NAME} PROPERTIES SOVERSION ${BULLET_VERSION})
