set(TARGET_NAME GwenGUISupport)
add_library(
  ${TARGET_NAME}
  GraphingTexture.cpp GwenParameterInterface.cpp GwenUserInterface.cpp
  GwenTextureWindow.cpp gwenInternalData.cpp)
target_link_libraries(${TARGET_NAME} PRIVATE BulletDynamics CommonInterfaces
                                             OpenGLWindow gwen)
target_include_directories(${TARGET_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
