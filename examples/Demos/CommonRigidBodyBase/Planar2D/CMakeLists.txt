set(TARGET_NAME Planar2D)
add_library(${TARGET_NAME} Planar2D.cpp Planar2D.h)
target_link_libraries(${TARGET_NAME} PRIVATE CommonRigidBodyBase)
