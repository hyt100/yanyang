#pragma once
#include "yyUtil.h"

#define yyMAX_BONE_NUM         200
#define yyMAX_BONE_INFLUENCE   4

struct yyKeyPosition {
    glm::vec3	position;
    float		timestamp;
};

struct yyKeyRotation {
    glm::quat	quaternion;
    float		timestamp;
};

struct yyKeyScale {
    glm::vec3	scale;
    float		timestamp;
};

class yyBone
{
public:
    using Ptr = std::shared_ptr<yyBone>;
    static Ptr create()
    {
        return Ptr(new yyBone());
    }
    void update(float time);
    glm::mat4& getLocalTransform() { return localTransform_; }

private:
    yyBone(): id_(0), name_(""), offsetMatrix_(1.0f), localTransform_(1.0f)
    {
    }
    float calLerpFactor(float lastTime, float nextTime, float curTime);
    unsigned int getPositionIndexByTime(float time);
    unsigned int getRotationIndexByTime(float time);
    unsigned int getScaleIndexByTime(float time);
    glm::mat4 interpolatePosition(float time);
    glm::mat4 interpolateRotation(float time);
    glm::mat4 interpolateScale(float time);

public:
    unsigned int id_;
    std::string  name_;
    glm::mat4    offsetMatrix_;
    glm::mat4    localTransform_;

    std::vector<yyKeyPosition> keyPositionArr_;
    std::vector<yyKeyRotation> keyRotationArr_;
    std::vector<yyKeyScale>    keyScaleArr_;
};