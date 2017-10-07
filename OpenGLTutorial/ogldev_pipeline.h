#pragma once
#include "ogldev_math_3d.h"
#include "ogldev_camera.h"

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

	void setPerspectiveProj(const PersProjInfo& p)
	{
		m_persProjInfo = p;
	}

	void setCamera(Vector3f pos, Vector3f target, Vector3f up)
	{
		m_camera.pos = pos;
		m_camera.target = target;
		m_camera.up = up;
	}

	void setCamera(const Camera* c)
	{
		setCamera(c->getPos(), c->getTarget(), c->getUp());
	}

	const Matrix4f& getWPTrans();
	const Matrix4f& getWVTrans();
	const Matrix4f& getVPTrans();
	const Matrix4f& getWVPTrans();
	const Matrix4f& getWorldTrans();
	const Matrix4f& getViewTrans();
	const Matrix4f& getProjTrans();
private:
	Vector3f m_scale;
	Vector3f m_worldPos;
	Vector3f m_rotateInfo;

	PersProjInfo m_persProjInfo;

	struct {
		Vector3f pos; //�������������ϵ�е�����
		Vector3f target; //�������ķ��򣬵�λ��������Ӧz��
		Vector3f up; //��������ϵķ��򣬵�λ��������Ӧy��
	} m_camera; //������ҷ�����target��up�Ĳ�˵õ�����Ӧx��

	Matrix4f m_WVPtransformation;
	Matrix4f m_VPtransformation;
	Matrix4f m_WPtransformation;
	Matrix4f m_WVtransformation;
	Matrix4f m_Wtransformation;
	Matrix4f m_Vtransformation;
	Matrix4f m_ProjTransformation;
};
