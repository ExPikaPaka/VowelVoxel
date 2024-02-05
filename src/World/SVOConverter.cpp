#include "SVOConverter.h"
#include <algorithm>

namespace ent {
	namespace world {
		model::Mesh SVOConverter::convert(SVO<Voxel>& svo, ui32 depth) {
			if (depth == -1) depth = svo.getMaxDepth();

			// Size of SVO object in voxels
			ui32 size = pow(2, depth);

			bool goodPosition = true;
			i32v3 badPosition(-1);
			int i = 0;
			while (goodPosition) {
				// Finding first block
				i32v3 startPos = findStartingVoxelRecursive(svo.getRoot(), size);
				i32v3 endPos = startPos;

				Voxel stVox = svo.get(startPos, depth);

				if (startPos == badPosition) {
					goodPosition = false;
					break;
				}
				i++;

				// Loop trough x axis
				for (ui32 x = startPos.x + 1; x < size; x++) {
					Voxel vox = svo.get({ x, startPos.y, startPos.z }, depth);
					if (vox && vox.color == stVox.color && !isVoxelIntersectingRectangles({ x, startPos.y, startPos.z })) {
						endPos.x++;
					} else {
						break;
					}
				}

				// Loop trough y axis
				for (ui32 y = startPos.y + 1; y < size; y++) {
					bool fullLine = true;

					// Loop trough x axis (Checks if line consists of blocks
					for (ui32 x = startPos.x; x <= endPos.x; x++) {
						Voxel vox = svo.get({ x, y, startPos.z }, depth);
						if (!vox || vox.color != stVox.color || isVoxelIntersectingRectangles({ x, y, startPos.z })) {
							fullLine = false;
							break;
						}
					}

					// Update y axis
					if (fullLine) {
						endPos.y++;
					} else {
						break;
					}

				}

				// Loop trough z axis
				for (ui32 z = startPos.z + 1; z < size; z++) {
					bool fullSquare = true;

					// Loop trough y axis
					for (ui32 y = startPos.y; y <= endPos.y; y++) {
						bool fullLine = true;

						// Loop trough x axis (Checks if line consists of blocks
						for (ui32 x = startPos.x; x <= endPos.x; x++) {
							Voxel vox = svo.get({ x, y, z }, depth);
							if (!vox || vox.color != stVox.color || isVoxelIntersectingRectangles({ x, y, z })) {
								fullLine = false;
								break;
							}
						}

						// Update y axis
						if (!fullLine) {
							fullSquare = false;
							break;
						}
					}

					if (fullSquare) {
						endPos.z++;
					} else {
						break;
					}
				}

				rectangles.push_back({ startPos, endPos, stVox.color});
			}

			//std::cout << "Total rectangles: " << i << "\n";
			return toMesh(size, svo.getSize());
		}

		i32v3 SVOConverter::findStartingVoxelRecursive(Node<Voxel>* node, ui32 size, i32v3 nPos) {
			// Special "continue" value
			i32v3 continueValue = { -1, -1, -1 };

			if (!node) return { -1, -1, -1 };

			// If we reached max depth check if 
			if (size == 2) {
				bool intersects = false;
				for (ui32 i = 0; i < 8; i++) {
					if (node->child[i]) {
						i32v3 indicator(0);

						// Accumulating child position
						if (i & 0b001) indicator.x++;
						if (i & 0b010) indicator.y++;
						if (i & 0b100) indicator.z++;

						// Check if desired voxel is inside already meshed area and skip
						if (isVoxelIntersectingRectangles(nPos + indicator)) {
							intersects = true;
							continue;
						}

						nPos += indicator;

						// Returning found voxel position
						return nPos;
					}
				}
				return { -1, -1, -1 }; // Return special "indicator", so we could try to find next position
			}

			// Loop into subnode
			bool intersects = false;
			for (ui32 i = 0; i < 8; i++) {
				if (node->child[i]) {
					ui32 halfSIze = size / 2;
					i32v3 indicator(0);

					// Accumulating child position
					if (i & 0b001) indicator.x += halfSIze;
					if (i & 0b010) indicator.y += halfSIze;
					if (i & 0b100) indicator.z += halfSIze;

					nPos += indicator;


					i32v3 value = findStartingVoxelRecursive(node->child[i], halfSIze, nPos);

					// Return position, if we found starting voxel OR continue with next area
					if (value == continueValue) {
						intersects = true;
						nPos -= indicator;
					} else {
						return value;
					}
				}
			}
			return { -1, -1, -1 };
		}

		bool SVOConverter::isVoxelIntersectingRectangles(i32v3 position) {
			for (const auto& rect : rectangles) {
				const i32v3& lowerLeft = rect.lowerLeft;
				const i32v3& upperRight = rect.upperRight;

				if (position.x >= lowerLeft.x && position.x <= upperRight.x &&
					position.y >= lowerLeft.y && position.y <= upperRight.y &&
					position.z >= lowerLeft.z && position.z <= upperRight.z) {

					return true;
				}
			}
			return false;
		}

		i32 SVOConverter::getVoxelIntersectingRectangleID(i32v3 position, std::vector<Rectangle>& array, ui32 startID, ui32 endID) {
			ui32 start = startID > array.size() ? 0 : startID;
			ui32 end = endID > array.size() ? array.size() : endID;

			for (ui32 id = start; id < end; id++) {
				Rectangle& rect = array[id];
				const i32v3& lowerLeft = rect.lowerLeft;
				const i32v3& upperRight = rect.upperRight;

				if (position.x >= lowerLeft.x && position.x <= upperRight.x &&
					position.y >= lowerLeft.y && position.y <= upperRight.y &&
					position.z >= lowerLeft.z && position.z <= upperRight.z) {

					return id; // Return ID number of rectangle that intersects
				}
			}
			return -1; // Indicate that no intersections found
		}

		model::Mesh SVOConverter::toMesh(f32 dimension, f32 size) {
			std::vector<model::Vertex> vertices;
			std::vector<ui32> indices;
			std::vector<model::Texture> textures;

			reMesh();

			ui8* texture = new ui8[rectangles.size() * 3];
			std::cout << "Rect count " << rectangles.size() << "\n";
			for (ui32 id = 0; id < rectangles.size(); id++) {
				Rectangle& rect = rectangles.at(id);

				texture[id * 3 + 0] = rect.color.r;
				texture[id * 3 + 1] = rect.color.g;
				texture[id * 3 + 2] = rect.color.b;

				f32v3 v0(rect.lowerLeft.x, rect.lowerLeft.y, rect.lowerLeft.z);
				f32v3 v1(rect.upperRight.x + 1, rect.lowerLeft.y, rect.lowerLeft.z);
				f32v3 v2(rect.upperRight.x + 1, rect.upperRight.y + 1, rect.lowerLeft.z);
				f32v3 v3(rect.lowerLeft.x, rect.upperRight.y + 1, rect.lowerLeft.z);
				f32v3 v4(rect.lowerLeft.x, rect.lowerLeft.y, rect.upperRight.z + 1);
				f32v3 v5(rect.upperRight.x + 1, rect.lowerLeft.y, rect.upperRight.z + 1);
				f32v3 v6(rect.upperRight.x + 1, rect.upperRight.y + 1, rect.upperRight.z + 1);
				f32v3 v7(rect.lowerLeft.x, rect.upperRight.y + 1, rect.upperRight.z + 1);

				// Calculate normals for each face
				glm::vec3 frontNormal = { 0, 0, -1 };
				glm::vec3 backNormal = { 0, 0, 1 };
				glm::vec3 leftNormal = { -1, 0, 0 };
				glm::vec3 rightNormal = { 1, 0 ,0 };
				glm::vec3 topNormal = { 0, 1, 0 };
				glm::vec3 bottomNormal = { 0, -1, 0 };

				ui32 disabledCount = 0; // Used to calculate correct indexes of vertexes
				ui32 lastIndex = vertices.size();

				f32 vID = (f32)id / (f32)rectangles.size() + 0.000001;
				//vID = 0;

				// Front face
				if (!rect.frontDisabled) {
					vertices.push_back({ v0 / dimension * size, frontNormal, {vID, 0.5} });
					vertices.push_back({ v3 / dimension * size, frontNormal, {vID, 0.5} });
					vertices.push_back({ v2 / dimension * size, frontNormal, {vID, 0.5} });
					vertices.push_back({ v1 / dimension * size, frontNormal, {vID, 0.5} });
				} else {
					disabledCount++;
				}

				// Back face
				if (!rect.backDisabled) {
					vertices.push_back({ v4 / dimension * size, backNormal, {vID, 0.5} });
					vertices.push_back({ v5 / dimension * size, backNormal, {vID, 0.5} });
					vertices.push_back({ v6 / dimension * size, backNormal, {vID, 0.5} });
					vertices.push_back({ v7 / dimension * size, backNormal, {vID, 0.5} });
				} else {
					disabledCount++;
				}

				// Left face
				if (!rect.leftDisabled) {
					vertices.push_back({ v0 / dimension * size, leftNormal, {vID, 0.5} });
					vertices.push_back({ v4 / dimension * size, leftNormal, {vID, 0.5} });
					vertices.push_back({ v7 / dimension * size, leftNormal, {vID, 0.5} });
					vertices.push_back({ v3 / dimension * size, leftNormal, {vID, 0.5} });
				} else {
					disabledCount++;
				}

				// Right face
				if (!rect.rightDisabled) {
					vertices.push_back({ v1 / dimension * size, rightNormal, {vID, 0.5} });
					vertices.push_back({ v2 / dimension * size, rightNormal, {vID, 0.5} });
					vertices.push_back({ v6 / dimension * size, rightNormal, {vID, 0.5} });
					vertices.push_back({ v5 / dimension * size, rightNormal, {vID, 0.5} });
				} else {
					disabledCount++;
				}

				// Top face
				if (!rect.topDisabled) {
					vertices.push_back({ v3 / dimension * size, topNormal, {vID, 0.5} });
					vertices.push_back({ v7 / dimension * size, topNormal, {vID, 0.5} });
					vertices.push_back({ v6 / dimension * size, topNormal, {vID, 0.5} });
					vertices.push_back({ v2 / dimension * size, topNormal, {vID, 0.5} });
				} else {
					disabledCount++;
				}

				// Bottom face
				if (!rect.bottomDisabled) {
					vertices.push_back({ v0 / dimension * size, bottomNormal, {vID, 0.5} });
					vertices.push_back({ v1 / dimension * size, bottomNormal, {vID, 0.5} });
					vertices.push_back({ v5 / dimension * size, bottomNormal, {vID, 0.5} });
					vertices.push_back({ v4 / dimension * size, bottomNormal, {vID, 0.5} });
				} else {
					disabledCount++;
				}

				// Indices
				for (ui32 i = 0; i < 6 - disabledCount; i++) {
					indices.insert(indices.end(), {
						0 + i * 4 + lastIndex, 1 + i * 4 + lastIndex, 2 + i * 4 + lastIndex, 2 + i * 4 + lastIndex, 3 + i * 4 + lastIndex, 0 + i * 4 + lastIndex
						});
				}

			}

			ui32 textureID;
			glGenTextures(1, &textureID);

			glBindTexture(GL_TEXTURE_2D, textureID);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, rectangles.size(), 1, 0, GL_RGB, GL_UNSIGNED_BYTE, texture);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			

			delete[] texture;


			model::Texture tex;

			tex.id = textureID;
			tex.type = "diffuse";

			textures.push_back(tex);

			clear();
			return model::Mesh(vertices, indices, textures);
		}

		void SVOConverter::reMesh() {
			i32v3 startPos;
			i32v3 endPos;
			i32v3 badPos(-1);

			rectCount = rectangles.size();

			//std::cout << "XY PLANE =========\n";
			for (ui32 i = 0; i < rectCount; i++) {
				xSearch = -1;
				ySearch = -1;
				zSearch = -1;

				startPos = getFreeVoxelPositionXY(i);;
				endPos = startPos;

				while (startPos != badPos) {
					endPos = getEndPosXY(i, startPos);

					if (startPos.x == rectangles[i].lowerLeft.x && startPos.y == rectangles[i].lowerLeft.y &&
						endPos.x == rectangles[i].upperRight.x && endPos.y == rectangles[i].upperRight.y) {
						break;
					}

					Rectangle& source = rectangles[getVoxelIntersectingRectangleID(startPos, rectangles)];
					reconRectXY.push_back({ startPos, endPos, source.color});
					reconRectXY[reconRectXY.size() - 1].setAllFlagsTo(true);
					reconRectXY[reconRectXY.size() - 1].backDisabled = false;

					startPos = getFreeVoxelPositionXY(i);
				}
			}

			//std::cout << "XY PLANE INVERSE =========\n";
			for (ui32 i = 0; i < toReconIDXY.size(); i++) {
				xSearchLow = -1;
				ySearchLow = -1;
				zSearchLow = -1;

				ui32 id = toReconIDXY[i];

				startPos = getFreeVoxelPositionXYLower(id);
				endPos = startPos;

				while (startPos != badPos) {
					endPos = getEndPosXYLower(id, startPos);

					if (startPos.x == rectangles[id].lowerLeft.x && startPos.y == rectangles[id].lowerLeft.y &&
						endPos.x == rectangles[id].upperRight.x && endPos.y == rectangles[id].upperRight.y) {
						break;
					}

					Rectangle& source = rectangles[getVoxelIntersectingRectangleID(startPos, rectangles)];
					reconRectLowXY.push_back({ startPos, endPos, source.color });
					reconRectLowXY[reconRectLowXY.size() - 1].setAllFlagsTo(true);
					reconRectLowXY[reconRectLowXY.size() - 1].frontDisabled = false;

					startPos = getFreeVoxelPositionXYLower(id);
				}
			}


			//std::cout << "XZ PLANE =========\n";
			for (ui32 i = 0; i < rectangles.size(); i++) {
				xSearch = -1;
				ySearch = -1;
				zSearch = -1;

				ui32 id = i;

				startPos = startPos = getFreeVoxelPositionXZ(id);;
				endPos = startPos;

				while (startPos != badPos) {
					endPos = getEndPosXZ(id, startPos);

					if (startPos.x == rectangles[id].lowerLeft.x && startPos.z == rectangles[id].lowerLeft.z &&
						endPos.x == rectangles[id].upperRight.x && endPos.z == rectangles[id].upperRight.z) {
						break;
					}

					Rectangle& source = rectangles[getVoxelIntersectingRectangleID(startPos, rectangles)];
					reconRectXZ.push_back({ startPos, endPos, source.color });
					reconRectXZ[reconRectXZ.size() - 1].setAllFlagsTo(true);
					reconRectXZ[reconRectXZ.size() - 1].topDisabled = false;

					startPos = getFreeVoxelPositionXZ(id);

				}
			}


			//std::cout << "XZ PLANE INVERSE =========\n";
			for (ui32 i = 0; i < toReconIDXZ.size(); i++) {
				xSearchLow = -1;
				ySearchLow = -1;
				zSearchLow = -1;

				ui32 id = toReconIDXZ[i];

				startPos = startPos = getFreeVoxelPositionXZLower(id);;
				endPos = startPos;

				while (startPos != badPos) {
					endPos = getEndPosXZLower(id, startPos);

					if (startPos.x == rectangles[id].lowerLeft.x && startPos.z == rectangles[id].lowerLeft.z &&
						endPos.x == rectangles[id].upperRight.x && endPos.z == rectangles[id].upperRight.z) {
						break;
					}

					Rectangle& source = rectangles[getVoxelIntersectingRectangleID(startPos, rectangles)];
					reconRectLowXZ.push_back({ startPos, endPos, source.color });
					reconRectLowXZ[reconRectLowXZ.size() - 1].setAllFlagsTo(true);
					reconRectLowXZ[reconRectLowXZ.size() - 1].bottomDisabled = false;

					startPos = getFreeVoxelPositionXZLower(id);

				}
			}

			
			//std::cout << "YZ PLANE =========\n";
			for (ui32 i = 0; i < rectangles.size(); i++) {
				xSearch = -1;
				ySearch = -1;
				zSearch = -1;

				ui32 id = i;

				startPos = getFreeVoxelPositionYZ(id);;
				endPos = startPos;

				while (startPos != badPos) {
					endPos = getEndPosYZ(id, startPos);

					if (startPos.y == rectangles[id].lowerLeft.y && startPos.z == rectangles[id].lowerLeft.z &&
						endPos.y == rectangles[id].upperRight.y && endPos.z == rectangles[id].upperRight.z) {
						break;
					}

					Rectangle& source = rectangles[getVoxelIntersectingRectangleID(startPos, rectangles)];
					reconRectYZ.push_back({ startPos, endPos, source.color });
					reconRectYZ[reconRectYZ.size() - 1].setAllFlagsTo(true);
					reconRectYZ[reconRectYZ.size() - 1].rightDisabled = false;

					startPos = getFreeVoxelPositionYZ(id);

				}
			}


			//std::cout << "YZ PLANE INVERSE =========\n";
			for (ui32 i = 0; i < toReconIDYZ.size(); i++) {
				xSearchLow = -1;
				ySearchLow = -1;
				zSearchLow = -1;

				ui32 id = toReconIDYZ[i];

				startPos = getFreeVoxelPositionYZLower(id);
				endPos = startPos;

				while (startPos != badPos) {
					endPos = getEndPosYZLower(id, startPos);

					if (startPos.y == rectangles[id].lowerLeft.y && startPos.z == rectangles[id].lowerLeft.z &&
						endPos.y == rectangles[id].upperRight.y && endPos.z == rectangles[id].upperRight.z) {
						break;
					}

					Rectangle& source = rectangles[getVoxelIntersectingRectangleID(startPos, rectangles)];
					reconRectLowYZ.push_back({ startPos, endPos, source.color });
					reconRectLowYZ[reconRectLowYZ.size() - 1].setAllFlagsTo(true);
					reconRectLowYZ[reconRectLowYZ.size() - 1].leftDisabled = false;


					startPos = getFreeVoxelPositionYZLower(id);

				}
			}


			rectangles.insert(rectangles.end(), reconRectXY.begin(), reconRectXY.end());
			rectangles.insert(rectangles.end(), reconRectXZ.begin(), reconRectXZ.end());
			rectangles.insert(rectangles.end(), reconRectYZ.begin(), reconRectYZ.end());

			rectangles.insert(rectangles.end(), reconRectLowXY.begin(), reconRectLowXY.end());
			rectangles.insert(rectangles.end(), reconRectLowXZ.begin(), reconRectLowXZ.end());
			rectangles.insert(rectangles.end(), reconRectLowYZ.begin(), reconRectLowYZ.end());
		}

		i32v3 SVOConverter::getFreeVoxelPositionXY(ui32 sourceID, bool disableFaceAbove) {
			if (sourceID >= rectangles.size()) return { -1, -1, -1 }; // Safety check

			Rectangle& source = rectangles[sourceID]; // Source rectangle

			if (xSearch == -1 || xSearch > source.upperRight.x) xSearch = source.lowerLeft.x;
			if (ySearch == -1 || ySearch > source.upperRight.y) ySearch = source.lowerLeft.y;

			i32v3 position(xSearch, ySearch, 0); // Position to return
			i32 z = source.upperRight.z + 1; // 1 voxel above source rectangle

			// Loop trough all source surface
			for (i32 y = ySearch; y <= source.upperRight.y; y++) {
				for (i32 x = xSearch; x <= source.upperRight.x; x++) {
					i32 targetID = getVoxelIntersectingRectangleID({ x, y, z }, rectangles); // Check if there are voxels above
					i32 reconID = getVoxelIntersectingRectangleID({ x, y, z - 1 }, reconRectXY); // Check if there is reconstructed region above 

					// Check if there are voxels
					if (targetID != -1 || reconID != -1) {
						// Update x, skipping some part
						if (targetID != -1) {
							x = rectangles[targetID].upperRight.x;

							rectangles[sourceID].backDisabled = disableFaceAbove; // disabling source face
							rectangles[targetID].frontDisabled = disableFaceAbove;

							if (isRectOutsideXY(sourceID, targetID)) {
								//rectangles[sourceID].frontDisabled = disableFaceAbove; // disabling source face
								if (!arrayXYContains(targetID)) {
									//std::cout << "Need to reconstruct rectangle XY " << targetID << "\n";
									toReconIDXY.push_back(targetID);
								}
							}
						}

						if (reconID != -1) {
							x = reconRectXY[reconID].upperRight.x;

						}

						// Update position to return
						xSearch = x + 1;
						position.x = x + 1;

						if (xSearch >= source.upperRight.x) xSearch = source.lowerLeft.x;
					}

					// Check if space is free
					if (targetID == -1 && reconID == -1) {
						position.z = z - 1;

						return position;
					}
				}
				position.y++;
				ySearch++;
				position.x = xSearch;
			}
			return { -1, -1, -1 }; // Indicate that no free space found
		}

		i32v3 SVOConverter::getFreeVoxelPositionXYLower(ui32 sourceID, bool disableFaceAbove) {
			if (sourceID >= rectangles.size()) return { -1, -1, -1 }; // Safety check

			Rectangle& source = rectangles[sourceID]; // Source rectangle

			if (xSearchLow == -1 || xSearchLow > source.upperRight.x) xSearchLow = source.lowerLeft.x;
			if (ySearchLow == -1 || ySearchLow > source.upperRight.y) ySearchLow = source.lowerLeft.y;

			i32v3 position(xSearchLow, ySearchLow, 0); // Position to return
			i32 z = source.lowerLeft.z - 1; // 1 voxel under source rectangle

			// Loop trough all source surface
			for (i32 y = ySearchLow; y <= source.upperRight.y; y++) {
				for (i32 x = xSearchLow; x <= source.upperRight.x; x++) {
					
					i32 targetID = getVoxelIntersectingRectangleID({ x, y, z }, rectangles); // Check if there are voxels above
					i32 reconID = getVoxelIntersectingRectangleID({ x, y, z + 1 }, reconRectLowXY); // Check if there is reconstructed region above 

					// Check if there are voxels
					if (targetID != -1 || reconID != -1) {
						// Update x, skipping some part
						if (targetID != -1) {
							x = rectangles[targetID].upperRight.x;

							//rectangles[sourceID].frontDisabled = disableFaceAbove; // disabling source face
						}

						if (reconID != -1) {
							x = reconRectLowXY[reconID].upperRight.x;
						}

						// Update position to return
						xSearchLow = x + 1;
						position.x = x + 1;

						if (xSearchLow >= source.upperRight.x) xSearchLow = source.lowerLeft.x;
					}

					// Check if space is free
					if (targetID == -1 && reconID == -1) {
						position.z = z + 1;

						return position;
					}
				}
				position.y++;
				ySearchLow++;
				position.x = xSearchLow;
			}
			return { -1, -1, -1 }; // Indicate that no free space found
		}

		i32v3 SVOConverter::getEndPosXY(ui32 sourceID, i32v3 startPos) {
			if (sourceID >= rectangles.size()) return { -1, -1, -1 }; // Safety check

			i32v3 endPos = startPos;
			Rectangle& source = rectangles[sourceID];

			// X axis
			for (ui32 x = startPos.x + 1; x <= source.upperRight.x; x++) {
				i32 targetID = getVoxelIntersectingRectangleID({ x, startPos.y, startPos.z + 1 }, rectangles); // Check if there are voxels above
				i32 reconID = getVoxelIntersectingRectangleID({ x, startPos.y, startPos.z }, reconRectXY); // Check if there is reconstructed region above 

				if (targetID != -1 || reconID != -1) { // If found voxel above source
					break;
				} else {
					endPos.x++;
				}
			}

			// Y axis
			for (ui32 y = startPos.y + 1; y <= source.upperRight.y; y++) {
				bool fullLine = true;

				for (ui32 x = startPos.x; x <= endPos.x; x++) {
					i32 targetID = getVoxelIntersectingRectangleID({ x, y, startPos.z + 1 }, rectangles); // Check if there are voxels above
					i32 reconID = getVoxelIntersectingRectangleID({ x, y, startPos.z }, reconRectXY); // Check if there is reconstructed region above 

					if (targetID != -1 || reconID != -1) { // If found voxel above source
						fullLine = false;
						break;
					}
				}

				if (fullLine) {
					endPos.y++;
				} else {
					break;
				}
			}

			return endPos;
		}

		i32v3 SVOConverter::getEndPosXYLower(ui32 sourceID, i32v3 startPos) {
			if (sourceID >= rectangles.size()) return { -1, -1, -1 }; // Safety check

			i32v3 endPos = startPos;
			Rectangle& source = rectangles[sourceID];

			// X axis
			for (ui32 x = startPos.x + 1; x <= source.upperRight.x; x++) {
				i32 targetID = getVoxelIntersectingRectangleID({ x, startPos.y, startPos.z - 1 }, rectangles); // Check if there are voxels under
				i32 reconID = getVoxelIntersectingRectangleID({ x, startPos.y, startPos.z }, reconRectLowXY); // Check if there is reconstructed region above 

				if (targetID != -1 || reconID != -1) { // If found voxel above source
					break;
				} else {
					endPos.x++;
				}
			}

			// Y axis
			for (ui32 y = startPos.y + 1; y <= source.upperRight.y; y++) {
				bool fullLine = true;

				for (ui32 x = startPos.x; x <= endPos.x; x++) {
					i32 targetID = getVoxelIntersectingRectangleID({ x, y, startPos.z - 1 }, rectangles); // Check if there are voxels under
					i32 reconID = getVoxelIntersectingRectangleID({ x, y, startPos.z }, reconRectLowXY); // Check if there is reconstructed region above 

					if (targetID != -1 || reconID != -1) { // If found voxel above source
						fullLine = false;
						break;
					}
				}

				if (fullLine) {
					endPos.y++;
				} else {
					break;
				}
			}

			return endPos;
		}

		bool SVOConverter::isRectOutsideXY(ui32 sourceID, ui32 targetID) {
			if (sourceID >= rectangles.size() || targetID >= rectangles.size()) return false; // Out of index check

			Rectangle& source = rectangles[sourceID];
			Rectangle& target = rectangles[targetID];
			

			if (target.upperRight.x > source.upperRight.x) return true;
			if (target.lowerLeft.x < source.lowerLeft.x) return true;

			if (target.upperRight.y > source.upperRight.y) return true;
			if (target.lowerLeft.y < source.lowerLeft.y) return true;

			return false;
		}

		i32v3 SVOConverter::getFreeVoxelPositionXZ(ui32 sourceID, bool disableFaceAbove) {
			if (sourceID >= rectangles.size()) return { -1, -1, -1 }; // Safety check

			Rectangle& source = rectangles[sourceID]; // Source rectangle

			if (xSearch == -1 || xSearch > source.upperRight.x) xSearch = source.lowerLeft.x;
			if (zSearch == -1 || zSearch > source.upperRight.z) zSearch = source.lowerLeft.z;

			i32v3 position(xSearch, 0, zSearch); // Position to return
			i32 y = source.upperRight.y + 1; // 1 voxel above source rectangle

			// Loop trough all source surface
			for (i32 z = zSearch; z <= source.upperRight.z; z++) {
				for (i32 x = xSearch; x <= source.upperRight.x; x++) {
					i32 targetID = getVoxelIntersectingRectangleID({ x, y, z }, rectangles); // Check if there are voxels above
					i32 reconID = getVoxelIntersectingRectangleID({ x, y - 1, z }, reconRectXZ); // Check if there is reconstructed region above 

					// Check if there are voxels
					if (targetID != -1 || reconID != -1) {
						// Update x, skipping some part
						if (targetID != -1) {
							x = rectangles[targetID].upperRight.x;

							rectangles[sourceID].topDisabled = disableFaceAbove; // disabling source face
							rectangles[targetID].bottomDisabled = disableFaceAbove;

							if (isRectOutsideXZ(sourceID, targetID)) {
								if (!arrayXZContains(targetID)) {
									//std::cout << "Need to reconstruct rectangle XZ " << targetID << "\n";

									toReconIDXZ.push_back(targetID);
								}
							} 
						}

						if (reconID != -1) {
							x = reconRectXZ[reconID].upperRight.x;
						}

						// Update position to return
						xSearch = x + 1;
						position.x = x + 1;

						if (xSearch >= source.upperRight.x) xSearch = source.lowerLeft.x;
					}

					// Check if space is free
					if (targetID == -1 && reconID == -1) {
						position.y = y - 1;

						return position;
					}
				}
				position.z++;
				zSearch++;
				position.x = xSearch;
			}
			return { -1, -1, -1 }; // Indicate that no free space found
		}

		i32v3 SVOConverter::getFreeVoxelPositionXZLower(ui32 sourceID, bool disableFaceAbove) {
			if (sourceID >= rectangles.size()) return { -1, -1, -1 }; // Safety check

			Rectangle& source = rectangles[sourceID]; // Source rectangle

			if (xSearchLow == -1 || xSearchLow > source.upperRight.x) xSearchLow = source.lowerLeft.x;
			if (zSearchLow == -1 || zSearchLow > source.upperRight.z) zSearchLow = source.lowerLeft.z;

			i32v3 position(xSearchLow, 0, zSearchLow); // Position to return
			i32 y = source.lowerLeft.y - 1; // 1 voxel above source rectangle

			// Loop trough all source surface
			for (i32 z = zSearchLow; z <= source.upperRight.z; z++) {
				for (i32 x = xSearchLow; x <= source.upperRight.x; x++) {
					i32 targetID = getVoxelIntersectingRectangleID({ x, y, z }, rectangles); // Check if there are voxels above
					i32 reconID = getVoxelIntersectingRectangleID({ x, y + 1, z }, reconRectLowXZ); // Check if there is reconstructed region above 

					// Check if there are voxels
					if (targetID != -1 || reconID != -1) {
						// Update x, skipping some part
						if (targetID != -1) {
							x = rectangles[targetID].upperRight.x;

							//rectangles[sourceID].topDisabled = disableFaceAbove; // disabling source face
						}

						if (reconID != -1) {
							x = reconRectLowXZ[reconID].upperRight.x;
						}

						// Update position to return
						xSearchLow = x + 1;
						position.x = x + 1;

						if (xSearchLow >= source.upperRight.x) xSearchLow = source.lowerLeft.x;
					}

					// Check if space is free
					if (targetID == -1 && reconID == -1) {
						position.y = y + 1;

						return position;
					}
				}
				position.z++;
				zSearchLow++;
				position.x = xSearchLow;
			}
			return { -1, -1, -1 }; // Indicate that no free space found
		}

		i32v3 SVOConverter::getEndPosXZ(ui32 sourceID, i32v3 startPos) {
			if (sourceID >= rectangles.size()) return { -1, -1, -1 }; // Safety check

			i32v3 endPos = startPos;
			Rectangle& source = rectangles[sourceID];

			// X axis
			for (ui32 x = startPos.x + 1; x <= source.upperRight.x; x++) {
				i32 targetID = getVoxelIntersectingRectangleID({ x, startPos.y + 1, startPos.z }, rectangles); // Check if there are voxels above
				i32 reconID = getVoxelIntersectingRectangleID({ x, startPos.y, startPos.z }, reconRectXZ); // Check if there is reconstructed region above 

				if (targetID != -1 || reconID != -1) { // If found voxel above source
					break;
				} else {
					endPos.x++;
				}
			}

			// Y axis
			for (ui32 z = startPos.z + 1; z <= source.upperRight.z; z++) {
				bool fullLine = true;

				for (ui32 x = startPos.x; x <= endPos.x; x++) {
					i32 targetID = getVoxelIntersectingRectangleID({ x, startPos.y + 1, z }, rectangles); // Check if there are voxels above
					i32 reconID = getVoxelIntersectingRectangleID({ x, startPos.y, z }, reconRectXY); // Check if there is reconstructed region above 

					if (targetID != -1 || reconID != -1) { // If found voxel above source
						fullLine = false;
						break;
					}
				}

				if (fullLine) {
					endPos.z++;
				} else {
					break;
				}
			}

			return endPos;
		}

		i32v3 SVOConverter::getEndPosXZLower(ui32 sourceID, i32v3 startPos) {
			if (sourceID >= rectangles.size()) return { -1, -1, -1 }; // Safety check

			i32v3 endPos = startPos;
			Rectangle& source = rectangles[sourceID];

			// X axis
			for (ui32 x = startPos.x + 1; x <= source.upperRight.x; x++) {
				i32 targetID = getVoxelIntersectingRectangleID({ x, startPos.y - 1, startPos.z }, rectangles); // Check if there are voxels above
				i32 reconID = getVoxelIntersectingRectangleID({ x, startPos.y, startPos.z }, reconRectLowXZ); // Check if there is reconstructed region above 

				if (targetID != -1 || reconID != -1) { // If found voxel above source
					break;
				} else {
					endPos.x++;
				}
			}

			// Y axis
			for (ui32 z = startPos.z + 1; z <= source.upperRight.z; z++) {
				bool fullLine = true;

				for (ui32 x = startPos.x; x <= endPos.x; x++) {
					i32 targetID = getVoxelIntersectingRectangleID({ x, startPos.y - 1, z }, rectangles); // Check if there are voxels above
					i32 reconID = getVoxelIntersectingRectangleID({ x, startPos.y, z }, reconRectLowXZ); // Check if there is reconstructed region above 

					if (targetID != -1 || reconID != -1) { // If found voxel above source
						fullLine = false;
						break;
					}
				}

				if (fullLine) {
					endPos.z++;
				} else {
					break;
				}
			}

			return endPos;
		}

		bool SVOConverter::isRectOutsideXZ(ui32 sourceID, ui32 targetID) {
			if (sourceID >= rectangles.size() || targetID >= rectangles.size()) return false; // Out of index check

			Rectangle& source = rectangles[sourceID];
			Rectangle& target = rectangles[targetID];


			if (target.upperRight.x > source.upperRight.x) return true;
			if (target.lowerLeft.x < source.lowerLeft.x) return true;

			if (target.upperRight.z > source.upperRight.z) return true;
			if (target.lowerLeft.z < source.lowerLeft.z) return true;

			return false;
		}

		i32v3 SVOConverter::getFreeVoxelPositionYZ(ui32 sourceID, bool disableFaceAbove) {
			if (sourceID >= rectangles.size()) return { -1, -1, -1 }; // Safety check

			Rectangle& source = rectangles[sourceID]; // Source rectangle

			if (ySearch == -1 || ySearch > source.upperRight.y) ySearch = source.lowerLeft.y;
			if (zSearch == -1 || zSearch > source.upperRight.z) zSearch = source.lowerLeft.z;

			i32v3 position(0, ySearch, zSearch); // Position to return
			i32 x = source.upperRight.x + 1; // 1 voxel above source rectangle

			// Loop trough all source surface
			for (i32 z = zSearch; z <= source.upperRight.z; z++) {
				for (i32 y = ySearch; y <= source.upperRight.y; y++) {
					i32 targetID = getVoxelIntersectingRectangleID({ x, y, z }, rectangles); // Check if there are voxels above
					i32 reconID = getVoxelIntersectingRectangleID({ x - 1, y, z }, reconRectYZ); // Check if there is reconstructed region above 

					// Check if there are voxels
					if (targetID != -1 || reconID != -1) {
						// Update x, skipping some part
						if (targetID != -1) {
							y = rectangles[targetID].upperRight.y;

							rectangles[sourceID].rightDisabled = disableFaceAbove; // disabling source face
							rectangles[targetID].leftDisabled = disableFaceAbove;

							if (isRectOutsideYZ(sourceID, targetID)) {
								if (!arrayYZContains(targetID)) {
									//std::cout << "Need to reconstruct rectangle YZ " << targetID << "\n";
									toReconIDYZ.push_back(targetID);
								}
							}
						}

						if (reconID != -1) {
							y = reconRectYZ[reconID].upperRight.y;
						}

						// Update position to return
						ySearch = y + 1;
						position.y = y + 1;

						if (ySearch >= source.upperRight.y) ySearch = source.lowerLeft.y;
					}

					// Check if space is free
					if (targetID == -1 && reconID == -1) {
						position.x = x - 1;

						return position;
					}
				}
				position.z++;
				zSearch++;
				position.y = ySearch;
			}
			return { -1, -1, -1 }; // Indicate that no free space found
		}

		i32v3 SVOConverter::getFreeVoxelPositionYZLower(ui32 sourceID, bool disableFaceAbove) {
			if (sourceID >= rectangles.size()) return { -1, -1, -1 }; // Safety check

			Rectangle& source = rectangles[sourceID]; // Source rectangle

			if (ySearchLow == -1 || ySearchLow > source.upperRight.y) ySearchLow = source.lowerLeft.y;
			if (zSearchLow == -1 || zSearchLow > source.upperRight.z) zSearchLow = source.lowerLeft.z;

			i32v3 position(0, ySearchLow, zSearchLow); // Position to return
			i32 x = source.lowerLeft.x - 1; // 1 voxel above source rectangle

			// Loop trough all source surface
			for (i32 z = zSearchLow; z <= source.upperRight.z; z++) {
				for (i32 y = ySearchLow; y <= source.upperRight.y; y++) {
					i32 targetID = getVoxelIntersectingRectangleID({ x, y, z }, rectangles); // Check if there are voxels above
					i32 reconID = getVoxelIntersectingRectangleID({ x + 1, y, z }, reconRectLowYZ); // Check if there is reconstructed region above 

					// Check if there are voxels
					if (targetID != -1 || reconID != -1) {
						// Update x, skipping some part
						if (targetID != -1) {
							y = rectangles[targetID].upperRight.y;
						}

						if (reconID != -1) {
							y = reconRectLowYZ[reconID].upperRight.y;
						}

						// Update position to return
						ySearchLow = y + 1;
						position.y = y + 1;

						if (ySearchLow >= source.upperRight.y) ySearchLow = source.lowerLeft.y;
					}

					// Check if space is free
					if (targetID == -1 && reconID == -1) {
						position.x = x + 1;

						return position;
					}
				}
				position.z++;
				zSearchLow++;
				position.y = ySearchLow;
			}
			return { -1, -1, -1 }; // Indicate that no free space found
		}

		i32v3 SVOConverter::getEndPosYZ(ui32 sourceID, i32v3 startPos) {
			if (sourceID >= rectangles.size()) return { -1, -1, -1 }; // Safety check

			i32v3 endPos = startPos;
			Rectangle& source = rectangles[sourceID];

			// Y axis
			for (ui32 y = startPos.y + 1; y <= source.upperRight.y; y++) {
				i32 targetID = getVoxelIntersectingRectangleID({ startPos.x + 1, y, startPos.z }, rectangles); // Check if there are voxels above
				i32 reconID = getVoxelIntersectingRectangleID({ startPos.x, y, startPos.z }, reconRectYZ); // Check if there is reconstructed region above 

				if (targetID != -1 || reconID != -1) { // If found voxel above source
					break;
				} else {
					endPos.y++;
				}
			}

			// Z axis
			for (ui32 z = startPos.z + 1; z <= source.upperRight.z; z++) {
				bool fullLine = true;

				for (ui32 y = startPos.y; y <= endPos.y; y++) {
					i32 targetID = getVoxelIntersectingRectangleID({ startPos.x + 1, y, z }, rectangles); // Check if there are voxels above
					i32 reconID = getVoxelIntersectingRectangleID({ startPos.x, y, z }, reconRectYZ); // Check if there is reconstructed region above 

					if (targetID != -1 || reconID != -1) { // If found voxel above source
						fullLine = false;
						break;
					}
				}

				if (fullLine) {
					endPos.z++;
				} else {
					break;
				}
			}

			return endPos;
		}

		i32v3 SVOConverter::getEndPosYZLower(ui32 sourceID, i32v3 startPos) {
			if (sourceID >= rectangles.size()) return { -1, -1, -1 }; // Safety check

			i32v3 endPos = startPos;
			Rectangle& source = rectangles[sourceID];

			// Y axis
			for (ui32 y = startPos.y + 1; y <= source.upperRight.y; y++) {
				i32 targetID = getVoxelIntersectingRectangleID({ startPos.x - 1, y, startPos.z }, rectangles); // Check if there are voxels above
				i32 reconID = getVoxelIntersectingRectangleID({ startPos.x, y, startPos.z }, reconRectLowYZ); // Check if there is reconstructed region above 

				if (targetID != -1 || reconID != -1) { // If found voxel above source
					break;
				} else {
					endPos.y++;
				}
			}

			// Z axis
			for (ui32 z = startPos.z + 1; z <= source.upperRight.z; z++) {
				bool fullLine = true;

				for (ui32 y = startPos.y; y <= endPos.y; y++) {
					i32 targetID = getVoxelIntersectingRectangleID({ startPos.x - 1, y, z }, rectangles); // Check if there are voxels above
					i32 reconID = getVoxelIntersectingRectangleID({ startPos.x, y, z }, reconRectLowYZ); // Check if there is reconstructed region above 

					if (targetID != -1 || reconID != -1) { // If found voxel above source
						fullLine = false;
						break;
					}
				}

				if (fullLine) {
					endPos.z++;
				} else {
					break;
				}
			}

			return endPos;
		}

		bool SVOConverter::isRectOutsideYZ(ui32 sourceID, ui32 targetID) {
			if (sourceID >= rectangles.size() || targetID >= rectangles.size()) return false; // Out of index check

			Rectangle& source = rectangles[sourceID];
			Rectangle& target = rectangles[targetID];


			if (target.upperRight.y > source.upperRight.y) return true;
			if (target.lowerLeft.y < source.lowerLeft.y) return true;

			if (target.upperRight.z > source.upperRight.z) return true;
			if (target.lowerLeft.z < source.lowerLeft.z) return true;

			return false;
		}


		bool SVOConverter::arrayXYContains(ui32 ID) {
			auto it = std::find(toReconIDXY.begin(), toReconIDXY.end(), ID);

			if (it != toReconIDXY.end()) {
				return true;
			} 
			return false;
		}

		bool SVOConverter::arrayXZContains(ui32 ID) {
			auto it = std::find(toReconIDXZ.begin(), toReconIDXZ.end(), ID);

			if (it != toReconIDXZ.end()) {
				return true;
			}
			return false;
		}

		bool SVOConverter::arrayYZContains(ui32 ID) {
			auto it = std::find(toReconIDYZ.begin(), toReconIDYZ.end(), ID);

			if (it != toReconIDYZ.end()) {
				return true;
			}
			return false;
		}

		void SVOConverter::clear() {
			rectangles.clear();

			reconRectXY.clear();
			reconRectXZ.clear();
			reconRectYZ.clear();

			reconRectLowXY.clear();
			reconRectLowXZ.clear();
			reconRectLowYZ.clear();

			toReconIDXY.clear();
			toReconIDXZ.clear();
			toReconIDYZ.clear();
		}

		void SVOConverter::Rectangle::setAllFlagsTo(bool value) {
			frontDisabled = value;
			backDisabled = value;
			leftDisabled = value;
			rightDisabled = value;
			topDisabled = value;
			bottomDisabled = value;
		}
	}
}


