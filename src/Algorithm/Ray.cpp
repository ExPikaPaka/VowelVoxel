#include "Ray.h"

namespace ent {
	namespace algorithm {
		f32 shootRay(f32v3 start, f32v3 direction, f32v3 p1, f32v3 p2, f32v3 p3) {
			f32v3 tEdge1 = p2 - p1;
			f32v3 tEdge2 = p3 - p2;
			f32v3 tEdge3 = p1 - p3;
			f32v3 tFlatNormal = glm::cross(tEdge1, tEdge2);

			f32 n_dot_d = glm::dot(tFlatNormal, direction);
			if (glm::abs(n_dot_d) < 0.0001f) {
				return -1;
			}
			f32 n_dot_ps = glm::dot(tFlatNormal, p1 - start);

			// Length to the plane
			f32 length = n_dot_ps / n_dot_d;
			f32v3 planePoint = start + direction * length;

			// Check if point is on the triangle
			f32v3 p1toPoint = planePoint - p1;
			f32v3 p2toPoint = planePoint - p2;
			f32v3 p3toPoint = planePoint - p3;

			f32v3 aTestVec = glm::cross(tEdge1, p1toPoint);
			f32v3 bTestVec = glm::cross(tEdge2, p2toPoint);
			f32v3 cTestVec = glm::cross(tEdge3, p3toPoint);

			bool aTestVec_matches = glm::dot(aTestVec, tFlatNormal) > 0.0f;
			bool bTestVec_matches = glm::dot(bTestVec, tFlatNormal) > 0.0f;
			bool cTestVec_matches = glm::dot(cTestVec, tFlatNormal) > 0.0f;

			if (aTestVec_matches && bTestVec_matches && cTestVec_matches) {
				return length;
			}

			return -1;
		}

		Ray::Ray() {
			hit = 0;
			length = -1;
			hitCount = 0;
		}

		bool Ray::operator<(Ray& other) {
			//if (!this->hit || !other.hit) return false;
			return this->length < other.length;
		}

		bool Ray::operator>(Ray& other) {
			//if (!this->hit || !other.hit) return false;
			return this->length > other.length;
		}

		bool Ray::operator<=(Ray& other) {
			//if (!this->hit || !other.hit) return false;
			return this->length <= other.length;
		}

		bool Ray::operator==(Ray& other) {
			//if (!this->hit || !other.hit) return false;
			return this->length == other.length;
		}

		bool Ray::operator>=(Ray& other) {
			//if (!this->hit || !other.hit) return false;
			return this->length >= other.length;
		}
	}
}