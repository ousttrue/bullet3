# HelloWorld is a minimal sample creating, stepping and deleting a Bullet
# dynamics world
set(TARGET_NAME App_HelloWorld)
add_executable(${TARGET_NAME} HelloWorld.cpp
                              ${BULLET_PHYSICS_SOURCE_DIR}/build3/bullet.rc)
target_link_libraries(${TARGET_NAME} PRIVATE BulletDynamics BulletCollision
                                             LinearMath)
