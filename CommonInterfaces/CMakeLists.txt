set(TARGET_NAME CommonInterfaces)
add_library(${TARGET_NAME} INTERFACE)
target_include_directories(${TARGET_NAME} INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(${TARGET_NAME} INTERFACE Bullet3Common)
