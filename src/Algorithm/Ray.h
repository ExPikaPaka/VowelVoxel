#pragma once
#include "../dataTypes.h"

namespace ent {
	namespace algorithm {
		struct Ray {
			bool hit;
			f32 length;
			ui32 hitCount;
			Ray();

			bool operator<(Ray& other);
			bool operator>(Ray& other);
			bool operator>=(Ray& other);
			bool operator<=(Ray& other);
			bool operator==(Ray& other);
		};

		f32 shootRay(f32v3 start, f32v3 direction, f32v3 p1, f32v3 p2, f32v3 p3);
	}
}