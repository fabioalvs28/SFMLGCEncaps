#pragma once
#include "math.h"

class GCVector2
{
public:
    float x, y;
    
    GCVector2( float x, float y );
    virtual ~GCVector2();

    static GCVector2 Zero() { return GCVector2( 0.0f, 0.0f ); };
    static GCVector2 One() { return GCVector2( 1.0f, 1.0f ); };
    void SetZero();
    
    float GetNorm() const { return sqrtf( x * x + y * y ); };
    
    void Normalize();
    
    GCVector2 operator+( const GCVector2& other ) const;
    GCVector2 operator-( const GCVector2& other ) const;
    GCVector2 operator*( const GCVector2& other ) const;
    GCVector2 operator/( const GCVector2& other ) const;
    
    GCVector2 operator+( const float& value ) const;
    GCVector2 operator-( const float& value ) const;
    GCVector2 operator*( const float& value ) const;
    GCVector2 operator/( const float& value ) const;
    
    void operator+=( const GCVector2& other );
    void operator-=( const GCVector2& other );
    void operator*=( const GCVector2& other );
    void operator/=( const GCVector2& other );
    
    void operator+=( const float& value );
    void operator-=( const float& value );
    void operator*=( const float& value );
    void operator/=( const float& value );
    
    void operator++();
    void operator--();
    
    bool operator==( const GCVector2& other ) const { return x == other.x && y == other.y; };
    bool operator!=( const GCVector2& other ) const { return x != other.x || y != other.y; };
    bool operator<( const GCVector2& other ) const { return ( x < other.x && y <= other.y ) || ( x <= other.x && y < other.y ); };
    bool operator<=( const GCVector2& other ) const { return x <= other.x && y <= other.y; };
    bool operator>( const GCVector2& other ) const { return x > other.x && y >= other.y || ( x >= other.x && y > other.y ); };
    bool operator>=( const GCVector2& other ) const { return x >= other.x && y >= other.y; };
    // int operator<=>( const GCVector2& other ) const;
    
    bool IsZero() const { return x == 0.0f && y == 0.0f; }; // ::IsZero(x) && ::IsZero(y)
};
