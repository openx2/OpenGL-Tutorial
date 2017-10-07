#pragma once

#include "ogldev_math_3d.h"

class Camera
{
public:
	Camera();
	Camera(const Vector3f& pos, const Vector3f& target, const Vector3f& up);
	// ���ܹ���Ӧ�İ������������ң�����true�����򷵻�false
	bool onKeyboard(int key);

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
		return m_up;;
	}

private:
	Vector3f m_pos;
	Vector3f m_target;
	Vector3f m_up;
};
