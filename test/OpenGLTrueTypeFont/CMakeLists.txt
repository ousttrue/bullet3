set(TARGET_NAME OpenGLTrueTypeFont)
add_executable(${TARGET_NAME} main.cpp)
target_link_libraries(${TARGET_NAME} PRIVATE GLFont Bullet3Common OpenGLWindow)
