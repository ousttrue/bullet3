set(TARGET_NAME CommonRigidBodyBase)
add_library(${TARGET_NAME} CommonRigidBodyBase.cpp)
target_link_libraries(${TARGET_NAME} PUBLIC CommonInterfaces BulletDynamics
                                            BulletCollision 
                                            BulletSoftBody
                                            Utils)
target_include_directories(${TARGET_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

subdirs(
  BasicDemo
  Constraints
  DeformableDemo
  DynamicControlDemo
  FractureDemo
  GyroscopicDemo
  Heightfield
  InverseDynamics
  Planar2D
  Raycast
  RigidBody
  RollingFrictionDemo
  SoftDemo
  Tutorial
  ExtendedTutorials
  Evolution
  Vehicles
  Importers
  )
