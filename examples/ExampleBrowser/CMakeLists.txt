set(TARGET_NAME BulletExampleBrowserLib)
file(GLOB GwenGUISupport_SRCS "GwenGUISupport/*")
file(GLOB GwenGUISupport_HDRS "GwenGUISupport/*")
add_library(
  ${TARGET_NAME}
  OpenGLExampleBrowser.cpp
  OpenGLGuiHelper.cpp
  GL_ShapeDrawer.cpp
  CollisionShape2TriangleMesh.cpp
  CollisionShape2TriangleMesh.h
  ../Utils/b3Clock.cpp
  ../Utils/b3Clock.h
  ../Utils/ChromeTraceUtil.cpp
  ../Utils/ChromeTraceUtil.h
  ../Utils/b3ResourcePath.cpp
  ../Utils/b3ResourcePath.h
  ../Utils/b3ERPCFMHelper.hpp
  ../Utils/b3ReferenceFrameHelper.hpp
  ${GwenGUISupport_SRCS}
  ${GwenGUISupport_HDRS})

set_target_properties(${TARGET_NAME} PROPERTIES VERSION ${BULLET_VERSION})
set_target_properties(${TARGET_NAME} PROPERTIES SOVERSION ${BULLET_VERSION})
target_compile_options(${TARGET_NAME} PRIVATE /wd4244 /wd4267 /wd4305 /wd4312)
target_link_libraries(
  ${TARGET_NAME}
  PUBLIC Bullet3Common
         BulletSoftBody
         BulletDynamics
         BulletCollision
         BulletInverseDynamics
         BulletInverseDynamicsUtils
         LinearMath
         OpenGLWindow
         gwen
         BussIK)

# ##############################################################################
#
# Bullet Example Browser main app
#
# ##############################################################################
set(TARGET_NAME BulletExampleBrowser)
add_executable(
  ${TARGET_NAME}
  main.cpp
  ExampleEntries.cpp
  ExampleEntries.h
  ../RobotSimulator/b3RobotSimulatorClientAPI.cpp
  ../RobotSimulator/b3RobotSimulatorClientAPI.h
  ../Utils/RobotLoggingUtil.cpp
  ../Utils/RobotLoggingUtil.h
  ../ThirdPartyLibs/tinyxml2/tinyxml2.cpp
  InProcessExampleBrowser.cpp
  ${BULLET_PHYSICS_SOURCE_DIR}/build3/bullet.rc)
target_link_libraries(
  ${TARGET_NAME}
  PRIVATE # ThirdParty
          gwen
          glad
          # bullet
          Bullet3Common
          BulletSoftBody
          BulletDynamics
          BulletCollision
          BulletInverseDynamics
          LinearMath
          # extras
          BulletFileLoader
          BulletWorldImporter
          # support lib
          BulletExampleBrowserLib
          OpenGLWindow
          MultiThreading
          TinyRenderer
          SharedMemory
          # Demos
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
          RigidBody
          )

target_compile_definitions(${TARGET_NAME} PRIVATE INCLUDE_CLOTH_DEMOS
                                                  GLEW_STATIC)
target_compile_options(${TARGET_NAME} PRIVATE /wd4244 /wd4267 /wd4305 /wd4312)
target_include_directories(${TARGET_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

# file(MAKE_DIRECTORY "${PROJECT_BINARY_DIR}/data")

# add_custom_command( TARGET App_ExampleBrowser POST_BUILD COMMAND
# ${CMAKE_COMMAND} ARGS -E copy_directory ${BULLET_PHYSICS_SOURCE_DIR}/data
# ${PROJECT_BINARY_DIR}/data)

# if(BULLET2_USE_TBB_MULTITHREADING AND WIN32) # add a post build command to
# copy some dlls to the executable directory set(TBB_VC_VER "vc12")
# set(TBB_VC_ARCH "ia32") # assume 32-bit build in VC12 for now checks can be
# added here at a later time add_custom_command( TARGET App_ExampleBrowser
# POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different
# "${BULLET2_TBB_INCLUDE_DIR}/../bin/${TBB_VC_ARCH}/${TBB_VC_VER}/tbb.dll"
# $<TARGET_FILE_DIR:App_ExampleBrowser>) add_custom_command( TARGET
# App_ExampleBrowser POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different
# "${BULLET2_TBB_INCLUDE_DIR}/../bin/${TBB_VC_ARCH}/${TBB_VC_VER}/tbbmalloc.dll"
# $<TARGET_FILE_DIR:App_ExampleBrowser>) endif(BULLET2_USE_TBB_MULTITHREADING
# AND WIN32)
