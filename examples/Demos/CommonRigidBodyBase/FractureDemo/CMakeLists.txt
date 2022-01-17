set(TARGET_NAME FractureDemo)
add_library(
  ${TARGET_NAME}
  FractureDemo.cpp btFractureBody.cpp btFractureDynamicsWorld.cpp
  FractureDemo.h btFractureBody.h btFractureDynamicsWorld.h)
target_link_libraries(${TARGET_NAME} PRIVATE CommonRigidBodyBase)
