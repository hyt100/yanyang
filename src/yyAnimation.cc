#include "yyAnimation.h"
#include <cmath>

yyAnimation::yyAnimation()
{
    boneMatArr_.reserve(yyMAX_BONE_NUM);
    for (int i = 0; i < yyMAX_BONE_NUM; ++i) {
        boneMatArr_.push_back(glm::mat4(1.0f));
    }
}

void yyAnimation::play()
{
    curTime_ = 0;
}

void yyAnimation::update(float deltaTime)
{
    // std::cout << "update animation ..." << std::endl;
    curTime_ += ticksPerSecond_ * deltaTime;
    curTime_ = std::fmod(curTime_, duration_);

    glm::mat4 rootTransfrom(1.0f);
    calBoneMat(rootNode_, rootTransfrom);
}

void yyAnimation::calBoneMat(yyAnimationNode &node, glm::mat4 &parentTransform)
{
    glm::mat4 transform;

    // 如果存在骨骼，则transform采用骨骼的transform；否则，使用Node的transform
    if (boneMap_.find(node.name) != boneMap_.end()) {
        auto& pBone = boneMap_[node.name];
        pBone->update(curTime_); // 插值更新

        transform = parentTransform * pBone->getLocalTransform();
        // transform = parentTransform * node.transform;

        // std::cout << ">>> " << std::endl;
        // std::cout << glm::to_string(pBone->getLocalTransform()) << std::endl;
        // std::cout << glm::to_string(node.transform) << std::endl;

        if (pBone->id_ < yyMAX_BONE_NUM) {
            boneMatArr_[pBone->id_] = transform * pBone->offsetMatrix_;  // 有骨骼的话才需要输出boneMat
            // std::cout << glm::to_string(boneMatArr_[pBone->id_]) << std::endl;
        }

    } else {
        transform = parentTransform * node.transform;
    }

    for (int i = 0; i < node.children.size(); ++i) {
        calBoneMat(node.children[i], transform);
    }
}