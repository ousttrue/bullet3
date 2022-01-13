set(TARGET_NAME BulletSoftBody)
add_library(
  ${TARGET_NAME}
  btSoftBody.cpp
  btSoftBodyConcaveCollisionAlgorithm.cpp
  btSoftBodyHelpers.cpp
  btSoftBodyRigidBodyCollisionConfiguration.cpp
  btSoftRigidCollisionAlgorithm.cpp
  btSoftRigidDynamicsWorld.cpp
  btSoftMultiBodyDynamicsWorld.cpp
  btSoftSoftCollisionAlgorithm.cpp
  btDefaultSoftBodySolver.cpp
  btDeformableBackwardEulerObjective.cpp
  btDeformableBodySolver.cpp
  btDeformableMultiBodyConstraintSolver.cpp
  btDeformableContactProjection.cpp
  btDeformableMultiBodyDynamicsWorld.cpp
  btDeformableContactConstraint.cpp
  poly34.cpp
  btSoftBody.h
  btSoftBodyData.h
  btSoftBodyConcaveCollisionAlgorithm.h
  btSoftBodyHelpers.h
  btSoftBodyRigidBodyCollisionConfiguration.h
  btSoftRigidCollisionAlgorithm.h
  btSoftRigidDynamicsWorld.h
  btSoftMultiBodyDynamicsWorld.h
  btSoftSoftCollisionAlgorithm.h
  btSparseSDF.h
  btSoftBodySolvers.h
  btDefaultSoftBodySolver.h
  btCGProjection.h
  btConjugateGradient.h
  btConjugateResidual.h
  btDeformableGravityForce.h
  btDeformableMassSpringForce.h
  btDeformableCorotatedForce.h
  btDeformableNeoHookeanForce.h
  btDeformableLinearElasticityForce.h
  btDeformableLagrangianForce.h
  btPreconditioner.h
  btDeformableBackwardEulerObjective.h
  btDeformableBodySolver.h
  btDeformableMultiBodyConstraintSolver.h
  btDeformableContactProjection.h
  btDeformableMultiBodyDynamicsWorld.h
  btDeformableContactConstraint.h
  btKrylovSolver.h
  poly34.h
  btSoftBodySolverVertexBuffer.h)

set_target_properties(${TARGET_NAME} PROPERTIES VERSION ${BULLET_VERSION})
set_target_properties(${TARGET_NAME} PROPERTIES SOVERSION ${BULLET_VERSION})
target_link_libraries(${TARGET_NAME} PUBLIC BulletDynamics)
