// Preprocessor Directives
#pragma once

// Assimp
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

// Bullet
#include <btBulletDynamicsCommon.h>

// GL Loader
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Simple constants
#include <simple.h>

//  Reference: https://github.com/nothings/stb/blob/master/stb_image.h#L4
//  To use stb_image, add this in *one* C++ source file.
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

// Constants
const int mWidth = 1920;
const int mHeight = 1080;
