#ifndef OPENGL_GUI_HELPER_H
#define OPENGL_GUI_HELPER_H
#include <CommonGUIHelperInterface.h>
#include "CommonCameraInterface.h"

class btCollisionShape;
class btTransform;
#include "LinearMath/btAlignedObjectArray.h"
#include <GLInstanceGraphicsShape.h>

struct OpenGLGuiHelper : public GUIHelperInterface
{
	int m_frame = 0;
	struct OpenGLGuiHelperInternalData* m_data;

	OpenGLGuiHelper(struct CommonGraphicsApp* glApp, bool useOpenGL2);
	~OpenGLGuiHelper() override;
	struct CommonRenderInterface* getRenderInterface() override;
	const struct CommonRenderInterface* getRenderInterface() const override;
	void createRigidBodyGraphicsObject(btRigidBody* body, const btVector3& color) override;
	void createCollisionObjectGraphicsObject(btCollisionObject* body, const btVector3& color) override;
	int registerTexture(const unsigned char* texels, int width, int height) override;
	int registerGraphicsShape(const float* vertices, int numvertices, const int* indices, int numIndices, int primitiveType, int textureId) override;
	int registerGraphicsInstance(int shapeIndex, const float* position, const float* quaternion, const float* color, const float* scaling) override;
	void removeAllGraphicsInstances() override;
	void removeGraphicsInstance(int graphicsUid) override;
	void changeInstanceFlags(int instanceUid, int flags) override;
	void changeRGBAColor(int instanceUid, const double rgbaColor[4]) override;
	void changeScaling(int instanceUid, const double scaling[3]) override;
	void changeSpecularColor(int instanceUid, const double specularColor[3]) override;
	void changeTexture(int textureUniqueId, const unsigned char* rgbTexels, int width, int height) override;
	void removeTexture(int textureUid) override;
	int getShapeIndexFromInstance(int instanceUid) override;
	void replaceTexture(int shapeIndex, int textureUid) override;
	void updateShape(int shapeIndex, float* vertices, int numVertices) override;
	void setBackgroundColor(const double rgbBackground[3]) override;
	void createCollisionShapeGraphicsObject(btCollisionShape* collisionShape) override;
	void syncPhysicsToGraphics(const btDiscreteDynamicsWorld* rbWorld) override;
	void render(const btDiscreteDynamicsWorld* rbWorld) override;
	void createPhysicsDebugDrawer(btDiscreteDynamicsWorld* rbWorld) override;
	struct Common2dCanvasInterface* get2dCanvasInterface() override;
	CommonParameterInterface* getParameterInterface() override;
	struct CommonGraphicsApp* getAppInterface() override;
	void copyCameraImageData(const float viewMatrix[16], const float projectionMatrix[16],
							 unsigned char* pixelsRGBA, int rgbaBufferSizeInPixels,
							 float* depthBuffer, int depthBufferSizeInPixels,
							 int* segmentationMaskBuffer, int segmentationMaskBufferSizeInPixels,
							 int startPixelIndex, int destinationWidth,
							 int destinationHeight, int* numPixelsCopied) override;
	void setProjectiveTextureMatrices(const float viewMatrix[16], const float projectionMatrix[16]) override;
	void setProjectiveTexture(bool useProjectiveTexture) override;
	void autogenerateGraphicsObjects(const btDiscreteDynamicsWorld* rbWorld) override;
	int addUserDebugLine(const double debugLineFromXYZ[3], const double debugLineToXYZ[3], const double debugLineColorRGB[3], double lineWidth, double lifeTime, int trackingVisualShapeIndex, int replaceItemUid) override
	{
		return -1;
	}
	int addUserDebugParameter(const char* txt, double rangeMin, double rangeMax, double startValue) override
	{
		return -1;
	}
	void removeUserDebugItem(int debugItemUniqueId) override
	{
	}
	void removeAllUserDebugItems() override
	{
	}
	void renderInternalGl2(int pass, const btDiscreteDynamicsWorld* dynamicsWorld);
	void setVRMode(bool vrMode);
	void setVisualizerFlag(int flag, int enable) override;
	void setVisualizerFlagCallback(VisualizerFlagCallback callback) override;
	void dumpFramesToVideo(const char* mp4FileName) override;
	int createCheckeredTexture(int r, int g, int b);
	void computeSoftBodyVertices(btCollisionShape* collisionShape,
								 btAlignedObjectArray<GLInstanceVertex>& gfxVertices,
								 btAlignedObjectArray<int>& indices);
};

#endif  //OPENGL_GUI_HELPER_H
