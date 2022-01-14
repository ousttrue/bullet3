set(TARGET_NAME Utils)
add_library(
  ${TARGET_NAME}
  b3Clock.cpp
  b3Clock.h
  ChromeTraceUtil.cpp
  ChromeTraceUtil.h
  b3ResourcePath.cpp
  b3ResourcePath.h
  b3ERPCFMHelper.hpp
  b3ReferenceFrameHelper.hpp
  RobotLoggingUtil.cpp
  RobotLoggingUtil.h
  urdfStringSplit.cpp
  urdfStringSplit.h)
target_link_libraries(${TARGET_NAME} PRIVATE Bullet3Common BulletDynamics)
target_include_directories(${TARGET_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
