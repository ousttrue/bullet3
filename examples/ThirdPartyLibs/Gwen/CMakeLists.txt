set(TARGET_NAME gwen)
file(
  GLOB
  gwen_SRCS
  "*.cpp"
  "Controls/*.cpp"
  "Controls/Dialog/*.cpp"
  "Controls/Dialogs/*.cpp"
  "Controls/Layout/*.cpp"
  "Controls/Property/*.cpp"
  "Input/*.cpp"
  "Platforms/*.cpp"
  "Renderers/*.cpp"
  "Skins/*.cpp")
file(
  GLOB
  gwen_HDRS
  "*.h"
  "Controls/*.h"
  "Controls/Dialog/*.h"
  "Controls/Dialogs/*.h"
  "Controls/Layout/*.h"
  "Controls/Property/*.h"
  "Input/*.h"
  "Platforms/*.h"
  "Renderers/*.h"
  "Skins/*.h")
add_library(${TARGET_NAME} ${gwen_SRCS} ${gwen_HDRS})
target_compile_options(${TARGET_NAME} PRIVATE /wd4267)
target_compile_definitions(${TARGET_NAME} PRIVATE GWEN_COMPILE_STATIC
                                                  _HAS_EXCEPTIONS=0)
target_include_directories(${TARGET_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/..)
target_link_libraries(${TARGET_NAME} PUBLIC glad)
