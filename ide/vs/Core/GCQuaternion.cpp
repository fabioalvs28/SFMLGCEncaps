#include "pch.h"
#include "GCQuaternion.h"
#include <math.h>

GCQuaternion::GCQuaternion(float _x, float _y, float _z, float _w)
{
	m_X = _x;
	m_Y = _y;
	m_Z = _z;
	m_W = _w;
}

void GCQuaternion::operator*=(const GCQuaternion& other)
{
	GCQuaternion pQ = *this;

	m_X = pQ.m_W * other.m_X + pQ.m_X * other.m_W + pQ.m_Y * other.m_Z - pQ.m_Z * other.m_Y;
	m_Y = pQ.m_W * other.m_Y + pQ.m_Y * other.m_W + pQ.m_Z * other.m_X - pQ.m_X * other.m_Z;
	m_Z = pQ.m_W * other.m_Z + pQ.m_Z * other.m_W + pQ.m_X * other.m_Y - pQ.m_Y * other.m_X;
	m_W = pQ.m_W * other.m_W - pQ.m_X * other.m_X - pQ.m_Y * other.m_Y - pQ.m_Z * other.m_Z;
}

void GCQuaternion::SetZero()
{
	m_X = 0.0f;
	m_Y = 0.0f;
	m_Z = 0.0f;
	m_W = 0.0f;
}

void GCQuaternion::SetIdentity()
{
	m_X = 0.0f;
	m_Y = 0.0f;
	m_Z = 0.0f;
	m_W = 1.0f;
}

void GCQuaternion::Normalize()
{
	float norm = sqrt(m_X * m_X + m_Y * m_Y + m_Z * m_Z + m_W * m_W);

	if (norm > 0.0f)
	{
		float invNorm = 1 / norm;
		m_X *= invNorm;
		m_Y *= invNorm;
		m_Z *= invNorm;
		m_W *= invNorm;
	}
}

void GCQuaternion::Inverse()
{
	m_X = -m_X;
	m_Y = -m_Y;
	m_Z = -m_Z;
}

void GCQuaternion::SLerp(const GCQuaternion& other, float t)
{
	GCQuaternion q1 = *this;
	GCQuaternion q2 = other;

	float cosTheta = q1.m_X * q2.m_X + q1.m_Y * q2.m_Y + q1.m_Z * q2.m_Z + q1.m_W * q2.m_W;

	if (cosTheta < 0.0f)
	{
		q2.m_X = -q2.m_X;
		q2.m_Y = -q2.m_Y;
		q2.m_Z = -q2.m_Z;
		q2.m_W = -q2.m_W;
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

	m_X = k0 * q1.m_X + k1 * q2.m_X;
	m_Y = k0 * q1.m_Y + k1 * q2.m_Y;
	m_Z = k0 * q1.m_Z + k1 * q2.m_Z;
	m_W = k0 * q1.m_W + k1 * q2.m_W;
}

