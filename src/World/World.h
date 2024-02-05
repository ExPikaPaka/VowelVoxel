#pragma once
#include "SVO.h"
#include "SVOConverter.h"
#include "..\Render\Shader.h"
#include <mutex>

namespace ent {
	namespace world {

		class World {
		public:
			World();

			i32v3 origin;

			void gen(i32v3 chunkPosition);
			void draw(render::Shader& shader);
			std::vector<i32v3> occupied;
		private:
			std::mutex chunkMutex; // Add a mutex to protect chunkMesh
			std::vector<SVO<Voxel>> chunk;
			std::vector<model::Mesh> chunkMesh;
		};
	}
}

