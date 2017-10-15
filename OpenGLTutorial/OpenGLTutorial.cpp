// OpenGLTutorial.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

#include "ogldev_util.h"
#include "ogldev_pipeline.h"
#include "ogldev_camera.h"
#include "ogldev_texture.h"

#define WINDOW_WIDTH 1366
#define WINDOW_HEIGHT 768

struct Vertex //顶点数据结构
{
	Vector3f m_pos;
	Vector2f m_tex;

	Vertex() {}
	Vertex(Vector3f pos, Vector2f tex)
	{
		m_pos = pos;
		m_tex = tex;
	}
};

GLuint VBO; //全局的GLuint引用，用于操作顶点缓冲器对象。大多OpenGL对象都是通过GLuint类型的变量来引用的
GLuint IBO; //索引缓冲器对象的GLuint引用
GLuint gWVPLocation; // 经过世界变换，视角变换和透视投影后的变换矩阵一致变量的位置
GLuint gSampler; //采样器一致变量

//透视变换配置参数数据结构
PersProjInfo gPersProjInfo;
//全局相机参数
Camera* pGameCamera = NULL;
//全局纹理对象
Texture* pTexture = NULL;

// 定义要使用的vertex shader和fragment shader的文件名，作为文件读取路径
const char* pVSfilename = "shader.vs";
const char* pFSfilename = "shader.fs";

/*渲染回调函数*/
static void RenderScenceCB()
{
	pGameCamera->onRender();

	// 维护一个不断慢慢扩大的浮点数
	static float scale = 0.0f;
	scale += 0.1f;

	Pipeline p;
	p.rotate(0.0f, scale, 0.0f);
	p.worldPos(0.0f, 0.0f, 7.0f);

	p.setCamera(pGameCamera); //设置相机变换

	p.setPerspectiveProj(gPersProjInfo); //设置透视配置
	// 将值通过得到的一致变量位置传递给shader
	glUniformMatrix4fv(gWVPLocation, 1, GL_TRUE, (const GLfloat*) p.getWVPTrans());

	//清空帧缓冲（使用clear color）
	glClear(GL_COLOR_BUFFER_BIT);

	glEnableVertexAttribArray(0);															//开启顶点属性对应的index，让顶点属性能够被shader看到。位置对应的顶点属性索引是0
	glEnableVertexAttribArray(1);															//纹理坐标对应的顶点属性索引是1
	glBindBuffer(GL_ARRAY_BUFFER, VBO);														//再次绑定Buffer准备开始draw call回调。在有多个buffer时可以看出它的作用
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);						//告诉管线解析Buffer中数据的方式，0对应位置属性，包括3个float值，不进行标准化，一个数据段的大小为sizeof(Vertex)，偏移量为0字节
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*) 12);	//和上面一样，1对应纹理坐标，包括2个float值，不进行标准化，一个数据段的大小为sizeof(Vertex)，偏移量为12字节
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);												//在绘制前绑定索引缓冲
	
	pTexture->bind(GL_TEXTURE0); //把纹理对象上传到纹理单元中，纹理单元0对应OpenGL的枚举值GL_TEXTURE0
	//调用参数回调来绘制几何图形。这个指令是GPU真正开始工作的地方
	glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);						//绘制四面体。第一个参数是图元类型，第二个是索引个数，第三个是索引的类型（即byte,short,int)，最后一个是从缓冲开始位置到扫描开始位置的偏移量（类型为GLvoid*）

	glDisableVertexAttribArray(0);												//禁用顶点属性index，在着色器不用时禁用可以提高性能
	glDisableVertexAttribArray(1);												//同上

	//交换双缓冲机制中前后缓存的位置
	glutSwapBuffers();
}

static void SpecialKeyboardCB(int key, int x, int y)
{
	pGameCamera->onKeyboard(key);
}

static void PassiveMouseCB(int x, int y)
{
	pGameCamera->onMouse(x, y);
}

static void KeyBoardCB(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'q':
		exit(0);
		break;
	default:
		break;
	}
}

static void initializeGlutCallbacks()
{
	glutDisplayFunc(RenderScenceCB); //设置显示控制回调函数
	//设置空闲回调函数，在不发生任何事件时会不断调用指定函数。如果指定函数不是渲染函数，
	//则要在它最后调用glutPostRedisplay()，标记当前窗口要被重新绘制，否则闲置回调会不断调用但不会重新渲染
	glutIdleFunc(RenderScenceCB);
	glutSpecialFunc(SpecialKeyboardCB); //注册响应特殊按键的回调函数
	glutPassiveMotionFunc(PassiveMouseCB); //注册响应鼠标移动的回调函数(没有键被按下)
	glutKeyboardFunc(KeyBoardCB); //注册响应常规按键的回调函数
}

static void createVertexBuffer()
{
	// 创建含有四个顶点的顶点数组，4个顶点构成一个四面体，顶点包括位置和纹理坐标属性
	Vertex vertices[4] = { Vertex(Vector3f(-1.0f, -1.0f, 0.5773f), Vector2f(0.0f, 0.0f)),
							Vertex(Vector3f(0.0f, -1.0f, -1.15475f), Vector2f(0.5f, 0.0f)),
							Vertex(Vector3f(1.0f, -1.0f, 0.5773f), Vector2f(1.0f, 0.0f)),
							Vertex(Vector3f(0.0f, 1.0f, 0.0f), Vector2f(0.5f, 1.0f)) };

	glGenBuffers(1, &VBO);														//分配1个对象的handle
	glBindBuffer(GL_ARRAY_BUFFER, VBO);											//将handle绑定到目标名称上，之后在目标名称上执行命令
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);	//对目标名称参数绑定顶点数据
}

static void createIndicesBuffer()
{
	// 顶点坐标的索引数组，3个一组
	unsigned int indices[] = {  0, 3, 1,
								1, 3, 2,
								2, 3, 0,
								0, 1, 2 };

	// 创建索引缓冲器并存入数据
	glGenBuffers(1, &IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
}

static void addShader(GLuint shaderProgram, const char* pShaderText, GLenum shaderType)
{
	// 根据shader类型定义shader对象
	GLuint shaderObj = glCreateShader(shaderType);
	// 检查是否定义成功
	if (shaderObj == 0) {
		fprintf(stderr, "Error creating shader type %d\n", shaderType);
		exit(1);
	}

	// 定义shader的源码
	const char* p[1];
	p[0] = pShaderText;
	GLint lengths[1];
	lengths[0] = strlen(pShaderText);
	glShaderSource(shaderObj, 1, p, lengths);
	glCompileShader(shaderObj);	//编译shader对象

	// 得到编译状态，检查shader源码是否有错误
	GLint success;
	glGetShaderiv(shaderObj, shaderType, &success);
	if (!success) {
		GLchar infoLog[1024];
		glGetShaderInfoLog(shaderObj, 1024, NULL, infoLog);
		fprintf(stderr, "Error compile shader type %d: '%s'\n", shaderType, infoLog);
		exit(1);
	}

	// 将编译好的ShaderObj绑定到ShaderProgram上
	glAttachShader(shaderProgram, shaderObj);
}

static void compileShader()
{
	GLuint shaderProgram = glCreateProgram(); //创建一个程序对象，之后会把所有shader链接起来，放到该对象中
	// 检查是否创建成功
	if (shaderProgram == 0) {
		fprintf(stderr, "Error creating shader program\n");
		exit(1);
	}

	// 存储shader源码的字符串缓存
	string vs, fs;
	if (!ReadFile(pVSfilename, vs))
		exit(1);
	if (!ReadFile(pFSfilename, fs))
		exit(1);

	// 添加vertex shader和fragment shader
	addShader(shaderProgram, vs.c_str(), GL_VERTEX_SHADER);
	addShader(shaderProgram, fs.c_str(), GL_FRAGMENT_SHADER);

	// 对程序对象进行链接操作，并检查有无错误
	glLinkProgram(shaderProgram);
	GLint success;
	GLchar errorLog[1024] = { 0 };
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, sizeof(errorLog), NULL, errorLog);
		fprintf(stderr, "Error linking shader program: '%s'\n", errorLog);
		exit(1);
	}

	// 检查验证当前管线状态下程序是否能被执行
	glValidateProgram(shaderProgram);
	glGetProgramiv(shaderProgram, GL_VALIDATE_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, sizeof(errorLog), NULL, errorLog);
		fprintf(stderr, "Invalid shader program: '%s'\n", errorLog);
		exit(1);
	}

	// 声明管线要使用上面建立的shader程序
	glUseProgram(shaderProgram);

	// 查询获取一致变量的位置
	// 在两种情况下会出现错误：1.变量名拼写错误;2.变量没有在程序中使用到，被编译器优化掉了
	gWVPLocation = glGetUniformLocation(shaderProgram, "gWVP");
	// 检查错误
	assert(gWVPLocation != 0xFFFFFFFF);
	//同上，得到采样器一致变量的位置
	gSampler = glGetUniformLocation(shaderProgram, "gSampler");
	assert(gSampler != 0xFFFFFFFF);
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
	glutInit(&argc, argv); //直接引用command line，初始化glut
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA); //设置双缓冲和颜色缓冲

	//设置窗口属性
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Tutorial 15");
	// 使用全屏模式,1344x768表示屏幕分辨率,32表示每个像素包含的bit数
	glutGameModeString(":32");
	glutEnterGameMode();

	//设置回调函数，GLUT通过回调函数与底层窗口系统交互
	initializeGlutCallbacks();

	pGameCamera = new Camera(WINDOW_WIDTH, WINDOW_HEIGHT); //初始化相机,里面会调用glutWarpPointer修改鼠标指针位置,如果glut没有被初始化会调用失败

	// 初始化GLEW，要在glut初始化之后
	GLenum res = glewInit();
	if (res != GLEW_OK) {
		fprintf(stderr, "Error: '%s'\n", glewGetErrorString(res));
		return 1;
	}

	printf("GL version: %s\n", glGetString(GL_VERSION)); //显示OpenGL版本

	//设置状态（Opengl是一个状态机）
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f); //将帧缓冲的clear color设为(0,0,0,0)(RGBA),4个值的范围都是0.0f~1.0f
	//让OpenGL进行背面剔除
	glFrontFace(GL_CW); //告诉OpenGL三角形的顶点按顺时针方向指定，从前面看三角形，可以发现顶点缓存中的顶点是顺时针排列的
	glCullFace(GL_BACK); //告诉OpenGL剔除三角形的背面，这意味着一个对象的内部是不需要被渲染的
	glEnable(GL_CULL_FACE);//最后启用背面剔除

	createVertexBuffer(); //创建顶点缓冲器
	createIndicesBuffer(); //创建索引缓冲器

	// 创建和设置要使用的Shader
	compileShader();

	glUniform1i(gSampler, 0); //把对应的纹理单元索引传递给采样器一致变量，注意这里用的是纹理单元真正的索引值，而不是OpenGL的枚举值GL_TEXTURE0

	pTexture = new Texture(GL_TEXTURE_2D, "test.png"); // 纹理目标为texture2D
	if (!pTexture->load()) {
		return 1;
	}

	// 初始化透视变换配置参数
	initPresProjInfo();

	//将控制交给GLUT内部循环
	glutMainLoop();

	delete pGameCamera; //循环结束后删除相机
	delete pTexture;
    return 0;
}

