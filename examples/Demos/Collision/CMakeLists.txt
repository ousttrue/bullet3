set(TARGET_NAME Collision)
add_library(
  ${TARGET_NAME}
  CollisionSdkC_Api.cpp
  CollisionSdkC_Api.h
  CollisionTutorialBullet2.cpp
  CollisionTutorialBullet2.h
  Internal/Bullet2CollisionSdk.cpp
  Internal/Bullet2CollisionSdk.h
  Internal/CollisionSdkInterface.h
  Internal/RealTimeBullet3CollisionSdk.cpp
  Internal/RealTimeBullet3CollisionSdk.h)
target_link_libraries(${TARGET_NAME} PRIVATE BulletDynamics CommonInterfaces stb_image)
