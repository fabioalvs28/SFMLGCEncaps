#include "pch.h"
#include "GCVector.h"

GCVector2::GCVector2( float x = 0.0f, float y = 0.0f )
{
    this->x = x;
    this->y = y;
}



void GCVector2::SetZero()
{
    x = 0.0f;
    y = 0.0f;
}



void GCVector2::Normalize()
{
    float norm = GetNorm();
    x /= norm;
    y /= norm;
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



GCVector2 GCVector2::operator+( const float& value ) const
{
    GCVector2 result = *this;
    result += value;
    return result;
}

GCVector2 GCVector2::operator-( const float& value ) const
{
    GCVector2 result = *this;
    result -= value;
    return result;
}

GCVector2 GCVector2::operator*( const float& value ) const
{
    GCVector2 result = *this;
    result *= value;
    return result;
}

GCVector2 GCVector2::operator/( const float& value ) const
{
    GCVector2 result = *this;
    result /= value;
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

void GCVector2::operator/=( const GCVector2& other ) //! Division by zero
{
    x /= other.x;
    y /= other.y;
}



void GCVector2::operator+=( const float& value )
{
    x += value;
    y += value;
}

void GCVector2::operator-=( const float& value )
{
    x -= value;
    y -= value;
}

void GCVector2::operator*=( const float& value )
{
    x *= value;
    y *= value;
}

void GCVector2::operator/=( const float& value ) //! Division by zero
{
    x /= value;
    y /= value;
}



void GCVector2::operator++()
{
    // this += 1;
    x++;
    y++;
}

void GCVector2::operator--()
{
    // this -= 1;
    x--;
    y--;
}