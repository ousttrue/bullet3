set(TARGET_NAME Test_BulletDynamics)
add_executable(${TARGET_NAME} ../../../examples/Demos/MultiBody/Pendulum.cpp)
target_compile_definitions(${TARGET_NAME} PRIVATE USE_GTEST _VARIADIC_MAX=10)
target_link_libraries(${TARGET_NAME} PRIVATE BulletDynamics BulletCollision
                                             LinearMath gtest CommonInterfaces)
