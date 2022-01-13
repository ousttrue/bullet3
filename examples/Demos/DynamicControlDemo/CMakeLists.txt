set(TARGET_NAME DynamicControlDemo)
add_library(${TARGET_NAME} MotorDemo.cpp MotorDemo.h)
target_link_libraries(${TARGET_NAME} PRIVATE BulletDynamics CommonInterfaces)
