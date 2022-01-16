set(TARGET_NAME UrdfParser)
add_library(${TARGET_NAME} UrdfParser.cpp)
target_include_directories(${TARGET_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(${TARGET_NAME} PUBLIC LinearMath Utils)
