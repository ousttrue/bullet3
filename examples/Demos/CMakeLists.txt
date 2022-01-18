subdirs(
  CommonRigidBodyBase
  BulletRobotics
  ForkLift
  MultiBody
  Benchmarks
  InverseKinematics
  MultiThreadedDemo
  Collision
  RenderingExamples
  SharedMemoryDemo
  VoronoiFracture
  RoboticsLearning
  )

set(TARGET_NAME Demos)
add_library(${TARGET_NAME} INTERFACE)
target_link_libraries(
  ${TARGET_NAME}
  INTERFACE # Demos
            CommonRigidBodyBase
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
            #
            VoronoiFracture
            RoboticsLearning
            BulletRobotics
            ForkLift
            MultiBody
            Benchmarks
            InverseKinematics
            MultiThreadedDemo
            Evolution
            Collision
            RenderingExamples
            SharedMemoryDemo)
target_include_directories(${TARGET_NAME} INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
