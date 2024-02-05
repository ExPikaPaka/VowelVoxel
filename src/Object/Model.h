#pragma once
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "../Render/Shader.h"
#include "../Algorithm/Ray.h"
#include "Mesh.h"



namespace ent {
	namespace model {
		// Model class, that consists of 1+ meshes
		class Model {
		public:
			Model(const char* path);
			Model(Model& other);
			Model(Model&& other);
			Model();
			void load(const char* path);
			void draw(render::Shader& shader, i32 index = -1);
			void clear();
			void addMesh(Mesh mesh);

			void setTextureFilter(int filter);

			// Loads texture and returns OpenGL id to the texture
			static ui32 TextureFromFile(const char* path, const std::string& directory, ui32 filterType);

			algorithm::Ray shootRay(f32v3 start, f32v3 direction, f32 maxLength = -1);
			f32v3 getCenter();

			Model& operator=(Model& other);
			Model& operator=(Model&& other);

			std::vector<Mesh> getMeshes();
			std::string getDirectory();
			std::vector<Texture> getTextures();
			bool getLinearFilter();
			bool getNearestFilter();
			int getMeshesCount();

			struct AABB {
				f32 xMin, xMax;
				f32 yMin, yMax;
				f32 zMin, zMax;
			} AABB;

			f32v3 center;

		private:
			util::Logger* logger;

			std::vector<Texture> textures_loaded;
			std::vector<Mesh> meshes;
			std::string directory;

			bool linearFilter = true;
			bool nearestFilter = false;

			void loadModel(std::string path);
			void processNode(aiNode* node, const aiScene* scene);
			Mesh processMesh(aiMesh* mesh, const aiScene* scene);
			std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);

			void updateCenterAndAABB();
			void setupCenterAndAABB();
		};
	}
}

