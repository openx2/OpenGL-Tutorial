// OpenGLTutorial.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <GL/glew.h>
#include <gl/glut.h>

#include "ogldev_math_3d.h"

GLuint VBO; //全局的GLuint引用，用于操作顶点缓冲器对象。大多OpenGL对象都是通过GLuint类型的变量来引用的

/*渲染回调函数*/
void RenderScenceCB()
{
	//清空帧缓冲（使用clear color）
	glClear(GL_COLOR_BUFFER_BIT);

	glEnableVertexAttribArray(0);												//开启顶点属性对应的index，让顶点属性能够被shader看到。位置对应的顶点属性索引是0
	glBindBuffer(GL_ARRAY_BUFFER, VBO);											//再次绑定Buffer准备开始draw call回调。在有多个buffer时可以看出它的作用
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);						//告诉管线解析Buffer中数据的方式
	
	//调用参数回调来绘制几何图形。这个指令是GPU真正开始工作的地方
	glDrawArrays(GL_POINTS, 0, 1);												//要绘制一系列点，从第一个顶点的索引0开始，绘制1个顶点

	glDisableVertexAttribArray(0);												//禁用顶点属性index，在着色器不用时禁用可以提高性能

	//交换双缓冲机制中前后缓存的位置
	glutSwapBuffers();
}

void createVertexBuffer()
{
	// 创建含有一个顶点的顶点数组
	Vector3f vertices[1];
	// 将点置于屏幕中央（在进入光栅器之前，x,y,z坐标的范围都是[-1.0, 1.0]）
	vertices[0] = Vector3f(0.0f, 0.0f, 0.0f);

	glGenBuffers(1, &VBO);														//分配1个对象的handle
	glBindBuffer(GL_ARRAY_BUFFER, VBO);											//将handle绑定到目标名称上，之后在目标名称上执行命令
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);	//对目标名称参数绑定顶点数据
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv); //直接引用command line，初始化glut
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA); //设置双缓冲和颜色缓冲

	//设置窗口属性
	glutInitWindowSize(480, 320);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Tutorial 02");

	//设置回调函数，GLUT通过回调函数与底层窗口系统交互
	glutDisplayFunc(RenderScenceCB);

	GLenum res = glewInit();
	if (res != GLEW_OK) {
		fprintf(stderr, "Error: '%s'\n", glewGetErrorString(res));
		return 1;
	}

	//设置状态（Opengl是一个状态机）
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f); //将帧缓冲的clear color设为(0,0,0,0)(RGBA),4个值的范围都是0.0f~1.0f

	createVertexBuffer(); //创建顶点缓冲器

	//将控制交给GLUT内部循环
	glutMainLoop();
    return 0;
}

