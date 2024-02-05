#include "Voxel.h"

namespace ent {
	namespace world {
		Voxel::Voxel() {
			id = -1; // Set -1 to indicate that it is a empty voxel
			color = { 0, 0, 0 };
		}
		void Voxel::operator+=(Voxel other) {
			this->color += other.color;
			this->id += other.id;
		}
		void Voxel::operator/=(i32 value) {
			this->color /= value;
			this->id /= value;
		}
		Voxel::operator bool() {
			if (id == (ui16)-1) return false;
			return true;
		}
	}
}
