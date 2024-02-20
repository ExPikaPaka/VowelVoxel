#include "Mesh.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


namespace ent {
	namespace model {
		Mesh::Mesh(std::vector<Vertex>& vertices, std::vector<unsigned int>& indices, std::vector<Texture>& textures) {
			this->vertices = vertices;
			this->indices = indices;
			this->textures = textures;

			VAO = 0;
			VBO = 0;
			EBO = 0;
			position = { 0, 0, 0 };
			usePosition = false;
			readyToDraw = false;
		}

		Mesh::Mesh(const Mesh& other) {
			vertices = other.vertices;
			indices = other.indices;
			textures = other.textures;

			VAO = 0;
			VBO = 0;
			EBO = 0;
			position = other.position;
			usePosition = other.usePosition;
			readyToDraw = false;
		}

		Mesh::Mesh(Mesh&& other) {
			vertices = other.vertices;
			indices = other.indices;
			textures = other.textures;

			VAO = 0;
			VBO = 0;
			EBO = 0;
			position = other.position;
			usePosition = other.usePosition;
			readyToDraw = false;
		}

		Mesh::Mesh() {
			VAO = 0;
			VBO = 0;
			EBO = 0;
			position = { 0, 0, 0 };
			usePosition = false;
			readyToDraw = false;
		}

		Mesh::~Mesh() {
			clear();
			clearBuffers();
		}

		void Mesh::draw(render::Shader& shader, ui32 mode) {
			// Check if Mesh exists
			if (VAO && readyToDraw) {
				// Apply textures
				unsigned int diffuseNr = 1;
				unsigned int specularNr = 1;
				unsigned int emissiveNr = 1;

				for (unsigned int i = 0; i < textures.size(); i++) {
					glActiveTexture(GL_TEXTURE0 + i); // Activating texture before binding

					std::string number;
					std::string type = textures[i].type;
					if (type == "diffuse")
						number = std::to_string(diffuseNr++);
					if (type == "specular")
						number = std::to_string(specularNr++);
					if (type == "emissive")
						number = std::to_string(emissiveNr++);

					shader.setInt(("material." + type + number).c_str(), i);
					glBindTexture(GL_TEXTURE_2D, textures[i].id);
				}

				// In case if position is specified, translate object to the destination
				f32m4 modelMatrixOld;
				if (usePosition) {
					modelMatrixOld = shader.getMat4("model");

					f32m4 modelMatrixNew = glm::translate(f32m4(1), position);
					shader.setMat4("model", modelMatrixNew);
				}

				// Render
				glBindVertexArray(VAO);
				glDrawElements(mode, indices.size(), GL_UNSIGNED_INT, 0);
				glBindVertexArray(0);

				// Return model matrix to previous
				if (usePosition) {
					shader.setMat4("model", modelMatrixOld);
				}
			}
		}

		bool Mesh::ready() {
			return readyToDraw;
		}

		void Mesh::setPosition(f32v3 position) {
			this->position = position;
			usePosition = true;
		}

		void Mesh::clear() {
			vertices.clear();
			indices.clear();
			textures.clear();
		}

		void Mesh::shrink_to_fit() {
			vertices.shrink_to_fit();
			indices.shrink_to_fit();
			textures.shrink_to_fit();
		}

		void Mesh::clearBuffers() {
			if (VAO) {
				glDeleteVertexArrays(1, &VAO);
			}
			if (VBO) {
				glDeleteBuffers(1, &VBO);
			}
			if (EBO) {
				glDeleteBuffers(1, &EBO);
			}
		}

		Mesh& Mesh::operator=(Mesh& other) {
			if (this == &other)
				return *this;

			vertices = other.vertices;
			indices = other.indices;
			textures = other.textures;

			VAO = 0;
			VBO = 0;
			EBO = 0;
			position = other.position;
			usePosition = other.usePosition;

			return *this;
		}

		Mesh& Mesh::operator=(Mesh&& other) {
			if (this == &other)
				return *this;

			vertices = other.vertices;
			indices = other.indices;
			textures = other.textures;

			VAO = 0;
			VBO = 0;
			EBO = 0;
			position = other.position;
			usePosition = other.usePosition;

			return *this;
		}

		void Mesh::setupTexture() {
			for (ui32 i = 0; i < textures.size(); i++) {
				if (!textures[i].id && textures[i].texImg) { // If texture is not configured & texture image provided

					ui32 textureID;
					glGenTextures(1, &textureID);

					glBindTexture(GL_TEXTURE_2D, textureID);
					glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textures[i].w, textures[i].h, 0, GL_RGB, GL_UNSIGNED_BYTE, textures[i].texImg);

					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);


					textures[i].id = textureID;
					delete[] textures[i].texImg;


					// Check for OpenGL errors
					GLenum error = glGetError();
					if (error != GL_NO_ERROR) {
						// Handle error: OpenGL error occurred
						// Print error message or log it
						// You might need to debug further to identify the cause of the error
						std::cout << "GL_ERROR (Mesh) error: " << error << "\n";
 
					}
				}
			}
		}

		void Mesh::setupMesh() {
			if (!readyToDraw) {
				if (vertices.size() && !VAO) {
					setupTexture();

					glGenVertexArrays(1, &VAO);

					glGenBuffers(1, &VBO);
					glGenBuffers(1, &EBO);

					glBindVertexArray(VAO);

					glBindBuffer(GL_ARRAY_BUFFER, VBO);
					glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

					glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
					glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(ui32), &indices[0], GL_STATIC_DRAW);

					// Position bind
					glEnableVertexAttribArray(0);
					glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(0));
					// Normal bind
					glEnableVertexAttribArray(1);
					glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
					// TexCoord bind
					glEnableVertexAttribArray(2);
					glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoord));

					glBindVertexArray(0);

					readyToDraw = true;
				}
			}
		}
	}
}