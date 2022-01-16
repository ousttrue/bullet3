set(TARGET_NAME SharedMemory)
add_library(
  ${TARGET_NAME}
  plugins/collisionFilterPlugin/collisionFilterPlugin.cpp
  plugins/pdControlPlugin/pdControlPlugin.cpp
  plugins/tinyRendererPlugin/tinyRendererPlugin.cpp
  plugins/tinyRendererPlugin/TinyRendererVisualShapeConverter.cpp
  IKTrajectoryHelper.cpp
  PhysicsServer.cpp
  PhysicsClientSharedMemory.cpp
  PhysicsClientSharedMemory_C_API.cpp
  PhysicsClient.cpp
  PhysicsClientC_API.cpp
  RemoteGUIHelper.cpp
  PosixSharedMemory.cpp
  Win32SharedMemory.cpp
  InProcessMemory.cpp
  PhysicsServerSharedMemory.cpp
  PhysicsDirect.cpp
  PhysicsDirectC_API.cpp
  SharedMemoryInProcessPhysicsC_API.cpp
  PhysicsServerExample.cpp
  PhysicsServerExampleBullet2.cpp
  PhysicsLoopBack.cpp
  PhysicsLoopBackC_API.cpp
  PhysicsServerCommandProcessor.cpp
  b3PluginManager.cpp
  b3RobotSimulatorClientAPI_NoGUI.cpp
  b3RobotSimulatorClientAPI_NoDirect.cpp
  #
  PhysicsServer.cpp
  PosixSharedMemory.cpp
  Win32SharedMemory.cpp
  InProcessMemory.cpp
  PhysicsDirect.cpp
  PhysicsDirectC_API.cpp
  PhysicsLoopBack.cpp
  PhysicsLoopBackC_API.cpp
  PhysicsClientSharedMemory_C_API.cpp
  PhysicsClientSharedMemory2_C_API.cpp
  PhysicsClientSharedMemory2.cpp
  SharedMemoryCommandProcessor.cpp
  PhysicsServerCommandProcessor.cpp
  plugins/tinyRendererPlugin/tinyRendererPlugin.cpp
  plugins/tinyRendererPlugin/TinyRendererVisualShapeConverter.cpp
  b3PluginManager.cpp)
target_link_libraries(
  ${TARGET_NAME}
  PRIVATE CommonInterfaces
          BussIK
          BulletDynamics
          BulletSoftBody
          Importers
          tiny_obj_loader
          TinyRenderer
          stb_image
          OpenGLWindow
          Utils
          UrdfImporter
          BulletInverseDynamicsUtils
          MultiThreading
  PUBLIC UrdfParser)
target_include_directories(${TARGET_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# #
# ##############################################################################
# set(TARGET_NAME App_PhysicsServer_SharedMemory) add_executable( ${TARGET_NAME}
# main.cpp ../MultiThreading/b3Win32ThreadSupport.cpp
# ${BULLET_PHYSICS_SOURCE_DIR}/build3/bullet.rc) target_link_libraries(
# ${TARGET_NAME} PRIVATE SharedMemory Bullet3Common CommonInterfaces UrdfParser
# BulletInverseDynamicsUtils BulletWorldImporter MultiThreading )

# ##############################################################################
# set(TARGET_NAME App_PhysicsServer_SharedMemory_GUI)
# add_executable(${TARGET_NAME} ../StandaloneMain/main_opengl_single_example.cpp
# ${BULLET_PHYSICS_SOURCE_DIR}/build3/bullet.rc) target_link_libraries(
# ${TARGET_NAME} PRIVATE SharedMemory Bullet3Common CommonInterfaces UrdfParser
# UrdfImporter BulletInverseDynamicsUtils BulletWorldImporter MultiThreading
# OpenGLWindow Utils) target_compile_definitions(${TARGET_NAME} PRIVATE
# B3_USE_STANDALONE_EXAMPLE)

# ADD_EXECUTABLE(App_PhysicsServer_SharedMemory_GUI ${SharedMemory_SRCS}
# ../StandaloneMain/main_opengl_single_example.cpp
# ../ExampleBrowser/OpenGLGuiHelper.cpp ../ExampleBrowser/GL_ShapeDrawer.cpp
# ../ExampleBrowser/CollisionShape2TriangleMesh.cpp
# ../MultiThreading/b3PosixThreadSupport.cpp
# ../MultiThreading/b3PosixThreadSupport.h )

# ELSE(APPLE) ADD_DEFINITIONS("-DGLEW_INIT_OPENGL11_FUNCTIONS=1")
# ADD_DEFINITIONS("-DGLEW_STATIC")
# ADD_DEFINITIONS("-DGLEW_DYNAMIC_LOAD_ALL_GLX_FUNCTIONS=1")

# ADD_EXECUTABLE(App_PhysicsServer_SharedMemory_GUI ${SharedMemory_SRCS}
# ../StandaloneMain/main_opengl_single_example.cpp
# ../ExampleBrowser/OpenGLGuiHelper.cpp ../ExampleBrowser/GL_ShapeDrawer.cpp
# ../ExampleBrowser/CollisionShape2TriangleMesh.cpp
# ../MultiThreading/b3PosixThreadSupport.cpp
# ../MultiThreading/b3PosixThreadSupport.h ) ENDIF(APPLE) ENDIF(WIN32)

# IF (INTERNAL_ADD_POSTFIX_EXECUTABLE_NAMES)
# SET_TARGET_PROPERTIES(App_PhysicsServer_SharedMemory_GUI PROPERTIES
# DEBUG_POSTFIX "_Debug")
# SET_TARGET_PROPERTIES(App_PhysicsServer_SharedMemory_GUI PROPERTIES
# MINSIZEREL_POSTFIX "_MinsizeRel")
# SET_TARGET_PROPERTIES(App_PhysicsServer_SharedMemory_GUI PROPERTIES
# RELWITHDEBINFO_POSTFIX "_RelWithDebugInfo")
# ENDIF(INTERNAL_ADD_POSTFIX_EXECUTABLE_NAMES)

# #VR/OpenVR on Windows and Mac OSX IF(USE_OPENVR)

# IF (WIN32 OR APPLE)

# INCLUDE_DIRECTORIES( ${BULLET_PHYSICS_SOURCE_DIR}/src
# ${BULLET_PHYSICS_SOURCE_DIR}/examples/ThirdPartyLibs
# ${BULLET_PHYSICS_SOURCE_DIR}/examples/ThirdPartyLibs/glad
# ${BULLET_PHYSICS_SOURCE_DIR}/examples/ThirdPartyLibs/openvr/headers
# ${BULLET_PHYSICS_SOURCE_DIR}/examples/ThirdPartyLibs/openvr/samples/shared )

# LINK_LIBRARIES( Bullet3Common BulletWorldImporter BulletInverseDynamicsUtils
# BulletInverseDynamics BulletDynamics BulletCollision LinearMath BussIK
# openvr_api OpenGLWindow )

# ADD_DEFINITIONS(-DGLEW_STATIC) LINK_LIBRARIES( ${OPENGL_gl_LIBRARY}
# ${OPENGL_glu_LIBRARY} ) IF(WIN32) SET(Platform_SRCS
# ../MultiThreading/b3Win32ThreadSupport.cpp
# ../MultiThreading/b3Win32ThreadSupport.h
# ${BULLET_PHYSICS_SOURCE_DIR}/build3/bullet.rc ) IF (CMAKE_CL_64)
# LINK_DIRECTORIES(${BULLET_PHYSICS_SOURCE_DIR}/examples/ThirdPartyLibs/openvr/lib/win64)
# ELSE(CMAKE_CL_64)
# LINK_DIRECTORIES(${BULLET_PHYSICS_SOURCE_DIR}/examples/ThirdPartyLibs/openvr/lib/win32)
# ENDIF(CMAKE_CL_64) ELSE(WIN32)

# set_source_files_properties(../ThirdPartyLibs/openvr/samples/shared/pathtools.cpp
# ../StandaloneMain/hellovr_opengl_main.cpp  PROPERTIES COMPILE_FLAGS "-x
# objective-c++") find_library(FOUNDATION_FRAMEWORK Foundation)
# mark_as_advanced(FOUNDATION_FRAMEWORK) set(EXTRA_LIBS ${EXTRA_LIBS}
# ${FOUNDATION_FRAMEWORK})

# set(CMAKE_MACOSX_RPATH 0)

# SET(Platform_SRCS ../MultiThreading/b3PosixThreadSupport.cpp
# ../MultiThreading/b3PosixThreadSupport.h )

# IF (CMAKE_SIZEOF_VOID_P MATCHES 8) MESSAGE("CL64")
# LINK_DIRECTORIES(${BULLET_PHYSICS_SOURCE_DIR}/examples/ThirdPartyLibs/openvr/bin/osx64)
# ELSE() MESSAGE("CL32") set(ARCH_TARGET osx32)
# LINK_DIRECTORIES(${BULLET_PHYSICS_SOURCE_DIR}/examples/ThirdPartyLibs/openvr/bin/osx32)
# ENDIF() add_definitions(-DOSX -DPOSIX)

# ENDIF(WIN32) ADD_EXECUTABLE(App_PhysicsServer_SharedMemory_VR
# ${SharedMemory_SRCS} ${Platform_SRCS}
# ../StandaloneMain/hellovr_opengl_main.cpp
# ../ExampleBrowser/OpenGLGuiHelper.cpp ../ExampleBrowser/GL_ShapeDrawer.cpp
# ../ExampleBrowser/CollisionShape2TriangleMesh.cpp
# ../RenderingExamples/TinyVRGui.cpp ../RenderingExamples/TimeSeriesCanvas.cpp
# ../RenderingExamples/TimeSeriesFontData.cpp
# ../ThirdPartyLibs/openvr/samples/shared/lodepng.cpp
# ../ThirdPartyLibs/openvr/samples/shared/lodepng.h
# ../ThirdPartyLibs/openvr/samples/shared/Matrices.cpp
# ../ThirdPartyLibs/openvr/samples/shared/Matrices.h
# ../ThirdPartyLibs/openvr/samples/shared/pathtools.cpp
# ../ThirdPartyLibs/openvr/samples/shared/pathtools.h
# ../ThirdPartyLibs/openvr/samples/shared/strtools.cpp
# ../ThirdPartyLibs/openvr/samples/shared/strtools.h
# ../ThirdPartyLibs/openvr/samples/shared/Vectors.h )

# IF (NOT INTERNAL_CREATE_DISTRIBUTABLE_MSVC_PROJECTFILES) IF (CMAKE_CL_64)
# ADD_CUSTOM_COMMAND( TARGET App_PhysicsServer_SharedMemory_VR POST_BUILD
# COMMAND ${CMAKE_COMMAND} ARGS -E copy_if_different
# ${BULLET_PHYSICS_SOURCE_DIR}/examples/ThirdPartyLibs/openvr/bin/win64/openvr_api.dll
# ${CMAKE_CURRENT_BINARY_DIR}/openvr64pi.dll ) ELSE(CMAKE_CL_64)
# ADD_CUSTOM_COMMAND( TARGET App_PhysicsServer_SharedMemory_VR POST_BUILD
# COMMAND ${CMAKE_COMMAND} ARGS -E copy_if_different
# ${BULLET_PHYSICS_SOURCE_DIR}/examples/ThirdPartyLibs/openvr/bin/win32/openvr_api.dll
# ${CMAKE_CURRENT_BINARY_DIR}/openvr_api.dll ) ENDIF(CMAKE_CL_64)

# ADD_CUSTOM_COMMAND( TARGET App_PhysicsServer_SharedMemory_VR POST_BUILD
# COMMAND ${CMAKE_COMMAND} ARGS -E copy_directory
# ${BULLET_PHYSICS_SOURCE_DIR}/data ${PROJECT_BINARY_DIR}/data ) ENDIF (NOT
# INTERNAL_CREATE_DISTRIBUTABLE_MSVC_PROJECTFILES)

# SET_TARGET_PROPERTIES(App_PhysicsServer_SharedMemory_VR PROPERTIES
# COMPILE_DEFINITIONS BT_ENABLE_VR )

# IF (INTERNAL_ADD_POSTFIX_EXECUTABLE_NAMES)
# SET_TARGET_PROPERTIES(App_PhysicsServer_SharedMemory_VR PROPERTIES
# DEBUG_POSTFIX "_Debug")
# SET_TARGET_PROPERTIES(App_PhysicsServer_SharedMemory_VR PROPERTIES
# MINSIZEREL_POSTFIX "_MinsizeRel")
# SET_TARGET_PROPERTIES(App_PhysicsServer_SharedMemory_VR PROPERTIES
# RELWITHDEBINFO_POSTFIX "_RelWithDebugInfo")
# ENDIF(INTERNAL_ADD_POSTFIX_EXECUTABLE_NAMES)

# #VR/OpenVR on Linux ELSE(WIN32 OR APPLE) IF(CMAKE_SIZEOF_VOID_P MATCHES 8)
# LINK_DIRECTORIES(${BULLET_PHYSICS_SOURCE_DIR}/examples/ThirdPartyLibs/openvr/bin/linux64)
# ELSE()
# LINK_DIRECTORIES(${BULLET_PHYSICS_SOURCE_DIR}/examples/ThirdPartyLibs/openvr/bin/linux32)
# ENDIF()

# ADD_EXECUTABLE(App_PhysicsServer_SharedMemory_VR ${SharedMemory_SRCS}
# ../StandaloneMain/hellovr_opengl_main.cpp
# ../ExampleBrowser/OpenGLGuiHelper.cpp ../ExampleBrowser/GL_ShapeDrawer.cpp
# ../ExampleBrowser/CollisionShape2TriangleMesh.cpp
# ../RenderingExamples/TinyVRGui.cpp ../RenderingExamples/TinyVRGui.h
# ../RenderingExamples/TimeSeriesCanvas.cpp
# ../RenderingExamples/TimeSeriesFontData.cpp
# ../MultiThreading/b3PosixThreadSupport.cpp
# ../MultiThreading/b3PosixThreadSupport.h
# ../ThirdPartyLibs/openvr/samples/shared/lodepng.cpp
# ../ThirdPartyLibs/openvr/samples/shared/lodepng.h
# ../ThirdPartyLibs/openvr/samples/shared/Matrices.cpp
# ../ThirdPartyLibs/openvr/samples/shared/Matrices.h
# ../ThirdPartyLibs/openvr/samples/shared/pathtools.cpp
# ../ThirdPartyLibs/openvr/samples/shared/pathtools.h
# ../ThirdPartyLibs/openvr/samples/shared/strtools.cpp
# ../ThirdPartyLibs/openvr/samples/shared/strtools.h
# ../ThirdPartyLibs/openvr/samples/shared/Vectors.h )

# target_include_directories(App_PhysicsServer_SharedMemory_VR PRIVATE
# ${BULLET_PHYSICS_SOURCE_DIR}/src
# ${BULLET_PHYSICS_SOURCE_DIR}/examples/ThirdPartyLibs
# ${BULLET_PHYSICS_SOURCE_DIR}/examples/ThirdPartyLibs/Glew
# ${BULLET_PHYSICS_SOURCE_DIR}/examples/ThirdPartyLibs/openvr/headers
# ${BULLET_PHYSICS_SOURCE_DIR}/examples/ThirdPartyLibs/openvr/samples
# ${BULLET_PHYSICS_SOURCE_DIR}/examples/ThirdPartyLibs/openvr/samples/shared )

# target_compile_definitions(App_PhysicsServer_SharedMemory_VR PRIVATE POSIX
# LINUX BT_ENABLE_VR GLEW_STATIC GLEW_INIT_OPENGL11_FUNCTIONS=1
# GLEW_DYNAMIC_LOAD_ALL_GLX_FUNCTIONS=1 )

# target_compile_options(App_PhysicsServer_SharedMemory_VR PRIVATE -std=c++11 )

# FIND_PACKAGE(Threads) target_link_libraries(App_PhysicsServer_SharedMemory_VR
# PRIVATE openvr_api ${CMAKE_THREAD_LIBS_INIT} ${DL} Bullet3Common
# BulletWorldImporter BulletInverseDynamicsUtils BulletInverseDynamics
# BulletDynamics BulletCollision LinearMath BussIK OpenGLWindow )

# IF (INTERNAL_ADD_POSTFIX_EXECUTABLE_NAMES)
# SET_TARGET_PROPERTIES(App_PhysicsServer_SharedMemory_VR PROPERTIES
# DEBUG_POSTFIX "_Debug")
# SET_TARGET_PROPERTIES(App_PhysicsServer_SharedMemory_VR PROPERTIES
# MINSIZEREL_POSTFIX "_MinsizeRel")
# SET_TARGET_PROPERTIES(App_PhysicsServer_SharedMemory_VR PROPERTIES
# RELWITHDEBINFO_POSTFIX "_RelWithDebugInfo")
# ENDIF(INTERNAL_ADD_POSTFIX_EXECUTABLE_NAMES)

# ENDIF(WIN32 OR APPLE) ENDIF(USE_OPENVR)
