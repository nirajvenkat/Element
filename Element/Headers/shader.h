#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

// System Headers
#include <glad/glad.h>
#include <GLFW/glfw3.h>

class Shader
{
public:
	GLuint Program;
	// Constructor generates the shader on the fly
	Shader(const GLchar* vertexPath, const GLchar* fragmentPath, const GLchar* geometryPath = nullptr, const GLchar* tessControlPath = nullptr, const GLchar* tessEvalPath = nullptr)
	{
		// Detects if geometry shader in use
		bool geomActive = true;
		if (!geometryPath) geomActive = false;

		// Detects if tesselation shaders in use
		bool tessControlActive = true;
		if (!tessControlPath) tessControlActive = false;
		bool tessEvalActive = true;
		if (!tessEvalPath) tessEvalActive = false;


		// 1. Retrieve the vertex/fragment source code from filePath
		std::string vertexCode;
		std::string fragmentCode;
		std::string geometryCode;
		std::string tessControlCode;
		std::string tessEvalCode;
		std::ifstream vShaderFile;
		std::ifstream fShaderFile;
		std::ifstream gShaderFile;
		std::ifstream tcShaderFile;
		std::ifstream teShaderFile;

		// ensures ifstream objects can throw exceptions:
		vShaderFile.exceptions(std::ifstream::badbit);
		fShaderFile.exceptions(std::ifstream::badbit);
		gShaderFile.exceptions(std::ifstream::badbit);
		tcShaderFile.exceptions(std::ifstream::badbit);
		teShaderFile.exceptions(std::ifstream::badbit);


		try
		{
			// Open files
			vShaderFile.open(vertexPath);
			fShaderFile.open(fragmentPath);
			std::stringstream vShaderStream, fShaderStream;
			// Read file's buffer contents into streams
			vShaderStream << vShaderFile.rdbuf();
			fShaderStream << fShaderFile.rdbuf();
			// close file handlers
			vShaderFile.close();
			fShaderFile.close();
			// Convert stream into string
			vertexCode = vShaderStream.str();
			fragmentCode = fShaderStream.str();
		}
		catch (std::ifstream::failure e)
		{
			std::cout << "ERROR::SHADER::VERTEX_FRAGMENT_SHADERS_NOT_SUCCESFULLY_READ" << std::endl;
		}
		if (geomActive) {
			try
			{
				// Open files
				gShaderFile.open(geometryPath);
				std::stringstream gShaderStream;
				// Read file's buffer contents into streams
				gShaderStream << gShaderFile.rdbuf();
				// close file handlers
				gShaderFile.close();
				// Convert stream into string
				geometryCode = gShaderStream.str();
			}
			catch (std::ifstream::failure e)
			{
				std::cout << "ERROR::SHADER::GEOMETRY_SHADER_NOT_SUCCESFULLY_READ" << std::endl;
			}
		}
		if (tessControlActive && tessEvalActive) {
			try
			{
				// Open files
				tcShaderFile.open(tessControlPath);
				teShaderFile.open(tessEvalPath);
				std::stringstream tcShaderStream;
				std::stringstream teShaderStream;
				// Read file's buffer contents into streams
				tcShaderStream << tcShaderFile.rdbuf();
				teShaderStream << teShaderFile.rdbuf();
				// close file handlers
				tcShaderFile.close();
				teShaderFile.close();
				// Convert stream into string
				tessControlCode = tcShaderStream.str();
				tessEvalCode = teShaderStream.str();
			}
			catch (std::ifstream::failure e)
			{
				std::cout << "ERROR::SHADER::TESSELATION_SHADERS_NOT_SUCCESFULLY_READ" << std::endl;
			}
		}
		const GLchar* vShaderCode = vertexCode.c_str();
		const GLchar* fShaderCode = fragmentCode.c_str();
		const GLchar* gShaderCode = geometryCode.c_str();
		const GLchar* tcShaderCode = tessControlCode.c_str();
		const GLchar* teShaderCode = tessEvalCode.c_str();


		// 2. Compile shaders
		GLuint vertex, fragment, geometry, tessControl, tessEval;
		GLint success;
		GLchar infoLog[512];
		// Vertex Shader
		vertex = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertex, 1, &vShaderCode, NULL);
		glCompileShader(vertex);
		// Print compile errors if any
		glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(vertex, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << '[ ' << vertexPath << ']: ' << infoLog << std::endl;
		}
		// Fragment Shader
		fragment = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragment, 1, &fShaderCode, NULL);
		glCompileShader(fragment);
		// Print compile errors if any
		glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(fragment, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << '[ ' << fragmentPath << ']: ' << infoLog << std::endl;
		}
		// Geometry Shader
		if (geomActive) {
			geometry = glCreateShader(GL_GEOMETRY_SHADER);
			glShaderSource(geometry, 1, &gShaderCode, NULL);
			glCompileShader(geometry);
			// Print compile errors if any
			glGetShaderiv(geometry, GL_COMPILE_STATUS, &success);
			if (!success)
			{
				glGetShaderInfoLog(geometry, 512, NULL, infoLog);
				std::cout << "ERROR::SHADER::GEOMETRY::COMPILATION_FAILED\n" << '[ ' << geometryPath << ']: ' << infoLog << std::endl;
			}
		}
		// Tesselation Shaders
		if (tessControlActive && tessEvalActive) {
			tessControl = glCreateShader(GL_TESS_CONTROL_SHADER);
			glShaderSource(tessControl, 1, &tcShaderCode, NULL);
			glCompileShader(tessControl);
			// Print compile errors if any
			glGetShaderiv(tessControl, GL_COMPILE_STATUS, &success);
			if (!success)
			{
				glGetShaderInfoLog(tessControl, 512, NULL, infoLog);
				std::cout << "ERROR::SHADER::TESS_CONTROL::COMPILATION_FAILED\n" << '[ ' << tessControlPath << ']: ' << infoLog << std::endl;
			}
			tessEval = glCreateShader(GL_TESS_EVALUATION_SHADER);
			glShaderSource(tessEval, 1, &teShaderCode, NULL);
			glCompileShader(tessEval);
			// Print compile errors if any
			glGetShaderiv(tessEval, GL_COMPILE_STATUS, &success);
			if (!success)
			{
				glGetShaderInfoLog(tessEval, 512, NULL, infoLog);
				std::cout << "ERROR::SHADER::TESS_EVAL::COMPILATION_FAILED\n" << '[ ' << tessEvalPath << ']: ' << infoLog << std::endl;
			}
		}

		// Shader Program
		this->Program = glCreateProgram();
		glAttachShader(this->Program, vertex);
		glAttachShader(this->Program, fragment);
		if (geomActive) {
			glAttachShader(this->Program, geometry);
		}
		if (tessControlActive && tessEvalActive) {
			glAttachShader(this->Program, tessControl);
			glAttachShader(this->Program, tessEval);
		}
		glLinkProgram(this->Program);
		// Print linking errors if any
		glGetProgramiv(this->Program, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(this->Program, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
		}
		// Delete the shaders as they're linked into our program now and no longer necessery
		glDeleteShader(vertex);
		glDeleteShader(fragment);
		if (geomActive) {
			glDeleteShader(geometry);
		}
		if (tessControlActive && tessEvalActive) {
			glDeleteShader(tessControl);
			glDeleteShader(tessEval);
		}

	}
	// Uses the current shader
	void Use()
	{
		glUseProgram(this->Program);
	}
};

#endif