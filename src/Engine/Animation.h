#pragma once
#include "../core/framework.h"

struct GCGeometry;

class Animation
{
public:
	Animation();
	~Animation() = default;

	void AddFrame( int frameID, float displayTime = 2.0f, bool isFlipingX = false, bool isFlipingY = false);
	void StartAnimation();
	bool Update( int* currentFrameIndex, float* currentFrameTime  );
	void IncrementFrame( int* currentFrameIndex );
	const GCFrame* GetCurrentFrame( int currentFrameIndex ) const;
	int GetNumberOfFrames() const { return static_cast<int>(m_pFrames.size()); }
	GCGeometry* GetGeometry() { return m_pGeometry; }
	GCTexture* GetTexture() { return m_pTexture; }

	int GetLastFrameIndex() { return m_pFrames.size() - 1; }

	void SetSpriteSheet(std::string fileName, int spriteSheetID );

private:

	int m_spriteSheetID;
	GCGeometry* m_pGeometry; 
	GCTexture* m_pTexture;

	std::vector<GCFrame*> m_pFrames;

};