set(TARGET_NAME ForkLift)
add_library(${TARGET_NAME} ForkLiftDemo.cpp ForkLiftDemo.h)
target_link_libraries(${TARGET_NAME} PRIVATE BulletDynamics CommonInterfaces)
