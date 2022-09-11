#include "yyModel.h"

static const std::map<aiTextureType, yyTextureType> kTextureTypeMapping = {
    {aiTextureType_DIFFUSE,  yyTextureType_DIFFUSE},
    {aiTextureType_SPECULAR, yyTextureType_SPECULAR},
    {aiTextureType_NORMALS,  yyTextureType_NORMAL},
    {aiTextureType_AMBIENT,  yyTextureType_AMBIENT}
};

yyModel::yyModel(const std::string &filename)
{
    Assimp::Importer importer;
    // 后处理参数：
    //   aiProcess_Triangulate - 三角剖分
    //   aiProcess_GenNormals  - 如果模型不包含法向量的话，就为每个顶点创建法线
    //   aiProcess_FlipUVs     - 翻转y轴的纹理坐标
    //   aiProcess_CalcTangentSpace - 
    const aiScene* scene = importer.ReadFile(filename, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        std::cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << std::endl;
        return;
    }
    // retrieve the directory path of the filepath
    directory_ = filename.substr(0, filename.find_last_of('/'));

    // process ASSIMP's root node recursively
    processNode(scene->mRootNode, scene);
}

void yyModel::processNode(aiNode *node, const aiScene *scene)
{
    // process each mesh located at the current node
    for (unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        // the node object only contains indices to index the actual objects in the scene. 
        // the scene contains all the data, node is just to keep stuff organized (like relations between nodes).
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        pMeshs_.push_back(processMesh(mesh, scene));
    }
    // after we've processed all of the meshes (if any) we then recursively process each of the children nodes
    for (unsigned int i = 0; i < node->mNumChildren; i++)
    {
        processNode(node->mChildren[i], scene);
    }
}

yyMesh::Ptr yyModel::processMesh(aiMesh *mesh, const aiScene *scene)
{
    std::vector<glm::vec3> vertexs;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec4> colors;
    std::vector<glm::vec2> texCoords;
    std::vector<glm::vec3> tangents;
    std::vector<glm::vec3> bitangents;
    std::vector<unsigned int> indices;
    std::vector<yyTexture::Ptr> pTextures;

    // walk through each of the mesh's vertices
    for(unsigned int i = 0; i < mesh->mNumVertices; i++) {
        vertexs.emplace_back(glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z));

        if (mesh->HasNormals()) {
            normals.emplace_back(glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z));
        }

        // a vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't 
        // use models where a vertex can have multiple texture coordinates so we always take the first set (0).
        if (mesh->mTextureCoords[0]) {
            texCoords.emplace_back(glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y));
            tangents.emplace_back(glm::vec3(mesh->mTangents[i].x, mesh->mTangents[i].y, mesh->mTangents[i].z));
            bitangents.emplace_back(glm::vec3(mesh->mBitangents[i].x, mesh->mBitangents[i].y, mesh->mBitangents[i].z));
        }

        if (mesh->mColors[0]) {
            colors.emplace_back(glm::vec4(mesh->mColors[0][i].r, mesh->mColors[0][i].g, mesh->mColors[0][i].b, mesh->mColors[0][i].a));
        }
    }

    // wak through each of the mesh's faces (a face is a mesh its triangle) and retrieve the corresponding vertex indices.
    for (unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        // retrieve all indices of the face and store them in the indices vector
        for(unsigned int j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);        
    }

    // process materials
    aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
    std::vector<yyTexture::Ptr> pTexturesDiffuse;
    std::vector<yyTexture::Ptr> pTexturesSpecular;
    std::vector<yyTexture::Ptr> pTexturesNormal;
    std::vector<yyTexture::Ptr> pTexturesAmbient;
    loadMaterialTextures(material, aiTextureType_DIFFUSE, pTexturesDiffuse);
    pTextures.insert(pTextures.end(), pTexturesDiffuse.begin(), pTexturesDiffuse.end());
    loadMaterialTextures(material, aiTextureType_SPECULAR, pTexturesSpecular);
    pTextures.insert(pTextures.end(), pTexturesSpecular.begin(), pTexturesSpecular.end());
    loadMaterialTextures(material, aiTextureType_NORMALS, pTexturesNormal);
    pTextures.insert(pTextures.end(), pTexturesNormal.begin(), pTexturesNormal.end());
    loadMaterialTextures(material, aiTextureType_AMBIENT, pTexturesAmbient);
    pTextures.insert(pTextures.end(), pTexturesAmbient.begin(), pTexturesAmbient.end());

    // create and build mesh
    auto pMesh = yyMesh::create();
    pMesh->addIndices(indices);
    pMesh->addVertex(vertexs);
    pMesh->addNormal(normals);
    pMesh->addColor(colors);
    pMesh->addTexCoords(texCoords);
    pMesh->addTangents(tangents);
    pMesh->addBitangents(bitangents);
    pMesh->addTextures(pTextures);
    pMesh->bulid();
    std::cout << "model vertex size: " << vertexs.size() << std::endl;
    std::cout << "model texture size: " << pTextures.size() << std::endl;
    return pMesh;
}

yyTextureType yyModel::convertTextureType(aiTextureType type)
{
    if (kTextureTypeMapping.find(type) != kTextureTypeMapping.end())
        return kTextureTypeMapping.at(type);
    else
        return yyTextureType_NONE;
}

void yyModel::loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::vector<yyTexture::Ptr> &out)
{
    out.clear();
    for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
    {
        aiString str;
        mat->GetTexture(type, i, &str);
        std::string filename = directory_ + "/" + str.C_Str();
        bool skip = false;

        // check if texture was loaded before and if so, continue to next iteration: skip loading a new texture
        for (unsigned int j = 0; j < pTexturesLoaded_.size(); j++)
        {
            auto filenameRef = pTexturesLoaded_[j]->getFilePath();
            if (filenameRef.size() == 0)
                continue;
            if (filename == filenameRef) {
                out.push_back(pTexturesLoaded_[j]);
                skip = true; // a texture with the same filepath has already been loaded
                break;
            }
        }

        if (!skip)
        {
            auto pTexture = yyTexture::create(filename, convertTextureType(type), false, yyImageEncodingLinear);
            out.push_back(pTexture);
            pTexturesLoaded_.push_back(pTexture);
        }
    }
}

