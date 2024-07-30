#pragma once

class GCTexture;

class GCFrame
{
public:
	GCFrame();
	GCFrame( int frameID, float displayTime, bool isFlipingX, bool isFlipingY);
	~GCFrame() = default;

	float GetDisplayTime() const;
	bool IsFlipingX() const;
	bool IsFlipingY() const;

	int GetID() { return m_frameID; }

private:

	float m_displayTime;
	bool m_isFlipingX;
	bool m_isFlipingY;

	int m_frameID;
};