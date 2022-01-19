# btIDebugDraw

<https://pybullet.org/Bullet/BulletFull/classbtIDebugDraw.html>

* (2017)[Bullet Physicsを使ってみる　(Bullet2.86 VS2017 DirectX11)](https://zerogram.info/blog/?p=2142)

```c++
btIDebugDraw* debugDrawer;

btDiscreteDynamicsWorld world;
world.setDebugDrawer(debugDrawer);
world.debugDrawWorld();
```
