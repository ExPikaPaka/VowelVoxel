#pragma once
#include "SVO.h"
#include "..\Object\Model.h"
#include <string>

namespace ent {
	namespace world {
		class MeshToSVO {
            static double mapValue(double x, double Xmin, double Xmax, double Ymin, double Ymax) {
                // Check for division by zero
                if (std::abs(Xmax - Xmin) < std::numeric_limits<double>::epsilon()) {
                    std::cerr << "Error: Division by zero in mapValue function." << std::endl;
                    return 0.0;
                }

                // Perform the mapping
                double result = Ymin + ((x - Xmin) / (Xmax - Xmin)) * (Ymax - Ymin);

                // Ensure the result is within the new range
                return std::min(std::max(result, Ymin), Ymax);
            }
		public:

			static void convert(SVO<Voxel>& svo, std::string& filePath, ui32 depth);
		};
	}
}


