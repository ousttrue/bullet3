set(TARGET_NAME CommonInterfaces)
add_library(${TARGET_NAME} CommonGraphicsAppInterface.cpp)
target_include_directories(${TARGET_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(${TARGET_NAME} PUBLIC Bullet3Common)
