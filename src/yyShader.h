#pragma once
#include "yyUtil.h"
#include "yyTexture.h"

class yyShader
{
public:
    yyShader() { programObject_ = 0; }
    ~yyShader()
    {
        if (programObject_)
            glDeleteProgram(programObject_);
    }

    void load(const std::string &vertexShaderFile, const std::string &fragmentShaderFile);

    void begin();
    void end();
    unsigned int getProgram() { return programObject_; }

    void setMat4(const std::string &name, const glm::mat4 &mat)
    {
        glUniformMatrix4fv(glGetUniformLocation(programObject_, name.c_str()), 1, GL_FALSE, glm::value_ptr(mat));
    }
    void setVec3(const std::string &name, const glm::vec3 &value)
    {
        glUniform3fv(glGetUniformLocation(programObject_, name.c_str()), 1, &value[0]); 
    }
    void setBool(const std::string &name, bool value)
    {
        glUniform1i(glGetUniformLocation(programObject_, name.c_str()), (int)value); 
    }
    void setInt(const std::string &name, int value)
    {
        glUniform1i(glGetUniformLocation(programObject_, name.c_str()), value); 
    }
    void setFloat(const std::string &name, float value)
    {
        glUniform1f(glGetUniformLocation(programObject_, name.c_str()), value); 
    }
    // textureUnit: 0...15
    void setTexture(const std::string &name, yyTexture &texture, unsigned int textureUnit)
    {
        glActiveTexture(GL_TEXTURE0 + textureUnit);           // 激活纹理单元
        glBindTexture(GL_TEXTURE_2D, texture.getTextureId()); // 绑定纹理对象到激活的纹理单元
        glUniform1i(glGetUniformLocation(programObject_, name.c_str()), textureUnit);  // 告诉着色器的采样器使用哪个纹理单元
    }

    static GLenum checkError(const char *file, int line);

private:
    unsigned int programObject_;  //handle to a progream object
};

#define yyShaderCheckError()   yyShader::checkError(__FILE__, __LINE__) 
