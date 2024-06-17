#pragma once
#include "Vectors.h"
#include "Matrix.h"

struct GCTransform
{
	GCVEC3 m_scale;
	GCVEC3 m_direction;
	GCVEC3 m_right;
	GCVEC3 m_up;
	GCQUATERNION m_rotation;
	GCMATRIX m_rotationMatrix;

	GCVEC3 m_position;
	GCMATRIX m_matrix;

	GCTransform() {};
	~GCTransform() {};

	void Identity();
	void FromMatrix(const GCMATRIX& matrix);
	void UpdateRotationFromVectors();
	void UpdateVectorsFromQuaternion();
	void UpdateRoationFromMatrix();
	void UpdateMatrix();
	void Rotate(float yaw, float pitch, float roll);
	void RotateYaw(float angle);
	void RotatePitch(float angle);
	void RotateRoll(float angle);
	void RotateWorld(const GCMATRIX& matrix);
	void RotateWorldX(float angle);
	void RotateWorldY(float angle);
	void RotateWorldZ(float angle);
	void Scale(const GCVEC3& scale);
	void Translate(const GCVEC3& translation);
};
