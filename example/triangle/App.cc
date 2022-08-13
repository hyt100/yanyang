#include "App.h"

void App::setup()
{
    std::vector<float> vertices = {
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        0.0f,  0.5f, 0.0f
    };
    std::vector<float> colors = {
        1.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 1.0f
    };
    std::vector<float> uvs = {
        0.0f, 0.0f, 
        1.0f, 0.0f,
        0.0f, 1.0f
    };
    std::vector<unsigned int> indices = {
        0, 1, 2
    };

    mesh_.addVertex(vertices);
    mesh_.addIndices(indices);
    mesh_.addColor(colors);
    mesh_.addUV(uvs);
    mesh_.bulid();

    shader_.load("../example/triangle/shader.vert", "../example/triangle/shader.frag");
    texture_.load("../assets/lena_512x512.jpg");
}

void App::update()
{

}

void App::draw()
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    shader_.begin();
    shader_.setBool("uUseTexture", false);
    shader_.setTexture("uTexture", texture_, 0);
    mesh_.draw(true);
    shader_.end();
    yyShaderCheckError();
}