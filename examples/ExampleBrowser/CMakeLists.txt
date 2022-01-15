set(TARGET_NAME ExampleBrowserLib)
add_library(${TARGET_NAME} EmptyBrowser.cpp InProcessExampleBrowser.cpp)
target_link_libraries(${TARGET_NAME} PRIVATE CommonInterfaces Utils)
target_compile_definitions(${TARGET_NAME} PRIVATE EXAMPLE_CONSOLE_ONLY)

set(TARGET_NAME App_ExampleBrowser)
add_executable(
  ${TARGET_NAME}
  main.cpp 
  OpenGLGuiHelper.cpp CollisionShape2TriangleMesh.cpp
  OpenGLExampleBrowser.cpp ExampleEntries.cpp
  ${BULLET_PHYSICS_SOURCE_DIR}/build3/bullet.rc
  )
target_link_libraries(
  ${TARGET_NAME}
  PRIVATE glfw
          Demos
          # support lib
          Utils
          OpenGLWindow
          MultiThreading
          TinyRenderer
          SharedMemory
          RobotSimulator
          ExampleBrowserLib
          GwenGUISupport
          # extras
          BulletFileLoader
          BulletWorldImporter
          # bullet
          BulletInverseDynamicsUtils
          )

target_compile_definitions(${TARGET_NAME} PRIVATE INCLUDE_CLOTH_DEMOS
                                                  GLEW_STATIC)
target_include_directories(${TARGET_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

# ##############################################################################
set(TARGET_NAME App_ExampleBrowserImGui)
add_executable(
  ${TARGET_NAME}
  #
  main_imgui.cpp ImGuiExampleBrowser.cpp OpenGLGuiHelper.cpp
  CollisionShape2TriangleMesh.cpp ExampleEntries.cpp
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
          RobotSimulator
          ExampleBrowserLib
          # extras
          BulletFileLoader
          BulletWorldImporter
          # bullet
          BulletInverseDynamicsUtils)

target_compile_definitions(${TARGET_NAME} PRIVATE INCLUDE_CLOTH_DEMOS
                                                  GLEW_STATIC)
target_include_directories(${TARGET_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
