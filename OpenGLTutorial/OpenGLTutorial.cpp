// OpenGLTutorial.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <gl/glut.h>

/*渲染回调函数*/
void RenderScenceCB()
{
	//清空帧缓冲（使用clear color）
	glClear(GL_COLOR_BUFFER_BIT);
	//交换双缓冲机制中前后缓存的位置
	glutSwapBuffers();
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv); //直接引用command line，初始化glut
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA); //设置双缓冲和颜色缓冲

	//设置窗口属性
	glutInitWindowSize(960, 640);
	glutInitWindowPosition(100, 30);
	glutCreateWindow("Tutorial 01");

	//设置回调函数，GLUT通过回调函数与底层窗口系统交互
	glutDisplayFunc(RenderScenceCB);

	//设置状态（Opengl是一个状态机）
	glClearColor(0.0f, 1.0f, 1.0f, 0.0f); //将帧缓冲的clear color设为(0,0,0,0)(RGBA),4个值的范围都是0.0f~1.0f

	//将控制交给GLUT内部循环
	glutMainLoop();
    return 0;
}

