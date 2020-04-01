#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <Windows.h>
#include <Shader.h>
#include <stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

bool inputing = false;
bool resetShape = false;
glm::mat4 transByInput = glm::mat4(1.0f);
glm::mat4 scaleByInput = glm::mat4(1.0f);
glm::mat4 rotateByInput = glm::mat4(1.0f);
const float translateForce = 0.005f;
const float scaleForce = 0.005f;
const float rotateForce = 1.0f;

int main()
{
	// ---------------------------------------------------------
	// --------------------------------------------------------- GlAD, GLFW and OpenGL setup
	// ---------------------------------------------------------

	// initilize the glfw library
	glfwInit();
	
	// ----- Setting glfw options

	// we want the opengl we us to be version 3 so we set the max and min version to 3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);

	// we specify that we only want the core features of OpenGL
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	// ----- 

	// creating our window and configuring it's width, height and name
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	// we tell the glfw that set our window to the current thread's context
	glfwMakeContextCurrent(window);

	// a callback to resize the window when the user resized the window
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// GLAD initilization
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initilize GLAD" << std::endl;
		return -1;
	}

	// ---------------------------------------------------------
	// --------------------------------------------------------- 
	// ---------------------------------------------------------

	// loading the shader
	Shader ourShader("textureShader.verts", "textureShader.frags");

	// ---------------------------------------------------------
	// --------------------------------------------------------- Texture
	// ---------------------------------------------------------


	// ---------------------------------------------------------
	// ---------------------------------------------------------
	// ---------------------------------------------------------

	// rectangle with texture
	float vertices[] = {
		// positions          // colors           // texture coords
		 0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
		 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
		-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
		-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left 
	};

	// triangle order indexes
	unsigned int indices[] = {  // note that we start from 0!
		0, 1, 3,   // first triangle
		1, 2, 3    // second triangle
	};

	unsigned int VBO, VAO, EBO;
	glGenBuffers(1, &VBO);
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);
	

	unsigned int texture1;
	glGenTextures(1, &texture1);
	glBindTexture(GL_TEXTURE_2D, texture1);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width1, height1, nrChannels1;
	unsigned char* data1 = stbi_load("container.jpg", &width1, &height1, &nrChannels1, 0);
	if (data1)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width1, height1, 0, GL_RGB, GL_UNSIGNED_BYTE, data1);
		glGenerateMipmap(texture1);
	}
	else
	{
		std::cout << "Failed to load texture1" << std::endl;
	}
	stbi_image_free(data1);

	unsigned int texture2;
	glGenTextures(1, &texture2);
	glBindTexture(GL_TEXTURE_2D, texture2);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width2, height2, nrChannels2;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* data2 = stbi_load("awesomeface.png", &width2, &height2, &nrChannels2, 0);
	if (data2)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width2, height2, 0, GL_RGBA, GL_UNSIGNED_BYTE, data2);
		glGenerateMipmap(texture2);
	}
	else
	{
		std::cout << "Failed to load texture2" << std::endl;
	}
	stbi_image_free(data2);

	ourShader.use(); // don't forget to activate the shader before setting uniforms!  
	glUniform1i(glGetUniformLocation(ourShader.ID, "texture1"), 0); // set it manually
	ourShader.setInt("texture2", 1); // or with shader class

	//// transform and translate
	//glm::mat4 trans = glm::mat4(1.0f);
	//trans = glm::rotate(trans, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	//trans = glm::translate(trans, glm::vec3(0.5f, 0.0f, 0.0f));
	//trans = glm::scale(trans, glm::vec3(0.5f, 0.5f, 0.5f));
	//unsigned int transformLoc = glGetUniformLocation(ourShader.ID, "transform");
	//glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));

	// uncomment this call to draw in wireframe polygons.
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// ---------------------------------------------------------
	// --------------------------------------------------------- our render loop (smth like update in unity!)
	// ---------------------------------------------------------

	unsigned int transformLoc = glGetUniformLocation(ourShader.ID, "transform");
	glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(glm::mat4(1.0f)));
	glm::mat4 shapeTransform = glm::mat4(1.0f);
	glm::mat4 shapeScale = glm::mat4(1.0f);
	glm::mat4 shapeRotate = glm::mat4(1.0f);
	while (!glfwWindowShouldClose(window))
	{
		// input handler
		processInput(window);

		if (inputing)
		{
			inputing = false;
			shapeTransform = transByInput * shapeTransform;
			shapeScale = scaleByInput * shapeScale;
			shapeRotate = rotateByInput * shapeRotate;
			transByInput = glm::mat4(1.0f);
			scaleByInput = glm::mat4(1.0f);
			rotateByInput = glm::mat4(1.0f);
			glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(shapeScale * shapeRotate * shapeTransform));
		}
		if (resetShape)
		{
			resetShape = false;
			glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(glm::mat4(1.0f)));
			shapeTransform = glm::mat4(1.0f);
			shapeScale = glm::mat4(1.0f);
			shapeRotate = glm::mat4(1.0f);
		}

		// ----- Rendering stuff

		// seting the clear color
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		// clear the window color buffer bit
		glClear(GL_COLOR_BUFFER_BIT);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);

		ourShader.use();
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		// double buffer mechanism to render things smoothly without user seeing the acutal drawings
		glfwSwapBuffers(window);
		// checks if any events are created
		glfwPollEvents();
	}

	// optional: de-allocate all resources once they've outlived their purpose:
	// ------------------------------------------------------------------------
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);

	// glfw: terminate, clearing all previously allocated GLFW resources.
	// ------------------------------------------------------------------
	glfwTerminate();

	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);


	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) // move right
	{
		transByInput = glm::translate(transByInput, glm::vec3(translateForce, 0.0f, 0.0f));
		inputing = true;
	}
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) // move up
	{
		transByInput = glm::translate(transByInput, glm::vec3(0.0f, translateForce, 0.0f));
		inputing = true;
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) // move left
	{
		transByInput = glm::translate(transByInput, glm::vec3(-translateForce, 0.0f, 0.0f));
		inputing = true;
	}
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) // move down
	{
		transByInput = glm::translate(transByInput, glm::vec3(0.0f, -translateForce, 0.0f));
		inputing = true;
	}

	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) // scale down
	{
		scaleByInput = glm::scale(scaleByInput, glm::vec3(1 - scaleForce, 1 - scaleForce, 1 - scaleForce));
		inputing = true;
	}
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) // scale up
	{
		scaleByInput = glm::scale(scaleByInput, glm::vec3(1 + scaleForce, 1 + scaleForce, 1 + scaleForce));
		inputing = true;
	}

	if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS) // Z axis rotate right
	{
		rotateByInput = glm::rotate(rotateByInput, glm::radians(rotateForce), glm::vec3(0.0f, 0.0f, 1.0f));
		inputing = true;
	}
	if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS) // Z axis rotate lefts
	{
		rotateByInput = glm::rotate(rotateByInput, glm::radians(-rotateForce), glm::vec3(0.0f, 0.0f, 1.0f));
		inputing = true;
	}

	if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
	{
		resetShape = true;
	}
}