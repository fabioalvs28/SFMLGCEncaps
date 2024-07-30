#include "pch.h"

GCFrame::GCFrame()
	: m_displayTime(0.0f), m_isFlipingX(false), m_isFlipingY(false) 
{}

GCFrame::GCFrame( int frameID, float displayTime, bool isFlipingX, bool isFlipingY)
{
	m_frameID = frameID;
	m_displayTime = displayTime;
	m_isFlipingX = isFlipingX;
	m_isFlipingY = isFlipingY;
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
