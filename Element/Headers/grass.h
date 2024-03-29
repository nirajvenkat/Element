#include <vector>

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <camera.h>
#include <shader.h>

class Grass {
public:
	Grass(GLuint textureID, GLfloat windIntensity);
	void setup();
	void updateWindIntensity();
	void assemblePatches();
	void drawPatches(Camera& camera, Shader& grassShader);

private:
	GLuint textureID;
	GLuint grassVAO, grassVBO, positionVBO;
	std::vector<glm::mat4> patchPositions;
	GLfloat windIntensity;

};