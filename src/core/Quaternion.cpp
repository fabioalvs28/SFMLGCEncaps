#include "pch.h"
#include "Quaternion.h"
#include <math.h>

GCQUATERNION::GCQUATERNION(float x, float y, float z, float w)
{
	this->x = x;
	this->y = y;
	this->z = z;
	this->w = w;
}

void GCQUATERNION::operator*=(const GCQUATERNION& other)
{
	GCQUATERNION pQ = *this;

	x = pQ.w * other.x + pQ.x * other.w + pQ.y * other.z - pQ.z * other.y;
	y = pQ.w * other.y + pQ.y * other.w + pQ.z * other.x - pQ.x * other.z;
	z = pQ.w * other.z + pQ.z * other.w + pQ.x * other.y - pQ.y * other.x;
	w = pQ.w * other.w - pQ.x * other.x - pQ.y * other.y - pQ.z * other.z;
}

void GCQUATERNION::SetZero()
{
	x = 0.0f;
	y = 0.0f;
	z = 0.0f;
	w = 0.0f;
}

void GCQUATERNION::SetIdentity()
{
	x = 0.0f;
	y = 0.0f;
	z = 0.0f;
	w = 1.0f;
}

void GCQUATERNION::Normalize()
{
	float norm = sqrt(x * x + y * y + z * z + w * w);

	if (norm > 0.0f)
	{
		float invNorm = 1 / norm;
		x *= invNorm;
		y *= invNorm;
		z *= invNorm;
		w *= invNorm;
	}
}

void GCQUATERNION::Inverse()
{
	x = -x;
	y = -y;
	z = -z;
}

void GCQUATERNION::SLerp(const GCQUATERNION& other, float t)
{
	GCQUATERNION q1 = *this;
	GCQUATERNION q2 = other;

	float cosTheta = q1.x * q2.x + q1.y * q2.y + q1.z * q2.z + q1.w * q2.w;

	if (cosTheta < 0.0f)
	{
		q2.x = -q2.x;
		q2.y = -q2.y;
		q2.z = -q2.z;
		q2.w = -q2.w;
		cosTheta = -cosTheta;
	}

	float k0, k1;

	if (cosTheta > 0.9999f)
	{
		k0 = 1.0f - t;
		k1 = t;
	}
	else
	{
		float sinTheta = sqrt(1.0f - cosTheta * cosTheta);
		float theta = atan2(sinTheta, cosTheta);
		float invSinTheta = 1.0f / sinTheta;

		k0 = sin((1.0f - t) * theta) * invSinTheta;
		k1 = sin(t * theta) * invSinTheta;
	}

	x = k0 * q1.x + k1 * q2.x;
	y = k0 * q1.y + k1 * q2.y;
	z = k0 * q1.z + k1 * q2.z;
	w = k0 * q1.w + k1 * q2.w;
}

