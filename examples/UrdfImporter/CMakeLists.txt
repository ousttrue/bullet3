set(TARGET_NAME UrdfImporter)
add_library(${TARGET_NAME} BulletUrdfImporter.cpp URDF2Bullet.cpp
                           MyMultiBodyCreator.cpp BulletMJCFImporter.cpp)
target_link_libraries(${TARGET_NAME} PUBLIC UrdfParser tiny_obj_loader
                                            OpenGLWindow tinyxml2)
target_include_directories(${TARGET_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
