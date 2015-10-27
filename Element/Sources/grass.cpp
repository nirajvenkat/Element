#include <grass.h>

// Grass rendering and animation class

#include <chrono>
#include <ctime>

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <simple.h>

Grass::Grass(GLuint textureID, GLfloat windIntensity = 0.0f){
	this->textureID = textureID;
	this->windIntensity = windIntensity;
}

void Grass::setup(){
	glGenVertexArrays(1, &grassVAO);
	glGenBuffers(1, &grassVBO);
	glBindVertexArray(grassVAO);
	glBindBuffer(GL_ARRAY_BUFFER, grassVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(point), point, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glBindVertexArray(0);
}

void Grass::assemblePatches(){
	patchPositions.push_back(glm::vec3(-1.5f, 0.0f, -0.48f));
	patchPositions.push_back(glm::vec3(1.5f, 0.0f, 0.51f));
	patchPositions.push_back(glm::vec3(0.0f, 0.0f, 0.7f));
	patchPositions.push_back(glm::vec3(-0.3f, 0.0f, -2.3f));
	patchPositions.push_back(glm::vec3(0.5f, 0.0f, -0.6f));
}

void Grass::updateWindIntensity() {
	if (windIntensity >= 1.0f) windIntensity = 0.0f;
	windIntensity += 0.05f;
}

void Grass::drawPatches(Camera& camera, Shader& grassShader){
	glBindVertexArray(grassVAO);
	for (glm::vec3 pos : patchPositions) {
		glm::mat4 model = glm::mat4();
		model = glm::translate(model, pos);
		//model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		glm::mat4 projection = glm::perspective(45.0f, 800.0f / 600.0f, 0.1f, 200.0f);
		glm::mat4 view = camera.GetViewMatrix();

		glUniformMatrix4fv(glGetUniformLocation(grassShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(glGetUniformLocation(grassShader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(glGetUniformLocation(grassShader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
		glUniform3f(glGetUniformLocation(grassShader.Program, "windDirection"), 0.0f, 0.0f, 1.0f);
		glUniform1f(glGetUniformLocation(grassShader.Program, "windIntensity"), windIntensity);
		glUniform1f(glGetUniformLocation(grassShader.Program, "time"), std::chrono::high_resolution_clock::now().time_since_epoch().count());
		glDrawArrays(GL_POINTS, 0, 1);
	}
	glBindVertexArray(0);
}