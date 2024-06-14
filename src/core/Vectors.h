#pragma once
#include "math.h"



struct GCVEC2
{
    float x, y;
    
    GCVEC2();
    GCVEC2( float x, float y );
    ~GCVEC2() {};

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
    
    float GetNorm() const { return sqrtf( GetNormSquared() ); }
    float GetNormSquared() const { return x * x + y * y; }
    float DotProduct( const GCVEC2& other ) const { return x * other.x + y * other.y; }
    float DistanceTo( const GCVEC2& other ) const { return ( other - *this ).GetNorm(); }
    bool IsDistanceLessThan( const GCVEC2& other, const float& distance ) { return ( other - *this ).GetNormSquared() < ( distance * distance ); };
    bool IsDistanceMoreThan( const GCVEC2& other, const float& distance ) { return ( other - *this ).GetNormSquared() > ( distance * distance ); };
    bool IsDistanceEqualTo( const GCVEC2& other, const float& distance ) { return ( other - *this ).GetNormSquared() == ( distance * distance ); };
    int DistanceCompare( const GCVEC2& other, const float& distance ); // Returns 1 if real distance is more than goal distance, -1 if real distance is less than goal distance and 0 otherwise
    
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



struct GCVEC3
{
    float x, y, z;
    
    GCVEC3();
    GCVEC3( float x, float y, float z );
    ~GCVEC3() {};

    static GCVEC3 Zero() { return GCVEC3( 0.0f, 0.0f, 0.0f ); }
    static GCVEC3 One() { return GCVEC3( 1.0f, 1.0f, 1.0f ); }
    static GCVEC3 Up() { return GCVEC3( 0.0f, 1.0f, 0.0f ); }
    static GCVEC3 Down() { return GCVEC3( 0.0f, -1.0f, 0.0f ); }
    static GCVEC3 Left() { return GCVEC3( -1.0f, 0.0f, 0.0f ); }
    static GCVEC3 Right() { return GCVEC3( 1.0f, 0.0f , 0.0f); }
    static GCVEC3 Forward() { return GCVEC3( 0.0f, 0.0f, 1.0f ); }
    static GCVEC3 Back() { return GCVEC3( 0.0f, 0.0f, -1.0f ); }
    void SetZero();
    void SetOne();
    void SetUp();
    void SetDown();
    void SetLeft();
    void SetRight();
    void SetForward();
    void SetBack();
    
    void Lerp( const GCVEC3& from, const GCVEC3& to, const float& t );
    void Clamp( const GCVEC3& min, const GCVEC3& max );
    void Normalize();
    
    float GetNorm() const { return sqrtf( GetNormSquared() ); }
    float GetNormSquared() const { return x * x + y * y + z * z; }
    float DotProduct( const GCVEC3& other ) const { return x * other.x + y * other.y + z * other.z; }
    float DistanceTo( const GCVEC3& other ) const { return ( other - *this ).GetNorm(); }
    bool IsDistanceLessThan( const GCVEC3& other, const float& distance ) { return ( other - *this ).GetNormSquared() < ( distance * distance ); };
    bool IsDistanceMoreThan( const GCVEC3& other, const float& distance ) { return ( other - *this ).GetNormSquared() > ( distance * distance ); };
    bool IsDistanceEqualTo( const GCVEC3& other, const float& distance ) { return ( other - *this ).GetNormSquared() == ( distance * distance ); };
    int DistanceCompare( const GCVEC3& other, const float& distance ); // Returns 1 if real distance is more than goal distance, -1 if real distance is less than goal distance and 0 otherwise
    
    GCVEC3 operator+( const GCVEC3& other ) const;
    GCVEC3 operator-( const GCVEC3& other ) const;
    GCVEC3 operator*( const GCVEC3& other ) const;
    GCVEC3 operator/( const GCVEC3& other ) const;
    
    GCVEC3 operator*( const float& value ) const;
    GCVEC3 operator/( const float& value ) const;
    
    void operator+=( const GCVEC3& other );
    void operator-=( const GCVEC3& other );
    void operator*=( const GCVEC3& other );
    void operator/=( const GCVEC3& other );
    
    void operator*=( const float& value );
    void operator/=( const float& value );
    
    bool operator==( const GCVEC3& other ) const { return x == other.x && y == other.y && z == other.z; };
    bool operator!=( const GCVEC3& other ) const { return x != other.x || y != other.y || z != other.z; };
    bool operator<( const GCVEC3& other ) const { return ( x < other.x && y <= other.y && z <= other.z ) || ( x <= other.x && y < other.y && z <= other.z ) || ( x <= other.x && y <= other.y && z < other.z ); };
    bool operator<=( const GCVEC3& other ) const { return x <= other.x && y <= other.y && z <= other.z; };
    bool operator>( const GCVEC3& other ) const { return ( x > other.x && y >= other.y && z >= other.z ) || ( x >= other.x && y > other.y && z >= other.z ) || ( x >= other.x && y >= other.y && z > other.z ); };
    bool operator>=( const GCVEC3& other ) const { return x >= other.x && y >= other.y && z >= other.z; };
    // int operator<=>( const GCVEC3& other ) const;
    
    bool IsZero() const { return x == 0.0f && y == 0.0f; }; //! ::IsZero(x) && ::IsZero(y) && ::IsZero(z)
};
