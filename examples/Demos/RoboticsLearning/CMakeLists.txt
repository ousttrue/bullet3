set(TARGET_NAME RoboticsLearning)
add_library(
  ${TARGET_NAME}
  GripperGraspExample.cpp GripperGraspExample.h R2D2GraspExample.cpp
  R2D2GraspExample.h KukaGraspExample.cpp KukaGraspExample.h)
target_link_libraries(${TARGET_NAME} PRIVATE BulletDynamics CommonInterfaces SharedMemory)
