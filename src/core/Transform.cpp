#include "pch.h"
#include "Transform.h"

GCTransform::GCTransform()
{
	Identity();
}

void GCTransform::Identity()
{
	m_direction.SetFront();
	m_right.SetRight();
	m_up.SetUp();
	m_rotation.SetIdentity();
	m_rotationMatrix.SetIdentity();
	
	m_position.SetZero();
	m_scale.SetOne();
	
	m_matrix.SetIdentity();
}

void GCTransform::IdentityRotation()
{
	m_direction.SetFront();
	m_right.SetRight();
	m_up.SetUp();
	m_rotation.SetIdentity();
	m_rotationMatrix.SetIdentity();
}

void GCTransform::FromMatrix(const GCMATRIX& matrix)
{
	m_matrix = matrix;
	m_position = GCVEC3(matrix._41, matrix._42, matrix._43);
	UpdateMatrixFromVectors();
}

void GCTransform::UpdateMatrixFromVectors()
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
	GCQUATERNION qYaw;
	qYaw.FromAxisAngle(m_up, yaw);

	GCQUATERNION qPitch;
	qPitch.FromAxisAngle(m_right, pitch);

	GCQUATERNION qRoll;
	qRoll.FromAxisAngle(m_direction, roll);

	GCQUATERNION qResult = qRoll;

	qResult *= qPitch;
	qResult *= qYaw;

	m_rotation *= qResult;

	UpdateVectorsFromQuaternion();
	UpdateMatrixFromVectors();
	UpdateMatrix();
}

void GCTransform::RotateYaw(float angle)
{
	Rotate(angle, 0, 0);
}

void GCTransform::RotatePitch(float angle)
{
	Rotate(0, angle, 0);
}

void GCTransform::RotateRoll(float angle)
{
	Rotate(0, 0, angle);
}

void GCTransform::SetRotation(float yaw, float pitch, float roll)
{
	IdentityRotation();
	Rotate(yaw, pitch, roll);
}

void GCTransform::SetRotationX(float angle)
{
	SetRotation(angle, 0, 0);
}

void GCTransform::SetRotationY(float angle)
{
	SetRotation(0, angle, 0);
}

void GCTransform::SetRotationZ(float angle)
{
	SetRotation(0, 0, angle);
}

void GCTransform::Translate(const GCVEC3& translation)
{
	m_position += translation;
	m_matrix._41 = m_position.x;
	m_matrix._42 = m_position.y;
	m_matrix._43 = m_position.z;
}

void GCTransform::SetPosition(const GCVEC3& position)
{
	m_position = position;
	m_matrix._41 = m_position.x;
	m_matrix._42 = m_position.y;
	m_matrix._43 = m_position.z;
}

void GCTransform::Scale(const GCVEC3& scale)
{
	m_scale *= scale;
	m_matrix._11 *= m_scale.x;
	m_matrix._22 *= m_scale.y;
	m_matrix._33 *= m_scale.z;
}

void GCTransform::Scale(const float scale)
{
	m_scale *= scale;
	m_matrix._11 *= m_scale.x;
	m_matrix._22 *= m_scale.y;
	m_matrix._33 *= m_scale.z;
}

void GCTransform::SetScale(const GCVEC3& scale)
{
	m_scale = scale;
	m_matrix._11 = m_scale.x;
	m_matrix._22 = m_scale.y;
	m_matrix._33 = m_scale.z;
}