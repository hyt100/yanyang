#include "yyCore.h"

int yyCore::framebuffWidth_ = 0;
int yyCore::framebuffHeight_ = 0;

void yyCore::setupWindow(const std::string &name, int width, int height)
{
    windowName_   = name;
    windowWidth_  = width;
    windowHeight_ = height;
}

int yyCore::init()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);  //固定窗口大小
    if (antialias_) {
        glfwWindowHint(GLFW_SAMPLES, 4);
    }

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

    glfwSetCursorPosCallback(window_, mouse_callback);
    glfwSetScrollCallback(window_, scroll_callback);
    // glfwSetInputMode(window_, GLFW_CURSOR, GLFW_CURSOR_DISABLED); //设置为DISABLED，则不会显示鼠标光标

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
        if (antialias_)
            glEnable(GL_MULTISAMPLE);
        if (outputSRGB_)
            glEnable(GL_FRAMEBUFFER_SRGB);
        glViewport(0, 0, framebuffWidth_, framebuffHeight_);
        app_->update();

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

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
void yyCore::processInput()
{
    if(glfwGetKey(window_, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window_, true);
    if(glfwGetKey(window_, GLFW_KEY_LEFT) == GLFW_PRESS)
        app_->keyprocess(YYEVENT_LEFT);
    if(glfwGetKey(window_, GLFW_KEY_RIGHT) == GLFW_PRESS)
        app_->keyprocess(YYEVENT_RIGHT);
    if(glfwGetKey(window_, GLFW_KEY_UP) == GLFW_PRESS)
        app_->keyprocess(YYEVENT_UP);
    if(glfwGetKey(window_, GLFW_KEY_DOWN) == GLFW_PRESS)
        app_->keyprocess(YYEVENT_DOWN);
    if(glfwGetKey(window_, GLFW_KEY_EQUAL) == GLFW_PRESS)
        app_->keyprocess(YYEVENT_ZOOMIN);
    if(glfwGetKey(window_, GLFW_KEY_MINUS) == GLFW_PRESS)
        app_->keyprocess(YYEVENT_ZOOMOUT);
}

// glfw: whenever the mouse moves, this callback is called
void yyCore::mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    // std::cout << "mouse_callback: " << xposIn << ", " << yposIn << std::endl;
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
void yyCore::scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    // std::cout << "scroll_callback: " << xoffset << ", " << yoffset << std::endl;
}