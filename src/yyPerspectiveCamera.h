#pragma once
#include "yyUtil.h"
#include <memory>

class yyPerspectiveCamera
{
public:
    using Ptr = std::shared_ptr<yyPerspectiveCamera>;
    static Ptr create(float width, float height, float fov, float near, float far, const glm::vec3 &position, const glm::vec3 &target)
    {
        return std::make_shared<yyPerspectiveCamera>(width, height, fov, near, far, position, target);
    }
    yyPerspectiveCamera(float width, float height, float fov, float near, float far, const glm::vec3 &position, const glm::vec3 &target);
    ~yyPerspectiveCamera() = default;

    void keyprocess(yyEvent event);
    glm::vec3 getPosition() { return position_; }
    glm::vec3 getTarget()   { return target_; }
    glm::vec3 getUp()       { return up_; }
    glm::mat4 getProjectMat()     { return projectionMat_; }
    glm::mat4 getViewMat()        { return viewMat_; }
    glm::mat4 getViewProjectMat() { return viewProjectionMat_; }

private:
    void update();

    void zoom_in();
    void zoom_out();
    void up();
    void down();
    void left();
    void right();

    float R_Min_, R_Max_, R_Step_;
    float R_;     // 由上而下构建球体（右手坐标系，Y轴正方向为上，Y轴负方向为下）
    float theta_; // theta为球体上当前位置向量与+Y轴的夹角，取值0-180
    float gamma_; // gama为圆周上当前位置向量与+Z轴的逆时针方向夹角，取值0-360
    glm::mat4 projectionMat_;
    glm::mat4 viewMat_;
    glm::mat4 viewProjectionMat_;
    glm::vec3 orbitCenter_;
    glm::vec3 position_;
    glm::vec3 target_;
    glm::vec3 up_;
};