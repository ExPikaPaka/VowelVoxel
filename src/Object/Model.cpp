#include "Model.h"

#define STB_IMAGE_IMPLEMENTATION
#include "../3dparty/stb_image/stb_image.h"
#include <cfloat>

namespace ent {
	namespace model {
		Model::Model(const char* path) {
			logger = util::Logger::getInstance();
			loadModel(path);
		}

		Model::Model(Model& other) {
			this->logger = logger->getInstance();
			this->textures_loaded = other.getTextures();
			this->meshes = other.getMeshes();
			this->directory = other.getDirectory();
			this->linearFilter = other.getLinearFilter();
			this->nearestFilter = other.getNearestFilter();
		}

		Model::Model(Model&& other) {
			this->logger = logger->getInstance();
			this->textures_loaded = other.getTextures();
			this->meshes = other.getMeshes();
			this->directory = other.getDirectory();
			this->linearFilter = other.getLinearFilter();
			this->nearestFilter = other.getNearestFilter();
		}

		Model::Model() {
			logger = util::Logger::getInstance();
		}

		void Model::load(const char* path) {
			loadModel(path);
		}

		void Model::draw(render::Shader& shader, i32 index) {
			// If mesh ID is specified - try to draw it
			if (index != -1) {
				// Skip drawing in case wrong index
				if (index > -1 && index < meshes.size()) {
					meshes[index].draw(shader);
				}
			} else { // Draw all meshes in case if index not specified
				for (ui32 i = 0; i < meshes.size(); i++) { 
					meshes[i].draw(shader);
				}
			}
		}

		void Model::clear() {
			meshes.clear();
			directory.clear();
			textures_loaded.clear();
			directory.clear();
		}

		void Model::addMesh(Mesh mesh) {
			meshes.push_back(mesh);
			updateCenterAndAABB();
		}

		void Model::setTextureFilter(int filter) {
			if (filter == GL_LINEAR) {
				linearFilter = true;
				nearestFilter = false;
			}
			if (filter == GL_NEAREST) {
				nearestFilter = true;
				linearFilter = false;
			}
		}

		void Model::loadModel(std::string path) {
			logger->addLog(std::string("Loading object \"") + path + std::string("\""), util::level::DEBUG);
			Assimp::Importer importer;
			const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

			if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
				std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
				return;
			}
			directory = path.substr(0, path.find_last_of('/'));

			processNode(scene->mRootNode, scene);
			updateCenterAndAABB();
			logger->addLog("Object loaded successfully", util::level::DEBUG);
		}

		void Model::processNode(aiNode* node, const aiScene* scene) {
			// Process all node's meshes
			for (ui32 i = 0; i < node->mNumMeshes; i++) {
				aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
				meshes.push_back(processMesh(mesh, scene));
			}

			// Do the same for each of its children
			for (ui32 i = 0; i < node->mNumChildren; i++) {
				processNode(node->mChildren[i], scene);
			}
		}

		Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene) {
			std::vector<Vertex>       vertices;
			std::vector<ui32> indices;
			std::vector<Texture>      textures;

			// Vertices processing
			for (ui32 i = 0; i < mesh->mNumVertices; i++) {
				Vertex vertex;

				f32v3 vector;
				vector.x = mesh->mVertices[i].x;
				vector.y = mesh->mVertices[i].y;
				vector.z = mesh->mVertices[i].z;
				vertex.position = vector;

				if (mesh->HasNormals()) {
					vector.x = mesh->mNormals[i].x;
					vector.y = mesh->mNormals[i].y;
					vector.z = mesh->mNormals[i].z;
					vertex.normal = vector;
				}

				if (mesh->mTextureCoords[0]) { // Is there a texture?
					f32v2 vec;
					vec.x = mesh->mTextureCoords[0][i].x;
					vec.y = mesh->mTextureCoords[0][i].y;
					vertex.texCoord = vec;
				} else {
					vertex.texCoord = f32v2(0);
				}

				vertices.push_back(vertex);
			}

			// Indices processing
			for (ui32 i = 0; i < mesh->mNumFaces; i++) {
				aiFace face = mesh->mFaces[i];

				for (ui32 j = 0; j < face.mNumIndices; j++) {
					indices.push_back(face.mIndices[j]);
				}
			}

			// Texture processing
			if (mesh->mMaterialIndex >= 0) {
				aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

				std::vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "diffuse");
				textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

				std::vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "specular");
				textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());

				std::vector<Texture> emissiveMaps = loadMaterialTextures(material, aiTextureType_EMISSIVE, "emissive");
				textures.insert(textures.end(), emissiveMaps.begin(), emissiveMaps.end());
			}

			return Mesh(vertices, indices, textures);
		}

		std::vector<Texture> Model::loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName) {
			std::vector<Texture> textures;

			for (ui32 i = 0; i < mat->GetTextureCount(type); i++) {
				aiString str;
				mat->GetTexture(type, i, &str);
				bool skip = false;

				for (ui32 j = 0; j < textures_loaded.size(); j++) {
					if (std::strcmp(textures_loaded[j].path.data(), str.C_Str()) == 0) {
						textures.push_back(textures_loaded[j]);
						skip = true;
						break;
					}
				}

				if (!skip) {   // if texture hasn't been loaded already, load it
					Texture tex;

					ui32 filterType = linearFilter ? GL_LINEAR : GL_NEAREST;
					tex.id = TextureFromFile(str.C_Str(), directory, filterType);
					tex.type = typeName;
					tex.path = str.C_Str();
					textures.push_back(tex);
					textures_loaded.push_back(tex); // add to loaded textures
				}
			}

			return textures;
		}

		ui32 Model::TextureFromFile(const char* path, const std::string& directory, ui32 filterType) {
			std::string filename = std::string(path);
			filename = directory + '/' + filename;

			ui32 textureID;
			glGenTextures(1, &textureID);

			int width, height, nrComponents;
			unsigned char* data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
			if (data) {
				ui32 format = 0;
				if (nrComponents == 1)
					format = GL_RED;
				else if (nrComponents == 3)
					format = GL_RGB;
				else if (nrComponents == 4)
					format = GL_RGBA;

				glBindTexture(GL_TEXTURE_2D, textureID);
				glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
				glGenerateMipmap(GL_TEXTURE_2D);

				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
				if (filterType == GL_LINEAR) {
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				}
				if (filterType == GL_NEAREST) {
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
				}
				stbi_image_free(data);
			} else {
				util::Logger* _logger = _logger->getInstance();
				_logger->addLog(std::string("Texture failed to load at path: ") + path, util::level::ERROR);
				stbi_image_free(data);
			}

			return textureID;
		}

		algorithm::Ray Model::shootRay(f32v3 start, f32v3 direction, f32 maxLength) {
			algorithm::Ray ray; // Ray to return

			maxLength = maxLength == -1 ? FLT_MAX : maxLength; // Defining maximum length
			f32 minLength = maxLength; // Defining starting minimal length

			for (ui32 i = 0; i < meshes.size(); i++) {
				for (ui32 j = 0; j < meshes[i].indices.size(); j += 3) {
					f32 i1 = meshes[i].indices[j + 0];
					f32 i2 = meshes[i].indices[j + 1];
					f32 i3 = meshes[i].indices[j + 2];

					f32v3 p1 = meshes[i].vertices[i1].position;
					f32v3 p2 = meshes[i].vertices[i2].position;
					f32v3 p3 = meshes[i].vertices[i3].position;

					f32 length = algorithm::shootRay(start, direction, p1, p2, p3);

					if (length > 0) {
						ray.hitCount++;
						if (length < minLength) {
							minLength = length;
						}
					}
				}
			}


			ray.hit = minLength < maxLength;
			ray.length = ray.hit ? minLength : maxLength;

			return ray;
		}

		f32v3 Model::getCenter() {
			f32v3 p(0);
			f32 vCnt = 0;
			for (ui32 i = 0; i < meshes.size(); i++) {
				vCnt += meshes[i].vertices.size();
				for (ui32 j = 0; j < meshes[i].vertices.size(); j++) {
					p += meshes[i].vertices[j].position;
				}
			}
			return p / vCnt;
		}

		Model& Model::operator=(Model& other) {
			if (this == &other) {
				return *this; // Handle self-assignment
			}

			this->logger = logger->getInstance();
			this->textures_loaded = other.getTextures();
			this->meshes = other.getMeshes();
			this->directory = other.getDirectory();
			this->linearFilter = other.getLinearFilter();
			this->nearestFilter = other.getNearestFilter();

			return *this;
		}

		Model& Model::operator=(Model&& other) {
			if (this == &other) {
				return *this; // Handle self-assignment
			}

			this->logger = logger->getInstance();
			this->textures_loaded = other.getTextures();
			this->meshes = other.getMeshes();
			this->directory = other.getDirectory();
			this->linearFilter = other.getLinearFilter();
			this->nearestFilter = other.getNearestFilter();

			return *this;
		}

		std::vector<Mesh> Model::getMeshes() {
			return meshes;
		}

		std::string Model::getDirectory() {
			return directory;
		}

		std::vector<Texture> Model::getTextures() {
			return textures_loaded;
		}

		bool Model::getLinearFilter() {
			return linearFilter;
		}

		bool Model::getNearestFilter() {
			return nearestFilter;
		}
		int Model::getMeshesCount() {
			return meshes.size();
		}
		void Model::updateCenterAndAABB() {
			setupCenterAndAABB(); // Setup some starting values

			// Loop trough each mesh
			for (ui32 id = 0; id < meshes.size(); id++) {
				Mesh& mesh = meshes[id];

				// Loop trough each vertex and update XYZ min max values
				ui32 verticesCnt = mesh.vertices.size();
				for (ui32 vId = 0; vId < verticesCnt; vId++) {
					Vertex& v = mesh.vertices[vId];
					// Center
					center.x += v.position.x;
					center.y += v.position.y;
					center.z += v.position.z;

					// AABB			
					// X axis
					if (v.position.x < AABB.xMin) AABB.xMin = v.position.x;
					if (v.position.x > AABB.xMax) AABB.xMax = v.position.x;
					// Y axis
					if (v.position.y < AABB.yMin) AABB.yMin = v.position.y;
					if (v.position.y > AABB.yMax) AABB.yMax = v.position.y;
					// Z axis
					if (v.position.z < AABB.zMin) AABB.zMin = v.position.z;
					if (v.position.z > AABB.zMax) AABB.zMax = v.position.z;
				}

				center /= verticesCnt;
			}
		}
		void Model::setupCenterAndAABB() {
			// Setup center
			center = { 0,0,0 };

			// Setup AAB
			for (ui32 id = 0; id < meshes.size(); id++) {
				Mesh& mesh = meshes[id];

				// Loop trough each vertex and update XYZ min max values
				if (mesh.vertices.size()) {
					Vertex& v = mesh.vertices[0];
					// X axis
					AABB.xMin = v.position.x;
					AABB.xMax = v.position.x;
					// Y axis
					AABB.yMin = v.position.y;
					AABB.yMax = v.position.y;
					// Z axis
					AABB.zMin = v.position.z;
					AABB.zMax = v.position.z;

					break;
				}
			}
		}
	}
}