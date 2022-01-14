set(TARGET_NAME stb_image)
add_library(${TARGET_NAME} stb_image_write.cpp stb_image.cpp)
target_include_directories(${TARGET_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
