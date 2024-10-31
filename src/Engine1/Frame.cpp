#include "pch.h"

GCFrame::GCFrame()
	: m_displayTime( 0.0f ), 
	m_isFlippingX( false ), 
	m_isFlippingY( false ), 
	m_frameID( 0 )
{}

GCFrame::GCFrame( int frameID, float displayTime, bool isFlipingX, bool isFlipingY )
{
	m_frameID = frameID;
	m_displayTime = displayTime;
	m_isFlippingX = isFlipingX;
	m_isFlippingY = isFlipingY;
}

float GCFrame::GetDisplayTime() const
{ return m_displayTime; }

bool GCFrame::IsFlippingX() const
{ return m_isFlippingX; }

bool GCFrame::IsFlippingY() const
{ return m_isFlippingY; }

int GCFrame::GetFrameID() const
{ return m_frameID; }