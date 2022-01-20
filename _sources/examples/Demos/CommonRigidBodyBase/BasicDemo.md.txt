# BasicDemo

```{digraph} BasicExample
rankdir="BT"
"BasicExample" -> "CommonRigidBodyBase";
```

```c++
	{
		// create a few dynamic rigidbodies
		// Re-using the same collision is better for memory usage and performance
		btBoxShape* colShape = physics->createBoxShape(btVector3(.1, .1, .1));

		/// Create Dynamic Objects
		btTransform startTransform;
		startTransform.setIdentity();

		//rigidbody is dynamic if and only if mass is non zero, otherwise static
		btScalar mass(1.f);
		bool isDynamic = (mass != 0.f);

		btVector3 localInertia(0, 0, 0);
		if (isDynamic)
		{
			colShape->calculateLocalInertia(mass, localInertia);
		}

		for (int k = 0; k < ARRAY_SIZE_Y; k++)
		{
			for (int i = 0; i < ARRAY_SIZE_X; i++)
			{
				for (int j = 0; j < ARRAY_SIZE_Z; j++)
				{
					startTransform.setOrigin(btVector3(
						btScalar(0.2 * i),
						btScalar(2 + .2 * k),
						btScalar(0.2 * j)));

					physics->createRigidBody(mass, startTransform, colShape);
				}
			}
		}
	}
```
