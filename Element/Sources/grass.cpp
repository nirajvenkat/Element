#include <grass.h>

// Grass rendering and animation class

#include <chrono>
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
	glGenBuffers(1, &positionVBO);
	glBindVertexArray(grassVAO);

	// Binding the vertices and length descriptor
	glBindBuffer(GL_ARRAY_BUFFER, grassVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(grassBlade), grassBlade, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)(0));
	glVertexAttribDivisor(0, 0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glVertexAttribDivisor(1, 0);

	// Binding the model matrices
	
	int pos = 2;
	int pos1 = pos + 0;
	int pos2 = pos + 1;
	int pos3 = pos + 2;
	int pos4 = pos + 3;
	glBindBuffer(GL_ARRAY_BUFFER, positionVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(patchPositions), &patchPositions, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(pos1);
	glEnableVertexAttribArray(pos2);
	glEnableVertexAttribArray(pos3);
	glEnableVertexAttribArray(pos4);
	glVertexAttribPointer(pos1, 4, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 4 * 4, (GLvoid*)(0));
	glVertexAttribPointer(pos2, 4, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 4 * 4, (GLvoid*)(sizeof(GLfloat) * 4));
	glVertexAttribPointer(pos3, 4, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 4 * 4, (GLvoid*)(sizeof(GLfloat) * 8));
	glVertexAttribPointer(pos4, 4, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 4 * 4, (GLvoid*)(sizeof(GLfloat) * 12));
	glVertexAttribDivisor(pos1, 1);
	glVertexAttribDivisor(pos2, 1);
	glVertexAttribDivisor(pos3, 1);
	glVertexAttribDivisor(pos4, 1);
	
	// So VAO isnt changed from the outside
	glBindVertexArray(0);
}

void Grass::assemblePatches(){
	// Seed random generator
	srand((unsigned)time(NULL));

	for (float x = -20; x < 20; x++) {
		for (float z = -2; z < 5; z++) {
			for (float i = 0; i < 20; i++) {
				// Float between 0 and 1
				float randomx = (float)rand() / RAND_MAX;
				float randomz = (float)rand() / RAND_MAX;

				glm::vec3 pos = glm::vec3(x + randomx, -1.75f, -z + randomz);
				glm::mat4 model = glm::mat4();
				model = glm::translate(model, pos);
				model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
				patchPositions.push_back(model);
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
	//for (glm::mat4 pos : patchPositions) {
		//glm::mat4 model = pos;
		glm::mat4 projection = glm::perspective(45.0f, 800.0f / 600.0f, 0.1f, 200.0f);
		glm::mat4 view = camera.GetViewMatrix();

		//glUniformMatrix4fv(glGetUniformLocation(grassShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(glGetUniformLocation(grassShader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(glGetUniformLocation(grassShader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
		glUniform3f(glGetUniformLocation(grassShader.Program, "windDirection"), 0.0f, 0.0f, 1.0f);
		glUniform1f(glGetUniformLocation(grassShader.Program, "windIntensity"), windIntensity);
		glUniform1f(glGetUniformLocation(grassShader.Program, "time"), (float) std::chrono::high_resolution_clock::now().time_since_epoch().count());
		glPatchParameteri(GL_PATCH_VERTICES, 3);
		//glDrawArrays(GL_PATCHES, 0, 3);
		glDrawArraysInstanced(GL_PATCHES, 0, 3, patchPositions.size());
	//}
	glBindVertexArray(0);
}