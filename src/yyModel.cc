#include "yyModel.h"

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
    rootDirectory_ = filename.substr(0, filename.find_last_of('/'));

    // process ASSIMP's root node recursively
    processNode(scene, scene->mRootNode);

    genAnimationNode(rootAnimationNode_, scene->mRootNode);

    if (scene->mNumAnimations > 0) {
        std::cout << "animation num is " << scene->mNumAnimations << std::endl;
        processAnimation(scene);
    }
}

void yyModel::processNode(const aiScene *scene, aiNode *node)
{
    // process each mesh located at the current node
    for (unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        // the node object only contains indices to index the actual objects in the scene. 
        // the scene contains all the data, node is just to keep stuff organized (like relations between nodes).
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        pMeshs_.push_back(processMesh(scene, mesh));
    }
    // after we've processed all of the meshes (if any) we then recursively process each of the children nodes
    for (unsigned int i = 0; i < node->mNumChildren; i++)
    {
        processNode(scene, node->mChildren[i]);
    }
}

yyMesh::Ptr yyModel::processMesh(const aiScene *scene, aiMesh *mesh)
{
    std::vector<glm::vec3> vertexs;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec4> colors;
    std::vector<glm::vec2> texCoords;
    std::vector<glm::vec3> tangents;
    std::vector<glm::vec3> bitangents;
    std::vector<glm::ivec4> boneIDs;
    std::vector<glm::vec4>  boneWeights;
    std::vector<unsigned int> indices;
    std::vector<yyTexture::Ptr> pTextures;

    // walk through each of the mesh's vertices
    for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
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

    // process bones
    if (mesh->HasBones()) {
        loadBoneWeightForVertices(scene, mesh, boneIDs, boneWeights);
    }

    // process materials
    if (scene->mNumMaterials > 0 && mesh->mMaterialIndex >= 0) {
        std::vector<yyTexture::Ptr> pTexturesDiffuse;
        std::vector<yyTexture::Ptr> pTexturesSpecular;
        std::vector<yyTexture::Ptr> pTexturesNormal;
        std::vector<yyTexture::Ptr> pTexturesAmbient;

        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
        loadMaterialTextures(scene, material, aiTextureType_DIFFUSE, pTexturesDiffuse);
        pTextures.insert(pTextures.end(), pTexturesDiffuse.begin(), pTexturesDiffuse.end());
        loadMaterialTextures(scene, material, aiTextureType_SPECULAR, pTexturesSpecular);
        pTextures.insert(pTextures.end(), pTexturesSpecular.begin(), pTexturesSpecular.end());
        loadMaterialTextures(scene, material, aiTextureType_NORMALS, pTexturesNormal);
        pTextures.insert(pTextures.end(), pTexturesNormal.begin(), pTexturesNormal.end());
        loadMaterialTextures(scene, material, aiTextureType_AMBIENT, pTexturesAmbient);
        pTextures.insert(pTextures.end(), pTexturesAmbient.begin(), pTexturesAmbient.end());
    }

    // create and build mesh
    auto pMesh = yyMesh::create();
    pMesh->setAttrIndice(indices);
    pMesh->setAttrVertex(vertexs);
    pMesh->setAttrNormal(normals);
    pMesh->setAttrColor(colors);
    pMesh->setAttrTexCoord(texCoords);
    pMesh->setAttrTangent(tangents);
    pMesh->setAttrBitangent(bitangents);
    pMesh->setAttrBoneID(boneIDs);
    pMesh->setAttrBoneWeight(boneWeights);
    pMesh->setTextures(pTextures);
    pMesh->build();
    std::cout << "model vertex size: " << vertexs.size() << std::endl;
    std::cout << "model texture size: " << pTextures.size() << std::endl;
    return pMesh;
}

void yyModel::loadBoneWeightForVertices(const aiScene *scene, 
                                        aiMesh *mesh, 
                                        std::vector<glm::ivec4> &boneIDs,
                                        std::vector<glm::vec4> &boneWeights)
{
    int vertexNum = mesh->mNumVertices;
    boneIDs.resize(vertexNum);
    boneWeights.resize(vertexNum);
    for (int i = 0; i < vertexNum; ++i) {
        boneIDs[i] = glm::ivec4(-1, -1, -1, -1);
        boneWeights[i] = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);
    }

    // mBones 数组里面存储了所有的骨骼，每个骨骼存储对应绑定的顶点和该顶点的权重
    for (int i = 0; i < mesh->mNumBones; ++i) {
        aiBone* bone = mesh->mBones[i];
        std::string boneName = bone->mName.C_Str();

        unsigned int boneID = -1;
        if (boneMap_.find(boneName) != boneMap_.end()) {
            boneID = boneMap_[boneName]->id_;

        } else {
            boneID = boneCounter_++;

            yyBone::Ptr pBone = yyBone::create();
            pBone->id_           = boneID;
            pBone->name_         = boneName;
            // 因为模型的所有顶点坐标都是在model space坐标系下的，而我们需要知道在某个骨骼上bone space坐标系下的点，
            // 我们可以逐级遍历得到 BoneToWorldMatrix, 这个矩阵的逆矩阵就可以把model space坐标系下的点转换到bone space下的点。
            // mOffsetMatrix就是这样的逆矩阵，也就是说，mOffsetMatrix是把model space坐标系下的点 转换到 bone space坐标系下。
            pBone->offsetMatrix_ = yyAssimpHelper::toGlmMat4(bone->mOffsetMatrix);

            boneMap_[boneName] = pBone;
        }

        for (int j = 0; j < bone->mNumWeights; ++j) {
            int vertexIndex    = bone->mWeights[j].mVertexId;
            float vertexWeight = bone->mWeights[j].mWeight;

            // 找到一个没有设置的骨骼权重，如果设置满了就不能再设置了，也就是说我们只取其中四个权重，超过了就不处理了
            for (int k = 0; k < yyMAX_BONE_INFLUENCE; ++k) {
                if (boneIDs[vertexIndex][k] < 0) {
                    boneIDs[vertexIndex][k]     = boneID;
                    boneWeights[vertexIndex][k] = vertexWeight;
                    break;  // 设置成功就退出
                }
            }
        }
    }
}

void yyModel::loadMaterialTextures(const aiScene *scene, aiMaterial *material, aiTextureType type, std::vector<yyTexture::Ptr> &out)
{
    out.clear();
    for (unsigned int i = 0; i < material->GetTextureCount(type); i++)
    {
        aiString texturePath;
        material->GetTexture(type, i, &texturePath);

        std::string filename = rootDirectory_ + "/" + texturePath.C_Str();
        std::cout << "texture filename: " << filename << std::endl;

        // check if texture was loaded before and if so, continue to next iteration: skip loading a new texture
        if (texturesMap_.find(filename) != texturesMap_.end()) {
            out.emplace_back(texturesMap_[filename]);
            continue;
        }

        yyTexture::Ptr pTexture;

        auto embededTextrure = scene->GetEmbeddedTexture(texturePath.C_Str()); //检测是否是内嵌的材质
        if (embededTextrure) {
            // 如果mHeight为0，则pcData中存储了压缩格式像素，mWidth指定数据大小;
            // 如果mHeight不为0，则pcData中存储了已解码的裸像素，mWidth * mHeight指定数据大小 (猜测是类似bmp格式这样有文件头的裸数据)
            if (embededTextrure->mHeight == 0) {
                pTexture = yyTexture::create(reinterpret_cast<uint8_t*>(embededTextrure->pcData), 
                                             embededTextrure->mWidth, 
                                             yyAssimpHelper::toTextureType(type), 
                                             false);
            } else {
                std::cout << "debug: embeded raw texture (" << embededTextrure->achFormatHint << ") " << std::endl;
                pTexture = yyTexture::create(reinterpret_cast<uint8_t*>(embededTextrure->pcData), 
                                             embededTextrure->mWidth * embededTextrure->mHeight, 
                                             yyAssimpHelper::toTextureType(type), 
                                             false);
            }
        } else {
            pTexture = yyTexture::create(filename, yyAssimpHelper::toTextureType(type), false);
        }

        if (pTexture) {
            out.emplace_back(pTexture);
            texturesMap_[filename] = pTexture;
        }
    }
}

void yyModel::genAnimationNode(yyAnimationNode &aniNode, aiNode *node)
{
    // 在Assimp的规定中，每一个 bone 必定会有一个相同名称的 node 与其对应，反过来不成立
    aniNode.name = node->mName.C_Str();
    aniNode.transform = yyAssimpHelper::toGlmMat4(node->mTransformation);

    for (int i = 0; i < node->mNumChildren; ++i) {
        yyAnimationNode aniNodeChild;
        genAnimationNode(aniNodeChild, node->mChildren[i]);
        aniNode.children.emplace_back(aniNodeChild);
    }
}

void yyModel::processAnimation(const aiScene *scene)
{
    for (int i = 0; i < scene->mNumAnimations; ++i) {
        aiAnimation* animation = scene->mAnimations[i];

        yyAnimation::Ptr p = yyAnimation::create();
        p->duration_       = (float)(animation->mDuration);
        p->ticksPerSecond_ = (float)(animation->mTicksPerSecond);
        p->boneCounter_    = boneCounter_;
        p->boneMap_        = boneMap_;
        p->rootNode_       = rootAnimationNode_;

        for (int j = 0; j < animation->mNumChannels; ++j) {
            aiNodeAnim* channel = animation->mChannels[i];

			// 补全boneInfoMap: 有一些bone在前面还没出现的
			std::string boneName = channel->mNodeName.C_Str();
			if (p->boneMap_.find(boneName) == p->boneMap_.end()) {
                yyBone::Ptr pBone = yyBone::create();
                pBone->id_        = p->boneCounter_ ++;
                pBone->name_      = boneName;
                p->boneMap_[boneName] = pBone;
			}

			// 读取骨骼关键帧信息
			auto& pBone = p->boneMap_[boneName];
            for (int k = 0; k < channel->mNumPositionKeys; ++k) {
                yyKeyPosition keyPosition;
                keyPosition.position  = yyAssimpHelper::toGlmVec3(channel->mPositionKeys[k].mValue);
                keyPosition.timestamp = (float)(channel->mPositionKeys[k].mTime);
                pBone->keyPositionArr_.emplace_back(keyPosition);
            }
            for (int k = 0; k < channel->mNumRotationKeys; ++k) {
                yyKeyRotation keyRotation;
                keyRotation.quaternion = yyAssimpHelper::toGlmQuat(channel->mRotationKeys[k].mValue);
                keyRotation.timestamp  = (float)(channel->mRotationKeys[k].mTime);
                pBone->keyRotationArr_.emplace_back(keyRotation);
            }
            for (int k = 0; k < channel->mNumScalingKeys; ++k) {
                yyKeyScale keyScale;
                keyScale.scale     = yyAssimpHelper::toGlmVec3(channel->mScalingKeys[k].mValue);
                keyScale.timestamp = (float)(channel->mScalingKeys[k].mTime);
                pBone->keyScaleArr_.emplace_back(keyScale);
            }
		}

        animations_.emplace_back(p);
    }
}
