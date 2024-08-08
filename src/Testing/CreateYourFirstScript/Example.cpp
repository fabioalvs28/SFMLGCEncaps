#include "pch.h"
#include "Example.h"

void GCScriptExample::ResetPosition()
{
    // Moves the GameObject to the top-left of the screen
    m_pGameObject->m_transform.SetPosition(GCVEC3(-960.0f, 540.0f, 0.0f));
}

void GCScriptExample::PassTime()
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

void GCScriptExample::ChangeTime()
{
    // If it was night time, change the Sprite to the sun, turn the night status off and add 1 to the day counter
    if (m_night)
    {
        // m_pSpriteRenderer->SetSprite("Sun.dds");
        m_night = false;
        m_days++;
    }
    // If it was day time, change the Sprite to the moon and turn the night status on
    else
    {
        // m_pSpriteRenderer->SetSprite("Moon.dds");
        m_night = true;
    }
}



void GCScriptExample::Start()
{
    // Set the initial position and Sprite, and initialize the day counter and night status
    ResetPosition();
    m_pSpriteRenderer = m_pGameObject->GetComponent<GCSpriteRenderer>();
    // m_pSpriteRenderer->SetSprite("Sun.dds");
    m_night = false;
    m_days = 0;
}

void GCScriptExample::Update()
{
    // Call the PassTime function every frame to move the GameObject and change the time
    PassTime();
}



// A necessary method allowing to duplicate the Script when trying to duplicate a GameObject
void GCScriptExample::CopyTo( GCComponent* pDestination )
{
    GCComponent::CopyTo( pDestination );
    GCScriptExample* pScript = static_cast<GCScriptExample*>( pDestination );
    pScript->m_night = m_night;
    pScript->m_days = m_days;
}