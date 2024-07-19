#include "pch.h"
#include "GameObjectTransform.h"

#include "GameObject.h"
#include "List.h"

// TODO AddChild doesn't use worldMatrix to recalculate the matrix and instead uses the matrix to calculate the worldMatrix



void GCGameObjectTransform::Translate( const GCVEC3& translation )
{
	GCTransform::Translate( translation );
	UpdateWorldMatrixFromParent();
}

void GCGameObjectTransform::SetPosition( const GCVEC3& position )
{
	GCTransform::SetPosition( position );
	UpdateWorldMatrixFromParent();
}



void GCGameObjectTransform::Scale( const GCVEC3& scale )
{
	GCTransform::Scale( scale );
	UpdateWorldMatrixFromParent();
}

void GCGameObjectTransform::Scale( const float scale )
{
	GCTransform::Scale( scale );
	UpdateWorldMatrixFromParent();
}

void GCGameObjectTransform::SetScale( const GCVEC3& scale )
{
	GCTransform::SetScale( scale );
	UpdateWorldMatrixFromParent();
}



void GCGameObjectTransform::Rotate(float yaw, float pitch, float roll)
{
	GCTransform::Rotate( yaw, pitch, roll );
	UpdateWorldMatrixFromParent();
}

void GCGameObjectTransform::RotateYaw( float angle )
{
    GCTransform::RotateYaw( angle );
	UpdateWorldMatrixFromParent();
}

void GCGameObjectTransform::RotatePitch( float angle )
{
    GCTransform::RotatePitch( angle );
    UpdateWorldMatrixFromParent();
}

void GCGameObjectTransform::RotateRoll( float angle )
{
    GCTransform::RotateRoll( angle );
    UpdateWorldMatrixFromParent();
}



void GCGameObjectTransform::SetRotation( float yaw, float pitch, float roll )
{
	GCTransform::SetRotation( yaw, pitch, roll );
	UpdateWorldMatrixFromParent();
}



void GCGameObjectTransform::UpdateWorldMatrixFromParent()
{
	if ( m_pGameObject->m_pParent == nullptr )
	{
		m_worldPosition = m_position;
		m_worldScale = m_scale;
		m_worldRotationMatrix = m_rotationMatrix;
		m_worldMatrix = m_matrix;
	}
	
	else
	{
		GCGameObjectTransform* pParentTransform = &m_pGameObject->m_pParent->m_transform;
		
		m_worldPosition = pParentTransform->m_worldPosition;
		m_worldPosition += m_position;
		
		m_worldScale = pParentTransform->m_worldScale;
		m_worldScale *= m_scale;
		
		m_worldRotationMatrix = pParentTransform->m_worldRotationMatrix;
		m_worldRotationMatrix *= m_rotationMatrix;
		
		m_worldMatrix = m_worldRotationMatrix;
		
		m_worldMatrix._11 *= m_worldScale.x;
		m_worldMatrix._22 *= m_worldScale.y;
		m_worldMatrix._33 *= m_worldScale.z;
		
		m_worldMatrix._41 = m_worldPosition.x;
		m_worldMatrix._42 = m_worldPosition.y;
		m_worldMatrix._43 = m_worldPosition.z;
	}
	
	UpdateChildren();
}

void GCGameObjectTransform::UpdateLocalMatrixFromWorld()
{
	if ( m_pGameObject->m_pParent == nullptr )
	{
		m_position = m_worldPosition;
		m_scale = m_worldScale;
		m_rotationMatrix = m_worldRotationMatrix;
		m_matrix = m_worldMatrix;
	}
	
	else
	{
		GCGameObjectTransform* pParentTransform = &m_pGameObject->m_pParent->m_transform;
		
		m_worldPosition -= pParentTransform->m_worldPosition;
		m_worldScale /= pParentTransform->m_worldScale; // TODO Prevent User from making scale.x = 0 or scale.y = 0 or scale.z = 0
		
		GCMATRIX tempWorldRotationMatrix = m_worldRotationMatrix;
		m_worldRotationMatrix = pParentTransform->m_worldRotationMatrix;
		m_worldRotationMatrix.Inverse();
		m_worldRotationMatrix *= tempWorldRotationMatrix;
		
		m_matrix = m_worldRotationMatrix;
		
        m_matrix._11 *= m_worldScale.x;
		m_matrix._22 *= m_worldScale.y;
		m_matrix._33 *= m_worldScale.z;
		
        m_matrix._41 = m_worldPosition.x;
		m_matrix._42 = m_worldPosition.y;
		m_matrix._43 = m_worldPosition.z;
	}
}

void GCGameObjectTransform::UpdateChildren()
{
    GCList<GCGameObject*>& childrenList = m_pGameObject->m_childrenList;
    for ( GCListNode<GCGameObject*>* pChildNode = childrenList.GetFirstNode(); pChildNode != nullptr; pChildNode = pChildNode->GetNext() )
        pChildNode->GetData()->m_transform.UpdateWorldMatrixFromParent();
}