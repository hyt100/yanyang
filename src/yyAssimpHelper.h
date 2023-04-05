#pragma once
#include "yyUtil.h"
#include "yyTexture.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

class yyAssimpHelper
{
public:
    static glm::vec3 toGlmVec3(aiVector3D from)
	{
		return glm::vec3(from.x, from.y, from.z);
	}

	static glm::quat toGlmQuat(aiQuaternion from)
	{
		return glm::quat(from.w, from.x, from.y, from.z);
	}

	static glm::mat4 toGlmMat4(aiMatrix4x4 from)
	{
		glm::mat4 to;
		//the a,b,c,d in assimp is the row ; the 1,2,3,4 is the column
		to[0][0] = from.a1; to[1][0] = from.a2; to[2][0] = from.a3; to[3][0] = from.a4;
		to[0][1] = from.b1; to[1][1] = from.b2; to[2][1] = from.b3; to[3][1] = from.b4;
		to[0][2] = from.c1; to[1][2] = from.c2; to[2][2] = from.c3; to[3][2] = from.c4;
		to[0][3] = from.d1; to[1][3] = from.d2; to[2][3] = from.d3; to[3][3] = from.d4;
		return to;
	}

    static yyTextureType toTextureType(aiTextureType from)
    {
        static const std::map<aiTextureType, yyTextureType> kTextureTypeMapping = {
            {aiTextureType_DIFFUSE,  yyTextureType_DIFFUSE  },
            {aiTextureType_SPECULAR, yyTextureType_SPECULAR },
            {aiTextureType_NORMALS,  yyTextureType_NORMAL   },
            {aiTextureType_AMBIENT,  yyTextureType_AMBIENT  }
        };

        if (kTextureTypeMapping.find(from) != kTextureTypeMapping.end())
            return kTextureTypeMapping.at(from);
        else
            return yyTextureType_NONE;
    }
};
