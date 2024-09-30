#pragma once

CREATE_SCRIPT_INHERIT_START(DarkGoat, EnemyBehaviour)

void Start() override;
void Update() override;
void SetAnimationWalk(int animation);
void Summon();

public:
void SetSummonedEnemy(GCGameObject* pGoat) { m_pSummonedGoat = pGoat; }
    //void SetAnimator(GCAnimator* pAnimator) { m_pAnimator = pAnimator; }

protected:
    bool m_summoning;
    float m_summonCount;
    float m_summonfrequency;
    int m_summoningAmmount;
    std::vector<std::string> m_animationList;
    GCAnimator* m_pAnimator;
    GCVEC3 m_lastDirection;
    int m_lastAnimation;
    GCGameObject* m_pSummonedGoat;

    enum DarkGoatAnimation
    {
        Forward, Backward, Left, Right, AnimSummon, count
    };

    CREATE_SCRIPT_END



CREATE_SCRIPT_INHERIT_START(DumbGoat, EnemyBehaviour)

friend class GCScriptDarkGoat;

void Start() override;
void Update() override;
void SetAnimationWalk(int animation);
void Summon();

public:
    //void SetAnimator(GCAnimator* pAnimator) { m_pAnimator = pAnimator; }

protected:
    bool m_spawning;

    std::vector<std::string> m_animationList;
    GCAnimator* m_pAnimator;
    GCVEC3 m_lastDirection;
    int m_lastAnimation;
    GCGameObject* m_pSummonedGoat;

    enum DumbGoatAnimation
    {
        Forward, Backward, Left, Right, count
    };

CREATE_SCRIPT_END