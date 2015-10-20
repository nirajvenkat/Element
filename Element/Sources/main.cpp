// Local Headers
#include "element.hpp"
#include "simple.hpp"

// System Headers
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// GL includes
#include "mesh.hpp"
#include "model.hpp"
#include "shader.hpp"
#include "camera.hpp"

// GLM Mathemtics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Standard Headers
#include <cstdio>
#include <cstdlib>
#include <time.h>

// Properties
GLuint screenWidth = 800, screenHeight = 600;
bool ALLOW_FULLSCREEN = false;
bool DRAW_MODELS = false;

// Function prototypes
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void Do_Movement();

// Camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
bool keys[1024];
GLfloat lastX = 400, lastY = 300;
bool firstMouse = true;

GLuint deltaTime = 0;
GLuint lastFrame = 0;

// Vertex array objects
GLuint transparentVAO, transparentVBO;

void physics(){
	//TODO
	glm::vec3 planet1Pos = glm::vec3(0.0, 0.1, 0.7);
	glm::vec3 planet1Vel = glm::vec3(0.3, 0.1, 0.7);
	//Update these values using bullet
	//std::cout << "first planet" << " (" << planet1Pos.x << "," << planet1Pos.y << "," << planet1Pos.z << ") " << "traveling at speed: " << "(" << planet1Vel.x << "," << planet1Vel.y << "," << planet1Vel.z << ")" << std::endl;
}

void setupGrassVAO(){
	glGenVertexArrays(1, &transparentVAO);
	glGenBuffers(1, &transparentVBO);
	glBindVertexArray(transparentVAO);
	glBindBuffer(GL_ARRAY_BUFFER, transparentVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(transparentVertices), transparentVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glBindVertexArray(0);
}

void updateDeltaTime(){
	GLuint currentFrame = clock();
	if (lastFrame == 0) deltaTime = 20;
	else deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;
}

void updateWindIntensity(GLfloat* windSpeed) {
	if (*windSpeed >= 1.0f) *windSpeed = 0.0f;
	*windSpeed += 0.05f;
}

// The MAIN function, from here we start our application and run our Game loop
int main()
{
	// Load GLFW and Create a Window
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	glfwWindowHint(GLFW_SAMPLES, 4);
	// Decide whether to hint to GLFW to use 64 bit
	/*glfwWindowHint(GLFW_RED_BITS, 8);
	glfwWindowHint(GLFW_GREEN_BITS, 8);
	glfwWindowHint(GLFW_BLUE_BITS, 8);
	glfwWindowHint(GLFW_ALPHA_BITS, 8);
	glfwWindowHint(GLFW_DEPTH_BITS, 32);*/

	auto primaryMonitor = glfwGetPrimaryMonitor();
	if (!ALLOW_FULLSCREEN) primaryMonitor = nullptr;
	auto mWindow = glfwCreateWindow(mWidth, mHeight, "Element - OpenGL", primaryMonitor, nullptr);
	
	// Check for Valid Context
	if (mWindow == nullptr) {
		fprintf(stderr, "Failed to Create OpenGL Context");
		return EXIT_FAILURE;
	}

	// Set keyboard event callback
	glfwSetKeyCallback(mWindow, key_callback);

	// Set mouse event callback
	glfwSetCursorPosCallback(mWindow, mouse_callback);

	// Set scroll wheel event callback
	// TODO

	// Create Context and Load OpenGL Functions
	glfwMakeContextCurrent(mWindow);
	gladLoadGL();
	fprintf(stderr, "OpenGL %s\n", glGetString(GL_VERSION));

	// Setup some OpenGL options
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_MULTISAMPLE);

	// Setup and compile our shaders
	Shader defaultShader("C:/Users/Niraj/Desktop/GitRepos/Element/Element/Shaders/sample_vert.glv", "C:/Users/Niraj/Desktop/GitRepos/Element/Element/Shaders/sample_frag.glf");
	Shader grassShader("C:/Users/Niraj/Desktop/GitRepos/Element/Element/Shaders/grass.glv", "C:/Users/Niraj/Desktop/GitRepos/Element/Element/Shaders/grass.glf");

	// Load models
	Model nano("C:/Users/Niraj/Desktop/GitRepos/Element/Element/Models/Nanosuit/nanosuit.obj");
	//Model sponza("C:/Users/Niraj/Desktop/GitRepos/Element/Element/Models/Sponza/SponzaNoFlag.obj");

	// Load textures
	GLuint transparentTexture = loadTexture("C:/Users/Niraj/Desktop/GitRepos/Element/Element/Textures/grass.png", true);

	// Draw in wireframe
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// Vars
	GLfloat windIntensity = 1.0f;

	// Rendering Loop
	while (glfwWindowShouldClose(mWindow) == false) {
		if (glfwGetKey(mWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(mWindow, true);

		updateDeltaTime();
		physics();

		// Background Fill Color
		glClearColor(0.25f, 0.25f, 0.25f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Transformation matrices
		glm::mat4 model = glm::mat4();
		glm::mat4 projection = glm::perspective(camera.Zoom, (float)screenWidth / (float)screenHeight, 0.1f, 200.0f);
		glm::mat4 view = camera.GetViewMatrix();

		grassShader.Use();    // <-- Grass
		std::vector<glm::vec3> grassVec;
		grassVec.push_back(glm::vec3(-1.5f, 0.0f, -0.48f));
		grassVec.push_back(glm::vec3(1.5f, 0.0f, 0.51f));
		grassVec.push_back(glm::vec3(0.0f, 0.0f, 0.7f));
		grassVec.push_back(glm::vec3(-0.3f, 0.0f, -2.3f));
		grassVec.push_back(glm::vec3(0.5f, 0.0f, -0.6f));

		setupGrassVAO();
		//updateWindIntensity(&windIntensity);

		glBindVertexArray(transparentVAO);
		glBindTexture(GL_TEXTURE_2D, transparentTexture);
		for (glm::vec3 grassPos : grassVec) {
			model = glm::mat4();
			model = glm::translate(model, grassPos);
			model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
			glUniformMatrix4fv(glGetUniformLocation(grassShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
			glUniformMatrix4fv(glGetUniformLocation(grassShader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
			glUniformMatrix4fv(glGetUniformLocation(grassShader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
			glUniform3f(glGetUniformLocation(grassShader.Program, "windDirection"), 0.0f, 0.0f, 1.0f);
			glUniform1f(glGetUniformLocation(grassShader.Program, "windIntensity"), windIntensity);
			glUniform1f(glGetUniformLocation(grassShader.Program, "time"), clock());
			glDrawArrays(GL_TRIANGLES, 0, 6);
			for (int i = 0; i < 4; i++){
				model = glm::translate(model, glm::vec3(0.5f, 0.0f, 0.5f));
				model = glm::rotate(model, 45.0f, glm::vec3(0.0f, 1.0f, 0.0f));
				glUniformMatrix4fv(glGetUniformLocation(grassShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
				glDrawArrays(GL_TRIANGLES, 0, 6);
			}
		}
		glBindVertexArray(0);

		if (DRAW_MODELS){
			defaultShader.Use();   // <-- First nano suit!
			glUniformMatrix4fv(glGetUniformLocation(defaultShader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
			glUniformMatrix4fv(glGetUniformLocation(defaultShader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));

			// Draw the loaded model
			model = glm::translate(model, glm::vec3(0.0f, -1.75f, 0.0f)); // Translate it down a bit so it's at the center of the scene
			model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));	// It's a bit too big for our scene, so scale it down
			glUniformMatrix4fv(glGetUniformLocation(defaultShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
			nano.Draw(defaultShader);

			defaultShader.Use();   // <-- Sponza
			glUniformMatrix4fv(glGetUniformLocation(defaultShader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
			glUniformMatrix4fv(glGetUniformLocation(defaultShader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));

			// Draw the loaded model
			model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));	// It's a bit too big for our scene, so scale it down
			glUniformMatrix4fv(glGetUniformLocation(defaultShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
			sponza.Draw(defaultShader);
		}

		// Flip Buffers and Draw
		glfwSwapBuffers(mWindow);
		glfwPollEvents();
	}   glfwTerminate();
	return EXIT_SUCCESS;
}

#pragma region "User input"

// Moves/alters the camera positions based on user input
void Do_Movement()
{
	// Camera controls
	if (keys[GLFW_KEY_W])
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (keys[GLFW_KEY_S])
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (keys[GLFW_KEY_A])
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (keys[GLFW_KEY_D])
		camera.ProcessKeyboard(RIGHT, deltaTime);
	if (keys[GLFW_KEY_SPACE])
		camera.ProcessKeyboard(UP, deltaTime);
	if (keys[GLFW_KEY_LEFT_SHIFT])
		camera.ProcessKeyboard(DOWN, deltaTime);
}

// Is called whenever a key is pressed/released via GLFW
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	Do_Movement();

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);

	if (action == GLFW_PRESS)
		keys[key] = true;
	else if (action == GLFW_RELEASE)
		keys[key] = false;
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	GLfloat xoffset = xpos - lastX;
	GLfloat yoffset = lastY - ypos;

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}

#pragma endregion