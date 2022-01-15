set(TARGET_NAME Raycast)
add_library(${TARGET_NAME} RaytestDemo.cpp)
target_link_libraries(${TARGET_NAME} PRIVATE CommonRigidBodyBase)
