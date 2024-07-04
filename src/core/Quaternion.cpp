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
GCQUATERNION::GCQUATERNION()
{
	x = 0.0f;
	y = 0.0f;
	z = 0.0f;
	w = 1.0f;
}

GCQUATERNION::GCQUATERNION(float x, float y, float z, float w)
{
	this->x = x;
	this->y = y;
	this->z = z;
	this->w = w;
}


/// <summary>
/// Multiply two quaternions
/// </summary>
/// <param name="other">Quaternion to multiply with</param>
void GCQUATERNION::operator*=(const GCQUATERNION& other)
{
	float qx = x;
	float qy = y;
	float qz = z;
	float qw = w;

	x = qw * other.x + qx * other.w + qy * other.z - qz * other.y;
	y = qw * other.y + qy * other.w + qz * other.x - qx * other.z;
	z = qw * other.z + qz * other.w + qx * other.y - qy * other.x;
	w = qw * other.w - qx * other.x - qy * other.y - qz * other.z;
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
	else
	{
		std::cout << "Zero norm quaternion, setting to identity" << std::endl;
		SetIdentity();
	}
	std::cout << "Quaternion after normalization: (" << x << ", " << y << ", " << z << ", " << w << ")" << std::endl;

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

	float halfYaw = yaw * 0.5f;
	float halfPitch = pitch * 0.5f;
	float halfRoll = roll * 0.5f;

	float cy = cos(halfYaw);
	float sy = sin(halfYaw);
	float cp = cos(halfPitch);
	float sp = sin(halfPitch);
	float cr = cos(halfRoll);
	float sr = sin(halfRoll);

	w = cy * cp * cr + sy * sp * sr;
	x = cy * cp * sr - sy * sp * cr;
	y = sy * cp * cr + cy * sp * sr;
	z = sy * cp * sr - cy * sp * cr;

	Normalize();

	std::cout << "From Euler: (" << x << ", " << y << ", " << z << ", " << w << ")" << std::endl;
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

GCVEC3 GCQUATERNION::ToEuler() const
{
	GCVEC3 euler;

	//Roll (x-axis rotation)
	float sinr_cosp = 2.0f * (w * x + y * z);
	float cosr_cosp = 1.0f - 2.0f * (x * x + y * y);
	euler.x = atan2(sinr_cosp, cosr_cosp);

	//Pitch (y-axis rotation)
	float sinp = 2.0f * (w * y - z * x);
	if (fabs(sinp) >= 1)
		euler.y = copysign(PI / 2.0f, sinp); // Use 90 degrees if out of range
	else
		euler.y = asin(sinp);

	//Yaw (z-axis rotation)
	float siny_cosp = 2.0f * (w * z + x * y);
	float cosy_cosp = 1.0f - 2.0f * (y * y + z * z);
	euler.z = atan2(siny_cosp, cosy_cosp);

	return euler;
}

