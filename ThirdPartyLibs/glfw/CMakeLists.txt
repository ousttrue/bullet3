set(TARGET_NAME glfw)
set(GLFW_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../_external/glfw)
add_library(
  ${TARGET_NAME}
  ${TARGET_DIR}/
  ${GLFW_SOURCE_DIR}/include/GLFW/glfw3.h
  ${GLFW_SOURCE_DIR}/include/GLFW/glfw3native.h
  ${GLFW_SOURCE_DIR}/src/internal.h
  ${GLFW_SOURCE_DIR}/src/platform.h
  ${GLFW_SOURCE_DIR}/src/mappings.h
  ${GLFW_SOURCE_DIR}/src/context.c
  ${GLFW_SOURCE_DIR}/src/init.c
  ${GLFW_SOURCE_DIR}/src/input.c
  ${GLFW_SOURCE_DIR}/src/monitor.c
  ${GLFW_SOURCE_DIR}/src/platform.c
  ${GLFW_SOURCE_DIR}/src/vulkan.c
  ${GLFW_SOURCE_DIR}/src/window.c
  ${GLFW_SOURCE_DIR}/src/egl_context.c
  ${GLFW_SOURCE_DIR}/src/osmesa_context.c
  ${GLFW_SOURCE_DIR}/src/null_platform.h
  ${GLFW_SOURCE_DIR}/src/null_joystick.h
  ${GLFW_SOURCE_DIR}/src/null_init.c
  ${GLFW_SOURCE_DIR}/src/null_monitor.c
  ${GLFW_SOURCE_DIR}/src/null_window.c
  ${GLFW_SOURCE_DIR}/src/null_joystick.c
  ${GLFW_SOURCE_DIR}/src/win32_time.h
  ${GLFW_SOURCE_DIR}/src/win32_thread.h
  ${GLFW_SOURCE_DIR}/src/win32_module.c
  ${GLFW_SOURCE_DIR}/src/win32_time.c
  ${GLFW_SOURCE_DIR}/src/win32_thread.c
  ${GLFW_SOURCE_DIR}/src/win32_platform.h
  ${GLFW_SOURCE_DIR}/src/win32_joystick.h
  ${GLFW_SOURCE_DIR}/src/win32_init.c
  ${GLFW_SOURCE_DIR}/src/win32_joystick.c
  ${GLFW_SOURCE_DIR}/src/win32_monitor.c
  ${GLFW_SOURCE_DIR}/src/win32_window.c
  ${GLFW_SOURCE_DIR}/src/wgl_context.c)
target_include_directories(${TARGET_NAME} PUBLIC ${GLFW_SOURCE_DIR}/include)

add_custom_target(
  update_mappings
  COMMAND
    "${CMAKE_COMMAND}" -P "${GLFW_SOURCE_DIR}/CMake/GenerateMappings.cmake"
    ${GLFW_SOURCE_DIR}/src/mappings.h.in ${GLFW_SOURCE_DIR}/src/mappings.h
  WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
  COMMENT "Updating gamepad mappings from upstream repository"
  SOURCES ${GLFW_SOURCE_DIR}/src/mappings.h.in
          "${GLFW_SOURCE_DIR}/CMake/GenerateMappings.cmake"
  VERBATIM)

set_target_properties(update_mappings PROPERTIES FOLDER "GLFW3")
target_compile_definitions(glfw PRIVATE _GLFW_WIN32)
target_sources(glfw PRIVATE)
target_link_libraries(${TARGET_NAME} PUBLIC OpenGL32.lib)
