set(TARGET_NAME Heightfield)
add_library(${TARGET_NAME} HeightfieldExample.cpp HeightfieldExample.h)
target_link_libraries(${TARGET_NAME} PRIVATE CommonRigidBodyBase stb_image
                                             OpenGLWindow Utils)
