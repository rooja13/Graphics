#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

// storing vertex and fragment shaders
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

GLuint VAO, VBO, EBO;
unsigned int shaderProgram;

GLfloat f_vertices[] = {
	// Standing part of F
	-0.15f, -0.33f, 0.0f, // bottom left
	0.0f, -0.33f, 0.0f, // bottom right
	-0.15f, 0.5f, 0.0f, // top left
	0.0f, 0.5f, 0.0f, // top right
	// Top Arm of F
	0.15f, 0.5f, 0.0f, // top right
	0.15f, 0.33f, 0.0f, // bottom right
	0.0f, 0.33f, 0.0f, // bottom left
	// Bottom Arm of F
	0.0f, 0.16f, 0.0f, // top left
	0.15f, 0.16f, 0.0f, // top right
	0.15f, 0.0f, 0.0f, // bottom right
	0.0f, 0.0f, 0.0f, // bottom left
};
GLuint f_indecies[] = {
	0, 1, 3,
	3, 2, 0,
	4, 3, 6,
	6, 5, 4,
	8, 7, 10,
	10, 9, 8
};

void setupShaders() {
	//Build and compile our shader program
	//------------------------------------
	//Vertex Shader
	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	// check for shader compile errors
	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	//Fragment Shader
	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	// check for shader compile errors
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	//Link Shaders with Shader Program
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

}

void letterBuffers() {
	// generate the buffers
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	//bind the Vertex Array Object
	glBindVertexArray(VAO);

	//Bind Vertex Buffer Object and store the data
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(f_vertices), f_vertices, GL_STATIC_DRAW);

	//Bind the Element Object Buffer and store the data
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(f_indecies), f_indecies, GL_STATIC_DRAW);

	//add the vertex attributes to the VAO
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//unbinding
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

// CALLBACK FUNCTIONS
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (action == GLFW_PRESS) {  // Only handle the key press event
		if (key == GLFW_KEY_ESCAPE) {
			glfwSetWindowShouldClose(window, true);
		}
	}
}

int main() {
	//initialize OpenGL and configure settings
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//Create Window
	GLFWwindow* window = glfwCreateWindow(800, 800, "Press F to Pay Respects", NULL, NULL);
	if (window == NULL) {
		std::cout << "Failed to create GLFW window\n";
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetKeyCallback(window, keyCallback);

	//Load OpenGl function pointers with GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD\n";
		return -1;
	}
	setupShaders();
	letterBuffers();

	//Render loop
	while (!glfwWindowShouldClose(window)) {

		// Render commands here
		glClearColor(0.1f, 0.0f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		//Draw our object
		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 18, GL_UNSIGNED_INT, 0);

		//check and call events and call buffers
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}