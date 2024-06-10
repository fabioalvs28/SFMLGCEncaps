#include "pch.h"
#include "GCVectors.h"





GCVEC2::GCVEC2()
{
    x = 0.0f;
    y = 0.0f;
}

GCVEC2::GCVEC2( float x, float y )
{
    this->x = x;
    this->y = y;
}



void GCVEC2::SetZero()
{
    x = 0.0f;
    y = 0.0f;
}

void GCVEC2::SetOne()
{
    x = 1.0f;
    y = 1.0f;
}

void GCVEC2::SetUp()
{
    x = 0.0f;
    y = 1.0f;
}

void GCVEC2::SetDown()
{
    x = 0.0f;
    y = -1.0f;
}

void GCVEC2::SetLeft()
 {
    x = -1.0f;
    y = 0.0f;
}

void GCVEC2::SetRight()
 {
    x = 1.0f;
    y = 0.0f;
}



void GCVEC2::Lerp( const GCVEC2& from, const GCVEC2& to, const float& t )
{
    x = from.x + ( to.x - from.x ) * t;
    y = from.y + ( to.y - from.y ) * t;
}

void GCVEC2::Clamp( const GCVEC2& min, const GCVEC2& max )
{
    if ( min > max ) return;
}

void GCVEC2::Normalize()
{
    float norm = GetNorm();
    x /= norm;
    y /= norm;
}



GCVEC2 GCVEC2::operator+( const GCVEC2& other ) const
{
    GCVEC2 result = *this;
    result += other;
    return result;
}

GCVEC2 GCVEC2::operator-( const GCVEC2& other ) const
{
    GCVEC2 result = *this;
    result -= other;
    return result;
}

GCVEC2 GCVEC2::operator*( const GCVEC2& other ) const
{
    GCVEC2 result = *this;
    result *= other;
    return result;
}

GCVEC2 GCVEC2::operator/( const GCVEC2& other ) const
{
    GCVEC2 result = *this;
    result /= other;
    return result;
}



GCVEC2 GCVEC2::operator*( const float& value ) const
{
    GCVEC2 result = *this;
    result *= value;
    return result;
}

GCVEC2 GCVEC2::operator/( const float& value ) const
{
    GCVEC2 result = *this;
    result /= value;
    return result;
}



void GCVEC2::operator+=( const GCVEC2& other )
{
    x += other.x;
    y += other.y;
}

void GCVEC2::operator-=( const GCVEC2& other )
{
    x -= other.x;
    y -= other.y;
}

void GCVEC2::operator*=( const GCVEC2& other )
{
    x *= other.x;
    y *= other.y;
}

void GCVEC2::operator/=( const GCVEC2& other ) //! Division by zero
{
    x /= other.x;
    y /= other.y;
}



void GCVEC2::operator*=( const float& value )
{
    x *= value;
    y *= value;
}

void GCVEC2::operator/=( const float& value ) //! Division by zero
{
    x /= value;
    y /= value;
}










GCVEC3::GCVEC3()
{
    x = 0.0f;
    y = 0.0f;
    z = 0.0f;
}

GCVEC3::GCVEC3( float x, float y, float z )
{
    this->x = x;
    this->y = y;
    this->z = z;
}



void GCVEC3::SetZero()
{
    x = 0.0f;
    y = 0.0f;
    z = 0.0f;
}

void GCVEC3::SetOne()
{
    x = 1.0f;
    y = 1.0f;
    z = 1.0f;
}

void GCVEC3::SetUp()
{
    x = 0.0f;
    y = 1.0f;
    z = 0.0f;
}

void GCVEC3::SetDown()
{
    x = 0.0f;
    y = -1.0f;
    z = 0.0f;
}

void GCVEC3::SetLeft()
 {
    x = -1.0f;
    y = 0.0f;
    z = 0.0f;
}

void GCVEC3::SetRight()
 {
    x = 1.0f;
    y = 0.0f;
    z = 0.0f;
}

void GCVEC3::SetForward()
{
    x = 0.0f;
    y = 0.0f;
    z = 1.0f;
}

void GCVEC3::SetBack()
{
    x = 0.0f;
    y = 0.0f;
    z = -1.0f;
}



void GCVEC3::Lerp( const GCVEC3& from, const GCVEC3& to, const float& t )
{
    x = from.x + ( to.x - from.x ) * t;
    y = from.y + ( to.y - from.y ) * t;
    z = from.z + ( to.z - from.z ) * t;
}

void GCVEC3::Clamp( const GCVEC3& min, const GCVEC3& max )
{
    if ( min > max ) return;
}

void GCVEC3::Normalize()
{
    float norm = GetNorm();
    x /= norm;
    y /= norm;
    z /= norm;
}



GCVEC3 GCVEC3::operator+( const GCVEC3& other ) const
{
    GCVEC3 result = *this;
    result += other;
    return result;
}

GCVEC3 GCVEC3::operator-( const GCVEC3& other ) const
{
    GCVEC3 result = *this;
    result -= other;
    return result;
}

GCVEC3 GCVEC3::operator*( const GCVEC3& other ) const
{
    GCVEC3 result = *this;
    result *= other;
    return result;
}

GCVEC3 GCVEC3::operator/( const GCVEC3& other ) const
{
    GCVEC3 result = *this;
    result /= other;
    return result;
}



GCVEC3 GCVEC3::operator*( const float& value ) const
{
    GCVEC3 result = *this;
    result *= value;
    return result;
}

GCVEC3 GCVEC3::operator/( const float& value ) const
{
    GCVEC3 result = *this;
    result /= value;
    return result;
}



void GCVEC3::operator+=( const GCVEC3& other )
{
    x += other.x;
    y += other.y;
    z += other.z;
}

void GCVEC3::operator-=( const GCVEC3& other )
{
    x -= other.x;
    y -= other.y;
    z -= other.z;
}

void GCVEC3::operator*=( const GCVEC3& other )
{
    x *= other.x;
    y *= other.y;
    z *= other.z;
}

void GCVEC3::operator/=( const GCVEC3& other ) //! Division by zero
{
    x /= other.x;
    y /= other.y;
    z /= other.z;
}



void GCVEC3::operator*=( const float& value )
{
    x *= value;
    y *= value;
    z *= value;
}

void GCVEC3::operator/=( const float& value ) //! Division by zero
{
    x /= value;
    y /= value;
    z /= value;
}
