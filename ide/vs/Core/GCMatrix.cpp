#include "pch.h"
#include "GCMatrix.h"

GCMatrix::GCMatrix() { SetZero(); }


void GCMatrix::operator*=(const GCMatrix& other)
{
	GCMatrix pM = *this;

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

void GCMatrix::SetZero()
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

void GCMatrix::SetIdentity()
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

void GCMatrix::Pow(int exp)
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

float GCMatrix::Determinant()
{
	return _11 * _21 * _33 * _44 - _11 * _22 * _34 * _43 - _11 * _23 * _32 * _44 + _11 * _23 * _34 * _42 + _11 * _24 * _32 * _43 - _11 * _24 * _33 * _42
		- _12 * _21 * _33 * _44 + _12 * _21 * _34 * _43 + _12 * _23 * _31 * _44 - _12 * _23 * _34 * _41 - _12 * _24 * _31 * _43 + _12 * _24 * _33 * _41
		+ _13 * _21 * _32 * _44 - _13 * _21 * _34 * _41 - _13 * _22 * _31 * _44 + _13 * _22 * _34 * _41 + _13 * _24 * _31 * _42 - _13 * _24 * _32 * _41
		- _14 * _21 * _32 * _43 + _14 * _21 * _33 * _42 + _14 * _22 * _31 * _43 - _14 * _22 * _33 * _41 - _14 * _23 * _31 * _42 + _14 * _23 * _32 * _41;
}


void GCMatrix::Inverse()
{

	float d = Determinant();

	if (d == 0)
		return;

	d = 1.0f / d;
	GCMatrix pM = *this;

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

void GCMatrix::Transpose()
{
	GCMatrix pM = *this;

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

void GCMatrix::Translation(float x, float y, float z)
{
	SetIdentity();
	_41 = x;
	_42 = y;
	_43 = z;
}

/*void GCMatrix::Rotation()
{
	
}*/

void GCMatrix::Scale(float x, float y, float z)
{
	SetIdentity();
	_11 = x;
	_22 = y;
	_33 = z;
}
