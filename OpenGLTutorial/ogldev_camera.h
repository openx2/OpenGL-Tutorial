#pragma once

#include "ogldev_math_3d.h"

class Camera
{
public:
	Camera(int windowWidth, int windowHeight);
	Camera(int windowWidth, int windowHeight, const Vector3f& pos, const Vector3f& target, const Vector3f& up);
	// ��ʼ��angleH, angleV, mouseOn*Edge, m_mousePos
	void Init();

	// ���ܹ���Ӧ�İ������������ң�����true�����򷵻�false
	bool onKeyboard(int key);

	//������ƶ�ʱ���ã��޸�angleH, angleV, mouseOn*Edge, m_mousePos
	void onMouse(int x, int y);
	//��ÿ������Ⱦѭ���е��ã������Ƿ��ڱ�Ե���޸�angleH, angleV
	void onRender(); 
	//����angleH��angleV������µ�m_target��m_up����
	void update();

	const Vector3f& getPos() const
	{
		return m_pos;
	}

	const Vector3f& getTarget() const
	{
		return m_target;
	}

	const Vector3f& getUp() const
	{
		return m_up;
	}

private:
	Vector3f m_pos;
	Vector3f m_target;
	Vector3f m_up;

	float angleH; // ˮƽ�Ƕȣ���ΧΪ[0��, 360��]��0��ΪX��������90��ΪZ�Ḻ����270��ΪZ��������
	float angleV; // ��ֱ�Ƕȣ���ΧΪ[-90��, 90��]��-90��Ϊ��Y��������

	int m_windowWidth;
	int m_windowHeight;

	bool mouseOnLeftEdge;
	bool mouseOnRightEdge;
	bool mouseOnUpperEdge;
	bool mouseOnLowerEdge;

	Vector2i m_mousePos;
};
