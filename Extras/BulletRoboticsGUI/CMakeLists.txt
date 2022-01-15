
INCLUDE_DIRECTORIES(
		${BULLET_PHYSICS_SOURCE_DIR}/src
		${BULLET_PHYSICS_SOURCE_DIR}/examples
		${BULLET_PHYSICS_SOURCE_DIR}/examples/SharedMemory
		${BULLET_PHYSICS_SOURCE_DIR}/examples/ThirdPartyLibs
		${BULLET_PHYSICS_SOURCE_DIR}/examples/ThirdPartyLibs/enet/include
		${BULLET_PHYSICS_SOURCE_DIR}/examples/ThirdPartyLibs/clsocket/src
)

SET(BulletRoboticsGUI_SRCS
	../../examples/ExampleBrowser/InProcessExampleBrowser.cpp
	../../examples/SharedMemory/GraphicsServerExample.cpp
	../../examples/SharedMemory/GraphicsClientExample.cpp
	../../examples/SharedMemory/RemoteGUIHelper.cpp
	../../examples/SharedMemory/RemoteGUIHelperTCP.cpp
	../../examples/SharedMemory/GraphicsServerExample.h
	../../examples/SharedMemory/GraphicsClientExample.h
	../../examples/SharedMemory/RemoteGUIHelper.h
	../../examples/SharedMemory/GraphicsSharedMemoryCommands.h
	../../examples/SharedMemory/GraphicsSharedMemoryPublic.h
	../../examples/SharedMemory/PhysicsServerExample.cpp
	../../examples/SharedMemory/PhysicsServerExampleBullet2.cpp
	../../examples/SharedMemory/SharedMemoryInProcessPhysicsC_API.cpp		
)

IF(BUILD_CLSOCKET)
 ADD_DEFINITIONS(-DBT_ENABLE_CLSOCKET)
ENDIF(BUILD_CLSOCKET)

IF(WIN32)
	IF(BUILD_ENET)
		ADD_DEFINITIONS(-DWIN32 -DBT_ENABLE_ENET)
	ENDIF(BUILD_ENET)
	IF(BUILD_CLSOCKET)
		ADD_DEFINITIONS(-DWIN32)
	ENDIF(BUILD_CLSOCKET)

ELSE(WIN32)
	IF(BUILD_ENET)
		ADD_DEFINITIONS(-DHAS_SOCKLEN_T -DBT_ENABLE_ENET)
	ENDIF(BUILD_ENET)

	IF(BUILD_CLSOCKET)	
		ADD_DEFINITIONS(${OSDEF})
	ENDIF(BUILD_CLSOCKET)
		
	IF(NOT APPLE)
		FIND_PACKAGE(Threads)
		LINK_LIBRARIES( ${CMAKE_THREAD_LIBS_INIT} ${DL} )
	ENDIF(NOT APPLE)
ENDIF(WIN32)

IF(BUILD_ENET)
  set(BulletRoboticsGUI_ENET_INCLUDES 
    ../../examples/SharedMemory/PhysicsClientUDP.h		
    ../../examples/SharedMemory/PhysicsClientUDP_C_API.h
    ../../examples/ThirdPartyLibs/enet/include/enet/win32.h
		../../examples/ThirdPartyLibs/enet/include/enet/unix.h
		../../examples/ThirdPartyLibs/enet/include/enet/callbacks.h
		../../examples/ThirdPartyLibs/enet/include/enet/list.h
		../../examples/ThirdPartyLibs/enet/include/enet/protocol.h
  )

  set(BulletRoboticsGUI_INCLUDES
    ${BulletRoboticsGUI_INCLUDES}
    ${BulletRoboticsGUI_ENET_INCLUDES}
	)



ENDIF(BUILD_ENET)


ADD_DEFINITIONS(-DPHYSICS_SERVER_DIRECT)


ADD_LIBRARY(BulletRoboticsGUI ${BulletRoboticsGUI_SRCS})

SET_TARGET_PROPERTIES(BulletRoboticsGUI PROPERTIES VERSION ${BULLET_VERSION})
SET_TARGET_PROPERTIES(BulletRoboticsGUI PROPERTIES SOVERSION ${BULLET_VERSION})

IF (BUILD_SHARED_LIBS)
	TARGET_LINK_LIBRARIES(BulletRoboticsGUI  BulletExampleBrowserLib BulletRobotics  BulletInverseDynamicsUtils BulletWorldImporter BulletFileLoader BulletSoftBody BulletDynamics BulletCollision BulletInverseDynamics LinearMath Bullet3Common)
ENDIF (BUILD_SHARED_LIBS)

  

	INSTALL(TARGETS
		BulletRoboticsGUI
		LIBRARY DESTINATION lib${LIB_SUFFIX}
		ARCHIVE DESTINATION lib${LIB_SUFFIX}
	)
		
  IF (APPLE AND BUILD_SHARED_LIBS AND FRAMEWORK)
    SET_TARGET_PROPERTIES(BulletRobotics  PROPERTIES FRAMEWORK true)
    SET_TARGET_PROPERTIES(BulletRobotics PROPERTIES PUBLIC_HEADER "PhysicsClientC_API.h" )
  ENDIF (APPLE AND BUILD_SHARED_LIBS AND FRAMEWORK)	

IF(NOT MSVC)
    CONFIGURE_FILE(${CMAKE_CURRENT_SOURCE_DIR}/bullet_robotics_gui.pc.cmake
                   ${CMAKE_CURRENT_BINARY_DIR}/bullet_robotics_gui.pc @ONLY)
  	INSTALL(
      FILES
      ${CMAKE_CURRENT_BINARY_DIR}/bullet_robotics_gui.pc
      DESTINATION
      ${PKGCONFIG_INSTALL_PREFIX}
    )
ENDIF(NOT MSVC)

