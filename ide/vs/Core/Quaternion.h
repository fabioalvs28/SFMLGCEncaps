#pragma once

struct GCQUATERNION
{
	float x, y, z, w;

	GCQUATERNION();
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

};