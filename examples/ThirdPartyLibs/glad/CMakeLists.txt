set(TARGET_NAME glad)
add_library(${TARGET_NAME} gl.c)
target_include_directories(${TARGET_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
