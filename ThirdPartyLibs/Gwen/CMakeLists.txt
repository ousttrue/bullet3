set(TARGET_NAME gwen)
file(
  GLOB
  gwen_SRCS
  "Gwen/*.cpp"
  "Gwen/Controls/*.cpp"
  "Gwen/Controls/Dialog/*.cpp"
  "Gwen/Controls/Dialogs/*.cpp"
  "Gwen/Controls/Layout/*.cpp"
  "Gwen/Controls/Property/*.cpp"
  "Gwen/Input/*.cpp"
  "Gwen/Platforms/*.cpp"
  "Gwen/Renderers/*.cpp"
  "Gwen/Skins/*.cpp")
file(
  GLOB
  gwen_HDRS
  "Gwen/*.h"
  "Gwen/Controls/*.h"
  "Gwen/Controls/Dialog/*.h"
  "Gwen/Controls/Dialogs/*.h"
  "Gwen/Controls/Layout/*.h"
  "Gwen/Controls/Property/*.h"
  "Gwen/Input/*.h"
  "Gwen/Platforms/*.h"
  "Gwen/Renderers/*.h"
  "Gwen/Skins/*.h")
add_library(${TARGET_NAME} SHARED ${gwen_SRCS} ${gwen_HDRS})
target_compile_options(${TARGET_NAME} PRIVATE /wd4267)
target_compile_definitions(${TARGET_NAME} PRIVATE GWEN_COMPILE_STATIC
                                                  _HAS_EXCEPTIONS=0)
target_include_directories(${TARGET_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(${TARGET_NAME} PUBLIC glad)
