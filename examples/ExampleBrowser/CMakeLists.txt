set(TARGET_NAME ExampleBrowserLib)
add_library(${TARGET_NAME} EmptyBrowser.cpp InProcessExampleBrowser.cpp
                           ExampleEntries.cpp OpenGLExampleBrowser.cpp)
target_link_libraries(
  ${TARGET_NAME}
  PUBLIC CommonInterfaces
  PRIVATE glfw
          Demos
          SharedMemoryDemo
          # support lib
          Utils
          OpenGLWindow
          MultiThreading
          TinyRenderer
          SharedMemory
          RobotSimulator
          # extras
          BulletFileLoader
          BulletWorldImporter
          # bullet
          BulletInverseDynamicsUtils)
target_compile_definitions(${TARGET_NAME} PRIVATE INCLUDE_CLOTH_DEMOS)
target_include_directories(${TARGET_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# # ##############################################################################
# set(TARGET_NAME App_ExampleBrowserGwen)
# add_executable(${TARGET_NAME} main.cpp
#                               ${BULLET_PHYSICS_SOURCE_DIR}/build3/bullet.rc)
# target_link_libraries(${TARGET_NAME} PRIVATE ExampleBrowserLib GwenGUISupport
#                                              Utils Importers)

# # ##############################################################################
# set(TARGET_NAME App_ExampleBrowserImGui)
# add_executable(
#   ${TARGET_NAME}
#   #
#   main_imgui.cpp ${BULLET_PHYSICS_SOURCE_DIR}/build3/bullet.rc)
# target_link_libraries(${TARGET_NAME} PRIVATE ExampleBrowserLib ImGuiInterface
#                                              Utils Importers)
