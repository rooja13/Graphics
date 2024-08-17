#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include <cmath>
#include <cstdlib>

const float PI = 3.14159265358979323846f;

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
"   FragColor = vec4(0.5f, 0.2f, 1.0f, 1.0f);\n"
"}\n\0";

unsigned int VAO, VBO, EBO;
unsigned int shaderProgram;
float increment = 5;
int numberOfInnerVertices = 0;
float innerRadius = 0.4f;
float outerRadius = 0.6f;
float radiusChange = 0.05f;

std::vector<float> vertex_data;
std::vector<unsigned int> donut_indices;

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
	for (int i = 0; i < increment * 2; i++) {
		donut_indices.push_back(i);
		donut_indices.push_back(i + numberOfInnerVertices);
		donut_indices.push_back(i + numberOfInnerVertices + 1);

		donut_indices.push_back(i + numberOfInnerVertices + 1);
		donut_indices.push_back(i + 1);
		donut_indices.push_back(i);
	}
}

void computeCircleCoordinates() {
	std::vector<float> inner_vertices, outer_vertices;
	float x = 0;
	float y = 0;
	float z = 0;
	for (float theta = 0; theta <= 2*PI + PI / increment; theta += PI / increment) {
		x = cos(theta);
		y = sin(theta);

		inner_vertices.push_back(x * innerRadius);
		inner_vertices.push_back(y * innerRadius);
		inner_vertices.push_back(z);

		outer_vertices.push_back(x * outerRadius);
		outer_vertices.push_back(y * outerRadius);
		outer_vertices.push_back(z);
	}
	numberOfInnerVertices = inner_vertices.size()/3;
	vertex_data.insert(vertex_data.end(), inner_vertices.begin(), inner_vertices.end());
	vertex_data.insert(vertex_data.end(), outer_vertices.begin(), outer_vertices.end());
}

void printVector() {
	std::cout << "Number of inner vertices: " << vertex_data.size() / 6 << std::endl;
	std::cout << "Number of total Vertices: " << vertex_data.size() / 3 << std::endl;
	std::cout << "Size of index buffer vector: " << donut_indices.size() << std::endl;
	int lineNumber = 1;
	for (int i = 0; i < donut_indices.size(); i += 3) {
		std::cout<< lineNumber<<": (" << donut_indices[i] << ", " << donut_indices[i + 1] << ", " << donut_indices[i + 2]<< ")" << std::endl;
		lineNumber++;
	}
}

void updateBuffers() {
	vertex_data.clear();
	donut_indices.clear();
	computeCircleCoordinates();
	computeCircleIndices();
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertex_data.size() * sizeof(float), &vertex_data[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, donut_indices.size() * sizeof(unsigned int), &donut_indices[0], GL_STATIC_DRAW);

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
			updateBuffers();
			std::cout << "Circle decreased to " << 2 * increment << " sides\n";
		}
		if (key == GLFW_KEY_W) {
			// Increase increment size
			if (increment >= 100) return;
			system("cls");
			increment++;
			updateBuffers();
			std::cout << "Circle increased to " << 2 * increment << " sides\n";
		}
		if (key == GLFW_KEY_A) {
			// Decrease donut thickness
			if ((outerRadius - radiusChange <= innerRadius + radiusChange) || (innerRadius + radiusChange >= outerRadius - radiusChange)) return;
			outerRadius -= radiusChange;
			innerRadius += radiusChange;
			updateBuffers();
		}
		if (key == GLFW_KEY_S) {
			// Increase donut thickness
			if ((outerRadius + radiusChange > 0.9f) || (innerRadius - radiusChange < 0.1f)) return;
			outerRadius += radiusChange;
			innerRadius -= radiusChange;
			updateBuffers();
		}
		if (key == GLFW_KEY_ESCAPE) {
			glfwSetWindowShouldClose(window, true);
		}
		if (key == GLFW_KEY_P) {
			printVector();
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
	GLFWwindow* window = glfwCreateWindow(800, 800, "Make a Donut", NULL, NULL);
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

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	updateBuffers();

	//Render loop
	while (!glfwWindowShouldClose(window)) {

		// Render commands here
		glClearColor(0.1f, 0.0f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		//Draw our object
		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, vertex_data.size(), GL_UNSIGNED_INT, 0);

		//check and call events and call buffers
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}