set(TARGET_NAME GLFont)
add_library(${TARGET_NAME} TwFonts.cpp)
target_include_directories(${TARGET_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
