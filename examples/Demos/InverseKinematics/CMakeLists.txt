set(TARGET_NAME InverseKinematics)
add_library(${TARGET_NAME} InverseKinematicsExample.cpp
                           InverseKinematicsExample.h)
target_link_libraries(${TARGET_NAME} PRIVATE BulletDynamics CommonInterfaces BussIK)
