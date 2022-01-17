set(TARGET_NAME BasicDemo)
add_library(${TARGET_NAME} BasicExample.cpp BasicExample.h)
target_link_libraries(${TARGET_NAME} PUBLIC CommonRigidBodyBase)

# ##############################################################################
# App_BasicExample is a minimal sample creating, stepping and deleting a Bullet
# dynamics world
set(TARGET_NAME App_BasicExample)

add_executable(${TARGET_NAME} main.cpp
                              ${BULLET_PHYSICS_SOURCE_DIR}/build3/bullet.rc)
target_link_libraries(${TARGET_NAME} PRIVATE BasicDemo CommonInterfaces)
