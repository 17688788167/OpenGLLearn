#define GLEW_STATIC

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "shader_s.h"
#include "stb_image.h"

using namespace std;
void framebuffer_size_callback1(GLFWwindow* window, int width, int height);
void processInput1(GLFWwindow* window);
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;


void framebuffer_size_callback1(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void processInput1(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
		cout << "关闭OpenGL Window!" << endl;
	}
	else if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
	{

	}
	else if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
	{

	}
}

bool glProcessInput(GLFWwindow* window, int GLFW_Key)
{
	
	return glfwGetKey(window, GLFW_Key) == GLFW_PRESS;
}

int main(int argc, char* argv[])
{
#pragma region GL窗口初始化
	//初始化
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		cout << "Failed to create GLFW window" << endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback1);


	//初始化 GLFW
	glewExperimental = true;
	if (glewInit() != GLEW_OK)
	{
		cout << "Init Glew Failed." << endl;
		glfwTerminate();
	}
#pragma endregion

	Shader ourShader("shader.vs", "shader.fs");

#pragma region VAO,VBO,EBO设置
	float vertices[] =
	{
		//// x      y    z        r     g    b            s    t
		// 0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,       1.0f,1.0f,          // top right
		// 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,       1.0f,0.0f,        // bottom right
		//-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,       0.0f,0.0f,        // bottom left
		//-0.5f,  0.5f, 0.0f,	  1.0f, 1.0f, 0.0f,		  0.0f,1.0f							 // top left 
			 0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
			 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
			-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
			-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left 
	};


	float texCoords[] = {
		0.0f,0.0f,
		1.0f,0.0f,
		0.5f,1.0f
	};

	unsigned int indices[] = {
		0,1,3,
		1,2,3
	};
	unsigned VAO, VBO, EBO;

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).

	//1.绑定VAO
	glBindVertexArray(VAO);
	//2.绑定VBO，配置顶点属性
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//3.绑定EBO，将元素索引数组配置到顶点属性中
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);


	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

#pragma endregion
#pragma region 纹理
	unsigned int texture1, texture2;
	glGenTextures(1, &texture1);
	glBindTexture(GL_TEXTURE_2D, texture1);
	//为当前绑定的纹理对象设置环绕，过滤方式
	//x重复
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	//y重复
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	float borderColor[] = { 1.0f, 1.0f, 0.0f, 1.0f };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
	//缩小线性过滤
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	//放大线性过滤
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);


	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load("container.jpg", &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		cout << "Failed to load texture1" << endl;
	}

	//释放图像的内存
	stbi_image_free(data);

	glGenTextures(1, &texture2);
	glBindTexture(GL_TEXTURE_2D, texture2);
	//x重复
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	//y重复
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);



	//缩小线性过滤
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	//放大线性过滤
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	data = stbi_load("awesomeface.png", &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		cout << "Failed to load texture2" << endl;
	}
	stbi_image_free(data);
#pragma endregion




	ourShader.use();


	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture1);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texture2);
	ourShader.setInt("texture1", 0);
	//glUniform1i(glGetUniformLocation(ourShader.ID, "texture1"), 0); // 手动设置
	ourShader.setInt("texture2", 1); // 或者使用着色器类设置
	//ourShader.use();
	glBindVertexArray(VAO);
	float lerpA = 0.5f;
	while (!glfwWindowShouldClose(window))
	{
		processInput1(window);
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);


		//float timeValue = glfwGetTime();
		//float greenValue = (sin(timeValue) / 2.0f) + 0.5f;
		
		if (glProcessInput(window,GLFW_KEY_UP))
		{
			lerpA = (lerpA + 0.001f < 1.0f) ? lerpA + 0.001f : 1.0f;			
		}

		if (glProcessInput(window, GLFW_KEY_DOWN))
		{
			lerpA = (lerpA - 0.001f > 0.0f) ? lerpA - 0.001f : 0.0f;
		}

		ourShader.setFloat("ourGreen", lerpA);


		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);


		glfwSwapBuffers(window);
		glfwPollEvents();

	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);


	glfwTerminate();
	return 0;
}












