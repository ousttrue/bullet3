set(TARGET_NAME MultiThreadedDemo)
add_library(
  ${TARGET_NAME}
  MultiThreadedDemo.cpp MultiThreadedDemo.h CommonRigidBodyMTBase.cpp
  CommonRigidBodyMTBase.h MultiThreadingExample.cpp)
target_link_libraries(${TARGET_NAME} PRIVATE BulletDynamics CommonInterfaces stb_image MultiThreading)
