set(TARGET_NAME App_ExampleBrowser)
add_executable(
  ${TARGET_NAME}
  main.cpp
  OpenGLExampleBrowser.cpp
  OpenGLGuiHelper.cpp
  CollisionShape2TriangleMesh.cpp
  ExampleEntries.cpp
  InProcessExampleBrowser.cpp
  ${BULLET_PHYSICS_SOURCE_DIR}/build3/bullet.rc)
target_link_libraries(
  ${TARGET_NAME}
  PRIVATE Demos
          # support lib
          Utils
          OpenGLWindow
          MultiThreading
          TinyRenderer
          SharedMemory
          GwenGUISupport
          RobotSimulator
          # extras
          BulletFileLoader
          BulletWorldImporter
          # bullet
          BulletInverseDynamicsUtils)

target_compile_definitions(${TARGET_NAME} PRIVATE INCLUDE_CLOTH_DEMOS
                                                  GLEW_STATIC)
target_include_directories(${TARGET_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
