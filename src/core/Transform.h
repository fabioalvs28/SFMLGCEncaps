#pragma once
#include "Vectors.h"
#include "Quaternion.h"
#include "Matrix.h"

struct GCTransform
{
	GCVEC3 m_position;
	GCVEC3 m_scale;
	GCQUATERNION m_rotation;
	
	GCMATRIX m_rotationMatrix;
	GCVEC3 m_direction;
	GCVEC3 m_up;
	GCVEC3 m_right;

	GCMATRIX m_matrix;

	GCTransform();
	~GCTransform() {};

	void Identity();
	void FromMatrix(const GCMATRIX& matrix);
	void UpdateMatrixFromVectors();
	void UpdateVectorsFromQuaternion();
	void UpdateVectorsFromMatrix();
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
