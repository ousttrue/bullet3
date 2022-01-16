set(TARGET_NAME SharedMemoryDemo)
add_library(${TARGET_NAME} GraphicsClientExample.cpp GraphicsServerExample.cpp
                           PhysicsClientExample.cpp)
target_link_libraries(${TARGET_NAME} PRIVATE CommonInterfaces SharedMemory
                                             Utils MultiThreading)
target_include_directories(${TARGET_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
