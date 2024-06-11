#pragma once

struct GCQUATERNION
{
	float m_X, m_Y, m_Z, m_W;

	GCQUATERNION();
	GCQUATERNION(float _x, float _y, float _z, float _w);
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