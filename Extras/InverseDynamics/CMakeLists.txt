set(TARGET_NAME BulletInverseDynamicsUtils)
add_library(
  ${TARGET_NAME}
  CloneTreeCreator.cpp
  CoilCreator.cpp
  MultiBodyTreeCreator.cpp
  btMultiBodyTreeCreator.cpp
  DillCreator.cpp
  MultiBodyTreeDebugGraph.cpp
  invdyn_bullet_comparison.cpp
  IDRandomUtil.cpp
  RandomTreeCreator.cpp
  SimpleTreeCreator.cpp
  MultiBodyNameMap.cpp
  User2InternalIndex.cpp)

set_target_properties(${TARGET_NAME} PROPERTIES VERSION ${BULLET_VERSION})
set_target_properties(${TARGET_NAME} PROPERTIES SOVERSION ${BULLET_VERSION})
target_link_libraries(${TARGET_NAME} BulletInverseDynamics BulletDynamics
                      BulletCollision Bullet3Common LinearMath)
