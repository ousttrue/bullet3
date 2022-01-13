set(TARGET_NAME Constraints)
add_library(
  ${TARGET_NAME}
  TestHingeTorque.cpp
  TestHingeTorque.h
  ConstraintDemo.cpp
  ConstraintDemo.h
  Dof6Spring2Setup.cpp
  Dof6Spring2Setup.h
  ConstraintPhysicsSetup.cpp
  ConstraintPhysicsSetup.h)
target_link_libraries(${TARGET_NAME} PRIVATE BulletDynamics CommonInterfaces)