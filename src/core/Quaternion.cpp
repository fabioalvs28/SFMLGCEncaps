#include "pch.h"
#include "Quaternion.h"

#include <math.h>
#include "Vectors.h"
#include "Matrix.h"

/// <summary>
/// Default constructor
/// </summary>
/// <param name="x">X value of the quaternion</param>
/// <param name="y">Y value of the quaternion</param>
/// <param name="z">Z value of the quaternion</param>
/// <param name="w">W value of the quaternion</param>
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

/// <summary>
/// Set the quaternion to zero
/// </summary>
void GCQUATERNION::SetZero()
{
	x = 0.0f;
	y = 0.0f;
	z = 0.0f;
	w = 0.0f;
}

/// <summary>
/// Set the quaternion to identity
/// </summary>
void GCQUATERNION::SetIdentity()
{
	x = 0.0f;
	y = 0.0f;
	z = 0.0f;
	w = 1.0f;
}

/// <summary>
/// Normalize the quaternion
/// </summary>
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

/// <summary>
/// Inverse the quaternion
/// </summary>
void GCQUATERNION::Inverse()
{
	x = -x;
	y = -y;
	z = -z;
}

/// <summary>
/// Spherical linear interpolation between two quaternions
/// </summary>
/// <param name="other">Quaternion to interpolate with</param>
/// <param name="t">Interpolation value</param>
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

/// <summary>
/// Create a quaternion from euler angles
/// </summary>
/// <param name="yaw">Yaw angle</param>
/// <param name="pitch">Pitch angle</param>
/// <param name="roll">Roll angle</param>
void GCQUATERNION::FromEuler(float yaw, float pitch, float roll)
{
	float cy = cos(yaw * 0.5f);
	float sy = sin(yaw * 0.5f);
	float cp = cos(pitch * 0.5f);
	float sp = sin(pitch * 0.5f);
	float cr = cos(roll * 0.5f);
	float sr = sin(roll * 0.5f);

	w = cy * cp * cr + sy * sp * sr;
	x = cy * cp * sr - sy * sp * cr;
	y = sy * cp * sr + cy * sp * cr;
	z = sy * cp * cr - cy * sp * sr;
}



/// <summary>
/// Create a quaternion from an axis and an angle
/// </summary>
/// <param name="axis">Axis of rotation</param>
/// <param name="angle">Angle of rotation</param>
void GCQUATERNION::FromAxisAngle(const GCVEC3& axis, float angle)
{
	float halfAngle = angle * 0.5f;
	float s = sin(halfAngle);

	x = axis.x * s;
	y = axis.y * s;
	z = axis.z * s;
	w = cos(halfAngle);
}

/// <summary>
/// Convert the quaternion to a matrix
/// </summary>
/// <returns>Matrix representation of the quaternion</returns>
GCMATRIX GCQUATERNION::ToMatrix()
{
	GCQUATERNION q = *this;
	GCMATRIX m;

	float x2 = q.x + q.x;
	float y2 = q.y + q.y;
	float z2 = q.z + q.z;
	float xx = q.x * x2;
	float xy = q.x * y2;
	float xz = q.x * z2;
	float yy = q.y * y2;
	float yz = q.y * z2;
	float zz = q.z * z2;
	float wx = q.w * x2;
	float wy = q.w * y2;
	float wz = q.w * z2;

	m._11 = 1.0f - (yy + zz);
	m._12 = xy - wz;
	m._13 = xz + wy;
	m._14 = 0.0f;
	 
	m._21 = xy + wz;
	m._22 = 1.0f - (xx + zz);
	m._23 = yz - wx;
	m._24 = 0.0f;
	 
	m._31 = xz - wy;
	m._32 = yz + wx;
	m._33 = 1.0f - (xx + yy);
	m._34 = 0.0f;
	 
	m._41 = 0.0f;
	m._42 = 0.0f;
	m._43 = 0.0f;
	m._44 = 1.0f;

	return m;
}

