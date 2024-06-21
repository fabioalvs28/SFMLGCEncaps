#pragma once
#include "math.h"



struct GCVEC2
{
    float x, y;
    
    GCVEC2();
    GCVEC2( const float& x, const float& y );
    ~GCVEC2() {};

    static GCVEC2 Zero();
    static GCVEC2 One();
    static GCVEC2 Right();
    static GCVEC2 Down();
    
    void SetZero();
    void SetOne();
    void SetRight();
    void SetDown();
    
    void Lerp( const GCVEC2& from, const GCVEC2& to, const float& t );
    void Clamp( const GCVEC2& min, const GCVEC2& max );
    void Normalize();
    
    float GetNorm() const;
    float GetNormSquared() const;
    float DotProduct( const GCVEC2& other ) const;
    float DistanceTo( const GCVEC2& other ) const;
    float DistanceToSquared( const GCVEC2& other ) const;
    
    bool IsDistanceLessThan( const GCVEC2& other, const float& distance ) const;
    bool IsDistanceMoreThan( const GCVEC2& other, const float& distance ) const;
    bool IsDistanceEqualTo( const GCVEC2& other, const float& distance ) const;
    int DistanceCompare( const GCVEC2& other, const float& distance ) const;
    
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
    
    bool operator==( const GCVEC2& other ) const;
    bool operator!=( const GCVEC2& other ) const;
    bool operator<( const GCVEC2& other ) const;
    bool operator<=( const GCVEC2& other ) const;
    bool operator>( const GCVEC2& other ) const;
    bool operator>=( const GCVEC2& other ) const;
    // int operator<=>( const GCVEC2& other ) const;
    
    bool IsZero() const;
};



struct GCVEC3
{
    float x, y, z;
    
    GCVEC3();
    GCVEC3( const float& x, const float& y, const float& z );
    ~GCVEC3() {};

    static GCVEC3 Zero();
    static GCVEC3 One();
    static GCVEC3 Right();
    static GCVEC3 Down();
    static GCVEC3 Front();
    
    void SetZero();
    void SetOne();
    void SetRight();
    void SetDown();
    void SetFront();
    
    void Lerp( const GCVEC3& from, const GCVEC3& to, const float& t );
    void Clamp( const GCVEC3& min, const GCVEC3& max );
    void Normalize();
    
    float GetNorm() const;
    float GetNormSquared() const;
    float DotProduct( const GCVEC3& other ) const;
    float DistanceTo( const GCVEC3& other ) const;
    float DistanceToSquared( const GCVEC3& other ) const;
    
    bool IsDistanceLessThan( const GCVEC3& other, const float& distance ) const;
    bool IsDistanceMoreThan( const GCVEC3& other, const float& distance ) const;
    bool IsDistanceEqualTo( const GCVEC3& other, const float& distance ) const;
    int DistanceCompare( const GCVEC3& other, const float& distance ) const;
    
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
    
    bool operator==( const GCVEC3& other ) const;
    bool operator!=( const GCVEC3& other ) const;
    bool operator<( const GCVEC3& other ) const;
    bool operator<=( const GCVEC3& other ) const;
    bool operator>( const GCVEC3& other ) const;
    bool operator>=( const GCVEC3& other ) const;
    // int operator<=>( const GCVEC3& other ) const;
    
    bool IsZero() const;
};
