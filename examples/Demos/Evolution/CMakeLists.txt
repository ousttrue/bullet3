set(TARGET_NAME Evolution)
add_library(${TARGET_NAME} NN3DWalkers.cpp NN3DWalkers.h
                           NN3DWalkersTimeWarpBase.h)
target_link_libraries(${TARGET_NAME} PRIVATE BulletDynamics CommonInterfaces)
