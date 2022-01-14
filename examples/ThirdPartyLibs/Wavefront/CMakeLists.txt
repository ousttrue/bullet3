set(TARGET_NAME tiny_obj_loader)
add_library(${TARGET_NAME} tiny_obj_loader.cpp)
target_include_directories(${TARGET_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(${TARGET_NAME} PRIVATE CommonInterfaces)
