#include "pch.h"
#include "GCVector.h"

GCVector2::GCVector2( float x = 0.0f, float y = 0.0f )
{
    this->x = x;
    this->y = y;
}



void GCVector2::Zero()
{
    x = 0.0f;
    y = 0.0f;
}



GCVector2 GCVector2::operator+( const GCVector2& other ) const
{
    GCVector2 result = *this;
    result += other;
    return result;
}

GCVector2 GCVector2::operator-( const GCVector2& other ) const
{
    GCVector2 result = *this;
    result -= other;
    return result;
}

GCVector2 GCVector2::operator*( const GCVector2& other ) const
{
    GCVector2 result = *this;
    result *= other;
    return result;
}

GCVector2 GCVector2::operator/( const GCVector2& other ) const
{
    GCVector2 result = *this;
    result /= other;
    return result;
}



void GCVector2::operator+=( const GCVector2& other )
{
    x += other.x;
    y += other.y;
}

void GCVector2::operator-=( const GCVector2& other )
{
    x -= other.x;
    y -= other.y;
}

void GCVector2::operator*=( const GCVector2& other )
{
    x *= other.x;
    y *= other.y;
}

void GCVector2::operator/=( const GCVector2& other )
{
    x /= other.x;
    y /= other.y;
}



