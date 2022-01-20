
#  Constraints

## AllConstraintDemo

> AllConstraintDemo shows how to create a constraint, like Hinge or btGenericD6constraint

## ConstraintPhysicsSetup

## TestHingeTorque

`btJointFeedback`

* [情報メディア実験 物理エンジンを使ったアプリケーション開発(4) 拘束条件(Constraint)](http://www.slis.tsukuba.ac.jp/~fujisawa.makoto.fu/lecture/iml/text/4_constraint.html)

## Dof6Spring2Setup

```c++
	btVector3 worldAabbMin(-10000, -10000, -10000);
	btVector3 worldAabbMax(10000, 10000, 10000);
	btBroadphaseInterface* broadphase = new btAxisSweep3(worldAabbMin, worldAabbMax);
	physics = new Physics(broadphase, SolverEnumType::NNCGSOLVER);
```
