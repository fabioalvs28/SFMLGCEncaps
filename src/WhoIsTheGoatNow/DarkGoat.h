#pragma once

CREATE_SCRIPT_INHERIT_START(DarkGoat, EnemyBehaviour)

void Start() override;
void Update() override;
void SetAnimationWalk(int animation);

public:
    void SetAnimator(GCAnimator* pAnimator) { m_pAnimator = pAnimator; }

protected:
    bool m_spawning;

    std::vector<std::string> m_animationList;
    GCAnimator* m_pAnimator;
    GCVEC3 m_lastDirection;
    int m_lastAnimation;
    enum DarkGoatAnimation
    {
        Forward, Backward, Left, Right, AnimSpawn, count
    };
    CREATE_SCRIPT_END