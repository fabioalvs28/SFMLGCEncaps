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

void GCParticleSystem::InitializeRandom(int maxParticles, bool isLooping, GCMesh* pMesh)
{
	m_Particles.resize(maxParticles);

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
        particle.lifetime = 3.0f;
        particle.age = 0.0f;

        m_pMesh->AddGeometry(particle.position);
	}
    m_duration = 5.0f;
    m_isLooping = isLooping;
}

void GCParticleSystem::UpdateParticleSystem(float deltaTime)
{
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
                m_Particles[i].position = { static_cast<float>(rand()) / RAND_MAX * 2.0f - 1.0f,
                                            static_cast<float>(rand()) / RAND_MAX * 2.0f - 1.0f,
                                            static_cast<float>(rand()) / RAND_MAX * 2.0f - 1.0f };

                m_Particles[i].velocity = { static_cast<float>(rand()) / RAND_MAX * 2.0f - 1.0f,
                                            static_cast<float>(rand()) / RAND_MAX * 2.0f - 1.0f,
                                            static_cast<float>(rand()) / RAND_MAX * 2.0f - 1.0f };

                m_Particles[i].lifetime = 3.0f;

                m_Particles[i].age = 0.0f;
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