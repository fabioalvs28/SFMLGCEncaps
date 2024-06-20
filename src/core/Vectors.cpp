#include "pch.h"
#include "Vectors.h"



///////////////////////////////////////////////////////////////
/// @brief Default constructor for the GCVEC2.
/// 
/// @note The default values are the same as GCVEC2::Zero().
///////////////////////////////////////////////////////////////
GCVEC2::GCVEC2() { SetZero(); }

///////////////////////////////////////////////////////////////////////
/// @brief Constructor for the GCVEC2 with specified x and y values.
/// 
/// @param x A float indicating the x value.
/// @param y A float indicating the y value.
///////////////////////////////////////////////////////////////////////
GCVEC2::GCVEC2( const float& x, const float& y )
{
    this->x = x;
    this->y = y;
}



//////////////////////////////////////////////////
/// @return A new GCVEC2 with SetZero() values.
//////////////////////////////////////////////////
GCVEC2 GCVEC2::Zero()
{
    GCVEC2 result;
    result.SetZero();
    return result;
}

/////////////////////////////////////////////////
/// @return A new GCVEC2 with SetOne() values.
/////////////////////////////////////////////////
GCVEC2 GCVEC2::One()
{
    GCVEC2 result;
    result.SetOne();
    return result;
}

////////////////////////////////////////////////
/// @return A new GCVEC2 with SetUp() values.
////////////////////////////////////////////////
GCVEC2 GCVEC2::Up()
{
    GCVEC2 result;
    result.SetUp();
    return result;
}

//////////////////////////////////////////////////
/// @return A new GCVEC2 with SetDown() values.
//////////////////////////////////////////////////
GCVEC2 GCVEC2::Down()
{
    GCVEC2 result;
    result.SetDown();
    return result;
}

//////////////////////////////////////////////////
/// @return A new GCVEC2 with SetLeft() values.
//////////////////////////////////////////////////
GCVEC2 GCVEC2::Left()
{
    GCVEC2 result;
    result.SetLeft();
    return result;
}

//////////////////////////////////////////////////
/// @return A new GCVEC2 with SetRight() values.
//////////////////////////////////////////////////
GCVEC2 GCVEC2::Right()
{
    GCVEC2 result;
    result.SetRight();
    return result;
}



//////////////////////////////////////////////
/// @brief Sets the x and y values to 0.0f.
//////////////////////////////////////////////
void GCVEC2::SetZero() { x = 0.0f; y = 0.0f; }

//////////////////////////////////////////////
/// @brief Sets the x and y values to 1.0f.
//////////////////////////////////////////////
void GCVEC2::SetOne() { x = 1.0f; y = 1.0f; }

////////////////////////////////////////////////////////////////
/// @brief Sets the x value to 0.0f and the y value to -1.0f.
////////////////////////////////////////////////////////////////
void GCVEC2::SetUp() { x = 0.0f; y = -1.0f; }

///////////////////////////////////////////////////////////////
/// @brief Sets the x value to 0.0f and the y value to 1.0f.
///////////////////////////////////////////////////////////////
void GCVEC2::SetDown() { x = 0.0f; y = 1.0f; }

////////////////////////////////////////////////////////////////
/// @brief Sets the x value to -1.0f and the y value to 0.0f.
////////////////////////////////////////////////////////////////
void GCVEC2::SetLeft() { x = -1.0f; y = 0.0f; }

///////////////////////////////////////////////////////////////
/// @brief Sets the x value to 1.0f and the y value to 0.0f.
///////////////////////////////////////////////////////////////
void GCVEC2::SetRight() { x = 1.0f; y = 0.0f; }



////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief Performs a linear interpolation between the specified GCVEC2.
/// 
/// @param from The starting GCVEC2.
/// @param to The ending GCVEC2.
/// @param t The interpolation factor where 0.0f represents 'from' and 1.0f represents 'to'.
/// 
/// @note The result of the linear interpolation is stored directly in this GCVEC2.
////////////////////////////////////////////////////////////////////////////////////////////////////////
void GCVEC2::Lerp( const GCVEC2& from, const GCVEC2& to, const float& t ) // todo Clamp t between 0.0f and 1.0f
{
    x = from.x + ( to.x - from.x ) * t;
    y = from.y + ( to.y - from.y ) * t;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief Makes sure that the values in the GCVEC2 are between the min and max values.
/// 
/// @param min A GCVEC2 indicating the minimum values.
/// @param max A GCVEC2 indicating the maximum values.
/// 
/// @note If the values in the GCVEC2 aren't in the specified range, they are changed to the nearest value in the range.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void GCVEC2::Clamp( const GCVEC2& min, const GCVEC2& max )
{
    if ( min > max ) return;
    
    if ( this->x < min.x ) this->x = min.x;
    if ( this->y < min.y ) this->y = min.y;
    
    if ( this->x > max.x ) this->x = max.x;
    if ( this->y > max.y ) this->y = max.y;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief Normalizes the GCVEC2 by dividing each value by the GCVEC2's norm.
////////////////////////////////////////////////////////////////////////////////
void GCVEC2::Normalize()
{
    float invNorm = 1 / GetNorm();
    x *= invNorm;
    y *= invNorm;
}



//////////////////////////////////////
/// @return The norm of the GCVEC2.
//////////////////////////////////////
float GCVEC2::GetNorm() const { return sqrtf( GetNormSquared() ); }

//////////////////////////////////////////////
/// @return The norm of the GCVEC2 squared.
//////////////////////////////////////////////
float GCVEC2::GetNormSquared() const { return x * x + y * y; }

///////////////////////////////////////////////////////////////////////////
/// @brief Performs a dot product between this GCVEC2 and the given one.
/// 
/// @param other The GCVEC2 to perform to dot product with.
/// 
/// @return A float value indicating the result of the dot product.
///////////////////////////////////////////////////////////////////////////
float GCVEC2::DotProduct( const GCVEC2& other ) const { return x * other.x + y * other.y; }

///////////////////////////////////////////////////////////////////////////////////////////
/// @param other The GCVEC2 to calculate the distance with.
/// 
/// @return A float value indicating the distance between this GCVEC2 and the given one.
///////////////////////////////////////////////////////////////////////////////////////////
float GCVEC2::DistanceTo( const GCVEC2& other ) const { return ( other - *this ).GetNorm(); }

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param other The GCVEC2 to calculate the distance with.
/// 
/// @return A float value indicating the distance between this GCVEC2 and the given one, squared.
////////////////////////////////////////////////////////////////////////////////////////////////////
float GCVEC2::DistanceToSquared( const GCVEC2& other ) const { return ( other - *this ).GetNormSquared(); }



//////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief Compares the distance between this GCVEC2 and the given one with the given distance.
/// 
/// @param other The GCVEC2 to compare the distance with.
/// @param distance A float value indicating the distance to compare.
/// 
/// @return true if the real distance is smaller than the given distance, false otherwise.
//////////////////////////////////////////////////////////////////////////////////////////////////
bool GCVEC2::IsDistanceLessThan( const GCVEC2& other, const float& distance ) const { return DistanceToSquared( other ) < ( distance * distance ); };

//////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief Compares the distance between this GCVEC2 and the given one with the given distance.
/// 
/// @param other The GCVEC2 to compare the distance with.
/// @param distance A float value indicating the distance to compare.
/// 
/// @return true if the real distance is greater than the given distance, false otherwise.
//////////////////////////////////////////////////////////////////////////////////////////////////
bool GCVEC2::IsDistanceMoreThan( const GCVEC2& other, const float& distance ) const { return DistanceToSquared( other ) > ( distance * distance ); };

//////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief Compares the distance between this GCVEC2 and the given one with the given distance.
/// 
/// @param other The GCVEC2 to compare the distance with.
/// @param distance A float value indicating the distance to compare.
/// 
/// @return true if both distances are equal, false otherwise.
//////////////////////////////////////////////////////////////////////////////////////////////////
bool GCVEC2::IsDistanceEqualTo( const GCVEC2& other, const float& distance ) const { return DistanceToSquared( other ) == ( distance * distance ); };

//////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief Compares the distance between this GCVEC2 and the given one with the given distance.
/// 
/// @param other The GCVEC2 to compare the distance with.
/// @param distance A float value indicating the distance to compare.
/// 
/// @return -1 if the real distance is smaller than the given distance.
/// @return  0 if both distances are equal.
/// @return  1 if the real distance is greater than the given distance.
//////////////////////////////////////////////////////////////////////////////////////////////////
int GCVEC2::DistanceCompare( const GCVEC2& other, const float& distance ) const
{
    float distanceSquared = distance * distance;
    float realDistanceSquared = DistanceToSquared( other );
    return realDistanceSquared > distanceSquared - realDistanceSquared < distanceSquared;
}



/////////////////////////////////////////////////////////////////////////////
/// @param other The GCVEC2 to add to this one.
/// 
/// @return A GCVEC2 indicating the result of the addition of both GCVEC2.
/// 
/// @note It doesn't change the values of this GCVEC2.
/////////////////////////////////////////////////////////////////////////////
GCVEC2 GCVEC2::operator+( const GCVEC2& other ) const
{
    GCVEC2 result = *this;
    result += other;
    return result;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param other The GCVEC2 to substract from this one.
/// 
/// @return A GCVEC2 indicating the result of the substraction of the given GCVEC2 with this one.
/// 
/// @note It doesn't change the values of this GCVEC2.
////////////////////////////////////////////////////////////////////////////////////////////////////
GCVEC2 GCVEC2::operator-( const GCVEC2& other ) const
{
    GCVEC2 result = *this;
    result -= other;
    return result;
}

///////////////////////////////////////////////////////////////////////////////////
/// @param other The GCVEC2 to multiply to this one.
/// 
/// @return A GCVEC2 indicating the result of the multiplication of both GCVEC2.
/// 
/// @note It doesn't change the values of this GCVEC2.
///////////////////////////////////////////////////////////////////////////////////
GCVEC2 GCVEC2::operator*( const GCVEC2& other ) const
{
    GCVEC2 result = *this;
    result *= other;
    return result;
}

////////////////////////////////////////////////////////////////////////////////////////////////
/// @param other The GCVEC2 to divide from this one.
/// 
/// @return A GCVEC2 indicating the result of the division of this GCVEC2 with the given one.
/// 
/// @note It doesn't change the values of this GCVEC2.
////////////////////////////////////////////////////////////////////////////////////////////////
GCVEC2 GCVEC2::operator/( const GCVEC2& other ) const
{
    GCVEC2 result = *this;
    result /= other;
    return result;
}



//////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param value The float value to multiply to this GCVEC2.
/// 
/// @return A GCVEC2 indicating the result of the multiplication of the GCVEC2 and the float value.
/// 
/// @note It doesn't change the values of this GCVEC2.
//////////////////////////////////////////////////////////////////////////////////////////////////////
GCVEC2 GCVEC2::operator*( const float& value ) const
{
    GCVEC2 result = *this;
    result *= value;
    return result;
}

/////////////////////////////////////////////////////////////////////////////////////////////////
/// @param value The float value to divide from this GCVEC2.
/// 
/// @return A GCVEC2 indicating the result of the division of the GCVEC2 with the float value.
/// 
/// @note It doesn't change the values of this GCVEC2.
/////////////////////////////////////////////////////////////////////////////////////////////////
GCVEC2 GCVEC2::operator/( const float& value ) const
{
    GCVEC2 result = *this;
    result /= value;
    return result;
}



///////////////////////////////////////////////////////////////////
/// @brief Adds the values from the given GCVEC2 to this GCVEC2.
/// 
/// @param other The GCVEC2 to add to this one.
///////////////////////////////////////////////////////////////////
void GCVEC2::operator+=( const GCVEC2& other )
{
    x += other.x;
    y += other.y;
}

///////////////////////////////////////////////////////////////////////////
/// @brief Substracts the values from the given GCVEC2 from this GCVEC2.
/// 
/// @param other The GCVEC2 to substract from this one.
///////////////////////////////////////////////////////////////////////////
void GCVEC2::operator-=( const GCVEC2& other )
{
    x -= other.x;
    y -= other.y;
}

//////////////////////////////////////////////////////////////////////////////////////
/// @brief Mulitplies each values of this GCVEC2 by the values of the given GCVEC2.
/// 
/// @param other The GCVEC2 to be multiplied by.
//////////////////////////////////////////////////////////////////////////////////////
void GCVEC2::operator*=( const GCVEC2& other )
{
    x *= other.x;
    y *= other.y;
}

///////////////////////////////////////////////////////////////////////////////////
/// @brief Divides each values of this GCVEC2 by the values of the given GCVEC2.
/// 
/// @param other The GCVEC2 to be divided by.
///////////////////////////////////////////////////////////////////////////////////
void GCVEC2::operator/=( const GCVEC2& other ) // todo protection from division by zero
{
    x /= other.x;
    y /= other.y;
}



///////////////////////////////////////////////////////////////////////
/// @brief Mulitplies each values of this GCVEC2 by the given value.
/// 
/// @param value The float value to be multiplied by.
///////////////////////////////////////////////////////////////////////
void GCVEC2::operator*=( const float& value )
{
    x *= value;
    y *= value;
}

////////////////////////////////////////////////////////////////////
/// @brief Divides each values of this GCVEC2 by the given value.
/// 
/// @param value The float value to be divided by.
////////////////////////////////////////////////////////////////////
void GCVEC2::operator/=( const float& value ) // todo protection from division by zero
{
    x /= value;
    y /= value;
}



/////////////////////////////////////////////////////////////////////////
/// @brief Checks if both GCVEC2 are equals.
/// 
/// @param other The GCVEC2 to compare with.
/// 
/// @return true if both GCVEC2 have the same values, false otherwise.
/////////////////////////////////////////////////////////////////////////
bool GCVEC2::operator==( const GCVEC2& other ) const { return x == other.x && y == other.y; };

////////////////////////////////////////////////////////////////////////////////////////////
/// @brief Checks if both GCVEC2 are different.
/// 
/// @param other The GCVEC2 to compare with.
/// 
/// @return true if at least one value of the given GCVEC2 is different, false otherwise.
////////////////////////////////////////////////////////////////////////////////////////////
bool GCVEC2::operator!=( const GCVEC2& other ) const { return x != other.x || y != other.y; };

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief Checks whether the given GCVEC2 has higher or equal values than this GCVEC2 and at least one stricly higher value than this GCVEC2.
/// 
/// @param other The GCVEC2 to compare with.
/// 
/// @return true if both values of the given GCVEC2 are higher or equal to the values of this GCVEC2 and at least one of them is stricly higher, false otherwise.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool GCVEC2::operator<( const GCVEC2& other ) const { return ( x < other.x && y <= other.y ) || ( x <= other.x && y < other.y ); };

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief Checks whether the given GCVEC2 has higher or equal values than this GCVEC.
/// 
/// @param other The GCVEC2 to compare with.
/// 
/// @return true if both values of the given GCVEC2 are higher or equal to the values of this GCVEC2, false otherwise.
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool GCVEC2::operator<=( const GCVEC2& other ) const { return x <= other.x && y <= other.y; };

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief Checks whether the given GCVEC2 has lower or equal values than this GCVEC2 and at least one stricly lower value than this GCVEC2.
/// 
/// @param other The GCVEC2 to compare with.
/// 
/// @return true if both values of the given GCVEC2 are lower or equal to the values of this GCVEC2 and at least one of them is stricly lower, false otherwise.
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool GCVEC2::operator>( const GCVEC2& other ) const { return x > other.x && y >= other.y || ( x >= other.x && y > other.y ); };

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief Checks whether the given GCVEC2 has lower or equal values than this GCVEC2.
/// 
/// @param other The GCVEC2 to compare with.
/// 
/// @return true if both values of the given GCVEC2 are lower or equal to the values of this GCVEC2, false otherwise.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool GCVEC2::operator>=( const GCVEC2& other ) const { return x >= other.x && y >= other.y; };



/////////////////////////////////////////////////////////////////////////////////
/// @return true if both values of the GCVEC2 are equal to 0, false otherwise.
/////////////////////////////////////////////////////////////////////////////////
bool GCVEC2::IsZero() const { return x == 0.0f && y == 0.0f; }; //! ::IsZero(x) && ::IsZero(y)






///////////////////////////////////////////////////////////////
/// @brief Default constructor for the GCVEC3.
/// 
/// @note The default values are the same as GCVEC3::Zero().
///////////////////////////////////////////////////////////////
GCVEC3::GCVEC3() { SetZero(); }

///////////////////////////////////////////////////////////////////////
/// @brief Constructor for the GCVEC3 with specified x and y values.
/// 
/// @param x A float indicating the x value.
/// @param y A float indicating the y value.
///////////////////////////////////////////////////////////////////////
GCVEC3::GCVEC3( const float& x, const float& y, const float& z )
{
    this->x = x;
    this->y = y;
    this->z = z;
}



//////////////////////////////////////////////////
/// @return A new GCVEC3 with SetZero() values.
//////////////////////////////////////////////////
GCVEC3 GCVEC3::Zero()
{
    GCVEC3 result;
    result.SetZero();
    return result;
}

/////////////////////////////////////////////////
/// @return A new GCVEC3 with SetOne() values.
/////////////////////////////////////////////////
GCVEC3 GCVEC3::One()
{
    GCVEC3 result;
    result.SetOne();
    return result;
}

////////////////////////////////////////////////
/// @return A new GCVEC3 with SetUp() values.
////////////////////////////////////////////////
GCVEC3 GCVEC3::Up()
{
    GCVEC3 result;
    result.SetUp();
    return result;
}

//////////////////////////////////////////////////
/// @return A new GCVEC3 with SetDown() values.
//////////////////////////////////////////////////
GCVEC3 GCVEC3::Down()
{
    GCVEC3 result;
    result.SetDown();
    return result;
}

//////////////////////////////////////////////////
/// @return A new GCVEC3 with SetLeft() values.
//////////////////////////////////////////////////
GCVEC3 GCVEC3::Left()
{
    GCVEC3 result;
    result.SetLeft();
    return result;
}

//////////////////////////////////////////////////
/// @return A new GCVEC3 with SetRight() values.
//////////////////////////////////////////////////
GCVEC3 GCVEC3::Right()
{
    GCVEC3 result;
    result.SetRight();
    return result;
}

//////////////////////////////////////////////////
/// @return A new GCVEC3 with SetFront() values.
//////////////////////////////////////////////////
GCVEC3 GCVEC3::Front()
{
    GCVEC3 result;
    result.SetFront();
    return result;
}

//////////////////////////////////////////////////
/// @return A new GCVEC3 with SetBack() values.
//////////////////////////////////////////////////
GCVEC3 GCVEC3::Back()
{
    GCVEC3 result;
    result.SetBack();
    return result;
}



/////////////////////////////////////////////////
/// @brief Sets the x, y and z values to 0.0f.
/////////////////////////////////////////////////
void GCVEC3::SetZero() { x = 0.0f; y = 0.0f; z = 0.0f; }

/////////////////////////////////////////////////
/// @brief Sets the x, y and z values to 1.0f.
/////////////////////////////////////////////////
void GCVEC3::SetOne() { x = 1.0f; y = 1.0f; z = 1.0f; }

//////////////////////////////////////////////////////////////////////
/// @brief Sets the x and z value to 0.0f and the y value to -1.0f.
//////////////////////////////////////////////////////////////////////
void GCVEC3::SetUp() { x = 0.0f; y = -1.0f; z = 0.0f; }

/////////////////////////////////////////////////////////////////////
/// @brief Sets the x and z value to 0.0f and the y value to 1.0f.
/////////////////////////////////////////////////////////////////////
void GCVEC3::SetDown() { x = 0.0f; y = 1.0f; z = 0.0f; }

//////////////////////////////////////////////////////////////////////
/// @brief Sets the x value to -1.0f and the y and z value to 0.0f.
//////////////////////////////////////////////////////////////////////
void GCVEC3::SetLeft() { x = -1.0f; y = 0.0f; z = 0.0f; }

/////////////////////////////////////////////////////////////////////
/// @brief Sets the x value to 1.0f and the y and z value to 0.0f.
/////////////////////////////////////////////////////////////////////
void GCVEC3::SetRight() { x = 1.0f; y = 0.0f; z = 0.0f; }

/////////////////////////////////////////////////////////////////////
/// @brief Sets the x and y value to 0.0f and the z value to 1.0f.
/////////////////////////////////////////////////////////////////////
void GCVEC3::SetFront() { x = 0.0f; y = 0.0f; z = 1.0f; }

//////////////////////////////////////////////////////////////////////
/// @brief Sets the x and y value to 0.0f and the z value to -1.0f.
//////////////////////////////////////////////////////////////////////
void GCVEC3::SetBack() { x = 0.0f; y = 0.0f; z = -1.0f; }



////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief Performs a linear interpolation between the specified GCVEC3.
/// 
/// @param from The starting GCVEC3.
/// @param to The ending GCVEC3.
/// @param t The interpolation factor where 0.0f represents 'from' and 1.0f represents 'to'.
/// 
/// @note The result of the linear interpolation is stored directly in this GCVEC3.
////////////////////////////////////////////////////////////////////////////////////////////////////////
void GCVEC3::Lerp( const GCVEC3& from, const GCVEC3& to, const float& t ) // todo Clamp t between 0.0f and 1.0f
{
    x = from.x + ( to.x - from.x ) * t;
    y = from.y + ( to.y - from.y ) * t;
    z = from.z + ( to.z - from.z ) * t;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief Makes sure that the values in the GCVEC3 are between the min and max values.
/// 
/// @param min A GCVEC3 indicating the minimum values.
/// @param max A GCVEC3 indicating the maximum values.
/// 
/// @note If the values in the GCVEC3 aren't in the specified range, they are changed to the nearest value in the range.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
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

////////////////////////////////////////////////////////////////////////////////
/// @brief Normalizes the GCVEC3 by dividing each value by the GCVEC3's norm.
////////////////////////////////////////////////////////////////////////////////
void GCVEC3::Normalize()
{
    float invNorm = 1 / GetNorm();
    x *= invNorm;
    y *= invNorm;
    z *= invNorm;
}



//////////////////////////////////////
/// @return The norm of the GCVEC3.
//////////////////////////////////////
float GCVEC3::GetNorm() const { return sqrtf( GetNormSquared() ); }

//////////////////////////////////////////////
/// @return The norm of the GCVEC3 squared.
//////////////////////////////////////////////
float GCVEC3::GetNormSquared() const { return x * x + y * y + z * z; }

///////////////////////////////////////////////////////////////////////////
/// @brief Performs a dot product between this GCVEC3 and the given one.
/// 
/// @param other The GCVEC3 to perform to dot product with.
/// 
/// @return A float value indicating the result of the dot product.
///////////////////////////////////////////////////////////////////////////
float GCVEC3::DotProduct( const GCVEC3& other ) const { return x * other.x + y * other.y + z * other.z; }

///////////////////////////////////////////////////////////////////////////////////////////
/// @param other The GCVEC3 to calculate the distance with.
/// 
/// @return A float value indicating the distance between this GCVEC3 and the given one.
///////////////////////////////////////////////////////////////////////////////////////////
float GCVEC3::DistanceTo( const GCVEC3& other ) const { return ( other - *this ).GetNorm(); }

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param other The GCVEC3 to calculate the distance with.
/// 
/// @return A float value indicating the distance between this GCVEC3 and the given one, squared.
////////////////////////////////////////////////////////////////////////////////////////////////////
float GCVEC3::DistanceToSquared( const GCVEC3& other ) const { return ( other - *this ).GetNormSquared(); }



//////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief Compares the distance between this GCVEC3 and the given one with the given distance.
/// 
/// @param other The GCVEC3 to compare the distance with.
/// @param distance A float value indicating the distance to compare.
/// 
/// @return true if the real distance is smaller than the given distance, false otherwise.
//////////////////////////////////////////////////////////////////////////////////////////////////
bool GCVEC3::IsDistanceLessThan( const GCVEC3& other, const float& distance ) const { return DistanceToSquared( other ) < ( distance * distance ); };

//////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief Compares the distance between this GCVEC3 and the given one with the given distance.
/// 
/// @param other The GCVEC3 to compare the distance with.
/// @param distance A float value indicating the distance to compare.
/// 
/// @return true if the real distance is greater than the given distance, false otherwise.
//////////////////////////////////////////////////////////////////////////////////////////////////
bool GCVEC3::IsDistanceMoreThan( const GCVEC3& other, const float& distance ) const { return DistanceToSquared( other ) > ( distance * distance ); };

//////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief Compares the distance between this GCVEC3 and the given one with the given distance.
/// 
/// @param other The GCVEC3 to compare the distance with.
/// @param distance A float value indicating the distance to compare.
/// 
/// @return true if both distances are equal, false otherwise.
//////////////////////////////////////////////////////////////////////////////////////////////////
bool GCVEC3::IsDistanceEqualTo( const GCVEC3& other, const float& distance ) const { return DistanceToSquared( other ) == ( distance * distance ); };

//////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief Compares the distance between this GCVEC3 and the given one with the given distance.
/// 
/// @param other The GCVEC3 to compare the distance with.
/// @param distance A float value indicating the distance to compare.
/// 
/// @return -1 if the real distance is smaller than the given distance.
/// @return  0 if both distances are equal.
/// @return  1 if the real distance is greater than the given distance.
//////////////////////////////////////////////////////////////////////////////////////////////////
int GCVEC3::DistanceCompare( const GCVEC3& other, const float& distance ) const
{
    float distanceSquared = distance * distance;
    float realDistanceSquared = DistanceToSquared( other );
    return realDistanceSquared > distanceSquared - realDistanceSquared < distanceSquared;
}



/////////////////////////////////////////////////////////////////////////////
/// @param other The GCVEC3 to add to this one.
/// 
/// @return A GCVEC3 indicating the result of the addition of both GCVEC3.
/// 
/// @note It doesn't change the values of this GCVEC3.
/////////////////////////////////////////////////////////////////////////////
GCVEC3 GCVEC3::operator+( const GCVEC3& other ) const
{
    GCVEC3 result = *this;
    result += other;
    return result;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param other The GCVEC3 to substract from this one.
/// 
/// @return A GCVEC3 indicating the result of the substraction of the given GCVEC3 with this one.
/// 
/// @note It doesn't change the values of this GCVEC3.
////////////////////////////////////////////////////////////////////////////////////////////////////
GCVEC3 GCVEC3::operator-( const GCVEC3& other ) const
{
    GCVEC3 result = *this;
    result -= other;
    return result;
}

///////////////////////////////////////////////////////////////////////////////////
/// @param other The GCVEC3 to multiply to this one.
/// 
/// @return A GCVEC3 indicating the result of the multiplication of both GCVEC3.
/// 
/// @note It doesn't change the values of this GCVEC3.
///////////////////////////////////////////////////////////////////////////////////
GCVEC3 GCVEC3::operator*( const GCVEC3& other ) const
{
    GCVEC3 result = *this;
    result *= other;
    return result;
}

////////////////////////////////////////////////////////////////////////////////////////////////
/// @param other The GCVEC3 to divide from this one.
/// 
/// @return A GCVEC3 indicating the result of the division of this GCVEC3 with the given one.
/// 
/// @note It doesn't change the values of this GCVEC3.
////////////////////////////////////////////////////////////////////////////////////////////////
GCVEC3 GCVEC3::operator/( const GCVEC3& other ) const
{
    GCVEC3 result = *this;
    result /= other;
    return result;
}



//////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param value The float value to multiply to this GCVEC3.
/// 
/// @return A GCVEC3 indicating the result of the multiplication of the GCVEC3 and the float value.
/// 
/// @note It doesn't change the values of this GCVEC3.
//////////////////////////////////////////////////////////////////////////////////////////////////////
GCVEC3 GCVEC3::operator*( const float& value ) const
{
    GCVEC3 result = *this;
    result *= value;
    return result;
}

/////////////////////////////////////////////////////////////////////////////////////////////////
/// @param value The float value to divide from this GCVEC3.
/// 
/// @return A GCVEC3 indicating the result of the division of the GCVEC3 with the float value.
/// 
/// @note It doesn't change the values of this GCVEC3.
/////////////////////////////////////////////////////////////////////////////////////////////////
GCVEC3 GCVEC3::operator/( const float& value ) const
{
    GCVEC3 result = *this;
    result /= value;
    return result;
}



///////////////////////////////////////////////////////////////////
/// @brief Adds the values from the given GCVEC3 to this GCVEC3.
/// 
/// @param other The GCVEC3 to add to this one.
///////////////////////////////////////////////////////////////////
void GCVEC3::operator+=( const GCVEC3& other )
{
    x += other.x;
    y += other.y;
    z += other.z;
}

///////////////////////////////////////////////////////////////////////////
/// @brief Substracts the values from the given GCVEC3 from this GCVEC3.
/// 
/// @param other The GCVEC3 to substract from this one.
///////////////////////////////////////////////////////////////////////////
void GCVEC3::operator-=( const GCVEC3& other )
{
    x -= other.x;
    y -= other.y;
    z -= other.z;
}

//////////////////////////////////////////////////////////////////////////////////////
/// @brief Mulitplies each values of this GCVEC3 by the values of the given GCVEC3.
/// 
/// @param other The GCVEC3 to be multiplied by.
//////////////////////////////////////////////////////////////////////////////////////
void GCVEC3::operator*=( const GCVEC3& other )
{
    x *= other.x;
    y *= other.y;
    z *= other.z;
}

///////////////////////////////////////////////////////////////////////////////////
/// @brief Divides each values of this GCVEC3 by the values of the given GCVEC3.
/// 
/// @param other The GCVEC3 to be divided by.
///////////////////////////////////////////////////////////////////////////////////
void GCVEC3::operator/=( const GCVEC3& other ) // todo protection from division by zero
{
    x /= other.x;
    y /= other.y;
    z /= other.z;
}



///////////////////////////////////////////////////////////////////////
/// @brief Mulitplies each values of this GCVEC3 by the given value.
/// 
/// @param value The float value to be multiplied by.
///////////////////////////////////////////////////////////////////////
void GCVEC3::operator*=( const float& value )
{
    x *= value;
    y *= value;
    z *= value;
}

////////////////////////////////////////////////////////////////////
/// @brief Divides each values of this GCVEC3 by the given value.
/// 
/// @param value The float value to be divided by.
////////////////////////////////////////////////////////////////////
void GCVEC3::operator/=( const float& value ) // todo protection from division by zero
{
    float invValue = 1 / value;
    x *= invValue;
    y *= invValue;
    z *= invValue;
}



/////////////////////////////////////////////////////////////////////////
/// @brief Checks if both GCVEC3 are equals.
/// 
/// @param other The GCVEC3 to compare with.
/// 
/// @return true if both GCVEC3 have the same values, false otherwise.
/////////////////////////////////////////////////////////////////////////
bool GCVEC3::operator==( const GCVEC3& other ) const { return x == other.x && y == other.y && z == other.z; };

////////////////////////////////////////////////////////////////////////////////////////////
/// @brief Checks if both GCVEC3 are different.
/// 
/// @param other The GCVEC3 to compare with.
/// 
/// @return true if at least one value of the given GCVEC3 is different, false otherwise.
////////////////////////////////////////////////////////////////////////////////////////////
bool GCVEC3::operator!=( const GCVEC3& other ) const { return x != other.x || y != other.y || z != other.z; };

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief Checks whether the given GCVEC3 has higher or equal values than this GCVEC3 and at least one stricly higher value than this GCVEC3.
/// 
/// @param other The GCVEC3 to compare with.
/// 
/// @return true if both values of the given GCVEC3 are higher or equal to the values of this GCVEC3 and at least one of them is stricly higher, false otherwise.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool GCVEC3::operator<( const GCVEC3& other ) const { return ( x < other.x && y <= other.y && z <= other.z ) || ( x <= other.x && y < other.y && z <= other.z ) || ( x <= other.x && y <= other.y && z < other.z ); };

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief Checks whether the given GCVEC3 has higher or equal values than this GCVEC.
/// 
/// @param other The GCVEC3 to compare with.
/// 
/// @return true if both values of the given GCVEC3 are higher or equal to the values of this GCVEC3, false otherwise.
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool GCVEC3::operator<=( const GCVEC3& other ) const { return x <= other.x && y <= other.y && z <= other.z; };

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief Checks whether the given GCVEC3 has lower or equal values than this GCVEC3 and at least one stricly lower value than this GCVEC3.
/// 
/// @param other The GCVEC3 to compare with.
/// 
/// @return true if both values of the given GCVEC3 are lower or equal to the values of this GCVEC3 and at least one of them is stricly lower, false otherwise.
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool GCVEC3::operator>( const GCVEC3& other ) const { return ( x > other.x && y >= other.y && z >= other.z ) || ( x >= other.x && y > other.y && z >= other.z ) || ( x >= other.x && y >= other.y && z > other.z ); };

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief Checks whether the given GCVEC3 has lower or equal values than this GCVEC3.
/// 
/// @param other The GCVEC3 to compare with.
/// 
/// @return true if both values of the given GCVEC3 are lower or equal to the values of this GCVEC3, false otherwise.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool GCVEC3::operator>=( const GCVEC3& other ) const { return x >= other.x && y >= other.y && z >= other.z; };



/////////////////////////////////////////////////////////////////////////////////
/// @return true if both values of the GCVEC3 are equal to 0, false otherwise.
/////////////////////////////////////////////////////////////////////////////////
bool GCVEC3::IsZero() const { return x == 0.0f && y == 0.0f && z == 0.0f; }; //! ::IsZero(x) && ::IsZero(y) && ::IsZero(z)