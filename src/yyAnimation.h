#pragma once
#include "yyBone.h"

struct yyAnimationNode
{
    std::string                  name;
    glm::mat4	                 transform;
    std::vector<yyAnimationNode> children;

    yyAnimationNode(): name(""), transform(1.0f)
    {
        children.clear();
    }
};

class yyAnimation
{
public:
    using Ptr = std::shared_ptr<yyAnimation>;
    static Ptr create()
    {
        return Ptr(new yyAnimation());
    }
    void play();
    void update(float deltaTime);
    std::vector<glm::mat4>& getBoneMatArr() { return boneMatArr_; }

private:
    yyAnimation();
    void calBoneMat(yyAnimationNode &node, glm::mat4 &parentTransform);

public:
    float duration_;    // in ticks
    float ticksPerSecond_;
    unsigned int boneCounter_;
    std::map<std::string, yyBone::Ptr> boneMap_;
    yyAnimationNode rootNode_;
    
private:
    float curTime_ = 0;
    std::vector<glm::mat4> boneMatArr_;
};

