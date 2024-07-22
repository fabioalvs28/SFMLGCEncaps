#include "pch.h"
#include "Frame.h"

GCFrame::GCFrame()
	: m_x(0), m_y(0), m_width(0), m_height(0), m_displayTime(0.0f), m_isFlipingX(false), m_isFlipingY(false) 
{}

GCFrame::GCFrame(int x, int y, int width, int height, float displayTime, bool isFlipingX, bool isFlipingY)
{
	m_x = x;
	m_y = y;
	m_width = width;
	m_height = height;
	m_displayTime = displayTime;
	m_isFlipingX = isFlipingX;
	m_isFlipingY = isFlipingY;
}

int GCFrame::GetX() const
{
	return m_x;
}

int GCFrame::GetY() const
{
	return m_y;
}

int GCFrame::GetWidth() const
{
	return m_width;
}

int GCFrame::GetHeight() const
{
	return m_height;
}

float GCFrame::GetDisplayTime() const
{
	return m_displayTime;
}

bool GCFrame::IsFlipingX() const
{
	return m_isFlipingX;
}

bool GCFrame::IsFlipingY() const
{
	return m_isFlipingY;
}
