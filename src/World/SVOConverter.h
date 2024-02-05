#pragma once
#include "..\dataTypes.h"
#include "..\Object\Mesh.h"
#include "..\Object\Model.h"
#include "SVO.h"
namespace ent {
	namespace world {
		class SVOConverter {
		public:
			// Converts world to greedy mesh
			model::Mesh convert(SVO<Voxel>& svo, ui32 depth = -1);

		private:
			struct Rectangle {
				i32v3 lowerLeft;
				i32v3 upperRight;

				ui8v3 color = { 140, 140, 240 };

				bool frontDisabled = false,
					backDisabled = false,
					leftDisabled = false,
					rightDisabled = false,
					topDisabled = false,
					bottomDisabled = false;

				void setAllFlagsTo(bool value);
			};

			// A vector that stores greedy rectangles
			std::vector<Rectangle> rectangles;

			std::vector<Rectangle> reconRectXY;
			std::vector<Rectangle> reconRectXZ;
			std::vector<Rectangle> reconRectYZ;

			std::vector<Rectangle> reconRectLowXY;
			std::vector<Rectangle> reconRectLowXZ;
			std::vector<Rectangle> reconRectLowYZ;

			std::vector<ui32> toReconIDXY;
			std::vector<ui32> toReconIDXZ;
			std::vector<ui32> toReconIDYZ;

			// Finds first "Voxel Octree" block
			i32v3 findStartingVoxelRecursive(Node<Voxel>* node, ui32 size, i32v3 nPos = { 0, 0, 0 });

			// Check if desired voxel is inside already meshed area and skip
			bool isVoxelIntersectingRectangles(i32v3 position);

			// Check if desired voxel is inside already meshed area and skip
			//               in 'rectangles' vector. In other case searches at corresponding XY XZ YZ vectors
	 		i32 getVoxelIntersectingRectangleID(i32v3 position, std::vector<Rectangle>& array, ui32 startID = 0, ui32 endID = -1);

            // Converts 3d rectangle into a vertices & indices
            model::Mesh toMesh(f32 dimension, f32 size);

			// Reconstructs overlapping regions of boxes, so that there won't be useles faces & triangles
			void reMesh();


		private: // XY plane
			// Used for reMesh() function. Finds first voxel that doesn't has voxel above
			// \param sourceID   id of rectangle in 'rectangles' array that is 'source' (will try find blocks above)
			// \param disableFaceAbove  specifies whether or not to disable down face of the rectangle above source
			// \return (i32v3) {-1, -1, -1}   in case no spot found/sourceID out of bounds
			i32v3 getFreeVoxelPositionXY(ui32 sourceID, bool disableFaceAbove = true);

			i32v3 getFreeVoxelPositionXYLower(ui32 sourceID, bool disableFaceAbove = true);

			// Used for reMesh() function. Finds first end position of sub-rectangle
			// \param sourceID   id of rectangle in 'rectangles' array that is 'source' (will try find blocks above)
			// \param startPos   starting position from which functions should start
			i32v3 getEndPosXY(ui32 sourceID, i32v3 startPos);

			i32v3 getEndPosXYLower(ui32 sourceID, i32v3 startPos);

			// Used for reMesh() function. Checks whether target rectangle is at least partly or completely outside source rectangle
			// \param sourceID   id of rectangle in 'rectangles' array that is 'source' 
			// \param targetID   id of rectangle in 'rectangles' array that is 'target' (to check if he is outside)
			// \return true   if target rectangle is at least partly or complitly outside source rectangle
			// \return false   in other case, or if sourceID/targetID out of range
			bool isRectOutsideXY(ui32 sourceID, ui32 targetID);


		private: // XZ plane
			// Used for reMesh() function. Finds first voxel that doesn't has voxel above
			// \param sourceID   id of rectangle in 'rectangles' array that is 'source' (will try find blocks above)
			// \param disableFaceAbove  specifies whether or not to disable down face of the rectangle above source
			// \return (i32v3) {-1, -1, -1}   in case no spot found/sourceID out of bounds
			i32v3 getFreeVoxelPositionXZ(ui32 sourceID, bool disableFaceAbove = true);

			i32v3 getFreeVoxelPositionXZLower(ui32 sourceID, bool disableFaceAbove = true);

			// Used for reMesh() function. Finds first end position of sub-rectangle
			// \param sourceID   id of rectangle in 'rectangles' array that is 'source' (will try find blocks above)
			// \param startPos   starting position from which functions should start
			i32v3 getEndPosXZ(ui32 sourceID, i32v3 startPos);

			i32v3 getEndPosXZLower(ui32 sourceID, i32v3 startPos);

			// Used for reMesh() function. Checks whether target rectangle is at least partly or completely outside source rectangle
			// \param sourceID   id of rectangle in 'rectangles' array that is 'source' 
			// \param targetID   id of rectangle in 'rectangles' array that is 'target' (to check if he is outside)
			// \return true   if target rectangle is at least partly or complitly outside source rectangle
			// \return false   in other case, or if sourceID/targetID out of range
			bool isRectOutsideXZ(ui32 sourceID, ui32 targetID);


		private: // YZ plane
			// Used for reMesh() function. Finds first voxel that doesn't has voxel above
			// \param sourceID   id of rectangle in 'rectangles' array that is 'source' (will try find blocks above)
			// \param disableFaceAbove  specifies whether or not to disable down face of the rectangle above source
			// \return (i32v3) {-1, -1, -1}   in case no spot found/sourceID out of bounds
			i32v3 getFreeVoxelPositionYZ(ui32 sourceID, bool disableFaceAbove = true);

			i32v3 getFreeVoxelPositionYZLower(ui32 sourceID, bool disableFaceAbove = true);

			// Used for reMesh() function. Finds first end position of sub-rectangle
			// \param sourceID   id of rectangle in 'rectangles' array that is 'source' (will try find blocks above)
			// \param startPos   starting position from which functions should start
			i32v3 getEndPosYZ(ui32 sourceID, i32v3 startPos);

			i32v3 getEndPosYZLower(ui32 sourceID, i32v3 startPos);

			// Used for reMesh() function. Checks whether target rectangle is at least partly or completely outside source rectangle
			// \param sourceID   id of rectangle in 'rectangles' array that is 'source' 
			// \param targetID   id of rectangle in 'rectangles' array that is 'target' (to check if he is outside)
			// \return true   if target rectangle is at least partly or complitly outside source rectangle
			// \return false   in other case, or if sourceID/targetID out of range
			bool isRectOutsideYZ(ui32 sourceID, ui32 targetID);


			bool arrayXYContains(ui32 ID);
			bool arrayXZContains(ui32 ID);
			bool arrayYZContains(ui32 ID);

			ui32 rectCount = 0; // Rectangles count before reMesh() called
			i32 xSearch = -1; // Used for getFreeVoxel() function to reduce amount of checks
			i32 ySearch = -1; // Used for getFreeVoxel() function to reduce amount of checks
			i32 zSearch = -1; // Used for getFreeVoxel() function to reduce amount of checks

			i32 xSearchLow = -1; // Used for getFreeVoxel() function to reduce amount of checks
			i32 ySearchLow = -1; // Used for getFreeVoxel() function to reduce amount of checks
			i32 zSearchLow = -1; // Used for getFreeVoxel() function to reduce amount of checks

			// Clears everything
			void clear();
		};
	}
}

