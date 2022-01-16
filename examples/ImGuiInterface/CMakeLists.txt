set(TARGET_NAME ImGuiInterface)
add_library(${TARGET_NAME} ImGuiInterface.cpp)
target_link_libraries(${TARGET_NAME} PRIVATE CommonInterfaces)
target_include_directories(${TARGET_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
