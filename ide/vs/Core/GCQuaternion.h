#pragma once

struct GCQuaternion
{
	float m_X, m_Y, m_Z, m_W;

	GCQuaternion();
	GCQuaternion(float _x, float _y, float _z, float _w);
	~GCQuaternion() = default;

	static GCQuaternion Identity() { return GCQuaternion(0.0f, 0.0f, 0.0f, 1.0f); };
	static GCQuaternion Zero() { return GCQuaternion(0.0f, 0.0f, 0.0f, 0.0f); };

	void operator*=(const GCQuaternion& other);
	void SetZero();
	void SetIdentity();
	void Normalize();
	void Inverse();
	void SLerp(const GCQuaternion& other, float t);

};