set(TARGET_NAME Vehicles)
add_library(${TARGET_NAME} Hinge2Vehicle.cpp Hinge2Vehicle.h)
target_link_libraries(${TARGET_NAME} PRIVATE BulletDynamics CommonInterfaces)
