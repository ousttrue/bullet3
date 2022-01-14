set(TARGET_NAME TinyRenderer)
add_library(${TARGET_NAME} geometry.cpp model.cpp tgaimage.cpp our_gl.cpp
                           TinyRenderer.cpp)
target_link_libraries(${TARGET_NAME} PRIVATE Bullet3Common CommonInterfaces)
target_include_directories(${TARGET_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/..)
