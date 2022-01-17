set(TARGET_NAME ExtendedTutorials)
add_library(
  ${TARGET_NAME}
  Chain.cpp
  Chain.h
  Bridge.cpp
  Bridge.h
  RigidBodyFromObj.cpp
  RigidBodyFromObj.h
  SimpleBox.cpp
  SimpleBox.h
  MultipleBoxes.cpp
  MultipleBoxes.h
  CompoundBoxes.cpp
  CompoundBoxes.h
  SimpleCloth.cpp
  SimpleCloth.h
  SimpleJoint.cpp
  SimpleJoint.h
  NewtonsCradle.cpp
  NewtonsCradle.h
  InclinedPlane.cpp
  InclinedPlane.h
  MultiPendulum.cpp
  MultiPendulum.h
  NewtonsRopeCradle.cpp
  NewtonsRopeCradle.h)
target_link_libraries(${TARGET_NAME} PRIVATE CommonRigidBodyBase UrdfImporter)
target_include_directories(${TARGET_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/..)
