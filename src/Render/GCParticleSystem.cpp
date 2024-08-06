#include "pch.h"

GCParticleSystem::GCParticleSystem()
    : m_duration(0),
    m_isLooping(false),
    m_pMesh(nullptr),
    m_pTemplateParticle(nullptr)
{
	m_Particles.clear();
}

GCParticleSystem::~GCParticleSystem()
{
}

void GCParticleSystem::InitializeRandom(size_t maxParticles, bool isLooping, GCMesh* pMesh)
{
	m_Particles.resize(maxParticles);

    m_type = PS_Random;

    m_pMesh = pMesh;
	for (GCPARTICLE& particle : m_Particles)
	{
        // Give each particle a random position and velocity for better visualization
        particle.position = { static_cast<float>(rand()) / RAND_MAX * 2.0f - 1.0f,
                              static_cast<float>(rand()) / RAND_MAX * 2.0f - 1.0f,
                              static_cast<float>(rand()) / RAND_MAX * 2.0f - 1.0f };

        particle.velocity = { static_cast<float>(rand()) / RAND_MAX * 2.0f - 1.0f,
                              static_cast<float>(rand()) / RAND_MAX * 2.0f - 1.0f,
                              static_cast<float>(rand()) / RAND_MAX * 2.0f - 1.0f };
        particle.lifetime = static_cast<float>(rand()) / RAND_MAX * 3.0f;
        particle.age = static_cast<float>(rand()) / RAND_MAX * -2.0f;

        m_pMesh->AddGeometry(particle.position);
	}
    m_duration = 5.0f;
    m_isLooping = isLooping;
    m_pMesh->DeleteGeometryAt(0);
}

void GCParticleSystem::InitializeCircle(size_t maxParticles, bool isLooping, float speed, GCMesh* pMesh)
{
    m_Particles.resize(maxParticles);
    m_maxParticles = maxParticles;
    m_type = PS_Circle;

    m_pMesh = pMesh;
    m_speed = speed;
    float radius = 1.0f; // Define the radius of the circle

    float randomLifetime = static_cast<float>(rand()) / RAND_MAX * 10.0f + 5.0f;

    for (int i = 0; i < maxParticles; ++i)
    {
        GCPARTICLE& particle = m_Particles[i];

        // Calculate the angle for the current particle
        float angle = static_cast<float>(i) / maxParticles * 2.0f * DirectX::XM_PI;

        // Set the position on the circumference of the circle
        particle.position = { radius * cos(angle), radius * sin(angle), 0.0f };

        // Set the velocity to point outward from the center
        particle.velocity = { cos(angle) * m_speed, sin(angle) * m_speed, 0.0f };

        particle.lifetime = randomLifetime;
        particle.age = 0.0f;

        m_pMesh->AddGeometry(particle.position);
    }
    m_duration = randomLifetime;
    m_isLooping = isLooping;
    m_pMesh->DeleteGeometryAt(0);
}

void GCParticleSystem::UpdateParticleSystem(float deltaTime)
{
    float radius = 1.0f;
    float randomLifetime = static_cast<float>(rand()) / RAND_MAX * 10.0f + 5.0f;
    m_duration = randomLifetime;
    for (int i =0; i<m_Particles.size(); i++)
    {
        if (m_Particles[i].age < m_Particles[i].lifetime)
        {
            // Update particle position based on velocity
            m_Particles[i].position.x += m_Particles[i].velocity.x * deltaTime;
            m_Particles[i].position.y += m_Particles[i].velocity.y * deltaTime;
            m_Particles[i].position.z += m_Particles[i].velocity.z * deltaTime;

            // Update age of the particle
            m_Particles[i].age += deltaTime;

        }
        else
        {
            if (m_isLooping && m_duration > 0.0f)
            {
                switch (m_type)
                {
                case PS_Random :
                    m_Particles[i].position = { static_cast<float>(rand()) / RAND_MAX * 2.0f - 1.0f,
                                                static_cast<float>(rand()) / RAND_MAX * 2.0f - 1.0f,
                                                static_cast<float>(rand()) / RAND_MAX * 2.0f - 1.0f };

                    m_Particles[i].velocity = { static_cast<float>(rand()) / RAND_MAX * 2.0f - 1.0f,
                                                static_cast<float>(rand()) / RAND_MAX * 2.0f - 1.0f,
                                                static_cast<float>(rand()) / RAND_MAX * 2.0f - 1.0f };

                    m_Particles[i].lifetime = static_cast<float>(rand()) / RAND_MAX * 3.0f;

                    m_Particles[i].age = static_cast<float>(rand()) / RAND_MAX * -2.0f;

                    break;

                case PS_Circle :
                    // Calculate the angle for the current particle
                    float angle = static_cast<float>(i) / m_maxParticles *2.0f * DirectX::XM_PI;

                    // Set the position on the circumference of the circle
                    m_Particles[i].position = { radius * cos(angle), radius * sin(angle), 0.0f };

                    // Set the velocity to point outward from the center
                    m_Particles[i].velocity = { cos(angle) * m_speed, sin(angle) * m_speed, 0.0f };

                    m_Particles[i].lifetime = randomLifetime;
                    m_Particles[i].age = 0.0f;

                    break;
                }
            }
            else
            {
                m_pMesh->DeleteGeometryAt(i);
                m_Particles.erase(m_Particles.begin() + i);
            }

        }
        m_pMesh->EditGeometryPositionAt(i, m_Particles[i].position);
    }
    m_pMesh->UpdateGeometryData(deltaTime);
}