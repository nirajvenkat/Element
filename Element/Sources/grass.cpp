#include <grass.h>

// Grass rendering and animation class

#include <chrono>
#include <ctime>
#include <cstdio>
#include <cstdlib>

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
	glBufferData(GL_ARRAY_BUFFER, sizeof(grassBlade), grassBlade, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glBindVertexArray(0);
}

void Grass::assemblePatches(){
	// Seed random generator
	srand((unsigned)time(NULL));

	for (float x = -20; x < 20; x++) {
		for (float z = -2; z < 5; z++) {
			for (float i = 0; i < 5; i++) {
				// Float between 0 and 1
				float randomx = (float)rand() / RAND_MAX;
				float randomz = (float)rand() / RAND_MAX;

				patchPositions.push_back(glm::vec3(x + randomx, -1.75f, -z + randomz));
			}
		}
	}
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
		glPatchParameteri(GL_PATCH_VERTICES, 3);
		glDrawArrays(GL_PATCHES, 0, 3);
	}
	glBindVertexArray(0);
}