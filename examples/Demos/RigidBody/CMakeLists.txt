set(TARGET_NAME RigidBody)
add_library(${TARGET_NAME} RigidBodySoftContact.cpp
                           KinematicRigidBodyExample.cpp)
target_link_libraries(${TARGET_NAME} PRIVATE BulletDynamics CommonInterfaces OpenGLWindow)
