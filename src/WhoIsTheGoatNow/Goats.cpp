#include "pch.h"
#include "Goats.h"

#pragma region DARKGOAT

void GCScriptDarkGoat::CopyTo(GCComponent* pDestination)
{
    GCComponent::CopyTo(pDestination);
    GCScriptDarkGoat* pNewComponent = static_cast<GCScriptDarkGoat*>(pDestination);
    pNewComponent->m_pTarget = m_pTarget;
    pNewComponent->m_pSummonedGoat = m_pSummonedGoat;
}

void GCScriptDarkGoat::Start()
{
    m_pAnimator = m_pGameObject->GetComponent<GCAnimator>();
    m_hp = 1;
    m_speed = 0.03f;
    m_spawning = false;
    m_summonCount = 0.0f;
    m_summonfrequency = 15.0f;
    m_summoningAmmount = 2;
    m_lastAnimation = -1;
    m_animationList = { "DarkGoatForward","DarkGoatBackWard","DarkGoatLeft","DarkGoatRight","DarkGoatSummon" };
    Spawn();
}

void GCScriptDarkGoat::Update()
{
    GCVEC3 delta = m_direction - m_lastDirection;
    GCVEC2 side = GCVEC2(1, 1);

    m_summonCount += GC::GetActiveTimer()->FixedDeltaTime();
    if (m_summonCount >= m_summonfrequency && m_summoning == false)
        Summon();

    if (m_direction.x < 0)
        side.x = -1;
    if (m_direction.y < 0)
        side.y = -1;

    if (m_summoning == true)
    {
        if (m_pAnimator->AnimationHasEnded())
        {
            m_summoning = false;
            m_summonCount = 0.0f;
            m_spawning = false;
        }
    }
    else {
        if (m_direction.x * side.x > m_direction.y * side.y)
        {
            if (m_direction.x > 0)
                SetAnimationWalk(Right);
            if (m_direction.x < 0)
                SetAnimationWalk(Left);
        }

        if (m_direction.y * side.y > m_direction.x * side.x)
        {
            if (m_direction.y > 0)
                SetAnimationWalk(Backward);
            if (m_direction.y < 0)
                SetAnimationWalk(Forward);
        }
    }
}


void GCScriptDarkGoat::SetAnimationWalk(int animation)
{
    if (animation != m_lastAnimation)
    {
        m_pAnimator->PlayAnimation(m_animationList[animation], true);
        m_lastAnimation = animation;
    }
}

void GCScriptDarkGoat::Summon()
{
    m_summoning = true;
    m_spawning = true;
    m_pAnimator->PlayAnimation(m_animationList[AnimSummon], false);
    for (int i = 0; i < m_summoningAmmount; i++)
    {
        GCGameObject* newEnemy = m_pSummonedGoat->Duplicate();
        newEnemy->Activate();
        GCVEC3 summonerPos = m_pGameObject->m_transform.m_position;
        newEnemy->GetComponent<GCScriptDumbGoat>()->m_spawning = true;
        newEnemy->m_transform.SetPosition(summonerPos);
        newEnemy->GetComponent<GCAnimator>()->PlayAnimation("DumbGoatSpawn", false);
    }

}


#pragma endregion


#pragma region DUMBGOAT

void GCScriptDumbGoat::CopyTo(GCComponent* pDestination)
{
    GCComponent::CopyTo(pDestination);
    GCScriptDumbGoat* pNewComponent = static_cast<GCScriptDumbGoat*>(pDestination);
    pNewComponent->m_pTarget = m_pTarget;
}

void GCScriptDumbGoat::Start()
{
    m_pAnimator = m_pGameObject->GetComponent<GCAnimator>();
    m_hp = 1;
    m_speed = 0.03f;
    m_spawning = false;
    m_lastAnimation = -1;
    m_animationList = { "DumbGoatForward","DumbGoatBackWard","DumbGoatLeft","DumbGoatRight","DumbGoatSpawn" };
    Spawn();
}

void GCScriptDumbGoat::Update()
{
    GCVEC3 delta = m_direction - m_lastDirection;
    GCVEC2 side = GCVEC2(1, 1);

    if (m_direction.x < 0)
        side.x = -1;
    if (m_direction.y < 0)
        side.y = -1;

    if (m_spawning == true)
    {
        if ( m_pAnimator->AnimationHasEnded() )
            m_spawning = false;
    }
    else
    {
        if (m_direction.x * side.x > m_direction.y * side.y)
        {
            if (m_direction.x > 0)
                SetAnimationWalk(Right);
            if (m_direction.x < 0)
                SetAnimationWalk(Left);
        }

        if (m_direction.y * side.y > m_direction.x * side.x)
        {
            if (m_direction.y > 0)
                SetAnimationWalk(Backward);
            if (m_direction.y < 0)
                SetAnimationWalk(Forward);
        }
    }

}

void GCScriptDumbGoat::SetAnimationWalk(int animation)
{
    if (animation != m_lastAnimation)
    {
        m_pAnimator->PlayAnimation(m_animationList[animation], true);
        m_lastAnimation = animation;
    }
}

#pragma endregion