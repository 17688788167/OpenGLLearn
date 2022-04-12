#define GLEW_STATIC

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "shader_s.h"
#include "camera.h"
#include "stb_image.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


using namespace std;
void framebuffer_size_callback1(GLFWwindow* window, int width, int height);
void processInput1(GLFWwindow* window);
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
const float sensitivity = 0.05f;

float lastX, lastY;
bool bIsFirstFrame = true;

Camera ourCamera(quat(1,1,1,1));
Shader ourShader;

mat4 viewFinel;
mat4 projectionFinel;
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);




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

}


void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_1) != GLFW_PRESS)
	{
		bIsFirstFrame = true;
		return;
	}
	if (bIsFirstFrame)
	{
		bIsFirstFrame = false;
		lastX = xpos;
		lastY = ypos;
		return;
	}

	float xoffset = (xpos - lastX) ;
	float yoffset = (ypos - lastY) ;

	lastX = xpos;
	lastY = ypos;


	viewFinel = ourCamera.UpdateCameraRotByQuat(xoffset, -yoffset)* viewFinel;
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	projectionFinel = ourCamera.UpdateCameraFov(yoffset);
}


void glProcessInput(GLFWwindow* window, float deltaScends)
{
	

	vec3 posoffset;
	posoffset = vec3(0, 0, 0);
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		posoffset.z = 1;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		posoffset.z = -1;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		posoffset.x = -1;
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		posoffset.x = 1;
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
		posoffset.y = 1;
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
		posoffset.y = -1;

	
	viewFinel = ourCamera.updateCameraPos(posoffset, deltaScends);
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


	ourShader = Shader("shader.vs", "shader.fs");



#pragma region VAO,VBO,EBO设置
	float vertices[] = {
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
	};

	glm::vec3 cubePositions[] = {
glm::vec3(0.0f,  0.0f,  0.0f),
glm::vec3(2.0f,  5.0f, -15.0f),
glm::vec3(-1.5f, -2.2f, -2.5f),
glm::vec3(-3.8f, -2.0f, -12.3f),
glm::vec3(2.4f, -0.4f, -3.5f),
glm::vec3(-1.7f,  3.0f, -7.5f),
glm::vec3(1.3f, -2.0f, -2.5f),
glm::vec3(1.5f,  2.0f, -2.5f),
glm::vec3(1.5f,  0.2f, -1.5f),
glm::vec3(-1.3f,  1.0f, -1.5f)
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


	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	//glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(6 * sizeof(float)));
	//glEnableVertexAttribArray(2);

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


#pragma region 变换

	//先缩放，再旋转，最后平移，矩阵相乘是从右往左与向量相乘
	glm::mat4 trans = glm::mat4(1.0f);

	trans = glm::translate(trans, glm::vec3(0.3, 0, 0));
	trans = glm::rotate(trans, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0));
	//trans = glm::scale(trans, glm::vec3(1.5, 0.5, 0.5));
	float CurrentTime = glfwGetTime();
	float lastTime = CurrentTime;

	glm::mat4 trans1 = glm::translate(glm::mat4(1.0f), glm::vec3(-0.5, 0.5, 0));



#pragma endregion




	ourShader.use();
	ourShader.setMatrix4x4("transform", trans);


#pragma region 3d
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));

	ourShader.setMatrix4x4("model", model);
	
	viewFinel = ourCamera.updateCameraPos(vec3(0, 0, 0), 0);
	//viewFinel = ourCamera.UpdateCameraRotByQuat(0,0);
	projectionFinel = ourCamera.UpdateCameraFov(0.0f);



#pragma endregion

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture1);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texture2);
	ourShader.setInt("texture1", 0);
	ourShader.setInt("texture2", 1); // 或者使用着色器类设置

	glBindVertexArray(VAO);
	float lerpA = 0.5f;
	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	while (!glfwWindowShouldClose(window))
	{
		//cout << ourCamera.name << endl;
		glEnable(GL_DEPTH_TEST);
		processInput1(window);
		glfwSetCursorPosCallback(window, mouse_callback);
		glfwSetScrollCallback(window, scroll_callback);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		CurrentTime = glfwGetTime();

		float deltaTime = CurrentTime - lastTime;
		glProcessInput(window, deltaTime);
		trans = glm::rotate(trans, CurrentTime - lastTime, glm::vec3(0.0f, 0.0f, 1.0));
		ourShader.setMatrix4x4("transform", trans);
		lastTime = CurrentTime;


		ourShader.setMatrix4x4("view", viewFinel);
		ourShader.setMatrix4x4("projection", projectionFinel);

		glm::mat4 model1 = glm::rotate(model, glm::radians(-55.0f) * CurrentTime, glm::vec3(1.0f, 1.0f, 1.0f));
		ourShader.setMatrix4x4("model", model1);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		for (unsigned int i = 0; i < 10; ++i)
		{
			glm::mat4 model2 = glm::mat4(1.0f);
			model2 = glm::translate(model2, cubePositions[i]);
			float angle = 20.0f * (i + 1);
			model2 = glm::rotate(model2, glm::radians(angle), glm::vec3(1.0, 0.3, 0.5));
			ourShader.setMatrix4x4("model", model2);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}




		glfwSwapBuffers(window);
		glfwPollEvents();

	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);


	glfwTerminate();
	return 0;
}












