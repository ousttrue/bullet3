set(TARGET_NAME BulletRobotics)
add_library(
  ${TARGET_NAME} FixJointBoxes.cpp BoxStack.cpp JointLimit.cpp # GraspBox.cpp
)
target_link_libraries(${TARGET_NAME} PRIVATE CommonInterfaces)
