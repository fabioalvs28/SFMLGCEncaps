#include "pch.h"
#include "Transform.h"

void GCTransform::Identity()
{
	m_direction = GCVEC3(0, 0, 1);
	m_right = GCVEC3(1, 0, 0);
	m_up = GCVEC3(0, 1, 0);
	m_rotation = GCQUATERNION(0, 0, 0, 1);
	m_rotationMatrix = GCMATRIX::Identity();
	m_position = GCVEC3(0, 0, 0);
	m_matrix = GCMATRIX::Identity();
}

void GCTransform::FromMatrix(const GCMATRIX& matrix)
{
	m_matrix = matrix;
	m_position = GCVEC3(matrix._41, matrix._42, matrix._43);
	UpdateRotationFromVectors();
}

void GCTransform::UpdateRotationFromVectors()
{
	m_rotationMatrix._11 = m_right.x;
	m_rotationMatrix._12 = m_right.y;
	m_rotationMatrix._13 = m_right.z;
	m_rotationMatrix._21 = m_up.x;
	m_rotationMatrix._22 = m_up.y;
	m_rotationMatrix._23 = m_up.z;
	m_rotationMatrix._31 = m_direction.x;
	m_rotationMatrix._32 = m_direction.y;
	m_rotationMatrix._33 = m_direction.z;
	m_rotationMatrix._41 = 0;
	m_rotationMatrix._42 = 0;
	m_rotationMatrix._43 = 0;
	m_rotationMatrix._44 = 1;
}

void GCTransform::UpdateVectorsFromQuaternion()
{
	m_rotationMatrix = m_rotation.ToMatrix();
	m_right = GCVEC3(m_rotationMatrix._11, m_rotationMatrix._12, m_rotationMatrix._13);
	m_up = GCVEC3(m_rotationMatrix._21, m_rotationMatrix._22, m_rotationMatrix._23);
	m_direction = GCVEC3(m_rotationMatrix._31, m_rotationMatrix._32, m_rotationMatrix._33);
}

void GCTransform::UpdateRoationFromMatrix()
{
	m_rotationMatrix.Transpose();
	m_right = GCVEC3(m_rotationMatrix._11, m_rotationMatrix._12, m_rotationMatrix._13);
	m_up = GCVEC3(m_rotationMatrix._21, m_rotationMatrix._22, m_rotationMatrix._23);
	m_direction = GCVEC3(m_rotationMatrix._31, m_rotationMatrix._32, m_rotationMatrix._33);
}

void GCTransform::UpdateMatrix()
{
	m_matrix.SetIdentity();
	m_matrix._11 = m_right.x;
	m_matrix._12 = m_right.y;
	m_matrix._13 = m_right.z;
	m_matrix._21 = m_up.x;
	m_matrix._22 = m_up.y;
	m_matrix._23 = m_up.z;
	m_matrix._31 = m_direction.x;
	m_matrix._32 = m_direction.y;
	m_matrix._33 = m_direction.z;
	m_matrix._41 = m_position.x;
	m_matrix._42 = m_position.y;
	m_matrix._43 = m_position.z;
}

void GCTransform::Rotate(float yaw, float pitch, float roll)
{
	GCQUATERNION q;
	q.FromEuler(yaw, pitch, roll);
	m_rotation *= q;
	m_rotation.Normalize();
	UpdateVectorsFromQuaternion();
}

void GCTransform::RotateYaw(float angle)
{
	GCQUATERNION q;
	q.FromAxisAngle(m_up, angle);
	m_rotation *= q;
	m_rotation.Normalize();
	UpdateVectorsFromQuaternion();
}

void GCTransform::RotatePitch(float angle)
{
	GCQUATERNION q;
	q.FromAxisAngle(m_right, angle);
	m_rotation *= q;
	m_rotation.Normalize();
	UpdateVectorsFromQuaternion();
}

void GCTransform::RotateRoll(float angle)
{
	GCQUATERNION q;
	q.FromAxisAngle(m_direction, angle);
	m_rotation *= q;
	m_rotation.Normalize();
	UpdateVectorsFromQuaternion();
}

void GCTransform::RotateWorld(const GCMATRIX& matrix)
{
	m_rotationMatrix *= matrix;
	UpdateRoationFromMatrix();
}

void GCTransform::RotateWorldX(float angle)
{
	GCMATRIX m;
	m.Rotation(GCVEC3(1, 0, 0), angle);
	RotateWorld(m);
}

void GCTransform::RotateWorldY(float angle)
{
	GCMATRIX m;
	m.Rotation(GCVEC3(0, 1, 0), angle);
	RotateWorld(m);
}

void GCTransform::RotateWorldZ(float angle)
{
	GCMATRIX m;
	m.Rotation(GCVEC3(0, 0, 1), angle);
	RotateWorld(m);
}

void GCTransform::Scale(const GCVEC3& scale)
{
	m_scale = scale;
	m_matrix.Scale(scale.x, scale.y, scale.z);
}

void GCTransform::Translate(const GCVEC3& translation)
{
	m_position += translation;
	m_matrix._41 = m_position.x;
	m_matrix._42 = m_position.y;
	m_matrix._43 = m_position.z;
}

