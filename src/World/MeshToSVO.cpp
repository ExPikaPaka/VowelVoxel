#include "MeshToSVO.h"
#include "..\Algorithm\Ray.h"

namespace ent {
	namespace world {
		void MeshToSVO::convert(SVO<Voxel>& svo, std::string& filePath, ui32 depth) {
			Voxel voxel;
			voxel.id = 10;

			model::Model model(filePath.c_str());
			ui32 dimension = svo.getDimension();

			f32 min;
			f32 max;

			f32 xl = abs(model.AABB.xMax - model.AABB.xMin);
			f32 yl = abs(model.AABB.yMax - model.AABB.yMin);
			f32 zl = abs(model.AABB.zMax - model.AABB.zMin);

			max = std::max({ xl,yl,zl });

			std::cout << "xl " << xl << "\n";
			std::cout << "yl " << yl << "\n";
			std::cout << "zl " << zl << "\n";

			// Loop trough XYZ
			for (ui32 z = 0; z < dimension; z++) {
				std::cout << "z " << z << "\n";
				for (ui32 y = 0; y < dimension; y++) {
					//std::cout << " y " << y << "\n";
					for (ui32 x = 0; x < dimension; x++) {
						//std::cout << "  x " << x << "\n";
						f32v3 start = { x, y, z };
						f32v3 dir = { 0, 0, 1 };
						
						start /= dimension;
	
						start.x += 0.001;
						start.y += 0.002;
						start.z += 0.003;

						start.x = mapValue(start.x, 0, 1, model.AABB.xMin, (model.AABB.xMin + max));
						start.y = mapValue(start.y, 0, 1, model.AABB.yMin, (model.AABB.yMin + max));
						start.z = mapValue(start.z, 0, 1, model.AABB.zMin, (model.AABB.zMin + max));

						//std::cout << " Shoot ray: " << start.x << " " << start.y << " " << start.z << "\n";
						ent::algorithm::Ray ray = model.shootRay(start, dir);

						if (ray.hitCount % 2 == 1) {
							//std::cout << "    hit\n";

							//std::cout << "    pos: " << x << " " << y << " " << z << "\n";
							svo.set({ x, y, z }, voxel);
						}
					}
				}
			}
		}
	}
}

