set(TARGET_NAME BulletFileLoader)
set(BulletFileLoader_SRCS bChunk.cpp bDNA.cpp bFile.cpp btBulletFile.cpp)
set(BulletFileLoader_HDRS bChunk.h bCommon.h bDefines.h bDNA.h bFile.h
                          btBulletFile.h)
add_library(${TARGET_NAME} ${BulletFileLoader_SRCS} ${BulletFileLoader_HDRS})
target_link_libraries(${TARGET_NAME} PRIVATE LinearMath)
set_target_properties(${TARGET_NAME} PROPERTIES VERSION ${BULLET_VERSION})
set_target_properties(${TARGET_NAME} PROPERTIES SOVERSION ${BULLET_VERSION})
