#pragma once
#include "Bullet3Common/b3AlignedObjectArray.h"
#include <CommonRenderInterface.h>
#include "SimpleCamera.h"
#include <GLVBO.h>
#include <Bullet3Common/b3Vector3.h>
#include <Bullet3Common/b3ResizablePool.h>

struct b3PublicGraphicsInstanceData
{
	int m_shapeIndex;
	int m_internalInstanceIndex;
	float m_position[4];
	float m_orientation[4];
	float m_color[4];
	float m_scale[4];

	void clear()
	{
	}
};

using b3PublicGraphicsInstance = b3PoolBodyHandle<b3PublicGraphicsInstanceData>;

struct InternalTextureHandle
{
	unsigned int m_glTexture;
	int m_width;
	int m_height;
	int m_enableFiltering;
};

struct InternalDataRenderer
{
	b3AlignedObjectArray<float> m_instance_positions_ptr;
	b3AlignedObjectArray<float> m_instance_quaternion_ptr;
	b3AlignedObjectArray<float> m_instance_colors_ptr;
	b3AlignedObjectArray<float> m_instance_scale_ptr;

	int m_vboSize;
	std::shared_ptr<GLVBO> m_vbo;
	int m_totalNumInstances;
	int m_maxNumObjectCapacity;
	int m_maxShapeCapacityInBytes;

	SimpleCamera m_defaultCamera1;
	CommonCameraInterface* m_activeCamera;

	float m_projectionMatrix[16] = {0};
	float m_viewMatrix[16] = {0};
	float m_projectiveTextureProjectionMatrix[16] = {0};
	float m_projectiveTextureViewMatrix[16] = {0};
	float m_viewMatrixInverse[16] = {0};
	bool m_useProjectiveTexture = false;

	b3Vector3 m_lightPos = b3MakeVector3(-50, 30, 40);
	b3Vector3 m_lightSpecularIntensity = b3MakeVector3(1, 1, 1);
	float m_shadowmapIntensity = 0.3f;
	unsigned int m_defaultTexturehandle;
	b3AlignedObjectArray<InternalTextureHandle> m_textureHandles;

	class GLRenderToTexture* m_shadowMap = nullptr;
	unsigned int m_shadowTexture = 0;

	unsigned int m_renderFrameBuffer = 0;

	b3ResizablePool<b3PublicGraphicsInstance> m_publicGraphicsInstances;

	int m_shadowMapWidth = 4096;
	int m_shadowMapHeight = 4096;
	float m_shadowMapWorldSize = 10;
	bool m_updateShadowMap = true;

	InternalDataRenderer() : m_activeCamera(&m_defaultCamera1)
	{
	}
};

class GLInstancingRenderer : public CommonRenderInterface
{
	b3AlignedObjectArray<struct b3GraphicsInstance*> m_graphicsInstances;

	struct InternalDataRenderer* m_data;

	bool m_textureenabled;
	bool m_textureinitialized;

	int m_planeReflectionShapeIndex;

	int registerGraphicsInstanceInternal(int shapeIndex, const float* position, const float* quaternion, const float* color, const float* scaling);
	void rebuildGraphicsInstances();

public:
	GLInstancingRenderer(int m_maxObjectCapacity, int maxShapeCapacityInBytes = 56 * 1024 * 1024);
	virtual ~GLInstancingRenderer();

	virtual void init();

	void renderScene() override;
	virtual void renderSceneInternal(int orgRenderMode = B3_DEFAULT_RENDERMODE);

	void InitShaders();
	void CleanupShaders();
	virtual void removeAllInstances();
	virtual void removeGraphicsInstance(int instanceUid);

	virtual void updateShape(int shapeIndex, const float* vertices, int numVertices);

	///vertices must be in the format x,y,z, nx,ny,nz, u,v
	virtual int registerShape(const float* vertices, int numvertices, const int* indices, int numIndices, int primitiveType = B3_GL_TRIANGLES, int textureIndex = -1);

	virtual int registerTexture(const unsigned char* texels, int width, int height, bool flipPixelsY = true);
	virtual void updateTexture(int textureIndex, const unsigned char* texels, bool flipPixelsY = true);
	virtual void activateTexture(int textureIndex);
	virtual void replaceTexture(int shapeIndex, int textureId);
	virtual int getShapeIndexFromInstance(int srcIndex);
	virtual void removeTexture(int textureIndex);

	///position x,y,z, quaternion x,y,z,w, color r,g,b,a, scaling x,y,z
	virtual int registerGraphicsInstance(int shapeIndex, const float* position, const float* quaternion, const float* color, const float* scaling);
	virtual int registerGraphicsInstance(int shapeIndex, const double* position, const double* quaternion, const double* color, const double* scaling);

	void writeTransforms();

	virtual bool readSingleInstanceTransformToCPU(float* position, float* orientation, int srcIndex);

	virtual void writeSingleInstanceTransformToCPU(const float* position, const float* orientation, int srcIndex);
	virtual void writeSingleInstanceTransformToCPU(const double* position, const double* orientation, int srcIndex)
	{
		float pos[4];
		float orn[4];
		pos[0] = (float)position[0];
		pos[1] = (float)position[1];
		pos[2] = (float)position[2];
		pos[3] = (float)position[3];
		orn[0] = (float)orientation[0];
		orn[1] = (float)orientation[1];
		orn[2] = (float)orientation[2];
		orn[3] = (float)orientation[3];
		writeSingleInstanceTransformToCPU(pos, orn, srcIndex);
	}

	virtual void readSingleInstanceTransformFromCPU(int srcIndex, float* position, float* orientation);

	virtual void writeSingleInstanceTransformToGPU(float* position, float* orientation, int srcIndex);

	virtual void writeSingleInstanceColorToCPU(const float* color, int srcIndex);
	virtual void writeSingleInstanceColorToCPU(const double* color, int srcIndex);
	virtual void writeSingleInstanceFlagsToCPU(int flags, int srcIndex2);

	virtual void writeSingleInstanceSpecularColorToCPU(const double* specular, int srcIndex2);
	virtual void writeSingleInstanceSpecularColorToCPU(const float* specular, int srcIndex2);

	virtual void writeSingleInstanceScaleToCPU(const float* scale, int srcIndex);
	virtual void writeSingleInstanceScaleToCPU(const double* scale, int srcIndex);

	virtual void drawLine(const float from[4], const float to[4], const float color[4], float lineWidth = 1);
	virtual void drawLine(const double from[4], const double to[4], const double color[4], double lineWidth = 1);
	virtual void drawLines(const float* positions, const float color[4], int numPoints, int pointStrideInBytes, const unsigned int* indices, int numIndices, float pointDrawSize);
	virtual void drawPoints(const float* positions, const float color[4], int numPoints, int pointStrideInBytes, float pointDrawSize);
	virtual void drawPoint(const float* position, const float color[4], float pointSize = 1);
	virtual void drawPoint(const double* position, const double color[4], double pointDrawSize = 1);
	virtual void drawTexturedTriangleMesh(float worldPosition[3], float worldOrientation[4], const float* vertices, int numvertices, const unsigned int* indices, int numIndices, float color[4], int textureIndex = -1, int vertexLayout = 0);

	virtual void updateCamera();

	virtual const CommonCameraInterface* getActiveCamera() const;
	virtual CommonCameraInterface* getActiveCamera();
	virtual void setActiveCamera(CommonCameraInterface* cam);

	virtual void setLightPosition(const float lightPos[3]);
	virtual void setLightPosition(const double lightPos[3]);
	virtual void setShadowMapResolution(int shadowMapResolution);
	virtual void setShadowMapIntensity(double shadowMapIntensity);
	virtual void setShadowMapWorldSize(float worldSize);
	void setLightSpecularIntensity(const float lightSpecularIntensity[3]);
	virtual void setProjectiveTextureMatrices(const float viewMatrix[16], const float projectionMatrix[16]);
	virtual void setProjectiveTexture(bool useProjectiveTexture);
	virtual void setBackgroundColor(const double rgbBackground[3]);
	virtual void resize(int width, int height);

	virtual int getMaxShapeCapacity() const;

	virtual int getInstanceCapacity() const;

	virtual int getTotalNumInstances() const;

	virtual void enableShadowMap();

	virtual void setPlaneReflectionShapeIndex(int index);

	virtual void clearZBuffer();

	virtual void setRenderFrameBuffer(unsigned int renderFrameBuffer);
};
