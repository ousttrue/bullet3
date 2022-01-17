set(TARGET_NAME App_BasicExampleGui)
add_executable(${TARGET_NAME} main_opengl_single_example.cpp
                              ${BULLET_PHYSICS_SOURCE_DIR}/build3/bullet.rc)
target_link_libraries(${TARGET_NAME} BasicDemo OpenGLWindow Utils)
