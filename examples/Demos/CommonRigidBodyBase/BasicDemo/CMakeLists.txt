set(TARGET_NAME BasicDemo)
add_library(${TARGET_NAME} BasicExample.cpp BasicExample.h)
target_link_libraries(${TARGET_NAME} PRIVATE CommonRigidBodyBase)

# ##############################################################################
# App_BasicExample is a minimal sample creating, stepping and deleting a Bullet
# dynamics world
set(TARGET_NAME App_BasicExample)

add_executable(${TARGET_NAME} main.cpp
                              ${BULLET_PHYSICS_SOURCE_DIR}/build3/bullet.rc)
target_link_libraries(${TARGET_NAME} PRIVATE BasicDemo CommonInterfaces)

# # ##############################################################################
# # Standalone BasicExampleGui using OpenGL (but not the example browser)
# set(TARGET_NAME App_BasicExampleGui)
# add_executable(
#   ${TARGET_NAME}
#   CreateFunc.cpp ${BULLET_PHYSICS_SOURCE_DIR}/build3/bullet.rc
#   ../../StandaloneMain/main_opengl_single_example.cpp)
# target_link_libraries(${TARGET_NAME} BasicDemo OpenGLWindow Utils)
