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
  ../SharedMemory/plugins/collisionFilterPlugin/collisionFilterPlugin.cpp
  ../SharedMemory/plugins/collisionFilterPlugin/collisionFilterPlugin.h
  ../SharedMemory/plugins/pdControlPlugin/pdControlPlugin.cpp
  ../SharedMemory/plugins/pdControlPlugin/pdControlPlugin.h
  ../SharedMemory/plugins/tinyRendererPlugin/tinyRendererPlugin.cpp
  ../SharedMemory/plugins/tinyRendererPlugin/TinyRendererVisualShapeConverter.cpp
  ../SharedMemory/IKTrajectoryHelper.cpp
  ../SharedMemory/IKTrajectoryHelper.h
  ../SharedMemory/PhysicsServer.cpp
  ../SharedMemory/PhysicsClientSharedMemory.cpp
  ../SharedMemory/PhysicsClientSharedMemory_C_API.cpp
  ../SharedMemory/PhysicsClient.cpp
  ../SharedMemory/PhysicsClientC_API.cpp
  ../SharedMemory/GraphicsServerExample.cpp
  ../SharedMemory/GraphicsClientExample.cpp
  ../SharedMemory/RemoteGUIHelper.cpp
  ../SharedMemory/GraphicsServerExample.h
  ../SharedMemory/GraphicsClientExample.h
  ../SharedMemory/RemoteGUIHelper.h
  ../SharedMemory/GraphicsSharedMemoryCommands.h
  ../SharedMemory/GraphicsSharedMemoryPublic.h
  ../SharedMemory/PhysicsServerExample.cpp
  ../SharedMemory/PhysicsServerExampleBullet2.cpp
  ../SharedMemory/PhysicsClientExample.cpp
  ../SharedMemory/PosixSharedMemory.cpp
  ../SharedMemory/Win32SharedMemory.cpp
  ../SharedMemory/InProcessMemory.cpp
  ../SharedMemory/PhysicsServerSharedMemory.cpp
  ../SharedMemory/PhysicsDirect.cpp
  ../SharedMemory/PhysicsDirect.h
  ../SharedMemory/PhysicsDirectC_API.cpp
  ../SharedMemory/PhysicsDirectC_API.h
  ../SharedMemory/SharedMemoryInProcessPhysicsC_API.cpp
  ../SharedMemory/PhysicsLoopBack.cpp
  ../SharedMemory/PhysicsLoopBack.h
  ../SharedMemory/PhysicsLoopBackC_API.cpp
  ../SharedMemory/PhysicsLoopBackC_API.h
  ../SharedMemory/PhysicsServerCommandProcessor.cpp
  ../SharedMemory/PhysicsServerCommandProcessor.h
  ../SharedMemory/SharedMemoryCommands.h
  ../SharedMemory/SharedMemoryPublic.h
  ../SharedMemory/b3PluginManager.cpp
  ../SharedMemory/b3RobotSimulatorClientAPI_NoGUI.cpp
  ../SharedMemory/b3RobotSimulatorClientAPI_NoGUI.h
  ../SharedMemory/b3RobotSimulatorClientAPI_NoDirect.cpp
  ../SharedMemory/b3RobotSimulatorClientAPI_NoDirect.h
  ../RobotSimulator/b3RobotSimulatorClientAPI.cpp
  ../RobotSimulator/b3RobotSimulatorClientAPI.h
  ../BasicDemo/BasicExample.cpp
  ../BasicDemo/BasicExample.h
  ../InverseDynamics/InverseDynamicsExample.cpp
  ../InverseDynamics/InverseDynamicsExample.h
  ../InverseKinematics/InverseKinematicsExample.cpp
  ../InverseKinematics/InverseKinematicsExample.h
  ../MultiThreadedDemo/MultiThreadedDemo.cpp
  ../MultiThreadedDemo/MultiThreadedDemo.h
  ../MultiThreadedDemo/CommonRigidBodyMTBase.cpp
  ../MultiThreadedDemo/CommonRigidBodyMTBase.h
  ../Heightfield/HeightfieldExample.cpp
  ../Heightfield/HeightfieldExample.h
  ../Tutorial/Tutorial.cpp
  ../Tutorial/Tutorial.h
  ../Tutorial/Dof6ConstraintTutorial.cpp
  ../Tutorial/Dof6ConstraintTutorial.h
  ../Evolution/NN3DWalkers.cpp
  ../Evolution/NN3DWalkers.h
  ../Evolution/NN3DWalkersTimeWarpBase.h
  ../Collision/CollisionSdkC_Api.cpp
  ../Collision/CollisionSdkC_Api.h
  ../Collision/CollisionTutorialBullet2.cpp
  ../Collision/CollisionTutorialBullet2.h
  ../Collision/Internal/Bullet2CollisionSdk.cpp
  ../Collision/Internal/Bullet2CollisionSdk.h
  ../Collision/Internal/CollisionSdkInterface.h
  ../Collision/Internal/RealTimeBullet3CollisionSdk.cpp
  ../Collision/Internal/RealTimeBullet3CollisionSdk.h
  ../GyroscopicDemo/GyroscopicSetup.cpp
  ../GyroscopicDemo/GyroscopicSetup.h
  ../Planar2D/Planar2D.cpp
  ../Planar2D/Planar2D.h
  ../RollingFrictionDemo/RollingFrictionDemo.cpp
  ../RollingFrictionDemo/RollingFrictionDemo.h
  ../DynamicControlDemo/MotorDemo.cpp
  ../DynamicControlDemo/MotorDemo.h
  ../MultiThreading/MultiThreadingExample.cpp
  ../MultiThreading/b3PosixThreadSupport.cpp
  ../MultiThreading/b3Win32ThreadSupport.cpp
  ../MultiThreading/b3ThreadSupportInterface.cpp
  ../RenderingExamples/TinyRendererSetup.cpp
  ../RenderingExamples/TimeSeriesCanvas.cpp
  ../RenderingExamples/TimeSeriesCanvas.h
  ../RenderingExamples/TimeSeriesFontData.cpp
  ../RenderingExamples/TimeSeriesFontData.h
  ../RenderingExamples/DynamicTexturedCubeDemo.cpp
  ../RenderingExamples/DynamicTexturedCubeDemo.h
  ../RenderingExamples/TinyVRGui.cpp
  ../RenderingExamples/TinyVRGui.h
  ../RoboticsLearning/GripperGraspExample.cpp
  ../RoboticsLearning/GripperGraspExample.h
  ../RoboticsLearning/R2D2GraspExample.cpp
  ../RoboticsLearning/R2D2GraspExample.h
  ../RoboticsLearning/KukaGraspExample.cpp
  ../RoboticsLearning/KukaGraspExample.h
  ../RenderingExamples/CoordinateSystemDemo.cpp
  ../RenderingExamples/CoordinateSystemDemo.h
  ../RenderingExamples/RaytracerSetup.cpp
  ../RenderingExamples/RaytracerSetup.h
  ../RenderingExamples/RenderInstancingDemo.cpp
  ../RenderingExamples/RenderInstancingDemo.h
  ../RenderingExamples/TimeSeriesExample.cpp
  ../Raycast/RaytestDemo.cpp
  ../Importers/ImportBsp/BspConverter.h
  ../Importers/ImportBullet/SerializeSetup.cpp
  ../Importers/ImportBullet/SerializeSetup.h
  ../Importers/ImportMeshUtility/b3ImportMeshUtility.cpp
  ../Importers/ImportMeshUtility/b3ImportMeshUtility.h
  ../../Extras/Serialize/BulletWorldImporter/btWorldImporter.cpp
  ../../Extras/Serialize/BulletWorldImporter/btMultiBodyWorldImporter.cpp
  ../../Extras/Serialize/BulletWorldImporter/btBulletWorldImporter.cpp
  ../../Extras/Serialize/BulletFileLoader/bChunk.cpp
  ../../Extras/Serialize/BulletFileLoader/bFile.cpp
  ../../Extras/Serialize/BulletFileLoader/bDNA.cpp
  ../../Extras/Serialize/BulletFileLoader/btBulletFile.cpp
  ../Importers/ImportBsp/BspLoader.h
  ../Importers/ImportBsp/ImportBspExample.h
  ../Importers/ImportColladaDemo/btMatrix4x4.h
  ../Importers/ImportColladaDemo/ColladaGraphicsInstance.h
  ../Importers/ImportColladaDemo/ImportColladaSetup.h
  ../Importers/ImportColladaDemo/LoadMeshFromCollada.h
  ../Importers/ImportObjDemo/ImportObjExample.h
  ../Importers/ImportObjDemo/LoadMeshFromObj.h
  ../Importers/ImportObjDemo/Wavefront2GLInstanceGraphicsShape.h
  ../Importers/ImportSTLDemo/ImportSTLSetup.h
  ../Importers/ImportSTLDemo/LoadMeshFromSTL.h
  ../Importers/ImportURDFDemo/ConvertRigidBodies2MultiBody.h
  ../Importers/ImportURDFDemo/ImportURDFSetup.h
  ../Importers/ImportURDFDemo/URDF2Bullet.h
  ../Importers/ImportURDFDemo/urdf_samples.h
  ../Importers/ImportURDFDemo/urdf_samples.h
  ../Importers/ImportMJCFDemo/BulletMJCFImporter.cpp
  ../Importers/ImportMJCFDemo/BulletMJCFImporter.h
  ../Importers/ImportMJCFDemo/ImportMJCFSetup.cpp
  ../Importers/ImportMJCFDemo/ImportMJCFSetup.h
  ../Importers/ImportBsp/BspConverter.cpp
  ../Importers/ImportBsp/BspLoader.cpp
  ../Importers/ImportBsp/ImportBspExample.cpp
  ../Importers/ImportColladaDemo/ImportColladaSetup.cpp
  ../Importers/ImportColladaDemo/LoadMeshFromCollada.cpp
  ../Importers/ImportObjDemo/ImportObjExample.cpp
  ../Importers/ImportObjDemo/LoadMeshFromObj.cpp
  ../Importers/ImportObjDemo/Wavefront2GLInstanceGraphicsShape.cpp
  ../Importers/ImportSTLDemo/ImportSTLSetup.cpp
  ../Importers/ImportSDFDemo/ImportSDFSetup.cpp
  ../Importers/ImportURDFDemo/ImportURDFSetup.cpp
  ../Importers/ImportURDFDemo/URDF2Bullet.cpp
  ../Importers/ImportURDFDemo/MyMultiBodyCreator.cpp
  ../Importers/ImportURDFDemo/MyMultiBodyCreator.h
  ../Importers/ImportURDFDemo/UrdfParser.cpp
  ../Utils/RobotLoggingUtil.cpp
  ../Utils/RobotLoggingUtil.h
  ../Importers/ImportURDFDemo/urdfStringSplit.cpp
  ../Importers/ImportURDFDemo/urdfStringSplit.h
  ../Importers/ImportURDFDemo/BulletUrdfImporter.cpp
  ../Importers/ImportURDFDemo/BulletUrdfImporter.h
  ../Vehicles/Hinge2Vehicle.cpp
  ../Vehicles/Hinge2Vehicle.h
  ../SoftDemo/SoftDemo.cpp
  ../SoftDemo/SoftDemo.h
  ../RigidBody/RigidBodySoftContact.cpp
  ../RigidBody/KinematicRigidBodyExample.cpp
  ../ThirdPartyLibs/stb_image/stb_image.cpp
  ../ThirdPartyLibs/stb_image/stb_image.h
  ../ThirdPartyLibs/stb_image/stb_image_write.cpp
  ../ThirdPartyLibs/Wavefront/tiny_obj_loader.cpp
  ../ThirdPartyLibs/tinyxml2/tinyxml2.cpp
  InProcessExampleBrowser.cpp
  ${BULLET_PHYSICS_SOURCE_DIR}/build3/bullet.rc)
target_link_libraries(
  ${TARGET_NAME}
  PRIVATE BulletExampleBrowserLib
          Bullet3Common
          BulletSoftBody
          BulletDynamics
          BulletCollision
          BulletInverseDynamics
          LinearMath
          OpenGLWindow
          gwen
          glad
          BussIK
          #
          ExtendedTutorials
          BulletRobotics
          TinyRenderer
          ForkLift
          Constraints
          DeformableDemo
          VoronoiFracture
          MultiBody
          FractureDemo
          Benchmarks
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
