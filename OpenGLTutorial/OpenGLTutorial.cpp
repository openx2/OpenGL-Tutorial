// OpenGLTutorial.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include <GL/glew.h>
#include <gl/glut.h>

#include "ogldev_math_3d.h"

GLuint VBO; //ȫ�ֵ�GLuint���ã����ڲ������㻺�������󡣴��OpenGL������ͨ��GLuint���͵ı��������õ�

/*��Ⱦ�ص�����*/
void RenderScenceCB()
{
	//���֡���壨ʹ��clear color��
	glClear(GL_COLOR_BUFFER_BIT);

	glEnableVertexAttribArray(0);												//�����������Զ�Ӧ��index���ö��������ܹ���shader������λ�ö�Ӧ�Ķ�������������0
	glBindBuffer(GL_ARRAY_BUFFER, VBO);											//�ٴΰ�Buffer׼����ʼdraw call�ص������ж��bufferʱ���Կ�����������
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);						//���߹��߽���Buffer�����ݵķ�ʽ
	
	//���ò����ص������Ƽ���ͼ�Ρ����ָ����GPU������ʼ�����ĵط�
	glDrawArrays(GL_POINTS, 0, 1);												//Ҫ����һϵ�е㣬�ӵ�һ�����������0��ʼ������1������

	glDisableVertexAttribArray(0);												//���ö�������index������ɫ������ʱ���ÿ����������

	//����˫���������ǰ�󻺴��λ��
	glutSwapBuffers();
}

void createVertexBuffer()
{
	// ��������һ������Ķ�������
	Vector3f vertices[1];
	// ����������Ļ���루�ڽ����դ��֮ǰ��x,y,z����ķ�Χ����[-1.0, 1.0]��
	vertices[0] = Vector3f(0.0f, 0.0f, 0.0f);

	glGenBuffers(1, &VBO);														//����1�������handle
	glBindBuffer(GL_ARRAY_BUFFER, VBO);											//��handle�󶨵�Ŀ�������ϣ�֮����Ŀ��������ִ������
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);	//��Ŀ�����Ʋ����󶨶�������
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv); //ֱ������command line����ʼ��glut
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA); //����˫�������ɫ����

	//���ô�������
	glutInitWindowSize(480, 320);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Tutorial 02");

	//���ûص�������GLUTͨ���ص�������ײ㴰��ϵͳ����
	glutDisplayFunc(RenderScenceCB);

	GLenum res = glewInit();
	if (res != GLEW_OK) {
		fprintf(stderr, "Error: '%s'\n", glewGetErrorString(res));
		return 1;
	}

	//����״̬��Opengl��һ��״̬����
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f); //��֡�����clear color��Ϊ(0,0,0,0)(RGBA),4��ֵ�ķ�Χ����0.0f~1.0f

	createVertexBuffer(); //�������㻺����

	//�����ƽ���GLUT�ڲ�ѭ��
	glutMainLoop();
    return 0;
}

