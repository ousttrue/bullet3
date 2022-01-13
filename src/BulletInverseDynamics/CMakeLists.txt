set(TARGET_NAME BulletInverseDynamics)

add_library(
  BulletInverseDynamics
  IDMath.cpp
  MultiBodyTree.cpp
  details/MultiBodyTreeInitCache.cpp
  details/MultiBodyTreeImpl.cpp
  IDConfig.hpp
  IDConfigEigen.hpp
  IDMath.hpp
  IDConfigBuiltin.hpp
  IDErrorMessages.hpp
  MultiBodyTree.hpp
  details/IDEigenInterface.hpp
  details/IDMatVec.hpp
  details/IDLinearMathInterface.hpp
  details/MultiBodyTreeImpl.hpp
  details/MultiBodyTreeInitCache.hpp)
target_compile_options(${TARGET_NAME} PRIVATE /wd4244 /wd4305)
target_link_libraries(${TARGET_NAME} PUBLIC Bullet3Common LinearMath)
set_target_properties(${TARGET_NAME} PROPERTIES VERSION ${BULLET_VERSION})
set_target_properties(${TARGET_NAME} PROPERTIES SOVERSION ${BULLET_VERSION})
