set(TARGET_NAME MultiThreading)
add_library(${TARGET_NAME} b3PosixThreadSupport.cpp b3Win32ThreadSupport.cpp
                           b3ThreadSupportInterface.cpp)
target_link_libraries(${TARGET_NAME} PRIVATE BulletDynamics CommonInterfaces)
target_include_directories(${TARGET_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
