#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include <cmath>
#include <cstdlib>

const float PI = 3.14159265358979323846;

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

unsigned int cir_VAO, cir_VBO, cir_EBO;
unsigned int shaderProgram;
float increment = 2;

std::vector<float> circle_vertices;
std::vector<unsigned int> circle_indices;

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

void computeCircleIndices() {
	for (int i = 2; i < circle_vertices.size(); i++) {
		circle_indices.push_back(0);
		circle_indices.push_back(i - 1);
		circle_indices.push_back(i);
	}
}

void computeCircleCoordinates(float radius) {
	float x, y, z = 0;
	for (float theta = 0; theta <= 2 * PI + PI / increment; theta += PI / increment) {
		x = cos(theta) * radius;
		y = sin(theta) * radius;
		circle_vertices.push_back(x);
		circle_vertices.push_back(y);
		circle_vertices.push_back(z);
	}
	for (int i = 0; i < 3; i++) {
		circle_vertices.insert(circle_vertices.begin(), 0.0f);
	}
}

void circleBuffers() {
	computeCircleCoordinates(0.5f);
	computeCircleIndices();
	glGenVertexArrays(1, &cir_VAO);
	glGenBuffers(1, &cir_VBO);
	glGenBuffers(1, &cir_EBO);

	glBindVertexArray(cir_VAO);

	glBindBuffer(GL_ARRAY_BUFFER, cir_VBO);
	glBufferData(GL_ARRAY_BUFFER, circle_vertices.size() * sizeof(float), &circle_vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cir_EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, circle_indices.size() * sizeof(unsigned int), &circle_indices[0], GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
void updateCircleBuffers() {
	circle_vertices.clear();
	circle_indices.clear();
	computeCircleCoordinates(0.5f);
	computeCircleIndices();
	glBindVertexArray(cir_VAO);

	glBindBuffer(GL_ARRAY_BUFFER, cir_VBO);
	glBufferData(GL_ARRAY_BUFFER, circle_vertices.size() * sizeof(float), &circle_vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cir_EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, circle_indices.size() * sizeof(unsigned int), &circle_indices[0], GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

//callback function handles resize of window
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (action == GLFW_PRESS) {  // Only handle the key press event
		if (key == GLFW_KEY_Q) {
			// Decrease increment size
			if (increment <= 2) return;
			system("cls");
			increment--;
			updateCircleBuffers();
			std::cout << "Circle decreased to " << 2 * increment << " sides\n";
		}
		if (key == GLFW_KEY_W) {
			// Increase increment size
			if (increment >= 100) return;
			system("cls");
			increment++;
			updateCircleBuffers();
			std::cout << "Circle increased to " << 2 * increment << " sides\n";
		}
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
	GLFWwindow* window = glfwCreateWindow(800, 800, "Make a Circle", NULL, NULL);
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
	circleBuffers();

	//Render loop
	while (!glfwWindowShouldClose(window)) {

		// Render commands here
		glClearColor(0.1f, 0.0f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		//Draw our object
		glUseProgram(shaderProgram);
		glBindVertexArray(cir_VAO);
		glDrawElements(GL_TRIANGLES, 6 * increment, GL_UNSIGNED_INT, 0);

		//check and call events and call buffers
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}