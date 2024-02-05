#include "World.h"

namespace ent {
	namespace world {
		World::World() {
		}
		void World::gen(i32v3 chunkPosition) {
			for (ui32 i = 0; i < occupied.size(); i++) {
				if (occupied[i] == chunkPosition) return;
			}
			occupied.push_back(chunkPosition);

			SVO<Voxel> svo;
			svo.setMaxDepth(4);

			Voxel voxel;
			voxel.id = 10;
			voxel.color = { 70, 80, 60 };
			i32 dim = svo.getDimension();

			for (i32 z = 0; z < 1; z++) {
				for (i32 y = 0; y < dim; y++) {
					for (i32 x = 0; x < dim; x++) {
						i32 h = ((float)rand() / RAND_MAX) * 1.1;

						if (h) {
							voxel.color = { 70, 240, 60 };
						} else {
							voxel.color = { 70, 80, 60 };
						}
						svo.set({ x, y, z + h }, voxel);
					}
				}
			}

			chunk.push_back(svo);


			SVOConverter conv;
			chunkMesh.push_back(conv.convert(svo));

			chunkMesh[chunkMesh.size() - 1].setPosition(chunkPosition);
		}
		void World::draw(render::Shader& shader) {
			for (ui32 i = 0; i < chunkMesh.size(); i++) {
				chunkMesh[i].draw(shader);
			}
		}
	}
}