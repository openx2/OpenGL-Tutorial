// OpenGLTutorial.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include <gl/glut.h>

/*��Ⱦ�ص�����*/
void RenderScenceCB()
{
	//���֡���壨ʹ��clear color��
	glClear(GL_COLOR_BUFFER_BIT);
	//����˫���������ǰ�󻺴��λ��
	glutSwapBuffers();
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv); //ֱ������command line����ʼ��glut
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA); //����˫�������ɫ����

	//���ô�������
	glutInitWindowSize(960, 640);
	glutInitWindowPosition(100, 30);
	glutCreateWindow("Tutorial 01");

	//���ûص�������GLUTͨ���ص�������ײ㴰��ϵͳ����
	glutDisplayFunc(RenderScenceCB);

	//����״̬��Opengl��һ��״̬����
	glClearColor(0.0f, 1.0f, 1.0f, 0.0f); //��֡�����clear color��Ϊ(0,0,0,0)(RGBA),4��ֵ�ķ�Χ����0.0f~1.0f

	//�����ƽ���GLUT�ڲ�ѭ��
	glutMainLoop();
    return 0;
}

