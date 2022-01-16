set(TARGET_NAME InverseDynamics)
add_library(${TARGET_NAME} InverseDynamicsExample.cpp InverseDynamicsExample.h)
target_link_libraries(${TARGET_NAME} PRIVATE CommonRigidBodyBase Utils UrdfImporter)
