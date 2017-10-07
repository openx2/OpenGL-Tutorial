#include "stdafx.h"
#include "ogldev_camera.h"

const static float STEP_SCALE = 1.0f;

Camera::Camera()
{
	m_pos = Vector3f(0.0f);
	m_target = Vector3f(0.0f, 0.0f, 1.0f);
	m_up = Vector3f(0.0f, 1.0f, 0.0f);
}

Camera::Camera(const Vector3f & pos, const Vector3f & target, const Vector3f & up)
{
	m_pos = pos;

	m_target = target;
	m_target.Normalize();

	m_up = up;
	m_up.Normalize();
}

bool Camera::onKeyboard(int key)
{
	bool ret = false;
	switch (key)
	{
	case GLUT_KEY_UP:
		m_pos += (m_target * STEP_SCALE);
		ret = true;
		break;
	case GLUT_KEY_DOWN:
		m_pos -= (m_target * STEP_SCALE);
		ret = true;
		break;
	case GLUT_KEY_LEFT:
	{
		Vector3f left = m_target.Cross(m_up);
		left.Normalize();
		left *= STEP_SCALE;
		m_pos += left;
		ret = true;
	}
	break;
	case GLUT_KEY_RIGHT:
	{
		Vector3f right = m_up.Cross(m_target);
		right.Normalize();
		right *= STEP_SCALE;
		m_pos += right;
		ret = true;
	}
	break;
	case GLUT_KEY_PAGE_UP:
		m_pos += (m_up * STEP_SCALE);
		ret = true;
		break;
	case GLUT_KEY_PAGE_DOWN:
		m_pos -= (m_up * STEP_SCALE);
		ret = true;
		break;
	default:
		break;
	}
	return ret;
}
