#include "pch.h"
#include "Matrix.h"

#include "Vectors.h"

/// <summary>
/// Default constructor
/// </summary>

GCMATRIX::GCMATRIX() { SetZero(); }

GCMATRIX::GCMATRIX(
	float _11, float _12, float _13, float _14,
	float _21, float _22, float _23, float _24,
	float _31, float _32, float _33, float _34,
	float _41, float _42, float _43, float _44
)
{
	this->_11 = _11, this->_12 = _12; this->_13 = _13; this->_14 = _14;
	this->_21 = _11, this->_22 = _12; this->_23 = _13; this->_24 = _14;
	this->_31 = _11, this->_32 = _12; this->_33 = _13; this->_34 = _14;
	this->_41 = _11, this->_42 = _12; this->_43 = _13; this->_44 = _14;
}


void GCMATRIX::operator*=(const GCMATRIX& other)
{
	GCMATRIX pM = *this;

	_11 = pM._11 * other._11 + pM._12 * other._21 + pM._13 * other._31 + pM._14 * other._41;
	_12 = pM._11 * other._12 + pM._12 * other._22 + pM._13 * other._32 + pM._14 * other._42;
	_13 = pM._11 * other._13 + pM._12 * other._23 + pM._13 * other._33 + pM._14 * other._43;
	_14 = pM._11 * other._14 + pM._12 * other._24 + pM._13 * other._34 + pM._14 * other._44;
	_21 = pM._21 * other._11 + pM._22 * other._21 + pM._23 * other._31 + pM._24 * other._41;
	_22 = pM._21 * other._12 + pM._22 * other._22 + pM._23 * other._32 + pM._24 * other._42;
	_23 = pM._21 * other._13 + pM._22 * other._23 + pM._23 * other._33 + pM._24 * other._43;
	_24 = pM._21 * other._14 + pM._22 * other._24 + pM._23 * other._34 + pM._24 * other._44;
	_31 = pM._31 * other._11 + pM._32 * other._21 + pM._33 * other._31 + pM._34 * other._41;
	_32 = pM._31 * other._12 + pM._32 * other._22 + pM._33 * other._32 + pM._34 * other._42;
	_33 = pM._31 * other._13 + pM._32 * other._23 + pM._33 * other._33 + pM._34 * other._43;
	_34 = pM._31 * other._14 + pM._32 * other._24 + pM._33 * other._34 + pM._34 * other._44;
	_41 = pM._41 * other._11 + pM._42 * other._21 + pM._43 * other._31 + pM._44 * other._41;
}

/// <summary>
/// Set the matrix to zero
/// </summary>

void GCMATRIX::SetZero()
{
	_11 = 0.0f;
	_12 = 0.0f;
	_13 = 0.0f;
	_14 = 0.0f;
	_21 = 0.0f;
	_22 = 0.0f;
	_23 = 0.0f;
	_24 = 0.0f;
	_31 = 0.0f;
	_32 = 0.0f;
	_33 = 0.0f;
	_34 = 0.0f;
	_41 = 0.0f;
	_42 = 0.0f;
	_43 = 0.0f;
	_44 = 0.0f;
}

/// <summary>
/// Set the matrix to identity
/// </summary>

void GCMATRIX::SetIdentity()
{
	_11 = 1.0f;
	_12 = 0.0f;
	_13 = 0.0f;
	_14 = 0.0f;
	_21 = 0.0f;
	_22 = 1.0f;
	_23 = 0.0f;
	_24 = 0.0f;
	_31 = 0.0f;
	_32 = 0.0f;
	_33 = 1.0f;
	_34 = 0.0f;
	_41 = 0.0f;
	_42 = 0.0f;
	_43 = 0.0f;
	_44 = 1.0f;
}

/// <summary>
/// Put the matrix to the power of exp
/// </summary>
/// <param name="exp">Exponent</param>

void GCMATRIX::Pow(int exp)
{
	if (exp == 0)
	{
		SetIdentity();
		return;
	}

	if (exp < 0)
	{
		this->Inverse();
		exp = -exp;
	}

	for (int i = 0; i < exp - 1; i++)
	{
		_11 *= _11;
		_12 *= _12;
		_13 *= _13;
		_14 *= _14;
		_21 *= _21;
		_22 *= _22;
		_23 *= _23;
		_24 *= _24;
		_31 *= _31;
		_32 *= _32;
		_33 *= _33;
		_34 *= _34;
		_41 *= _41;
		_42 *= _42;
		_43 *= _43;
		_44 *= _44;
	}
}

/// <summary>
/// Returnn the determinant of the matrix
/// </summary>

float GCMATRIX::Determinant()
{
	return _41 * _32 * _23 * _14 - _31 * _42 * _23 * _14 -
		_41 * _22 * _33 * _14 + _21 * _42 * _33 * _14 +
		_31 * _22 * _43 * _14 - _21 * _32 * _43 * _14 -
		_41 * _32 * _13 * _24 + _31 * _42 * _13 * _24 +
		_41 * _12 * _33 * _24 - _11 * _42 * _33 * _24 -
		_31 * _12 * _43 * _24 + _11 * _32 * _43 * _24 +
		_41 * _22 * _13 * _34 - _21 * _42 * _13 * _34 -
		_41 * _12 * _23 * _34 + _11 * _42 * _23 * _34 +
		_21 * _12 * _43 * _34 - _11 * _22 * _43 * _34 -
		_31 * _22 * _13 * _44 + _21 * _32 * _13 * _44 +
		_31 * _12 * _23 * _44 - _11 * _32 * _23 * _44 -
		_21 * _12 * _33 * _44 + _11 * _22 * _33 * _44;
}


/// <summary>
/// Inverse the matrix. WARNING : If the determinant is 0, the function will return without doing anything
/// </summary>

void GCMATRIX::Inverse()
{

	float d = Determinant();

	if (d == 0)
		return;

	d = 1.0f / d;
	GCMATRIX pM = *this;

	_11 = pM._22 * pM._33 * pM._44 * d + pM._23 * pM._34 * pM._42 * d + pM._24 * pM._32 * pM._43 * d - pM._22 * pM._34 * pM._43 * d - pM._23 * pM._32 * pM._44 * d - pM._24 * pM._33 * pM._42 * d;
	_12 = pM._12 * pM._34 * pM._43 * d + pM._13 * pM._32 * pM._44 * d + pM._14 * pM._33 * pM._42 * d - pM._12 * pM._33 * pM._44 * d - pM._13 * pM._34 * pM._42 * d - pM._14 * pM._32 * pM._43 * d;
	_13 = pM._12 * pM._23 * pM._44 * d + pM._13 * pM._24 * pM._42 * d + pM._14 * pM._22 * pM._43 * d - pM._12 * pM._24 * pM._43 * d - pM._13 * pM._22 * pM._44 * d - pM._14 * pM._23 * pM._42 * d;
	_14 = pM._12 * pM._24 * pM._33 * d + pM._13 * pM._22 * pM._34 * d + pM._14 * pM._23 * pM._32 * d - pM._12 * pM._23 * pM._34 * d - pM._13 * pM._24 * pM._32 * d - pM._14 * pM._22 * pM._33 * d;
	_21 = pM._21 * pM._34 * pM._43 * d + pM._23 * pM._31 * pM._44 * d + pM._24 * pM._33 * pM._41 * d - pM._21 * pM._33 * pM._44 * d - pM._23 * pM._34 * pM._41 * d - pM._24 * pM._31 * pM._43 * d;
	_22 = pM._11 * pM._33 * pM._44 * d + pM._13 * pM._34 * pM._41 * d + pM._14 * pM._31 * pM._43 * d - pM._11 * pM._34 * pM._43 * d - pM._13 * pM._31 * pM._44 * d - pM._14 * pM._33 * pM._41 * d;
	_23 = pM._11 * pM._24 * pM._43 * d + pM._13 * pM._21 * pM._44 * d + pM._14 * pM._23 * pM._41 * d - pM._11 * pM._23 * pM._44 * d - pM._13 * pM._24 * pM._41 * d - pM._14 * pM._21 * pM._43 * d;
	_24 = pM._11 * pM._23 * pM._34 * d + pM._13 * pM._24 * pM._31 * d + pM._14 * pM._21 * pM._33 * d - pM._11 * pM._24 * pM._33 * d - pM._13 * pM._21 * pM._34 * d - pM._14 * pM._23 * pM._31 * d;
	_31 = pM._21 * pM._32 * pM._44 * d + pM._22 * pM._34 * pM._41 * d + pM._24 * pM._31 * pM._42 * d - pM._21 * pM._34 * pM._42 * d - pM._22 * pM._31 * pM._44 * d - pM._24 * pM._32 * pM._41 * d;
	_32 = pM._11 * pM._34 * pM._42 * d + pM._12 * pM._31 * pM._44 * d + pM._14 * pM._32 * pM._41 * d - pM._11 * pM._32 * pM._44 * d - pM._12 * pM._34 * pM._41 * d - pM._14 * pM._31 * pM._42 * d;
	_33 = pM._11 * pM._22 * pM._44 * d + pM._12 * pM._24 * pM._41 * d + pM._14 * pM._21 * pM._42 * d - pM._11 * pM._24 * pM._42 * d - pM._12 * pM._21 * pM._44 * d - pM._14 * pM._22 * pM._41 * d;
	_34 = pM._11 * pM._24 * pM._32 * d + pM._12 * pM._21 * pM._34 * d + pM._14 * pM._22 * pM._31 * d - pM._11 * pM._22 * pM._34 * d - pM._12 * pM._24 * pM._31 * d - pM._14 * pM._21 * pM._32 * d;
	_41 = pM._21 * pM._33 * pM._42 * d + pM._22 * pM._31 * pM._43 * d + pM._23 * pM._32 * pM._41 * d - pM._21 * pM._32 * pM._43 * d - pM._22 * pM._33 * pM._41 * d - pM._23 * pM._31 * pM._42 * d;
	_42 = pM._11 * pM._32 * pM._43 * d + pM._12 * pM._33 * pM._41 * d + pM._13 * pM._31 * pM._42 * d - pM._11 * pM._33 * pM._42 * d - pM._12 * pM._31 * pM._43 * d - pM._13 * pM._32 * pM._41 * d;
	_43 = pM._11 * pM._23 * pM._42 * d + pM._12 * pM._21 * pM._43 * d + pM._13 * pM._22 * pM._41 * d - pM._11 * pM._22 * pM._43 * d - pM._12 * pM._23 * pM._41 * d - pM._13 * pM._21 * pM._42 * d;
	_44 = pM._11 * pM._22 * pM._33 * d + pM._12 * pM._23 * pM._31 * d + pM._13 * pM._21 * pM._32 * d - pM._11 * pM._23 * pM._32 * d - pM._12 * pM._21 * pM._33 * d - pM._13 * pM._22 * pM._31 * d;
}


/// <summary>
/// Swap rows with columns of the matrix
/// </summary>

void GCMATRIX::Transpose()
{
	GCMATRIX pM = *this;

	_11 = pM._11;
	_12 = pM._21;
	_13 = pM._31;
	_14 = pM._41;
	_21 = pM._12;
	_22 = pM._22;
	_23 = pM._32;
	_24 = pM._42;
	_31 = pM._13;
	_32 = pM._23;
	_33 = pM._33;
	_34 = pM._43;
	_41 = pM._14;
	_42 = pM._24;
	_43 = pM._34;
	_44 = pM._44;
}

/// <summary>
/// Transformation matrix for moving a point in an XYZ axis system
/// </summary>
/// <param name="x">Translation on the X axis</param>
/// <param name="y">Translation on the Y axis</param>
/// <param name="z">Translation on the Z axis</param>

void GCMATRIX::Translation(float x, float y, float z)
{
	SetIdentity();
	_41 = x;
	_42 = y;
	_43 = z;
}


/// <summary>
/// Transformation matrix for rotating a point around an axis
/// </summary>
/// <param name="axes">Axis of rotation</param>
/// <param name="angle">Angle of rotation</param>

void GCMATRIX::Rotation(GCVEC3 axes, float angle)
{
	SetIdentity();
	float c = cosf(angle);
	float s = sinf(angle);
	float t = 1 - c;

	_11 = t * axes.x * axes.x + c;
	_12 = t * axes.x * axes.y + s * axes.z;
	_13 = t * axes.x * axes.z - s * axes.y;
	_21 = t * axes.x * axes.y - s * axes.z;
	_22 = t * axes.y * axes.y + c;
	_23 = t * axes.y * axes.z + s * axes.x;
	_31 = t * axes.x * axes.z + s * axes.y;
	_32 = t * axes.y * axes.z - s * axes.x;
	_33 = t * axes.z * axes.z + c;
}


/// <summary>
/// Transformation matrix for scaling a point
/// </summary>
/// <param name="x">Scaling on the X axis</param>
/// <param name="y">Scaling on the Y axis</param>
/// <param name="z">Scaling on the Z axis</param>

void GCMATRIX::Scale(float x, float y, float z)
{
	SetIdentity();
	_11 = x;
	_22 = y;
	_33 = z;
}
