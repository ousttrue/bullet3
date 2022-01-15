set(TARGET_NAME VoronoiFracture)
add_library(
  ${TARGET_NAME} VoronoiFractureDemo.cpp VoronoiFractureDemo.h
                 btConvexConvexMprAlgorithm.cpp btConvexConvexMprAlgorithm.h)
target_link_libraries(${TARGET_NAME} PRIVATE CommonRigidBodyBase)
