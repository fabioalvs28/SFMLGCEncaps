#include "pch.h"
#include "Vectors.h"




// <summary>
// Initializes the vector components to zero.
// </summary>
GCVEC2::GCVEC2()
{
    SetZero();
}

// <summary>
// Initializes the vector components with the given values.
// </summary>
// <param name="x"> The x-coordinate of the vector. </param>
// <param name="y"> The y-coordinate of the vector. </param>
GCVEC2::GCVEC2( float x, float y )
{
    this->x = x;
    this->y = y;
}


// <summary>
// 
// </summary>
const char* GCVEC2::ToString() const
{
    char buffer[22];
    sprintf_s(buffer, "{%f ; %f}", x, y);
    return buffer;
}


// <summary>
// This function sets the x and y components of the vector to zero.
// </summary>
void GCVEC2::SetZero()
{
    x = 0.0f;
    y = 0.0f;
}

// <summary>
// This function sets the x and y components of the vector to one.
// </summary>
void GCVEC2::SetOne()
{
    x = 1.0f;
    y = 1.0f;
}

// <summary>
// This function sets the x and y components of the vector to (0.0f, 1.0f) respectively.
// It is useful for initializing a 2D vector to represent the up direction.
// </summary>
void GCVEC2::SetUp()
{
    x = 0.0f;
    y = 1.0f;
}

// <summary>
// This function sets the x and y components of the vector to (0.0f, -1.0f) respectively.
// It is useful for initializing a 2D vector to represent the down direction.
// </summary>
void GCVEC2::SetDown()
{
    x = 0.0f;
    y = -1.0f;
}

// <summary>
// This function sets the x and y components of the vector to (-1.0f, 0.0f) respectively.
// It is useful for initializing a 2D vector to represent the left direction.
// </summary>
void GCVEC2::SetLeft()
 {
    x = -1.0f;
    y = 0.0f;
}

// <summary>
// This function sets the x and y components of the vector to (1.0f, 0.0f) respectively.
// It is useful for initializing a 2D vector to represent the right direction.
// </summary>
void GCVEC2::SetRight()
 {
    x = 1.0f;
    y = 0.0f;
}


// <summary>
// This function calculates the linear interpolation between two vectors based on a given interpolation factor.
// The resulting vector is a combination of the 'from' and 'to' vectors, where 't' determines the weight given to 'to'.
// </summary>
// <param name="from"> The starting vector for interpolation. </param>
// <param name="to"> The ending vector for interpolation. </param>
// <param name="t"> The interpolation factor, where 0.0f represents 'from' and 1.0f represents 'to'. </param>
void GCVEC2::Lerp( const GCVEC2& from, const GCVEC2& to, const float& t )
{
    x = from.x + ( to.x - from.x ) * t;
    y = from.y + ( to.y - from.y ) * t;
}

// <summary>
// This function ensures that the x and y components of the vector are within the specified minimum and maximum values.
// If any component is less than the corresponding minimum value, it is set to the minimum value.
// If any component is greater than the corresponding maximum value, it is set to the maximum value.
// This function does not return a value. It modifies the vector in-place.
// If the minimum value is greater than the maximum value, this function does nothing.
// </summary>
// <param name="min"> The minimum values for the x and y components. </param>
// <param name="max"> The maximum values for the x and y components. </param>
void GCVEC2::Clamp( const GCVEC2& min, const GCVEC2& max )
{
    if ( min > max ) return;
    
    if ( this->x < min.x ) this->x = min.x;
    if ( this->y < min.y ) this->y = min.y;
    
    if ( this->x > max.x ) this->x = max.x;
    if ( this->y > max.y ) this->y = max.y;
}

// <summary>
// This function normalizes the vector, making its length equal to 1 while maintaining its direction.
// If the vector is already a zero vector, this function does nothing.
// This function does not return a value. It modifies the vector in-place.
// </summary>
void GCVEC2::Normalize()
{
    float norm = GetNorm();
    x /= norm;
    y /= norm;
}


// <summary>
// This function compares the given distance and the distance between this vector and another vector.
// If the distance between this vector and the other vector is less than the given distance, this function returns -1.
// If the distance between this vector and the other vector is equal to the given distance, this function returns 0.
// If the distance between this vector and the other vector is greater than the given distance, this function returns 1.
// </summary>
// <param name="other"> The other vector to compare the distance with. </param>
// <param name="distance"> The distance to compare with. </param>
int GCVEC2::DistanceCompare( const GCVEC2& other, const float& distance )
{
    float goalDistanceSquared = distance * distance;
    float realDistanceSquared = ( other - *this ).GetNormSquared();
    return realDistanceSquared > goalDistanceSquared - realDistanceSquared < goalDistanceSquared;
}


// <summary>
// This operator adds the components of the current vector with the components of another vector.
// It returns a new GCVEC2 instance with the result of the addition.
// </summary>
// <param name="other"> The vector to add to the current vector. </param>
GCVEC2 GCVEC2::operator+( const GCVEC2& other ) const
{
    GCVEC2 result = *this;
    result += other;
    return result;
}

// <summary>
// This operator subtracts the components of another vector from the current vector.
// It returns a new GCVEC2 instance with the result of the subtraction.
// </summary>
// <param name="other"> The vector to subtract from the current vector. </param>
GCVEC2 GCVEC2::operator-( const GCVEC2& other ) const
{
    GCVEC2 result = *this;
    result -= other;
    return result;
}

// <summary>
// This operator multiplies the components of the current vector with the components of another vector.
// It returns a new GCVEC2 instance with the result of the multiplication.
// </summary>
// <param name="other"> The vector to multiply with the current vector. </param>
GCVEC2 GCVEC2::operator*( const GCVEC2& other ) const
{
    GCVEC2 result = *this;
    result *= other;
    return result;
}

// <summary>
// This operator divides the components of the current vector by the components of another vector.
// It returns a new GCVEC2 instance with the result of the division.
// </summary>
// <param name="other"> The vector to divide the current vector by. </param>
GCVEC2 GCVEC2::operator/( const GCVEC2& other ) const
{
    GCVEC2 result = *this;
    result /= other;
    return result;
}


// <summary>
// This operator multiplies each component of the current vector by a scalar value.
// It returns a new GCVEC2 instance with the result of the multiplication.
// </summary>
// <param name="value"> The scalar value to multiply with the vector components. </param>
GCVEC2 GCVEC2::operator*( const float& value ) const
{
    GCVEC2 result = *this;
    result *= value;
    return result;
}

// <summary>
// This operator divides each component of the current vector by a scalar value.
// It returns a new GCVEC2 instance with the result of the division.
// </summary>
// <param name="value"> The scalar value to divide with the vector components. </param>
GCVEC2 GCVEC2::operator/( const float& value ) const
{
    GCVEC2 result = *this;
    result /= value;
    return result;
}


// <summary>
// This operator adds the components of the current vector with the components of another vector.
// It modifies the current vector in-place.
// </summary>
// <param name="other"> The vector to add to the current vector. </param>
void GCVEC2::operator+=( const GCVEC2& other )
{
    x += other.x;
    y += other.y;
}

// <summary>
// This operator subtracts the components of another vector from the current vector.
// It modifies the current vector in-place.
// </summary>
// <param name="other"> The vector to subtract from the current vector. </param>
void GCVEC2::operator-=( const GCVEC2& other )
{
    x -= other.x;
    y -= other.y;
}

// <summary>
// This operator multiplies each component of the current vector by the corresponding component of another vector.
// It modifies the current vector in-place.
// </summary>
// <param name="other"> The vector to multiply with the current vector. </param>
void GCVEC2::operator*=( const GCVEC2& other )
{
    x *= other.x;
    y *= other.y;
}

// <summary>
// This operator divides each component of the current vector by the corresponding component of another vector.
// It modifies the current vector in-place.
// </summary>
// <param name="other"> The vector to divide the current vector by. </param
void GCVEC2::operator/=( const GCVEC2& other ) //! Division by zero
{
    x /= other.x;
    y /= other.y;
}


// <summary>
// This operator multiplies each component of the current vector by a scalar value.
// It modifies the current vector in-place.
// </summary>
// <param name="value"> The scalar value to multiply with the vector components. </param>
void GCVEC2::operator*=( const float& value )
{
    x *= value;
    y *= value;
}

// <summary>
// This operator divides each component of the current vector by a scalar value.
// It modifies the current vector in-place.
// </summary>
// <param name="value"> The scalar value to divide with the vector components. </param>
void GCVEC2::operator/=( const float& value ) //! Division by zero
{
    x /= value;
    y /= value;
}









// <summary>
// Initializes the vector components to zero.
// </summary>
GCVEC3::GCVEC3()
{
    x = 0.0f;
    y = 0.0f;
    z = 0.0f;
}

// <summary>
// Initializes the vector components with the given values.
// </summary>
// <param name="x"> The x-coordinate of the vector. </param>
// <param name="y"> The y-coordinate of the vector. </param>
// <param name="z"> The z-coordinate of the vector. </param>
GCVEC3::GCVEC3( float x, float y, float z )
{
    this->x = x;
    this->y = y;
    this->z = z;
}


// <summary>
// 
// </summary>
const char* GCVEC3::ToString() const
{
    char buffer[30];
    sprintf_s(buffer, "{%f ; %f ; %f}", x, y, z);
    return buffer;
}


// <summary>
// This function sets the x, y and z components of the vector to zero.
// </summary>
void GCVEC3::SetZero()
{
    x = 0.0f;
    y = 0.0f;
    z = 0.0f;
}

// <summary>
// This function sets the x, y and z components of the vector to one.
// </summary>
void GCVEC3::SetOne()
{
    x = 1.0f;
    y = 1.0f;
    z = 1.0f;
}

// <summary>
// This function sets the x, y and z components of the vector to (0.0f, 1.0f, 0.0f) respectively.
// It is useful for initializing a 3D vector to represent the up direction.
// </summary>
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
    
    if ( this->x < min.x ) this->x = min.x;
    if ( this->y < min.y ) this->y = min.y;
    if ( this->z < min.z ) this->z = min.z;
    
    if ( this->x > max.x ) this->x = max.x;
    if ( this->y > max.y ) this->y = max.y;
    if ( this->z > max.z ) this->z = max.z;
}

void GCVEC3::Normalize()
{
    float norm = GetNorm();
    x /= norm;
    y /= norm;
    z /= norm;
}



int GCVEC3::DistanceCompare( const GCVEC3& other, const float& distance )
{
    float goalDistanceSquared = distance * distance;
    float realDistanceSquared = ( other - *this ).GetNormSquared();
    return realDistanceSquared > goalDistanceSquared - realDistanceSquared < goalDistanceSquared;
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
