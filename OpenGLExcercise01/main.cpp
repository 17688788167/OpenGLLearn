#define GLEW_STATIC
#pragma once

//#include <glad/glad.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
using namespace std;




void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";

const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"	FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\n\0 ";

const char* fragmentShaderSource2 = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"	FragColor = vec4(1.0f, 1.0f, 0.0f, 0.5f);\n"
"}\n\0 ";

#pragma region
/*
int main(int argc, char* argv[])
{
	//初始化
	glfwInit();
	//提示
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


	//打开GLFW 窗口
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "abc", NULL, NULL);
	if (window == NULL)
	{

		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);


	//初始化 GLFW
	glewExperimental = true;
	if (glewInit() != GLEW_OK)
	{
		cout << "Init Glew Failed." << endl;
		glfwTerminate();
	}



	//1.创建一个顶点着色器，并编译
	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	//编译shader
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);
	//验证是否编译成功
	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << endl;
	}



	//2创建一个片段着色器，并编译
	//先创建片段着色器，然后将片段着色器的源码绑定，最后编译片段着色器
	unsigned int fragmentShader2;
	fragmentShader2 = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader2, 1, &fragmentShaderSource2, NULL);
	glCompileShader(fragmentShader2);

	unsigned int shaderProgram2;
	shaderProgram2 = glCreateProgram();
	glAttachShader(shaderProgram2, vertexShader);
	glAttachShader(shaderProgram2, fragmentShader2);
	glLinkProgram(shaderProgram2);
	glDeleteShader(fragmentShader2);


	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << endl;
	}

	//3.创建一个着色器程序，并链接顶点着色器和片段着色器，并编译
	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();
	//将顶点着色器和片段着色器使用着色器程序链接起来
	//链接着色器至一个程序的时候，会把每个着色器的输出链接到下个着色器的输入
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (success)
	{
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		cout << "ERROR::SHADER::SharderProgram::LINK_SUCCESS\n" << infoLog << endl;
	}
	//删除顶点着色器和片段着色器
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);



	float vertices[] =
	{
		-0.9f, -0.5f, 0.0f,  // left 
		-0.0f, -0.5f, 0.0f,  // right
		-0.45f, 0.5f, 0.0f,  // top 
		// second triangle
		 0.0f, -0.5f, 0.0f,  // left
		 0.9f, -0.5f, 0.0f,  // right
		 0.45f, 0.5f, 0.0f   // top 
	};

	unsigned int indices[] = {
		0,1,3,
		1,2,3,
		0,1,2
	};
	unsigned int EBO;
	glGenBuffers(1, &EBO);
	//将EBO绑定到缓冲类型GL_ELEMENT_ARRAY_BUFFER上
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);



	

	//生成一个顶点缓冲对象
	unsigned int VBO,VAO;

	glGenBuffers(1, &VBO);
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	
	//将VBO绑定在GL_ARRAY_BUFFER（缓冲类型）上
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//将之前定义的顶点数据vertices复制到缓冲的内存中
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//解析顶点数据，启用顶点参数
	//第一个参数是定点函数中的Location=0
	//第二个参数是顶点属性是一个vec3，它由3个值组成，所以大小是3。
	//第六个参数是它表示位置数据在缓冲中起始位置的偏移量(Offset)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	
	//设置为线框模式
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	while (!glfwWindowShouldClose(window))
	{
		processInput(window);
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
	

		//使用链接好的着色器程序
		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);


		//第一个参数，三角形
		//第二个参数，顶点数组的起始索引
		//第三个参数，绘制多少个顶点
		glDrawArrays(GL_TRIANGLES, 0, 6);
		//glBindVertexArray(0);
		

		//glUseProgram(shaderProgram2);
		//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		//glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
	

		//双缓冲
//双缓冲(Double Buffer)
 //	应用程序使用单缓冲绘图时可能会存在图像闪烁的问题。
	//这是因为生成的图像不是一下子被绘制出来的，
	//而是按照从左到右，由上而下逐像素地绘制而成的。
	//最终图像不是在瞬间显示给用户，而是通过一步一步生成的，
	//这会导致渲染的结果很不真实。为了规避这些问题，
	//我们应用双缓冲渲染窗口应用程序。前缓冲保存着最终输出的图像，
	//它会在屏幕上显示；而所有的的渲染指令都会在后缓冲上绘制。
	//当所有的渲染指令执行完毕后，我们交换(Swap)前缓冲和后缓冲，
	//这样图像就立即呈显出来，之前提到的不真实感就消除了。
		glfwSwapBuffers(window);
		glfwPollEvents();
	}





	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteProgram(shaderProgram);



	//删除释放之前分配的资源
	glfwTerminate();
	return 0;
}

*/
#pragma endregion
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
		cout << "关闭OpenGL Window!" << endl;
	}
}
