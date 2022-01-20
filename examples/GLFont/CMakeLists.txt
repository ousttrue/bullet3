set(TARGET_NAME GLFont)
add_library(
  ${TARGET_NAME}
  TwFonts.cpp
  fontstash.cpp
  OpenSans.cpp
  opengl_fontstashcallbacks.cpp
  GLPrimitiveRenderer.cpp
  GLShader.cpp
  GLVBO.cpp
  GLVAO.cpp
  GLTexture.cpp
  )
target_include_directories(${TARGET_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(${TARGET_NAME} PRIVATE stb_image glad)
