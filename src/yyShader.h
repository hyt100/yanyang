#pragma once
#include "yyUtil.h"
#include "yyTexture.h"

class yyShader
{
public:
    using Ptr = std::shared_ptr<yyShader>;
    static Ptr create(const std::string &vertexShaderFile, const std::string &fragmentShaderFile)
    {
        // return std::make_shared<yyShader>(vertexShaderFile, fragmentShaderFile);
        return Ptr(new yyShader(vertexShaderFile, fragmentShaderFile));
    }

    ~yyShader()
    {
        if (programObject_)
            glDeleteProgram(programObject_);
    }

    void begin();
    void end();
    unsigned int getProgram() { return programObject_; }

    void setMat4(const std::string &name, const glm::mat4 &mat)
    {
        beginTemp();
        glUniformMatrix4fv(glGetUniformLocation(programObject_, name.c_str()), 1, GL_FALSE, glm::value_ptr(mat));
        endTemp();
    }
    void setMat3(const std::string &name, const glm::mat3 &mat)
    {
        beginTemp();
        glUniformMatrix3fv(glGetUniformLocation(programObject_, name.c_str()), 1, GL_FALSE, glm::value_ptr(mat));
        endTemp();
    }
    void setVec3(const std::string &name, const glm::vec3 &value)
    {
        beginTemp();
        glUniform3fv(glGetUniformLocation(programObject_, name.c_str()), 1, &value[0]);
        endTemp();
    }
    void setBool(const std::string &name, bool value)
    {
        beginTemp();
        glUniform1i(glGetUniformLocation(programObject_, name.c_str()), (int)value);
        endTemp();
    }
    void setInt(const std::string &name, int value)
    {
        beginTemp();
        glUniform1i(glGetUniformLocation(programObject_, name.c_str()), value);
        endTemp();
    }
    void setFloat(const std::string &name, float value)
    {
        beginTemp();
        glUniform1f(glGetUniformLocation(programObject_, name.c_str()), value);
        endTemp();
    }
    // textureUnit: 0...15
    void setTextureUnit(const std::string &samplerName, unsigned int textureUnit)
    {
        if (textureUnit > 15) {
            std::cout << "[yyShader] warning: textureUnit value greater than 15. " << std::endl;
        }
        setInt(samplerName, textureUnit);  // 告诉着色器的采样器使用哪个纹理单元
    }

    static GLenum checkError(const char *file, int line);

private:
    yyShader(const std::string &vertexShaderFile, const std::string &fragmentShaderFile);
    void beginTemp();
    void endTemp();
private:
    unsigned int programObject_;  //handle to a progream object
    bool isUseProgram_;
    bool isUseProgramBackup_;
};

#define yyShaderCheckError()   yyShader::checkError(__FILE__, __LINE__) 
