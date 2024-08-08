#pragma once
enum ParticleSystemType {
	PS_Random,
	PS_Circle,
};
struct GCPARTICLE 
{
	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT3 velocity;

	float lifetime;
	float age;
};

class GCParticleSystem {
public :
	GCParticleSystem();
	~GCParticleSystem();

	void InitializeRandom(size_t maxParticle, bool isLooping, GCMesh* pMesh);
	void InitializeCircle(size_t maxParticles, bool isLooping, float speed, GCMesh* pMesh);
	void UpdateParticleSystem(float deltaTime);

	std::vector<GCPARTICLE> GetParticles() { return m_Particles; }
	float GetDuration() { return m_duration; }
private :
	std::vector<GCPARTICLE> m_Particles;

	GCMesh* m_pMesh;
	float m_duration;
	float m_speed;
	bool m_isLooping;
	size_t m_maxParticles;
	GCPARTICLE* m_pTemplateParticle;
	ParticleSystemType m_type;
};