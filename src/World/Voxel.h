#pragma once
#include "..\dataTypes.h"

namespace ent {
	namespace world {
		class Voxel {
		public:
			Voxel();
			ui16 id;
			ui32v3 color;
			void operator+=(Voxel other);
			void operator/=(i32 value);

			// Returns 'true' if voxel exists, 'false' if is empty (new created)
			operator bool();
 		};
	}
}


