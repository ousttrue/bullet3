set(TARGET_NAME RigidBody)
add_library(${TARGET_NAME} RigidBodySoftContact.cpp
                           KinematicRigidBodyExample.cpp)
target_link_libraries(${TARGET_NAME} PRIVATE CommonRigidBodyBase OpenGLWindow)
target_compile_definitions(${TARGET_NAME} PRIVATE USE_MOTIONSTATE)
