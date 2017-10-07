// OpenGLTutorial.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"

#include "ogldev_util.h"
#include "ogldev_pipeline.h"
#include "ogldev_camera.h"

#define WINDOW_WIDTH 1024
#define WINDOW_HEIGHT 640

GLuint VBO; //ȫ�ֵ�GLuint���ã����ڲ������㻺�������󡣴��OpenGL������ͨ��GLuint���͵ı��������õ�
GLuint IBO; //���������������GLuint����
GLuint gWorldLocation; // ƽ�Ʊ任��һ�±���world��λ��

//͸�ӱ任���ò������ݽṹ
PersProjInfo gPersProjInfo;
//ȫ���������
Camera* pGameCamera = NULL;

// ����Ҫʹ�õ�vertex shader��fragment shader���ļ�������Ϊ�ļ���ȡ·��
const char* pVSfilename = "shader.vs";
const char* pFSfilename = "shader.fs";

/*��Ⱦ�ص�����*/
static void RenderScenceCB()
{
	// ά��һ��������������ĸ�����
	static float scale = 0.0f;
	scale += 0.1f;

	Pipeline p;
	p.rotate(0.0f, scale, 0.0f);
	p.worldPos(0.0f, 0.0f, 7.0f);

	p.setCamera(pGameCamera); //��������任

	p.setPerspectiveProj(gPersProjInfo); //����͸������
	// ��ֵͨ���õ���һ�±���λ�ô��ݸ�shader
	glUniformMatrix4fv(gWorldLocation, 1, GL_TRUE, (const GLfloat*) p.getWVPTrans());

	//���֡���壨ʹ��clear color��
	glClear(GL_COLOR_BUFFER_BIT);

	glEnableVertexAttribArray(0);												//�����������Զ�Ӧ��index���ö��������ܹ���shader������λ�ö�Ӧ�Ķ�������������0
	glBindBuffer(GL_ARRAY_BUFFER, VBO);											//�ٴΰ�Buffer׼����ʼdraw call�ص������ж��bufferʱ���Կ�����������
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);						//���߹��߽���Buffer�����ݵķ�ʽ
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);									//�ڻ���ǰ����������
	
	//���ò����ص������Ƽ���ͼ�Ρ����ָ����GPU������ʼ�����ĵط�
	glDrawElements(GL_POLYGON, 24, GL_UNSIGNED_INT, 0);						//���������塣��һ��������ͼԪ���ͣ��ڶ��������������������������������ͣ���byte,short,int)�����һ���Ǵӻ��忪ʼλ�õ�ɨ�迪ʼλ�õ�ƫ����������ΪGLvoid*��

	glDisableVertexAttribArray(0);												//���ö�������index������ɫ������ʱ���ÿ����������

	//����˫���������ǰ�󻺴��λ��
	glutSwapBuffers();
}

static void SpecialKeyboardCB(int key, int x, int y)
{
	pGameCamera->onKeyboard(key);
}

static void initializeGlutCallbacks()
{
	glutDisplayFunc(RenderScenceCB); //������ʾ���ƻص�����
	//���ÿ��лص��������ڲ������κ��¼�ʱ�᲻�ϵ���ָ�����������ָ������������Ⱦ������
	//��Ҫ����������glutPostRedisplay()����ǵ�ǰ����Ҫ�����»��ƣ��������ûص��᲻�ϵ��õ�����������Ⱦ
	glutIdleFunc(RenderScenceCB);
	glutSpecialFunc(SpecialKeyboardCB); //��Ӧ���ⰴ���Ļص�����
}

static void createVertexBuffer()
{
	// ��������8������Ķ������飬8�����㹹��һ��������
	Vector3f vertices[8];
	// �������4�����������
	vertices[0] = Vector3f(-1.0f, -1.0f, 1.0f);
	vertices[1] = Vector3f(1.0f, -1.0f, 1.0f);
	vertices[2] = Vector3f(1.0f, 1.0f, 1.0f);
	vertices[3] = Vector3f(-1.0f, 1.0f, 1.0f);
	vertices[4] = Vector3f(-1.0f, 1.0f, -1.0f);
	vertices[5] = Vector3f(1.0f, 1.0f, -1.0f);
	vertices[6] = Vector3f(1.0f, -1.0f, -1.0f);
	vertices[7] = Vector3f(-1.0f, -1.0f, -1.0f);

	glGenBuffers(1, &VBO);														//����1�������handle
	glBindBuffer(GL_ARRAY_BUFFER, VBO);											//��handle�󶨵�Ŀ�������ϣ�֮����Ŀ��������ִ������
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);	//��Ŀ�����Ʋ����󶨶�������
}

static void createIndicesBuffer()
{
	// ����������������飬4��һ��
	unsigned int indices[] = {  0, 1, 2, 3,
								2, 3, 4, 5,
								4, 5, 6, 7,
								0, 1, 6, 7,
								1, 2, 5, 6,
								0, 3, 4, 7 };

	// ������������������������
	glGenBuffers(1, &IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
}

static void addShader(GLuint shaderProgram, const char* pShaderText, GLenum shaderType)
{
	// ����shader���Ͷ���shader����
	GLuint shaderObj = glCreateShader(shaderType);
	// ����Ƿ���ɹ�
	if (shaderObj == 0) {
		fprintf(stderr, "Error creating shader type %d\n", shaderType);
		exit(1);
	}

	// ����shader��Դ��
	const char* p[1];
	p[0] = pShaderText;
	GLint lengths[1];
	lengths[0] = strlen(pShaderText);
	glShaderSource(shaderObj, 1, p, lengths);
	glCompileShader(shaderObj);	//����shader����

	// �õ�����״̬�����shaderԴ���Ƿ��д���
	GLint success;
	glGetShaderiv(shaderObj, shaderType, &success);
	if (!success) {
		GLchar infoLog[1024];
		glGetShaderInfoLog(shaderObj, 1024, NULL, infoLog);
		fprintf(stderr, "Error compile shader type %d: '%s'\n", shaderType, infoLog);
		exit(1);
	}

	// ������õ�ShaderObj�󶨵�ShaderProgram��
	glAttachShader(shaderProgram, shaderObj);
}

static void compileShader()
{
	GLuint shaderProgram = glCreateProgram(); //����һ���������֮��������shader�����������ŵ��ö�����
	// ����Ƿ񴴽��ɹ�
	if (shaderProgram == 0) {
		fprintf(stderr, "Error creating shader program\n");
		exit(1);
	}

	// �洢shaderԴ����ַ�������
	string vs, fs;
	if (!ReadFile(pVSfilename, vs))
		exit(1);
	if (!ReadFile(pFSfilename, fs))
		exit(1);

	// ���vertex shader��fragment shader
	addShader(shaderProgram, vs.c_str(), GL_VERTEX_SHADER);
	addShader(shaderProgram, fs.c_str(), GL_FRAGMENT_SHADER);

	// �Գ������������Ӳ�������������޴���
	glLinkProgram(shaderProgram);
	GLint success;
	GLchar errorLog[1024] = { 0 };
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, sizeof(errorLog), NULL, errorLog);
		fprintf(stderr, "Error linking shader program: '%s'\n", errorLog);
		exit(1);
	}

	// �����֤��ǰ����״̬�³����Ƿ��ܱ�ִ��
	glValidateProgram(shaderProgram);
	glGetProgramiv(shaderProgram, GL_VALIDATE_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, sizeof(errorLog), NULL, errorLog);
		fprintf(stderr, "Invalid shader program: '%s'\n", errorLog);
		exit(1);
	}

	// ��������Ҫʹ�����潨����shader����
	glUseProgram(shaderProgram);

	// ��ѯ��ȡһ�±�����λ��
	// ����������»���ִ���1.������ƴд����;2.����û���ڳ�����ʹ�õ������������Ż�����
	gWorldLocation = glGetUniformLocation(shaderProgram, "gWorld");
	// ������
	assert(gWorldLocation != 0xFFFFFFFF);
}

static void initPresProjInfo()
{
	gPersProjInfo.FOV = 30.0f;
	gPersProjInfo.Width = WINDOW_WIDTH;
	gPersProjInfo.Height = WINDOW_HEIGHT;
	gPersProjInfo.zNear = 1.0f;
	gPersProjInfo.zFar = 100.0f;
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv); //ֱ������command line����ʼ��glut
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA); //����˫�������ɫ����

	//���ô�������
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	glutInitWindowPosition(100, 0);
	glutCreateWindow("Tutorial 12");

	//���ûص�������GLUTͨ���ص�������ײ㴰��ϵͳ����
	initializeGlutCallbacks();

	// ��ʼ��GLEW��Ҫ��glut��ʼ��֮��
	GLenum res = glewInit();
	if (res != GLEW_OK) {
		fprintf(stderr, "Error: '%s'\n", glewGetErrorString(res));
		return 1;
	}

	printf("GL version: %s\n", glGetString(GL_VERSION)); //��ʾOpenGL�汾

	//����״̬��Opengl��һ��״̬����
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f); //��֡�����clear color��Ϊ(0,0,0,0)(RGBA),4��ֵ�ķ�Χ����0.0f~1.0f

	pGameCamera = new Camera();
	createVertexBuffer(); //�������㻺����
	createIndicesBuffer(); //��������������

	// ����������Ҫʹ�õ�Shader
	compileShader();

	// ��ʼ��͸�ӱ任���ò���
	initPresProjInfo();

	//�����ƽ���GLUT�ڲ�ѭ��
	glutMainLoop();

	delete pGameCamera; //ѭ��������ɾ�����
    return 0;
}

