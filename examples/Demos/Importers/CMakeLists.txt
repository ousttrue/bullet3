set(TARGET_NAME Importers)
add_library(
  ${TARGET_NAME}
  ImportMJCFDemo/ImportMJCFSetup.cpp
  ImportBsp/BspConverter.cpp
  ImportBsp/BspLoader.cpp
  ImportBsp/ImportBspExample.cpp
  ImportColladaDemo/ImportColladaSetup.cpp
  ImportObjDemo/ImportObjExample.cpp
  ImportSTLDemo/ImportSTLSetup.cpp
  ImportSDFDemo/ImportSDFSetup.cpp
  ImportURDFDemo/ImportURDFSetup.cpp
  ImportBullet/SerializeSetup.cpp)
target_link_libraries(
  ${TARGET_NAME}
  PRIVATE CommonRigidBodyBase stb_image OpenGLWindow tinyxml2 Utils
  PUBLIC tiny_obj_loader UrdfParser UrdfImporter)
target_include_directories(${TARGET_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
