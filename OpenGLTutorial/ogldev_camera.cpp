#include "stdafx.h"
#include "ogldev_camera.h"

const static float STEP_SCALE = 1.0f;
const static float EDGE_STEP = 0.5f;
const static int MARGIN = 10;

Camera::Camera(int windowWidth, int windowHeight)
{
	m_windowWidth = windowWidth;
	m_windowHeight = windowHeight;
	m_pos = Vector3f(0.0f);
	m_target = Vector3f(0.0f, 0.0f, 1.0f);
	m_up = Vector3f(0.0f, 1.0f, 0.0f);

	Init();
}

Camera::Camera(int windowWidth, int windowHeight, const Vector3f & pos, const Vector3f & target, const Vector3f & up)
{
	m_windowWidth = windowWidth;
	m_windowHeight = windowHeight;
	m_pos = pos;

	m_target = target;
	m_target.Normalize();

	m_up = up;
	m_up.Normalize();
	
	Init();
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
		m_pos.y += STEP_SCALE;
		ret = true;
		break;
	case GLUT_KEY_PAGE_DOWN:
		m_pos.y -= STEP_SCALE;
		ret = true;
		break;
	default:
		break;
	}
	return ret;
}

void Camera::onMouse(int x, int y)
{
	const int deltaX = x - m_mousePos.x;
	const int deltaY = y - m_mousePos.y;

	m_mousePos.x = x;
	m_mousePos.y = y;

	float scaleFactor = 10.0f;
	angleH += (float)deltaX / scaleFactor;
	angleV += (float)deltaY / scaleFactor;

	//在x轴改变量为0时，检查鼠标是否在左右边缘上
	if (deltaX == 0) {
		if (x < MARGIN)
			mouseOnLeftEdge = true;
		else if (x > (m_windowWidth - MARGIN))
			mouseOnRightEdge = true;
		else { //在delta为0且又不在边缘时也应该把全局变量设为false
			mouseOnLeftEdge = false;
			mouseOnRightEdge = false;
		}
	}
	else {
		mouseOnLeftEdge = false;
		mouseOnRightEdge = false;
	}

	//在y轴改变量为0时，检查鼠标是否在上下边缘上
	if (deltaY == 0) {
		if (y < MARGIN)
			mouseOnUpperEdge = true;
		else if (y > (m_windowHeight - MARGIN))
			mouseOnLowerEdge = true;
		else { //在delta为0且又不在边缘时也应该把全局变量设为false
			mouseOnLowerEdge = false;
			mouseOnUpperEdge = false;
		}
	}
	else {
		mouseOnUpperEdge = false;
		mouseOnLowerEdge = false;
	}

	update();
}

void Camera::onRender()
{
	bool shouldUpdate = false;

	if (mouseOnLeftEdge) {
		angleH -= 0.1f;
		shouldUpdate = true;
	}
	else if (mouseOnRightEdge) {
		angleH += 0.1f;
		shouldUpdate = true;
	}

	if (mouseOnUpperEdge) {
		if (angleV > -90.0f) {
			angleV -= 0.1f;
			shouldUpdate = true;
		}
	}
	else if (mouseOnLowerEdge) {
		if (angleV < 90.0f) {
			angleV += 0.1f;
			shouldUpdate = true;
		}
	}

	if (shouldUpdate)
		update();
}

void Camera::update()
{
	// vertical axis
	const Vector3f vAxis(0.0f, 1.0f, 0.0f);

	// 从朝x轴正方向的初始位置开始旋转,先绕vAxis旋转angleH
	Vector3f view(1.0f, 0.0f, 0.0f);
	view.Rotate(angleH, vAxis);
	view.Normalize();

	// 先通过view和vAxis的叉乘计算出horizon axis,然后让view绕hAxis旋转angleV
	Vector3f hAxis = vAxis.Cross(view);
	hAxis.Normalize();
	view.Rotate(angleV, hAxis);
	view.Normalize();

	m_target = view;

	m_up = m_target.Cross(hAxis);
	m_up.Normalize();
}

void Camera::Init()
{
	Vector3f hTarget(m_target.x, 0, m_target.z);
	hTarget.Normalize();

	// 计算水平角度
	if (hTarget.z >= 0) {
		if (hTarget.x >= 0)
			angleH = 360.0f - ToDegree(asin(hTarget.z));
		else
			angleH = 180.0f + ToDegree(asin(hTarget.z));
	}
	else {
		if (hTarget.x >= 0)
			angleH = ToDegree(asin(-hTarget.z));
		else
			angleH = 180.0f - ToDegree(asin(-hTarget.z));
	}

	//计算垂直角度
	angleV = -ToDegree(asin(m_target.y));

	bool mouseOnLeftEdge = false;
	bool mouseOnRightEdge = false;
	bool mouseOnUpperEdge = false;
	bool mouseOnLowerEdge = false;
	m_mousePos.x = m_windowWidth / 2;
	m_mousePos.y = m_windowHeight / 2;
	glutWarpPointer(m_mousePos.x, m_mousePos.y);
}
