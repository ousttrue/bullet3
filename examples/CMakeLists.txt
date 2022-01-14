subdirs(ThirdPartyLibs)
subdirs(OpenGLWindow GwenGUISupport CommonInterfaces Demos Importers MultiThreading TinyRenderer SharedMemory Utils)
subdirs(HelloWorld ExampleBrowser)
# IF(BUILD_BULLET3) SUBDIRS(  RobotSimulator SharedMemory ThirdPartyLibs/Gwen
# ThirdPartyLibs/BussIK ThirdPartyLibs/clsocket OpenGLWindow TwoJoint ) ENDIF()
# IF(BUILD_PYBULLET) SUBDIRS(pybullet) ENDIF(BUILD_PYBULLET)
