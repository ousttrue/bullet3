set(TARGET_NAME OpenGLWindow)
file(GLOB OpenGLWindow_HDRS "*.h")
file(GLOB OpenGLWindowMac_CPP "Mac*.cpp")
file(GLOB OpenGLWindowMacObjC_CPP "Mac*.m")
file(GLOB OpenGLWindowWin32_CPP "Win32*.cpp")
file(GLOB OpenGLWindowLinux_CPP "X11*.cpp")
file(GLOB OpenGLWindowCommon_CPP "*.cpp")
list(REMOVE_ITEM OpenGLWindowCommon_CPP ${OpenGLWindowMac_CPP})
list(REMOVE_ITEM OpenGLWindowCommon_CPP ${OpenGLWindowWin32_CPP})
list(REMOVE_ITEM OpenGLWindowCommon_CPP ${OpenGLWindowLinux_CPP})
list(REMOVE_ITEM OpenGLWindowCommon_CPP X11OpenGLWindow.cpp)
list(REMOVE_ITEM OpenGLWindowCommon_CPP MacOpenGLWindow.cpp)

add_library(
  ${TARGET_NAME} STATIC ${OpenGLWindow_SRCS} ${OpenGLWindow_HDRS}
                        ${OpenGLWindowWin32_CPP} ${OpenGLWindowCommon_CPP})
target_compile_definitions(${TARGET_NAME} PUBLIC GLEW_STATIC)
target_compile_options(${TARGET_NAME} PRIVATE /wd4244)
target_include_directories(
  ${TARGET_NAME}
  PRIVATE ${BULLET_PHYSICS_SOURCE_DIR}/examples/ThirdPartyLibs
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(
  ${TARGET_NAME}
  PUBLIC glad
         Bullet3Common
         CommonInterfaces
         OpenGL32.lib
         stb_image
         glad)
