#pragma once
#include "../dataTypes.h"
#include "Voxel.h"

namespace ent {
	namespace world {

		// Sparse Voxel Octree (SVO) node
		template <typename T>
		struct Node {
			bool isLeaf;
			Node* child[8];
			T data;

			Node();
		};

		// Sparse Voxel Octree (SVO) class
		template <typename T>
		class SVO {
		public:
			SVO(ui32 maxDepth);
			SVO();
			void set(i32v3 position, T data);
			T get(i32v3 position, ui32 depth = -1);
			bool hasChild(i32v3 position, ui32 depth = -1);
			void setMaxDepth(ui32 value);
			ui32 getMaxDepth();
			ui32 getDimension();
			i32 getSize();
			void setSize(i32 value);
			void calculate();
			void init(ui32 maxDepth);
			void refRoot(Node<T>* node);

			Node<T>* getRoot();
			f32v3 getPosition();
		private:
			Node<T> m_Root;
			f32v3 m_Position;
			i32 m_Size;
			ui32 m_MaxDepth;

			// Checks if voxel is in SVO space
			// @return true } If each of coordinate position is less than (2^depth) and >= 0
			// @return false } If any of coordinate position is equal or greater than (2^depth) or < 0
			bool validPosition(i32v3& position, ui32 depth = -1);

			void setRecursive(Node<T>* node, i32v3 pos, i32v3 nPos, T data, i32 size);
			T getRecursive(Node<T>* node, i32v3 pos, i32v3 nPos, i32 size);
			T calculateRecursive(Node<T>* node, i32 size);
			bool hasChildRecursive(Node<T>* node, i32v3 pos, i32v3 nPos, i32 size);

			void copyRecursive(Node<T>* targetNode, Node<T>* sourceNode);
		};



		///////////////////////////////////////////////////////////////////////////////////
		// SVO Implementation															 //
		///////////////////////////////////////////////////////////////////////////////////
		template<typename T>
		inline SVO<T>::SVO(ui32 maxDepth) {
			init(maxDepth);
		}

		template<typename T>
		inline SVO<T>::SVO() {
			init(0);
		}

		template<typename T>
		inline void SVO<T>::set(i32v3 position, T data) {
			if (validPosition(position)) {
				setRecursive(&m_Root, position, i32v3(0), data, pow(2, m_MaxDepth));
			}
		}

		template<typename T>
		inline T SVO<T>::get(i32v3 position, ui32 depth) {
			ui32 size = pow(2, depth == -1 ? m_MaxDepth : (depth > m_MaxDepth ? m_MaxDepth : depth));
			if (validPosition(position, depth)) {
				return getRecursive(&m_Root, position, i32v3(0), size);
			}
			return T();
		}

		template<typename T>
		inline bool SVO<T>::hasChild(i32v3 position, ui32 depth) {
			ui32 size = pow(2, depth == -1 ? m_MaxDepth : (depth > m_MaxDepth ? m_MaxDepth : depth));
			if (validPosition(position, depth)) {
				return hasChildRecursive(&m_Root, position, i32v3(0),size);
			}
			return false;
		}

		template<typename T>
		inline void SVO<T>::setMaxDepth(ui32 value) {
			m_MaxDepth = value;
		}

		template<typename T>
		inline ui32 SVO<T>::getMaxDepth() {
			return m_MaxDepth;
		}
		template<typename T>
		inline ui32 SVO<T>::getDimension() {
			return pow(2, m_MaxDepth);
		}
		template<typename T>
		inline i32 SVO<T>::getSize() {
			return m_Size;
		}

		template<typename T>
		inline void SVO<T>::setSize(i32 value) {
			m_Size = value;
		}

		template<typename T>
		inline void SVO<T>::calculate() {
			calculateRecursive(&m_Root, pow(2, m_MaxDepth));
		}

		template<typename T>
		inline void SVO<T>::init(ui32 maxDepth) {
			m_MaxDepth = maxDepth;
			m_Root.isLeaf = !maxDepth;
			m_Position = { 0, 0, 0 };
			m_Size = 1;
		}

		template<typename T>
		inline void SVO<T>::refRoot(Node<T>* node) {
			if (node) {
				m_Root.data = node->data;
				m_Root.isLeaf = node->isLeaf;

				for (ui8 i = 0; i < 8; i++) {
					m_Root.child[i] = node->child[i];
				}
			}
		}

		template<typename T>
		inline Node<T>* SVO<T>::getRoot() {
			return &m_Root;
		}

		template<typename T>
		inline f32v3 SVO<T>::getPosition() {
			return m_Position;
		}

		template<typename T>
		inline bool SVO<T>::validPosition(i32v3& position, ui32 depth) {
			if (position.x < 0 || position.y < 0 || position.z < 0) return false;

			ui32 tDepth = depth == -1 ? m_MaxDepth : (depth > m_MaxDepth ? m_MaxDepth : depth);
			ui32 maxCoord = pow(2, tDepth);
			return position.x < maxCoord && position.y < maxCoord && position.z < maxCoord;
		}

		template<typename T>
		inline void SVO<T>::setRecursive(Node<T>* node, i32v3 pos, i32v3 nPos, T data, i32 size) {
			// Set data if position reached
			if (size == 1) {
				node->data = data;
				return;
			}

			// Define child index
			i32 halfSize = size / 2;
			ui32 index = 0;
			if (pos.x >= nPos.x + halfSize) index |= 1; // 001 bit - Check if a coordinate is less or more that a half of our space in X dimension
			if (pos.y >= nPos.y + halfSize) index |= 2; // 010 bit - Check if a coordinate is less or more that a half of our space in Y dimension
			if (pos.z >= nPos.z + halfSize) index |= 4; // 100 bit - Check if a coordinate is less or more that a half of our space in Z dimension

			// Create new node if needed
			if (!node->child[index]) {
				node->isLeaf = false;
				node->child[index] = new Node<T>();
			}

			nPos.x += (index & 1) * halfSize;		 // If our index is is more than half of our space in X, accumulate X change by half of space
			nPos.y += ((index >> 1) & 1) * halfSize; // If our index is is more than half of our space in Y, accumulate Y change by half of space
			nPos.z += ((index >> 2) & 1) * halfSize; // If our index is is more than half of our space in Z, accumulate Z change by half of space
			// Loop recursive
			setRecursive(node->child[index], pos, nPos, data, halfSize);
		}

		template<typename T>
		inline T SVO<T>::getRecursive(Node<T>* node, i32v3 pos, i32v3 nPos, i32 size) {
			if (size == 1) {
				return node->data;
			}

			int halfSize = size / 2;
			int index = 0;

			if (pos.x >= nPos.x + halfSize) index |= 1;
			if (pos.y >= nPos.y + halfSize) index |= 2;
			if (pos.z >= nPos.z + halfSize) index |= 4;

			if (!node->child[index]) {
				return Voxel(); // Return empty voxel with "id = -1"
			}

			nPos.x += (index & 1) * halfSize;
			nPos.y += ((index >> 1) & 1) * halfSize;
			nPos.z += ((index >> 2) & 1) * halfSize;
			return getRecursive(node->child[index], pos, nPos, halfSize);
		}

		template<typename T>
		inline T SVO<T>::calculateRecursive(Node<T>* node, i32 size) {
			if (size == 1) {
				return node->data;
			} 

			T data;
			if (node) {
				i32 childs = 0;
				for (ui32 i = 0; i < 8; i++) {
					if (node->child[i]) {
						data += calculateRecursive(node->child[i], size / 2);
						childs++;
					}
				}
				if(childs) 	data /= childs;
				node->data = data;
			}
			return data;
		}

		template<typename T>
		inline bool SVO<T>::hasChildRecursive(Node<T>* node, i32v3 pos, i32v3 nPos, i32 size) {
			if (size == 1) {
				for (ui32 i = 0; i < 8; i++) {
					if (node->child[i]) return true;
				}
				return false;
			}

			int halfSize = size / 2;
			int index = 0;

			if (pos.x >= nPos.x + halfSize) index |= 1;
			if (pos.y >= nPos.y + halfSize) index |= 2;
			if (pos.z >= nPos.z + halfSize) index |= 4;

			if (!node->child[index]) {
				return false;
			}

			nPos.x += (index & 1) * halfSize;
			nPos.y += ((index >> 1) & 1) * halfSize;
			nPos.z += ((index >> 2) & 1) * halfSize;
			return hasChildRecursive(node->child[index], pos, nPos, halfSize);
			
		}

		template<typename T>
		inline void SVO<T>::copyRecursive(Node<T>* targetNode, Node<T>* sourceNode) {
			//if (!targetNode) { // Create node in case need
			//	targetNode = new Node<T>;
			//}


			//if (sourceNode) {
			//	for (i32 i = 0; i < 8; i++) {
			//		if (sourceNode[i]) {
			//			targetNode[i] = new Node<T>;
			//			copyRecursive(targetNode[i], sourceNode[i]);
			//		}
			//	}

			//}
		}

		///////////////////////////////////////////////////////////////////////////////////
		// Node Implementation															 //
		///////////////////////////////////////////////////////////////////////////////////
		template<typename T>
		inline Node<T>::Node() {
			for (ui32 i = 0; i < 8; i++) {
				child[i] = nullptr;
			}
			isLeaf = true;
		}
	}
}
