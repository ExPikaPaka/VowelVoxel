#pragma once
#include "../dataTypes.h"
#include "../Object/Mesh.h"
#include "../Object/Model.h"

namespace ent {
	namespace generation {
		static model::Mesh genCone(f32v3 start, f32v3 end, ui32 edges = 6, f32 radius = 0.5) {
			std::vector<model::Vertex> vertices;
			std::vector<model::Texture> textures;
			std::vector<ui32> indicies;

			// Generating cnoe
			ui32 coneEdges = edges;
			f32 coneRadius = radius;

			f32v3 coneDirection = end - start;

			f32v3 notCollinearVector;
			if (coneDirection.x == 0.0f && coneDirection.y == 0.0f) {
				notCollinearVector = { 1.0f, 0.0f, 0.0f };
			} else if (coneDirection.x == 0.0f && coneDirection.z == 0.0f) {
				notCollinearVector = { 1.0f, 0.0f, 0.0f };
			} else {
				notCollinearVector = { 0.0f, 1.0f, 0.0f };
			}

			f32v3 u = glm::normalize(glm::cross(coneDirection, notCollinearVector)); // Perpendicular to the cone
			f32v3 v = glm::normalize(glm::cross(coneDirection, u)); // Perpendicular to the U and cone
			f32 t = 0; // Theta (angle)
			f32 PI = 3.1415926535;
			f32 PI_2 = 6.2831853072;

			// Bottom 
			for (ui32 i = 0; i < coneEdges; i++) {
				f32 t = (PI_2 / coneEdges) * (double)i;
				f32v3 p = (u * cos(t) + v * sin(t)) * coneRadius; // Finding point on our UV plane
				p += start;

				model::Vertex v = { p, glm::normalize(-coneDirection), {0, 0} };
				vertices.push_back(v);

				if (i > 1) {
					indicies.push_back(0);
					indicies.push_back(i);
					indicies.push_back(i - 1);
				}
			}

			// Wall
			for (ui32 i = 0; i < coneEdges; i++) {
				f32v3 p1 = vertices[i].position;
				f32v3 p2 = vertices[i + 1].position;
				f32v3 p3 = end;

				// Calculating normal
				f32v3 n1 = glm::normalize(glm::cross(p3 - p1 , glm::cross(p3 - p1, start - p1)));
				f32v3 n2 = glm::normalize(glm::cross(p3 - p2, glm::cross(p3 - p2, start - p2)));
				f32v3 n3 = glm::normalize(n1 + n2);
				n3 = glm::normalize(end - start);
				
				model::Vertex v1 = { p1, n1, {0, 0} };
				model::Vertex v2 = { p2, n2, {0, 0} };
				model::Vertex v3 = { p3, n3, {0, 0} };

				vertices.push_back(v1);
				vertices.push_back(v2);
				vertices.push_back(v3);

			}

			for (ui32 i = 0; i < coneEdges * 3; i++) {
				indicies.push_back(coneEdges + i);

			}
			

			return model::Mesh(vertices, indicies, textures);
		}

		static model::Mesh genCylinder(f32v3 start, f32v3 end, ui32 edges = 6, f32 radius = 0.5, bool flipNormals = false) {
			std::vector<model::Vertex> vertices;
			std::vector<model::Texture> textures;
			std::vector<ui32> indicies;
			f32 normalDirection = flipNormals ? -1 : 1;

			// Generating cylinder
			ui32 cylEdges = edges;
			f32 cylRadius = radius;

			f32v3 cylDirection = end - start;

			f32v3 notCollinearVector;
			if (cylDirection.x == 0.0f && cylDirection.y == 0.0f) {
				notCollinearVector = { 1.0f, 0.0f, 0.0f };
			} else if (cylDirection.x == 0.0f && cylDirection.z == 0.0f) {
				notCollinearVector = { 1.0f, 0.0f, 0.0f };
			} else {
				notCollinearVector = { 0.0f, 1.0f, 0.0f };
			}

			f32v3 u = glm::normalize(glm::cross(cylDirection, notCollinearVector)); // Perpendicular to the cylinder
			f32v3 v = glm::normalize(glm::cross(cylDirection, u)); // Perpendicular to the U and cylinder
			f32 t = 0; // Theta (angle)
			f32 PI = 3.1415926535;
			f32 PI_2 = 6.2831853072;

			// Bottom 
			for (ui32 i = 0; i < cylEdges; i++) {
				f32 t = (PI_2 / cylEdges) * (double)i;
				f32v3 p = (u * cos(t) + v * sin(t)) * cylRadius; // Finding point on our UV plane
				p += start;

				model::Vertex v = { p, glm::normalize(-cylDirection * normalDirection), {0, 0} };
				vertices.push_back(v);

				if (i > 1) {
					indicies.push_back(0);
					indicies.push_back(i);
					indicies.push_back(i - 1);
				}
			}

			// Top
			for (ui32 i = 0; i < cylEdges; i++) {
				f32 t = (PI_2 / cylEdges) * (double)i;
				f32v3 p = (u * cos(t) + v * sin(t)) * cylRadius; // Finding point on our UV plane
				p += end;

				model::Vertex v = { p, glm::normalize(cylDirection * normalDirection), {0, 0} };
				vertices.push_back(v);

				if (i > 1) {
					indicies.push_back(cylEdges + i - 1);
					indicies.push_back(cylEdges + i);
					indicies.push_back(cylEdges + 0);
				}
			}

			// Wall
			for (ui32 i = 0; i < cylEdges; i++) {
				f32v3 p = vertices[i].position;
				f32v3 n = glm::normalize(p - start);
				model::Vertex v = { p, n * normalDirection, {0, 0} };
				vertices.push_back(v);

				f32v3 p2 = vertices[cylEdges + i].position;
				model::Vertex v2 = { p2, n * normalDirection, {0, 0} };
				vertices.push_back(v2);
			}

			ui32 index = cylEdges * 2;
			for (ui32 i = 0; i < cylEdges * 2; i += 2) {
				indicies.push_back((index + i + 2) == index + cylEdges * 2 ? index : index + i + 2);
				indicies.push_back(index + i + 1);
				indicies.push_back(index + i);

				indicies.push_back(index + i + 3 == index + cylEdges * 2 + 1 ? index + 1 : index + i + 3);
				indicies.push_back(index + i + 1);
				indicies.push_back((index + i + 2) == index + cylEdges * 2 ? index : index + i + 2);
			}


			return model::Mesh(vertices, indicies, textures);
		}

		static model::Model genArrow(f32v3 start, f32v3 end, ui32 edges = 6, f32 cylRadius = 0.5, f32 coneRadius = 1.0) {
			model::Model arrow;
			arrow.addMesh(genCylinder(start, end - glm::normalize(end - start) * coneRadius * 2.0001f, edges, cylRadius));
			arrow.addMesh(genCone(end - glm::normalize(end - start) * coneRadius * 2.0f, end, edges, coneRadius));
			
			return arrow;
		}
	}
}