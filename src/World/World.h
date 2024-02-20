#pragma once
#include "SVO.h"
#include "SVOConverter.h"
#include "..\Render\Shader.h"
#include "..\Utility\ThreadPool.h"
#include "..\Algorithm\PerlinNoise.h"
#include <mutex>
#include <list>

namespace ent {
	namespace world {

		class World {
		public:
			World();
			util::ThreadPool pool;

			i32v3 origin;
			void gen(i32v3 chunkPosition);
			void draw(render::Shader& shader);
			void clear();

			void s();
			std::vector<i32v3> occupied;
		private:
			algorithm::PerlinNoise noise;

			i32 chunkCount = 0;
			std::mutex chunkMutex; // Add a mutex to protect chunkMesh
			std::vector<SVO<Voxel>> chunk;
			std::list<model::Mesh> chunkMesh;
		};
	}
}

