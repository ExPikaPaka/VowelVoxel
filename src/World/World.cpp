#include "World.h"
#include "../Algorithm/PerlinNoise.h"

namespace ent {
	namespace world {
		World::World() {
            unsigned int numCores = std::thread::hardware_concurrency();
            std::cout << numCores << "\n";

            if (numCores > 2) {
                pool.initThreads(numCores - 2);
            } else {
                pool.initThreads(1);
            }

            pool.pause();

		}
        float generateTerrain(float x) {

            return 3;
        }

		void World::gen(i32v3 chunkPosition) {
			{
				//std::lock_guard<std::mutex> lock(chunkMutex);
				for (const auto& occupiedChunk : occupied) {
					if (occupiedChunk == chunkPosition)
						return;
				}
				occupied.push_back(chunkPosition);
			}

            
            
            // Convert the chunk to mesh in a thread-safe manner
            auto convertAndSetMesh = [this, chunkPosition]() {
                //std::this_thread::sleep_for(std::chrono::milliseconds(1));
                //srand(chunkPosition.x * 3 + chunkPosition.y * 2 + chunkPosition.z);
                SVO<Voxel> svo;
                svo.setMaxDepth(4);

                Voxel voxel;
                voxel.id = 10;
                voxel.color = { 70, 80, 60 };
                const int dim = svo.getDimension();
                algorithm::PerlinNoise noise(0.05);


                voxel.color = { 80, 140, 220 };
                voxel.id = 1;
                for (int z = 0; z < dim; z++) {
                    for (int y = 0; y < dim; y++) {
                        for (int x = 0; x < dim; x++) {
                          
                            if (z + chunkPosition.z * dim < 25) {

                                svo.set({ x, y, z }, voxel);
                            }
                            // svo.set({ x, y, z }, voxel);
                        }
                    }
                }

                voxel.id = 10;
                voxel.color = { 123, 123, 123 };
                for (int z = 0; z < dim; z++) {
                    for (int y = 0; y < dim; y++) {
                        for (int x = 0; x < dim; x++) {
                            f32 xF = (float)x / (float)dim + chunkPosition.x;
                            f32 yF = (float)y / (float)dim + chunkPosition.y;
                            f32 zF = (float)z / (float)dim + chunkPosition.z;

                            

                            f32 v = noise.fractal(5, xF, yF, zF) + 1;
                            v *= 0.5;
                            v *= 255;


                            //v += 10;
                            v *= 1.0 - zF * 0.1;
                            //v *= 2.0 - zF * 0.2;
                            v = (ui8)v;

                            voxel.color = { 123, 123, 123 };
                            

                            //v = v > 120 ? v < 180 ? 255 : 0 : 0;
                            v = v > 120 ? 255 : 0;


                            if (v) {
                                svo.set({ x, y, z }, voxel);
                            } else {
                                if (z + chunkPosition.z * dim > 27) {
                                    voxel.color = { 70, 100, 50 };
                                    if (svo.get({ x,y,z - 1 })) {
                                        svo.set({ x,y,z - 1 }, voxel);
                                    }


                                } else {

                                    if (z + chunkPosition.z * dim > 24) {
                                        voxel.color = { 220, 210, 170 };
                                        Voxel vox = svo.get({ x,y,z - 1 });

                                        if (vox && vox.id != 1) {
                                            svo.set({ x,y,z - 1 }, voxel);
                                        }
                                    }
                                }
                            }
                           // svo.set({ x, y, z }, voxel);
                        }
                    }
                }

                
                SVOConverter conv;
                svo.calculate();
                auto mesh = conv.convert(svo);
                //std::cout << mesh.indices.size() / 2.5 << "   triangles-\n";

                f32v3 p = chunkPosition;
                mesh.setPosition(p);

                //std::cout << mesh.textures.size() << " - textures size\n";
               /* for (ui32 i = 0; i < mesh.textures.size(); i++) {
                    std::cout << "Setup texture " << i << "\n";
                    std::cout << "id   " << mesh.textures[i].id << "\n";
                    std::cout << "src  " << &mesh.textures[i].texImg << "\n";
                    std::cout << "type " << mesh.textures[i].type << "\n";
                    std::cout << "w    " << mesh.textures[i].w << "\n";
                    std::cout << "h    " << mesh.textures[i].h << "\n\n";
                }*/

                {
                    std::lock_guard<std::mutex> lock(chunkMutex);
                    chunkMesh.push_back(mesh);
                    
                   // std::cout << "Conversion done\n";
                }
                chunkCount++;
            };

            pool.enqueue(convertAndSetMesh);
		}

		void World::draw(render::Shader& shader) {
            pool.resume();
            for (auto& mesh : chunkMesh) {
                if (!mesh.ready()) {
                    mesh.setupMesh();
                } else {
                    mesh.draw(shader);
                }
            }
		}
        void World::clear() {

        }
	}
}