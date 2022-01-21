#pragma once
#include "OpenGLInclude.h"
#include "Bullet3Common/b3AlignedObjectArray.h"
#include <GLVBO.h>
struct GLInstanceRendererInternalData
{
	b3AlignedObjectArray<GLfloat> m_instance_positions_ptr;
	b3AlignedObjectArray<GLfloat> m_instance_quaternion_ptr;
	b3AlignedObjectArray<GLfloat> m_instance_colors_ptr;
	b3AlignedObjectArray<GLfloat> m_instance_scale_ptr;

	int m_vboSize;
	std::shared_ptr<GLVBO> m_vbo;
	int m_totalNumInstances;
	int m_maxNumObjectCapacity;
	int m_maxShapeCapacityInBytes;
};
