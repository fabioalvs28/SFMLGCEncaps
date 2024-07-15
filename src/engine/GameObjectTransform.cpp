#include "pch.h"
#include "GameObjectTransform.h"

#include "GameObject.h"
#include "List.h"



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
		m_worldMatrix = m_matrix;
	
	else
	{
		m_worldMatrix = m_pGameObject->m_pParent->m_transform.m_worldMatrix;
		m_worldMatrix *= m_matrix;
	}
	
	UpdateChildren();
}

void GCGameObjectTransform::UpdateLocalMatrixFromWorld()
{
	if ( m_pGameObject->m_pParent == nullptr )
		m_matrix = m_worldMatrix;
	
	else
	{
		m_matrix = m_pGameObject->m_pParent->m_transform.m_worldMatrix;
		m_matrix.Inverse();
		m_matrix *= m_worldMatrix;
	}
}

void GCGameObjectTransform::UpdateChildren()
{
    GCList<GCGameObject*>& childrenList = m_pGameObject->m_childrenList;
    for ( GCListNode<GCGameObject*>* pChildNode = childrenList.GetFirstNode(); pChildNode != nullptr; pChildNode = pChildNode->GetNext() )
        pChildNode->GetData()->m_transform.UpdateWorldMatrixFromParent();
}