#include "ModelImport.hpp"

#include <iostream>

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include <GraphicsUtils.hpp>
#include <Mesh.hpp>

namespace simulator {

constexpr GLuint kInvalid = 0xFFFFFFFF;

static inline size_t isImageLoaded(const std::string &file_name,
                                   std::vector<model::Texture> &texture_list) {
  for (size_t i = 0; i < texture_list.size(); ++i)
    if (file_name.compare(texture_list[i].m_image_name) == 0) {
      return texture_list[i].m_texture_id;
    }
  return kInvalid;
};

static void loadTexture(std::vector<model::Texture> &texture_vec,
                        model::Mesh &mesh, aiMaterial *material,
                        const char *textures_path, const int tex_count) {

  aiString string_postfix;
  material->GetTexture(aiTextureType_DIFFUSE, tex_count, &string_postfix);

  aiString path(textures_path);
  path.Append(string_postfix.C_Str());

  auto already_loaded = (isImageLoaded(path.C_Str(), texture_vec) != kInvalid);

  if (already_loaded) {
    return;
  }

  bool load_success = false;
  GLuint texture_id;
  if (graphics_utils::loadTextureFromImage(path.C_Str(), texture_id) ==
      graphics_utils::GraphicsRes::FAIL) {
    return;
  }

  model::Texture texture;
  texture.m_image_name = path.C_Str();
  texture.m_texture_id = texture_id;

  texture_vec.push_back(texture);
  mesh.m_tex_handle = texture_id;
}

void loadModel(const char *model_path, const char *textures_path,
               model::Model &model) {
  try {
    Assimp::Importer importer;
    aiNode *root_node = nullptr;
    auto &mesh_vec = model.getMeshVec();
    auto &texture_vec = model.getTextureVec();

    const aiScene *scene = importer.ReadFile(
        model_path, aiProcess_JoinIdenticalVertices | aiProcess_Triangulate |
                        aiProcess_FlipUVs);

    if (!scene || !scene->mRootNode ||
        scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE) {
      throw std::runtime_error("Error importing scene");
    }

    const size_t num_meshes = scene->mNumMeshes;
    mesh_vec.resize(num_meshes);

    for (size_t i = 0; i < num_meshes; ++i) {

      auto mesh = scene->mMeshes[i];
      mesh_vec[i].m_name = mesh->mName.C_Str();

      aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];

      const auto total_textures =
          material->GetTextureCount(aiTextureType_DIFFUSE);
      texture_vec.reserve(total_textures);

      // Load textures
      for (size_t tex_count = 0; tex_count < total_textures; ++tex_count) {
        loadTexture(texture_vec, mesh_vec[i], material, textures_path,
                    tex_count);
      }

      for (size_t v = 0; v < mesh->mNumVertices; ++v) {
        glm::vec3 position{};
        position.x = mesh->mVertices[v].x;
        position.y = mesh->mVertices[v].y;
        position.z = mesh->mVertices[v].z;
        mesh_vec[i].m_vert_positions.push_back(position);

        // Normals
        if (mesh->HasNormals()) {
          glm::vec3 normal{};
          normal.x = mesh->mNormals[v].x;
          normal.y = mesh->mNormals[v].y;
          normal.z = mesh->mNormals[v].z;
          mesh_vec[i].m_vert_normals.push_back(normal);
        } else {
          mesh_vec[i].m_vert_normals.emplace_back(glm::vec3(0.0f, 0.0f, 0.0f));
        }

        // Textures
        if (mesh->HasTextureCoords(0)) {
          glm::vec2 tex_coords{};
          tex_coords.x = mesh->mTextureCoords[0][v].x;
          tex_coords.y = mesh->mTextureCoords[0][v].y;
          mesh_vec[i].m_tex_coords.push_back(tex_coords);
        } else {
          mesh_vec[i].m_tex_coords.emplace_back(glm::vec2(0.0f, 0.0f));
        }
      }

      std::cout << "Mesh has " << mesh->mNumFaces << " faces." << std::endl;

      for (unsigned int i = 0; i < mesh->mNumFaces; ++i) {
        const aiFace &face = mesh->mFaces[i];
        std::cout << "Face " << i << " has " << face.mNumIndices
                  << " indices: ";
        for (unsigned int j = 0; j < face.mNumIndices; ++j) {
          std::cout << face.mIndices[j] << " ";
        }
        std::cout << std::endl;
      }

      // Faces
      for (size_t f = 0; f < mesh->mNumFaces; ++f) {
        for (size_t ind = 0; ind < mesh->mFaces[f].mNumIndices; ++ind) {
          mesh_vec[i].m_vert_indices.push_back(mesh->mFaces[f].mIndices[ind]);
        }
      }
    }

  } catch (std::exception &ex) {
    std::cerr << ex.what() << "/n";
    exit(0);
  }
}

} // namespace simulator
