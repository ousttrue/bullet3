set(TARGET_NAME CommonRigidBodyBase)
add_library(${TARGET_NAME} CommonRigidBodyBase.cpp)
target_link_libraries(${TARGET_NAME} PUBLIC CommonInterfaces BulletDynamics
                                            BulletCollision)
target_include_directories(${TARGET_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

subdirs(
  ExtendedTutorials
  BulletRobotics
  ForkLift
  Constraints
  DeformableDemo
  VoronoiFracture
  MultiBody
  FractureDemo
  Benchmarks
  DynamicControlDemo
  SoftDemo
  GyroscopicDemo
  InverseDynamics
  InverseKinematics
  Heightfield
  MultiThreadedDemo
  Vehicles
  Evolution
  Planar2D
  Raycast
  Collision
  Tutorial
  RenderingExamples
  RollingFrictionDemo
  RoboticsLearning
  BasicDemo
  RigidBody
  Importers)

set(TARGET_NAME Demos)
add_library(${TARGET_NAME} INTERFACE)
target_link_libraries(
  ${TARGET_NAME}
  INTERFACE # Demos
            ExtendedTutorials
            BulletRobotics
            ForkLift
            Constraints
            DeformableDemo
            VoronoiFracture
            MultiBody
            FractureDemo
            Benchmarks
            DynamicControlDemo
            SoftDemo
            GyroscopicDemo
            InverseDynamics
            InverseKinematics
            Heightfield
            MultiThreadedDemo
            Importers
            Vehicles
            Evolution
            Planar2D
            Raycast
            Collision
            Tutorial
            RenderingExamples
            RollingFrictionDemo
            RoboticsLearning
            BasicDemo
            RigidBody)
