#pragma once
#include <glad/glad.h>
#include "../dataTypes.h"
#include "../Logger/Logger.h"
#include <string>

namespace ent {
	namespace render {
		class Shader {
		public:
			// Basic constructor. Does not load or modify any shaders
			Shader();
			// Loads & compile shaders. Use .use() function to bind shader to current render
			Shader(std::string& vertexShPath, std::string& fragmentShPath);
			~Shader() = default;

			// Loads & compile shaders. Use .use() function to bind shader to current render
			bool load(std::string& vertexShPath, std::string& fragmentShPath);
			// use/activate shaders
			void use();

			// utility uniform functions
			// Sets uniform value type bool to desired state
			void setBool(const char* name, bool value);
			// Sets uniform value type int to desired state
			void setInt(const char* name, int value);
			// Sets uniform value type float to desired state
			void setFloat(const char* name, float value);
			// Sets uniform value type vec3 to desired state
			void setVec3(const char* name, float value1, float value2, float value3);
			// Sets uniform value type vec3 to desired state
			void setVec3(const char* name, glm::vec3 value);
			// Sets uniform value type mat4 to desired state
			void setMat4(const char* name, glm::mat4 matrix);

			// Returns uniform value type mat4 to desired state
			glm::mat4 getMat4(const char* name);
		private:
			ui32 id;

			std::string readShaderFromFile(std::string shaderPath);

			util::Logger* logger;
		};
	}
}