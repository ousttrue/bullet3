set(TARGET_NAME gtest)
add_library(${TARGET_NAME} src/gtest-all.cc)
target_include_directories(
  ${TARGET_NAME}
  PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_compile_definitions(${TARGET_NAME} PUBLIC _VARIADIC_MAX=10)
