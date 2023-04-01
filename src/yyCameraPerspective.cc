#include "yyCameraPerspective.h"
#include <cmath>

yyCameraPerspective::yyCameraPerspective(float width, float height, float fov, float near, float far, const glm::vec3 &position, const glm::vec3 &target)
{
    // 检查参数是否有效
    target_ = target;
    position_ = position;
    glm::vec3 positionSphere = position_ - target_;  // 移动到以原点为中心的球体上
    if (yyFloatEQ(positionSphere.x, 0.0f) && yyFloatEQ(positionSphere.y, 0.0f) && yyFloatEQ(positionSphere.z, 0.0f)) {
        std::cout << "R error, use default." << std::endl;
        target_ = glm::vec3(0.0f, 0.0f, 0.0f);
        position_ = glm::vec3(0.0f, 0.0f, 2.0f);
    }
    positionSphere = position_ - target_;
    if (yyFloatEQ(positionSphere.x, 0.0f) && yyFloatEQ(positionSphere.z, 0.0f)) {
        std::cout << "Y axis, use bias." << std::endl;
        target_ += glm::vec3(0.01f, 0.01f, 0.01f);
    }
    // target被用作旋转中心点 (实现了OrbitControl)
    positionSphere = position_ - target_;
    R_ = glm::length(target_ - position_);
    R_Min_  = R_ * 0.1f;
    R_Max_  = R_ * 2.0f;
    R_Step_ = R_ * 0.1f;

    // 计算theta, gamma
    float thetaRad = std::acos(positionSphere.y / R_);  // acos: [0, PI]
    float layerRaduis = R_ * std::sin(thetaRad);
    float gammaRad = std::acos(positionSphere.z / layerRaduis);
    if (positionSphere.x < 0.0f)
        gammaRad = 2*yyPI - gammaRad;
    gamma_ = yyRad2Deg(gammaRad);
    theta_ = yyRad2Deg(thetaRad);

    // 计算VP矩阵
    projectionMat_ = glm::perspective(glm::radians(fov), 1.0f * width / height, near, far);
    update();
}

void yyCameraPerspective::update()
{
    // //计算相机位置
    float thetaRad = glm::radians(theta_);
    float gammaRad = glm::radians(gamma_);
    float layerRaduis = R_ * std::sin(thetaRad);
    float pos_x = layerRaduis * std::sin(gammaRad);
    float pos_y = R_ * std::cos(thetaRad);
    float pos_z = layerRaduis * std::cos(gammaRad);
    position_ = glm::vec3(pos_x, pos_y, pos_z) + target_;

    // 计算up向量
    glm::vec3 targetDir = glm::normalize(target_ - position_);
    glm::vec3 rightDir = glm::normalize(glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), -targetDir));
    up_ = glm::normalize(glm::cross(-targetDir, rightDir));

    // 计算VP矩阵
    viewMat_ = glm::lookAt(position_, target_, up_);
    viewProjectionMat_ = projectionMat_ * viewMat_;
}

void yyCameraPerspective::keyprocess(yyEvent event)
{
    switch (event) {
    case YYEVENT_UP:
        up();
        break;
    case YYEVENT_DOWN:
        down();
        break;
    case YYEVENT_LEFT:
        left();
        break;
    case YYEVENT_RIGHT:
        right();
        break;
    case YYEVENT_ZOOMIN:
        zoom_in();
        break;
    case YYEVENT_ZOOMOUT:
        zoom_out();
        break;
    default:
        break;
    }
}

void yyCameraPerspective::up()
{
    theta_ -= 3.0f;
    if (theta_ < 0.1f)
        theta_ = 0.1f;
    update();
}

void yyCameraPerspective::down()
{
    theta_ += 3.0f;
    if (theta_ > 179.9f)
        theta_ = 179.9f;
    update();
}

void yyCameraPerspective::left()
{
    gamma_ -= 3.0f;
    if (gamma_ < 0.0f)
        gamma_ += 360.0f;
    update();
}

void yyCameraPerspective::right()
{
    gamma_ += 3.0f;
    if (gamma_ > 360.0f)
        gamma_ -= 360.0f;
    update();
}

void yyCameraPerspective::zoom_in()
{
    R_ -= R_Step_;
    if (R_ < R_Min_)
        R_ = R_Min_;
    update();
}

void yyCameraPerspective::zoom_out()
{
    R_ += R_Step_;
    if (R_ > R_Max_)
        R_ = R_Max_;
    update();
}
