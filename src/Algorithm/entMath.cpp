#include "entMath.h"


namespace ent {
	namespace math {
		f64 mapValue(f64 value, f64 fromMin, f64 fromMax, f64 toMin, f64 toMax) {
            // Ensure the value is within the source range
            f64 clampedValue = std::max(fromMin, std::min(value, fromMax));

            // Calculate the mapped value in the target range
            f64 mappedValue = toMin + (clampedValue - fromMin) * (toMax - toMin) / (fromMax - fromMin);

            return mappedValue;
        }
	}
}