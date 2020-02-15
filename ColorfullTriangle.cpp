#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

const char* vertexShaderSource = "#version 330 core\n"
	"layout (location = 0) in vec3 aPos;\n"
	"layout (location = 1) in vec3 aColor;\n"
	"out vec3 ourColor;\n"
	"void main()\n"
	"{\n"
	"   gl_Position = vec4(aPos, 1.0);\n"
	"	ourColor = aColor;\n"
	"}\0";

const char* fragmentShaderSource = "#version 330 core\n"
	"out vec4 FragColor;\n"
	"in vec3 ourColor;\n"
	"void main()\n"
	"{\n"
	"   FragColor = vec4(ourColor, 1.0);\n"
	"}\n\0";

const char* fragmentShaderSourceNew = "#version 330 core\n"
	"in vec3 ourColor;\n"
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

	unsigned int fragmentShaderNew = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShaderNew, 1, &fragmentShaderSourceNew, NULL);
	glCompileShader(fragmentShaderNew);
	glGetShaderiv(fragmentShaderNew, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShaderNew, 512, NULL, infoLog);
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

	unsigned int shaderProgramNew = glCreateProgram();
	glAttachShader(shaderProgramNew, vertexShader);
	glAttachShader(shaderProgramNew, fragmentShaderNew);
	glLinkProgram(shaderProgramNew);
	glGetShaderiv(fragmentShaderNew, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShaderNew, 512, NULL, infoLog);
		std::cout << "MY SHADER SEEMS TO SUCK! COMPULE ERROR!\n" << infoLog << std::endl;
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	glDeleteShader(fragmentShaderNew);

	// ---------------------------------------------------------
	// ---------------------------------------------------------
	// ---------------------------------------------------------

	// four points of our rectangle
	float vertex1[] = {
		// positions         // colors
		 0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   // bottom right
		-0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   // bottom left
		 0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f    // top 
	};

	float vertex2[] = {
		0.5, -0.5, 0,
		0, 0.5, 0,
		-0.5, -0.5, 0
	};

	unsigned int VBOs[2], VAOs[2];
	glGenBuffers(2, VBOs);
	glGenVertexArrays(2, VAOs);

	glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
	glBindVertexArray(VAOs[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertex1), vertex1, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

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

		// draw our first triangle
		glUseProgram(shaderProgram);
		glBindVertexArray(VAOs[0]); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glUseProgram(shaderProgramNew);
		glBindVertexArray(VAOs[1]);
		glDrawArrays(GL_TRIANGLES, 0, 3);

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