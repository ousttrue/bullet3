set(TARGET_NAME RollingFrictionDemo)
add_library(${TARGET_NAME} RollingFrictionDemo.cpp RollingFrictionDemo.h)
target_link_libraries(${TARGET_NAME} PRIVATE BulletDynamics CommonInterfaces Utils)
