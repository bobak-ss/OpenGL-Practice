#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <Windows.h>
#include <Shader.h>
#include <stb_image.h>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

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
	

	unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height, nrChannels;
	unsigned char* data = stbi_load("Container.jpg", &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(texture);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);

	// uncomment this call to draw in wireframe polygons.
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// ---------------------------------------------------------
	// --------------------------------------------------------- our render loop (smth like update in unity!)
	// ---------------------------------------------------------

	while (!glfwWindowShouldClose(window))
	{
		// input handler
		processInput(window);

		// ----- Rendering stuff

		// seting the clear color
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		// clear the window color buffer bit
		glClear(GL_COLOR_BUFFER_BIT);

		glBindTexture(GL_TEXTURE_2D, texture);

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
}