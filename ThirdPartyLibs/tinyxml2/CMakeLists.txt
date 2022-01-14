set(TARGET_NAME tinyxml2)
add_library(${TARGET_NAME} tinyxml2.cpp)
target_include_directories(${TARGET_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
