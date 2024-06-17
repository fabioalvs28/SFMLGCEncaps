#pragma once
class GCMATRIX;
class GCVEC3;


struct GCQUATERNION
{
	float x, y, z, w;

	GCQUATERNION() = default;
	GCQUATERNION(float x, float y, float z, float w);
	~GCQUATERNION() = default;

	static GCQUATERNION Identity() { return GCQUATERNION(0.0f, 0.0f, 0.0f, 1.0f); };
	static GCQUATERNION Zero() { return GCQUATERNION(0.0f, 0.0f, 0.0f, 0.0f); };

	void operator*=(const GCQUATERNION& other);
	void SetZero();
	void SetIdentity();
	void Normalize();
	void Inverse();
	void SLerp(const GCQUATERNION& other, float t);
	void FromEuler(float yaw, float pitch, float roll);
	void FromAxisAngle(const GCVEC3& axis, float angle);
	GCMATRIX ToMatrix();

};