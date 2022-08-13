#include "yyFile.h"
#include "yyShader.h"

static unsigned int loadShader(GLenum type, const char *shaderSource)
{
    // create the shader object
    unsigned int shader = glCreateShader(type);
    if (0 == shader) {
        std::cout << "create shader failed" << std::endl;
        return 0;
    }
    // load the shader source
    glShaderSource(shader, 1, &shaderSource, NULL);
    // compile the shader
    glCompileShader(shader);

    // check the compile status
    GLint compiled;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
    if (!compiled) {
        GLint infoLen = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);
        if (infoLen > 1) {
            char *infoLog = new char[infoLen];
            glGetShaderInfoLog(shader, infoLen, NULL, infoLog);
            std::cout << "Error compiling shader: \n" << infoLog << "\n" << std::endl;
            delete[] infoLog;
        }
        glDeleteShader(shader);
        return 0;
    }

    return shader;
}

void yyShader::load(const std::string &vertexShaderFile, const std::string &fragmentShaderFile)
{
    yyFileReader vertexShaderSource(vertexShaderFile.c_str());
    yyFileReader fragmentShaderSource(fragmentShaderFile.c_str());
    if (vertexShaderSource.isError() || fragmentShaderSource.isError()) {
        throw std::runtime_error("Read Shader file failed!");
    }

    // load shaders
    unsigned int vertexShader = loadShader(GL_VERTEX_SHADER, (const char *)vertexShaderSource.data());
    if (0 == vertexShader) {
        throw std::runtime_error("LoadShader failed!");
    }

    unsigned int fragmentShader = loadShader(GL_FRAGMENT_SHADER, (const char *)fragmentShaderSource.data());
    if (0 == fragmentShader) {
        glDeleteShader(vertexShader);
        throw std::runtime_error("LoadShader failed!");
    }

    // link shaders
    unsigned int shaderProgram = glCreateProgram();
    if (0 == shaderProgram) {
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
        throw std::runtime_error("create program failed!");
    }
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    // check for linking errors
    int success;
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        GLint infoLen = 0;
        glGetProgramiv(shaderProgram, GL_INFO_LOG_LENGTH, &infoLen);
        if (infoLen > 1) {
            char *infoLog = new char[infoLen];
            glGetProgramInfoLog(shaderProgram, infoLen, NULL, infoLog);
            std::cout << "Error linking program: \n" << infoLog << "\n" << std::endl;
            delete[] infoLog;
        }
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
        glDeleteProgram(shaderProgram);
        return;
    }

    // 删除着色器，它们已经链接到我们的程序中了，已经不再需要了
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    programObject_ = shaderProgram;
    std::cout << "init prog ok" << std::endl;
}

void yyShader::begin()
{
    glUseProgram(programObject_);
}

void yyShader::end()
{
    glUseProgram(0);
}

GLenum yyShader::checkError(const char *file, int line)
{
    GLenum errorCode;
    while ((errorCode = glGetError()) != GL_NO_ERROR)
    {
        std::string error;
        switch (errorCode)
        {
            case GL_INVALID_ENUM:                  error = "INVALID_ENUM"; break;
            case GL_INVALID_VALUE:                 error = "INVALID_VALUE"; break;
            case GL_INVALID_OPERATION:             error = "INVALID_OPERATION"; break;
            // case GL_STACK_OVERFLOW:                error = "STACK_OVERFLOW"; break;
            // case GL_STACK_UNDERFLOW:               error = "STACK_UNDERFLOW"; break;
            case GL_OUT_OF_MEMORY:                 error = "OUT_OF_MEMORY"; break;
            case GL_INVALID_FRAMEBUFFER_OPERATION: error = "INVALID_FRAMEBUFFER_OPERATION"; break;
        }
        std::cout << error << " | " << file << " (" << line << ")" << std::endl;
    }
    return errorCode;
}
