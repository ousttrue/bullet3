set(TARGET_NAME BussIK)
file(GLOB BussIK_SRCS "*.cpp")
file(GLOB BussIK_HDRS "*.h")
add_library(${TARGET_NAME} ${BussIK_SRCS} ${BussIK_HDRS})
target_include_directories(${TARGET_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/..)
