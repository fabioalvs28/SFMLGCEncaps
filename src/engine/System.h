#pragma once

/// <summary>
/// Base class for any other system
/// </summary>
class GCSystem {
public:
	virtual ~GCSystem() = default;

	virtual void Init();

	virtual void Update(float deltaTime) = 0;
	
	virtual void CleanUp();
};
