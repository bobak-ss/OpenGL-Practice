#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

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
"   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\n\0";

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



	// ---------------------------------------------------------
	// --------------------------------------------------------- Shader
	// ---------------------------------------------------------

	// we create a refrence to our shader (we create an object to store our shader)
	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);

	// we attach the source code for our shader to our shader object
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	// check for vertex shader compile errors
	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	// fragmant shader
	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADERR::FRAGMENT::COMILATION_FAILD\n" << infoLog << std::endl;
	}

	// we create a shader program to link our shaders togheter
	// (out put of first shader goes to input of second shader and so on...)
	unsigned int shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADERR::PROGRAM::LINKING_FAILD\n" << infoLog << std::endl;
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	// ---------------------------------------------------------
	// ---------------------------------------------------------
	// ---------------------------------------------------------

	// array of our triangle vertices in 3D space
	float vertices[] = {
		-0.5f, -0.5f, 0.0f,
		 0.5f, -0.5f, 0.0f,
		 0.0f,  0.5f, 0.0f
	};

	// we create Vertex Buffer Object to store our vertices data on GPU
	// to avoid multiple calls from CPU to GPU 
	unsigned int VBO;
	glGenBuffers(1, &VBO);

	// we bind buffer type GL_ARRAY_BUFFER to our vertex buffer object(vbo)
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	// copy the vertex data we created to the buffer's memory
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// generating a Vertex Array Object
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);

	glBindVertexArray(VAO);

	// Linking vertex attributes
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0); // unbindes the VAO

	// uncomment this call to draw in wireframe polygons.
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);


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

		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		//glBindVertexArray(0); // unbindes the VAO

		// double buffer mechanism to render things smoothly without user seeing the acutal drawings
		glfwSwapBuffers(window);
		// checks if any events are created
		glfwPollEvents();
	}

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