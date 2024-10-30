#pragma once

class GCTexture;

class GCFrame
{
public:
	GCFrame();
	GCFrame( int frameID, float displayTime, bool isFlipingX, bool isFlipingY );
	~GCFrame() = default;

	float GetDisplayTime() const;
	bool IsFlippingX() const;
	bool IsFlippingY() const;

	int GetFrameID() const;

private:

	float m_displayTime;
	bool m_isFlippingX;
	bool m_isFlippingY;

	int m_frameID;
};