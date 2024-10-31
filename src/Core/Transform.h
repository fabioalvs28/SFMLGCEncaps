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
	void IdentityRotation();
	
	void FromMatrix(const GCMATRIX& matrix);
	void UpdateMatrixFromVectors();
	void UpdateVectorsFromQuaternion();
	void UpdateMatrix();
	
	void Translate(const GCVEC3& translation);
	void SetPosition( const GCVEC3& position );
	
	void Scale(const GCVEC3& scale);
	void Scale(const float scale);
	void SetScale(const GCVEC3& scale);
	
	void Rotate(float yaw, float pitch, float roll);
	void RotateYaw(float angle);
	void RotatePitch(float angle);
	void RotateRoll(float angle);
	
	void SetRotation(float yaw, float pitch, float roll); //! RotationWorld
	void SetRotationX(float angle);
	void SetRotationY(float angle);
	void SetRotationZ(float angle);
};