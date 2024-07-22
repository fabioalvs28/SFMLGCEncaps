#pragma once

class GCFrame
{
public:
	GCFrame();
	GCFrame(int x, int y, int width, int height, float displayTime, bool isFlipingX, bool isFlipingY);
	~GCFrame() = default;

	int GetX() const;
	int GetY() const;
	int GetWidth() const;
	int GetHeight() const;
	float GetDisplayTime() const;
	bool IsFlipingX() const;
	bool IsFlipingY() const;


private:
	int m_x;
	int m_y;
	int m_width;
	int m_height;
	float m_displayTime;
	bool m_isFlipingX;
	bool m_isFlipingY;
};