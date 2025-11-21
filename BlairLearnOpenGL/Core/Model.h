//
// Created by Blair Ren on 2025/11/3.
//

#ifndef MODEL_H
#define MODEL_H

#include <vector>
#include <string>
#include "Shader.hpp"
#include "Mesh.h"
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/material.h"

using namespace std;

class Model
{
public:
    /*  函数   */
    Model(std::string const &path)
    {
        loadModel(path);
    }
    void Draw(Shader &shader);
private:
    /*  模型数据  */
    std::vector<Mesh> meshes;
    std::vector<Texture> textures_loaded;
    string directory;
    /*  函数   */
    void loadModel(std::string const path);
    void processNode(aiNode *node, const aiScene *scene);
    Mesh processMesh(aiMesh *mesh, const aiScene *scene);
    std::vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type,
                                         string typeName);
    unsigned int TextureFromFile(const char* path, const std::string& directory);
};



#endif //MODEL_H
