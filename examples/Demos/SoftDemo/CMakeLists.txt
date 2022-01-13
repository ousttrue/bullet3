set(TARGET_NAME SoftDemo)
add_library(${TARGET_NAME} SoftDemo.cpp SoftDemo.h)
target_link_libraries(${TARGET_NAME} PRIVATE BulletDynamics CommonInterfaces)
