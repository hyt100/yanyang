#include "yyBone.h"

float yyBone::calLerpFactor(float lastTime, float nextTime, float curTime)
{
    if (yyFloatEQ(nextTime - lastTime, 0.0f)) {
        std::cout << "fatal: (nextTime - lastTime) is 0  " << std::endl;
    }
    return (curTime - lastTime) / (nextTime - lastTime);
}

unsigned int yyBone::getPositionIndexByTime(float time)
{
    for (unsigned int i = 0; i < keyPositionArr_.size() - 1; ++i) {
        if (time < keyPositionArr_[i + 1].timestamp) {
            return i;
        }
    }
    return keyPositionArr_.size() - 1;
}

unsigned int yyBone::getRotationIndexByTime(float time)
{
    for (unsigned int i = 0; i < keyRotationArr_.size() - 1; ++i) {
        if (time < keyRotationArr_[i + 1].timestamp) {
            return i;
        }
    }
    return keyRotationArr_.size() - 1;
}

unsigned int yyBone::getScaleIndexByTime(float time)
{
    for (unsigned int i = 0; i < keyScaleArr_.size() - 1; ++i) {
        if (time < keyScaleArr_[i + 1].timestamp) {
            return i;
        }
    }
    return keyScaleArr_.size() - 1;
}

glm::mat4 yyBone::interpolatePosition(float time)
{
    int keyNum = keyPositionArr_.size();
    if (!keyNum) {
		return glm::mat4(1.0f);
    }

    // 只有一个关键帧，直接返回该帧
    if (1 == keyNum) {
        return glm::translate(glm::mat4(1.0f), keyPositionArr_[0].position);
    }

    // 找到当前时间的上一帧与下一帧
    unsigned int lastIndex = getPositionIndexByTime(time);
    unsigned int nextIndex = (lastIndex + 1) % keyNum;

    // 插值
    float k = calLerpFactor(keyPositionArr_[lastIndex].timestamp, keyPositionArr_[nextIndex].timestamp, time);
    glm::vec3 result = glm::mix(keyPositionArr_[lastIndex].position, keyPositionArr_[nextIndex].position, k);

    return glm::translate(glm::mat4(1.0f), result);
}

glm::mat4 yyBone::interpolateScale(float time)
{
    int keyNum = keyScaleArr_.size();
    if (!keyNum) {
		return glm::mat4(1.0f);
    }

    // 只有一个关键帧，直接返回该帧
    if (1 == keyNum) {
        return glm::scale(glm::mat4(1.0f), keyScaleArr_[0].scale);
    }

    // 找到当前时间的上一帧与下一帧
    unsigned int lastIndex = getScaleIndexByTime(time);
    unsigned int nextIndex = (lastIndex + 1) % keyNum;

    // 插值
    float k = calLerpFactor(keyScaleArr_[lastIndex].timestamp, keyScaleArr_[nextIndex].timestamp, time);
    glm::vec3 result = glm::mix(keyScaleArr_[lastIndex].scale, keyScaleArr_[nextIndex].scale, k);

    return glm::scale(glm::mat4(1.0f), result);
}

glm::mat4 yyBone::interpolateRotation(float time)
{
    int keyNum = keyRotationArr_.size();
    if (!keyNum) {
		return glm::mat4(1.0f);
    }

    // 只有一个关键帧，直接返回该帧
    if (1 == keyNum) {
        return glm::toMat4(keyRotationArr_[0].quaternion);
    }

    // 找到当前时间的上一帧与下一帧
    unsigned int lastIndex = getRotationIndexByTime(time);
    unsigned int nextIndex = (lastIndex + 1) % keyNum;

    // 插值
    float k = calLerpFactor(keyRotationArr_[lastIndex].timestamp, keyRotationArr_[nextIndex].timestamp, time);
    glm::quat result = glm::slerp(keyRotationArr_[lastIndex].quaternion, keyRotationArr_[nextIndex].quaternion, k);

    return glm::toMat4(result);
}

void yyBone::update(float time)
{
    glm::mat4 rotation    = interpolateRotation(time);
    glm::mat4 scale       = interpolateScale(time);
    glm::mat4 translation = interpolatePosition(time);

    localTransform_ = translation * scale * rotation;
}
