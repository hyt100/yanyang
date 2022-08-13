#include "yyCore.h"

int yyCore::framebuffWidth_ = 0;
int yyCore::framebuffHeight_ = 0;

int yyCore::setupWindow(const std::string &name, int width, int height)
{
    windowName_   = name;
    windowWidth_  = width;
    windowHeight_ = height;
    return 0;
}

int yyCore::init()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    window_ = glfwCreateWindow(windowWidth_, windowHeight_, windowName_.c_str(), NULL, NULL);
    if (window_ == NULL) {
        throw std::runtime_error("Failed to create GLFW window");
    }
    glfwMakeContextCurrent(window_);
    glfwSetFramebufferSizeCallback(window_, framebuffer_size_callback);
    glfwGetFramebufferSize(window_, &framebuffWidth_, &framebuffHeight_); //获取渲染缓冲区大小

    // glad: load all OpenGL function pointers
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        throw std::runtime_error("Failed to initialize GLAD");
    }
    return 0;
}

int yyCore::run(yyBaseApp *app)
{
    app_ = std::shared_ptr<yyBaseApp>(app);
    app_->setup();

    // render loop
    while (!glfwWindowShouldClose(window_))
    {
        // input
        processInput();

        // render
        glViewport(0, 0, framebuffWidth_, framebuffHeight_);
        app_->update();
        app_->draw();

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        glfwSwapBuffers(window_);
        glfwPollEvents();
    }
    app_.reset();

    glfwDestroyWindow(window_);
    return 0;
}

void yyCore::framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    std::cout << "window size changed: " << width << "x" << height << std::endl; 
    glViewport(0, 0, width, height);
    framebuffWidth_  = width;
    framebuffHeight_ = height;
}

void yyCore::processInput()
{
    if(glfwGetKey(window_, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window_, true);
}
