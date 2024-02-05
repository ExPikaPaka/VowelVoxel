#include "Shader.h"
#include <fstream>
#include <sstream>

ent::render::Shader::Shader() {
	id = 0;
	logger = util::Logger::getInstance();
}

ent::render::Shader::Shader(std::string& vertexShPath, std::string& fragmentShPath) {
	load(vertexShPath, fragmentShPath);
}

bool ent::render::Shader::load(std::string& vertexShPath, std::string& fragmentShPath) {
	logger->addLog("Loading vertex shader \"" + vertexShPath + "\", fragment shader \"" + fragmentShPath + "\"", util::level::DEBUG);

	/////////////////////////////////////
	// Reading shaders
	//
	std::string vShaderCode = readShaderFromFile(vertexShPath);
	std::string fShaderCode = readShaderFromFile(fragmentShPath);

	bool error = false;
	if (!vShaderCode.size()) {
		logger->addLog("Failed to load vertex shader \"" + vertexShPath + "\"", util::level::ERROR);
		error = true;
	}
	if (!fShaderCode.size()) {
		logger->addLog("Failed to load fragment shader \"" +fragmentShPath + "\"", util::level::ERROR);
		error = true;
	}
	if (error) {
		return false;
	}


	/////////////////////////////////////
	// Compiling shaders
	//
	ui32 v, f;
	i32 success = 0;
	char infoLog[512];

	// Vertex shader
	v = glCreateShader(GL_VERTEX_SHADER);
	const char* vShCStr = vShaderCode.c_str();

	glShaderSource(v, 1, &vShCStr, NULL);
	glCompileShader(v);
	
	// Checking errors
	glGetShaderiv(v, GL_COMPILE_STATUS, &success);
	if (!success) {
		error = true;
		glGetShaderInfoLog(v, 512, NULL, infoLog);
		logger->addLog("Failed to compile vertex shader \"" + vertexShPath + "\". Log:" + std::string(infoLog), util::level::ERROR);
	}
	success = 1;

	// Fragment shader
	f = glCreateShader(GL_FRAGMENT_SHADER);
	const char* fShCStr = fShaderCode.c_str();

	glShaderSource(f, 1, &fShCStr, NULL);

	// Checking errors
	glGetShaderiv(v, GL_COMPILE_STATUS, &success);
	if (!success) {
		error = true;
		glGetShaderInfoLog(v, 512, NULL, infoLog);
		logger->addLog("Failed to compile fragment shader \"" + fragmentShPath + "\". Log:" + std::string(infoLog), util::level::ERROR);
	}
	success = 1;

	if (error) {
		return false;
	}

	/////////////////////////////////////
	// Creating shader program
	//
	id = glCreateProgram();
	glAttachShader(id, v);
	glAttachShader(id, f);
	glLinkProgram(id);

	// Check errors
	glGetProgramiv(id, GL_LINK_STATUS, &success);
	if (!success) {
		error = true;
		glGetProgramInfoLog(id, 512, NULL, infoLog);
		logger->addLog("Failed to link shader program. Log:" + std::string(infoLog), util::level::ERROR);
	}
	if (error) {
		return false;
	}

	/////////////////////////////////////
	// Resources free
	//
	glDeleteShader(v);
	glDeleteShader(f);

	logger->addLog("Shaders created successfully", util::level::DEBUG);
	return true;
}

void ent::render::Shader::use() {
	glUseProgram(id);
}

void ent::render::Shader::setBool(const char* name, bool value) {
	glUniform1i(glGetUniformLocation(id, name), (int)value);
}

void ent::render::Shader::setInt(const char* name, int value) {
	glUniform1i(glGetUniformLocation(id, name), value);
}

void ent::render::Shader::setFloat(const char* name, float value) {
	glUniform1f(glGetUniformLocation(id, name), value);
}

void ent::render::Shader::setVec3(const char* name, float value1, float value2, float value3) {
	glUniform3f(glGetUniformLocation(id, name), value1, value2, value3);
}

void ent::render::Shader::setVec3(const char* name, glm::vec3 value) {
	glUniform3f(glGetUniformLocation(id, name), value.x, value.y, value.z);
}

void ent::render::Shader::setMat4(const char* name, glm::mat4 matrix) {
	glUniformMatrix4fv(glGetUniformLocation(id, name), 1, GL_FALSE, &matrix[0][0]);
}

f32m4 ent::render::Shader::getMat4(const char* name) {
	int location = glGetUniformLocation(id, name);

	if (location != -1) {
		f32m4 matrix;
		glGetUniformfv(id, location, &matrix[0][0]);
		return matrix;
	} else {
		// Handle error: the uniform variable with the specified name was not found
		return f32m4(); // Return a default value or handle the error in a suitable way
	}
}

std::string ent::render::Shader::readShaderFromFile(std::string shaderPath) {
	std::ifstream ifs;
	
	// Ensure ifstream object can throw exceptions
	ifs.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	// Reading 
	std::string buffer;
	try {
		// Opening file
		ifs.open(shaderPath);
		std::stringstream shStream;

		// Reading 
		shStream << ifs.rdbuf();

		// Closing file
		ifs.close();

		buffer = shStream.str();

	} catch (const std::ifstream::failure e) {
		logger->addLog("Failed to read shader file \"" + shaderPath + "\"", util::level::ERROR);
	}

	return buffer;
}
