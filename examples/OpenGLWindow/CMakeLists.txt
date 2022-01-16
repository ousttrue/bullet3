set(TARGET_NAME OpenGLWindow)
add_library(
  ${TARGET_NAME}
  fontstash.cpp
  GlfwApp.cpp
  GLInstancingRenderer.cpp
  GLPrimitiveRenderer.cpp
  GLRenderToTexture.cpp
  LoadShader.cpp
  opengl_fontstashcallbacks.cpp
  OpenSans.cpp
  SimpleCamera.cpp
  TwFonts.cpp)
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
         glad
         glfw)
