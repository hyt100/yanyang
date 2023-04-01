#pragma once
#include "yyUtil.h"
#include "yyShader.h"
#include "yyBaseApp.h"
#include "yyMesh.h"
#include "yyTexture.h"
#include "yyCameraPerspective.h"
#include "yyModel.h"
#include "yyLight.h"
#include "yyPostProcess.h"
#include "yyMeshSkybox.h"
#include "yyMeshPlane.h"
#include "yyMeshBox.h"
#include "yyMeshSphere.h"

class yyCore
{
public:
    virtual ~yyCore() {
        // glfw: terminate, clearing all previously allocated GLFW resources.
        glfwTerminate();  // 调用前必须已释放了所有的资源，否则会引起coredump
    }

    static yyCore& getInstance()
    {
        static yyCore instance;
        return instance;
    }

    void setupWindow(const std::string &name, int width, int height);
    void setAntiAliasing(bool val) { antialias_ = val; }
    void setOutputSRGB(bool val)   { outputSRGB_ = val; }
    int init();
    int run(yyBaseApp *app);

public:
    static int framebuffWidth_;
    static int framebuffHeight_;

private:
    yyCore(): antialias_(false), outputSRGB_(false)
    {}
    void processInput();
    static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
    static void mouse_callback(GLFWwindow* window, double xposIn, double yposIn);
    static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

private:
    std::string windowName_;
    int windowWidth_, windowHeight_;
    bool antialias_;
    bool outputSRGB_;
    GLFWwindow* window_;
    std::shared_ptr<yyBaseApp> app_;
};

#define yyCoreInstance (yyCore::getInstance())
#define yyFrambuffWidth   (yyCoreInstance.framebuffWidth_)
#define yyFrambuffHeight  (yyCoreInstance.framebuffHeight_)