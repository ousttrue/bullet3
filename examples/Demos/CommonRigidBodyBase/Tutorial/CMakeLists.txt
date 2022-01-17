set(TARGET_NAME Tutorial)
add_library(${TARGET_NAME} Tutorial.cpp Tutorial.h Dof6ConstraintTutorial.cpp
                           Dof6ConstraintTutorial.h)
target_link_libraries(${TARGET_NAME} PRIVATE CommonRigidBodyBase stb_image)
