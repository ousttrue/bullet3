set(TARGET_NAME GLFont)
add_library(
  ${TARGET_NAME}
  TwFonts.cpp
  OpenSans.cpp
  fontstash.cpp
  opengl_fontstashcallbacks.cpp
  GLPrimitiveRenderer.cpp
  GLShader.cpp
  GLVBO.cpp
  GLVAO.cpp
  GLTexture.cpp
  GLMesh.cpp
  )
target_include_directories(${TARGET_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(
  ${TARGET_NAME}
  PRIVATE stb_image
  PUBLIC glad)
