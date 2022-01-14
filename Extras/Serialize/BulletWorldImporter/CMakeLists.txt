set(TARGET_NAME BulletWorldImporter)
add_library(
  ${TARGET_NAME} btMultiBodyWorldImporter.cpp btBulletWorldImporter.cpp
                 btBulletWorldImporter.h btWorldImporter.cpp btWorldImporter.h)
set_target_properties(${TARGET_NAME} PROPERTIES VERSION ${BULLET_VERSION})
set_target_properties(${TARGET_NAME} PROPERTIES SOVERSION ${BULLET_VERSION})
target_link_libraries(${TARGET_NAME} PRIVATE BulletDynamics BulletCollision
                                             BulletFileLoader LinearMath)
