set(TARGET_NAME GyroscopicDemo)
add_library(${TARGET_NAME} GyroscopicSetup.cpp GyroscopicSetup.h)
target_link_libraries(${TARGET_NAME} PRIVATE BulletDynamics CommonInterfaces)
