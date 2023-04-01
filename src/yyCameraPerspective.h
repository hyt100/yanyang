#pragma once
#include "yyUtil.h"
#include "yyCamera.h"
#include <memory>

class yyCameraPerspective: public yyCamera
{
public:
    using Ptr = std::shared_ptr<yyCameraPerspective>;
    static Ptr create(float width, float height, float fov, float near, float far, const glm::vec3 &position, const glm::vec3 &target)
    {
        // return std::make_shared<yyCameraPerspective>(width, height, fov, near, far, position, target);
        return Ptr(new yyCameraPerspective(width, height, fov, near, far, position, target));
    }
    ~yyCameraPerspective() = default;

    void keyprocess(yyEvent event);
    
private:
    yyCameraPerspective(float width, float height, float fov, float near, float far, const glm::vec3 &position, const glm::vec3 &target);
private:
    void update();

    void zoom_in();
    void zoom_out();
    void up();
    void down();
    void left();
    void right();

    float R_Min_, R_Max_, R_Step_; // 缩放时距离旋转中心的最小值、最大值、缩放步长
    float R_;     // 由上而下构建球体（右手坐标系，Y轴正方向为上，Y轴负方向为下）
    float theta_; // theta为球体上当前位置向量与+Y轴的夹角，取值0-180
    float gamma_; // gama为圆周上当前位置向量与+Z轴的逆时针方向夹角，取值0-360
};