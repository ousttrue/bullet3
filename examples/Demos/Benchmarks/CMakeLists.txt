set(TARGET_NAME Benchmarks)
add_library(${TARGET_NAME} BenchmarkDemo.cpp BenchmarkDemo.h landscapeData.h
                           TaruData.h)
target_link_libraries(${TARGET_NAME} PRIVATE BulletDynamics CommonInterfaces)
