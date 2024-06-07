#pragma once
#include "math.h"

struct GCVEC2
{
    float x, y;
    
    GCVEC2();
    GCVEC2( float x, float y );
    ~GCVEC2();

    static GCVEC2 Zero() { return GCVEC2( 0.0f, 0.0f ); }
    static GCVEC2 One() { return GCVEC2( 1.0f, 1.0f ); }
    static GCVEC2 Up() { return GCVEC2( 0.0f, 1.0f ); }
    static GCVEC2 Down() { return GCVEC2( 0.0f, -1.0f ); }
    static GCVEC2 Left() { return GCVEC2( -1.0f, 0.0f ); }
    static GCVEC2 Right() { return GCVEC2( 1.0f, 0.0f ); }
    void SetZero();
    void SetOne();
    void SetUp();
    void SetDown();
    void SetLeft();
    void SetRight();
    
    void Lerp( const GCVEC2& from, const GCVEC2& to, const float& t );
    void Clamp( const GCVEC2& min, const GCVEC2& max );
    void Normalize();
    
    float GetNorm() const { return sqrtf( x * x + y * y ); }
    float DotProduct( const GCVEC2& other ) const { return x * other.x + y * other.y; }
    float Distance( const GCVEC2& other ) const { return ; }
    
    GCVEC2 operator+( const GCVEC2& other ) const;
    GCVEC2 operator-( const GCVEC2& other ) const;
    GCVEC2 operator*( const GCVEC2& other ) const;
    GCVEC2 operator/( const GCVEC2& other ) const;
    
    GCVEC2 operator*( const float& value ) const;
    GCVEC2 operator/( const float& value ) const;
    
    void operator+=( const GCVEC2& other );
    void operator-=( const GCVEC2& other );
    void operator*=( const GCVEC2& other );
    void operator/=( const GCVEC2& other );
    
    void operator*=( const float& value );
    void operator/=( const float& value );
    
    bool operator==( const GCVEC2& other ) const { return x == other.x && y == other.y; };
    bool operator!=( const GCVEC2& other ) const { return x != other.x || y != other.y; };
    bool operator<( const GCVEC2& other ) const { return ( x < other.x && y <= other.y ) || ( x <= other.x && y < other.y ); };
    bool operator<=( const GCVEC2& other ) const { return x <= other.x && y <= other.y; };
    bool operator>( const GCVEC2& other ) const { return x > other.x && y >= other.y || ( x >= other.x && y > other.y ); };
    bool operator>=( const GCVEC2& other ) const { return x >= other.x && y >= other.y; };
    // int operator<=>( const GCVEC2& other ) const;
    
    bool IsZero() const { return x == 0.0f && y == 0.0f; }; //! ::IsZero(x) && ::IsZero(y)
};
