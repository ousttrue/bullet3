# GLInstancingRenderer

OpenGL3 で bullet physics の `const *btDiscreteDynamicsWorld` を描画する。

## registerGraphicsShape

shape から graphics を作る

```c++
void OpenGLGuiHelper::autogenerateGraphicsObjects(const btDiscreteDynamicsWorld* rbWorld)
```

```c++
void OpenGLGuiHelper::createCollisionShapeGraphicsObject(btCollisionShape* collisionShape)

		int graphicsShapeIndex = -1;
		int* graphicsShapeIndexPtr = m_data->m_hashShapes[shape];

		if (graphicsShapeIndexPtr)
		{
			graphicsShapeIndex = *graphicsShapeIndexPtr;
		}
		else
		{
			int numVertices = sizeof(cube_vertices_textured) / strideInBytes;
			transformedVertices.resize(numVertices * 9);
			for (int i = 0; i < numVertices; i++)
			{
				btVector3 vert;
				vert.setValue(cube_vertices_textured[i * 9 + 0],
							  cube_vertices_textured[i * 9 + 1],
							  cube_vertices_textured[i * 9 + 2]);

				btVector3 trVer = halfExtents * vert;
				transformedVertices[i * 9 + 0] = trVer[0];
				transformedVertices[i * 9 + 1] = trVer[1];
				transformedVertices[i * 9 + 2] = trVer[2];
				transformedVertices[i * 9 + 3] = cube_vertices_textured[i * 9 + 3];
				transformedVertices[i * 9 + 4] = cube_vertices_textured[i * 9 + 4];
				transformedVertices[i * 9 + 5] = cube_vertices_textured[i * 9 + 5];
				transformedVertices[i * 9 + 6] = cube_vertices_textured[i * 9 + 6];
				transformedVertices[i * 9 + 7] = cube_vertices_textured[i * 9 + 7];
				transformedVertices[i * 9 + 8] = cube_vertices_textured[i * 9 + 8];
			}

			int numIndices = sizeof(cube_indices) / sizeof(int);
			graphicsShapeIndex = registerGraphicsShape(&transformedVertices[0], numVertices, cube_indices, numIndices, B3_GL_TRIANGLES, m_data->m_checkedTextureGrey);
			m_data->m_hashShapes.insert(shape, graphicsShapeIndex);
		}

		collisionShape->setUserIndex(graphicsShapeIndex);
```

```c++
int OpenGLGuiHelper::registerGraphicsShape(const float* vertices, int numvertices, const int* indices, int numIndices, int primitiveType, int textureId)
```

```c++
int GLInstancingRenderer::registerShape(const float* vertices, int numvertices, const int* indices, int numIndices, int primitiveType, int textureId)
```

## transform の同期？

```c++
void OpenGLGuiHelper::syncPhysicsToGraphics(const btDiscreteDynamicsWorld* rbWorld)

			btVector3 pos = colObj->getWorldTransform().getOrigin();
			btQuaternion orn = colObj->getWorldTransform().getRotation();
			int index = colObj->getUserIndex();
			if (index >= 0)
			{
				m_data->m_glApp->m_renderer->writeSingleInstanceTransformToCPU(pos, orn, index);
			}
```

glBufferSubData で TRS をGPU に送り込む

```c++
void GLInstancingRenderer::writeTransforms()
```

## 描画

```c++
void OpenGLGuiHelper::render(const btDiscreteDynamicsWorld* rbWorld)
```

```c++
void GLInstancingRenderer::renderScene()
```

```c++
// マルチパスレンダリング
void GLInstancingRenderer::renderSceneInternal(int orgRenderMode)
```
