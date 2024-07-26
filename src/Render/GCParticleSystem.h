#pragma once
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

	void InitializeRandom(int maxParticle, bool isLooping, GCMesh* pMesh);
	void UpdateParticleSystem(float deltaTime);

private :
	std::vector<GCPARTICLE> m_Particles;

	GCMesh* m_pMesh;
	float m_duration;
	bool m_isLooping;
	GCPARTICLE* m_pTemplateParticle;
};