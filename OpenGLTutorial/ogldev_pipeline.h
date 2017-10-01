#pragma once
#include "ogldev_math_3d.h"

class Pipeline
{
public:
	Pipeline() 
	{
		m_scale = Vector3f(1.0f);
		m_worldPos = Vector3f(0.0f);
		m_rotateInfo = Vector3f(0.0f);
	}

	void scale(float s) { scale(s, s, s); }
	void scale(const Vector3f& v) { scale(v.x, v.y, v.z); }
	void scale(float x, float y, float z)
	{
		m_scale.x = x;
		m_scale.y = y;
		m_scale.z = z;
	}

	void worldPos(float x, float y, float z)
	{
		m_worldPos.x = x;
		m_worldPos.y = y;
		m_worldPos.z = z;
	}
	
	/*angleX��ʾ��X����ת�ĽǶȣ��Դ�����*/
	void rotate(float angleX, float angleY, float angleZ)
	{
		m_rotateInfo.x = angleX;
		m_rotateInfo.y = angleY;
		m_rotateInfo.z = angleZ;
	}

	const Matrix4f* getTrans();
private:
	Vector3f m_scale;
	Vector3f m_worldPos;
	Vector3f m_rotateInfo;
	Matrix4f m_transformation;
};
