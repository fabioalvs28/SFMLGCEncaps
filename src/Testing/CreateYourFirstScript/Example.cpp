#include "pch.h"
#include "Example.h"

void ScriptExample::ResetPosition()
{
    // Moves the GameObject to the top-left of the screen
    m_pGameObject->m_transform.SetPosition(GCVEC3(-960.0f, 540.0f, 0.0f));
}

void ScriptExample::PassTime()
{
    // Moves the GameObject 0.5 units to the right each frame
    m_pGameObject->m_transform.Translate(GCVEC3(0.5f, 0.0f, 0.0f));
    
    // If the GameObject has passed the Screen's limits, reset the position and change the time
    if (m_pGameObject->m_transform.m_position.x >= 960.0f)
    {
        ResetPosition();
        ChangeTime();
    }
}

void ScriptExample::ChangeTime()
{
    // If it was night time, change the Sprite to the sun, turn the night status off and add 1 to the day counter
    if (m_night)
    {
        m_pSpriteRenderer->SetSprite("Sun.dds");
        m_night = false;
        m_days++;
    }
    // If it was day time, change the Sprite to the moon and turn the night status on
    else
    {
        m_pSpriteRenderer->SetSprite("Moon.dds");
        m_night = true;
    }
}



void ScriptExample::Start()
{
    // Set the initial position and Sprite, and initialize the day counter and night status
    ResetPosition();
    m_pSpriteRenderer = m_pGameObject->GetComponent<SpriteRenderer>();
    m_pSpriteRenderer->SetSprite("Sun.dds");
    m_night = false;
    m_days = 0;
}

void ScriptExample::Update()
{
    // Call the PassTime function every frame to move the GameObject and change the time
    PassTime();
}



// A necessary method allowing to duplicate the Script when trying to duplicate a GameObject
void ScriptExample::CopyTo( Component* pDestination )
{
    Component::CopyTo( pDestination );
    ScriptExample* pScript = static_cast<ScriptExample*>( pDestination );
    pScript->m_night = m_night;
    pScript->m_days = m_days;
}