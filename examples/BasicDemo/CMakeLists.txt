# App_BasicExample is a minimal sample creating, stepping and deleting a Bullet
# dynamics world
set(TARGET_NAME App_BasicExample)

add_executable(${TARGET_NAME} BasicExample.cpp main.cpp
                              ${BULLET_PHYSICS_SOURCE_DIR}/build3/bullet.rc)
target_compile_options(${TARGET_NAME} PRIVATE /wd4305)
target_link_libraries(${TARGET_NAME} PRIVATE BulletDynamics BulletCollision
                                             LinearMath)

# ##############################################################################
# Standalone BasicExampleGui using OpenGL (but not the example browser)
set(TARGET_NAME AppBasicExampleGui)
add_executable(
  ${TARGET_NAME}
  BasicExample.cpp
  ${BULLET_PHYSICS_SOURCE_DIR}/build3/bullet.rc
  ../StandaloneMain/main_opengl_single_example.cpp
  ../ExampleBrowser/OpenGLGuiHelper.cpp
  ../ExampleBrowser/GL_ShapeDrawer.cpp
  ../ExampleBrowser/CollisionShape2TriangleMesh.cpp
  ../Utils/b3Clock.cpp)

target_include_directories(
  ${TARGET_NAME}
  PRIVATE ${BULLET_PHYSICS_SOURCE_DIR}/btgui
          ${BULLET_PHYSICS_SOURCE_DIR}/examples
          ${BULLET_PHYSICS_SOURCE_DIR}/examples/ThirdPartyLibs/glad)

# this define maps StandaloneExampleCreateFunc to the right 'CreateFunc'
target_compile_definitions(${TARGET_NAME} PRIVATE B3_USE_STANDALONE_EXAMPLE)
target_link_libraries(${TARGET_NAME} BulletDynamics BulletCollision LinearMath
                      OpenGLWindow Bullet3Common)

# # some code to support OpenGL and Glew cross platform if(WIN32)
# include_directories(
# ${BULLET_PHYSICS_SOURCE_DIR}/btgui/OpenGLWindow/GlewWindows)
# add_definitions(-DGLEW_STATIC) link_libraries(${OPENGL_gl_LIBRARY}
# ${OPENGL_glu_LIBRARY}) else(WIN32) if(APPLE) find_library(COCOA NAMES Cocoa)
# message(${COCOA}) link_libraries(${COCOA} ${OPENGL_gl_LIBRARY}
# ${OPENGL_glu_LIBRARY})

# else(APPLE) include_directories(
# ${BULLET_PHYSICS_SOURCE_DIR}/btgui/OpenGLWindow/GlewWindows)
# add_definitions("-DGLEW_INIT_OPENGL11_FUNCTIONS=1")
# add_definitions("-DGLEW_STATIC")
# add_definitions("-DGLEW_DYNAMIC_LOAD_ALL_GLX_FUNCTIONS=1")

# find_package(Threads) link_libraries(${CMAKE_THREAD_LIBS_INIT} ${DL})
# endif(APPLE) endif(WIN32)

# if(INTERNAL_ADD_POSTFIX_EXECUTABLE_NAMES)
# set_target_properties(AppBasicExampleGui PROPERTIES DEBUG_POSTFIX "_Debug")
# set_target_properties(AppBasicExampleGui PROPERTIES MINSIZEREL_POSTFIX
# "_MinsizeRel") set_target_properties(AppBasicExampleGui PROPERTIES
# RELWITHDEBINFO_POSTFIX "_RelWithDebugInfo")
# endif(INTERNAL_ADD_POSTFIX_EXECUTABLE_NAMES)
