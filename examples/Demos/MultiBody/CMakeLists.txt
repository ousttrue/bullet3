set(TARGET_NAME MultiBody)
add_library(
  ${TARGET_NAME}
  Pendulum.cpp
  MultiBodySoftContact.cpp
  TestJointTorqueSetup.cpp
  TestJointTorqueSetup.h
  InvertedPendulumPDControl.cpp
  InvertedPendulumPDControl.h
  MultiBodyConstraintFeedback.cpp
  KinematicMultiBodyExample.cpp
  MultiDofDemo.cpp
  MultiDofDemo.h)
target_link_libraries(${TARGET_NAME} PRIVATE BulletDynamics CommonInterfaces OpenGLWindow)
