#pragma once
#include <vector>
#include <string>
#include "..\dataTypes.h"
#include "..\Render\Shader.h"

namespace ent {
	namespace model {
		// Single vertex structure
		struct Vertex {
			f32v3 position;
			f32v3 normal;
			f32v2 texCoord;
		};

		// Texture structure
		struct Texture {
			ui32 id = 0;
			std::string type;
			std::string path;
			ui8* texImg = 0;
			ui32 w = 0;
			ui32 h = 0;
		};

		// Single Mesh class
		class Mesh {
		public:
			std::vector<Vertex>       vertices;
			std::vector<unsigned int> indices;
			std::vector<Texture>      textures;
			f32v3 position;
			bool usePosition;

			Mesh(std::vector<Vertex>& vertices, std::vector<unsigned int>& indices, std::vector<Texture>& textures);
			Mesh(const Mesh& other);
			Mesh(Mesh&& other);
			Mesh();
			~Mesh();
			void setupMesh();
			void draw(render::Shader& shader, ui32 mode = GL_TRIANGLES);
			bool ready();
			void setPosition(f32v3 position);

			void clear();
			void shrink_to_fit();

			void clearBuffers();

			Mesh& operator=(Mesh& other);
			Mesh& operator=(Mesh&& other);

		private:
			void setupTexture();
			bool readyToDraw;
			unsigned int VAO, VBO, EBO;
		};
	}
}
