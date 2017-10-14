#pragma once

#include "ogldev_math_3d.h"

class Camera
{
public:
	Camera(int windowWidth, int windowHeight);
	Camera(int windowWidth, int windowHeight, const Vector3f& pos, const Vector3f& target, const Vector3f& up);
	// 初始化angleH, angleV, mouseOn*Edge, m_mousePos
	void Init();

	// 对能够响应的按键（上下左右）返回true，否则返回false
	bool onKeyboard(int key);

	//在鼠标移动时调用，修改angleH, angleV, mouseOn*Edge, m_mousePos
	void onMouse(int x, int y);
	//在每个主渲染循环中调用，根据是否在边缘来修改angleH, angleV
	void onRender(); 
	//根据angleH和angleV计算出新的m_target和m_up向量
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

	float angleH; // 水平角度，范围为[0°, 360°]，0°为X轴正方向，90°为Z轴负方向，270°为Z轴正方向
	float angleV; // 垂直角度，范围为[-90°, 90°]，-90°为朝Y轴正方向看

	int m_windowWidth;
	int m_windowHeight;

	bool mouseOnLeftEdge;
	bool mouseOnRightEdge;
	bool mouseOnUpperEdge;
	bool mouseOnLowerEdge;

	Vector2i m_mousePos;
};
