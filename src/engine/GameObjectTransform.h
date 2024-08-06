#pragma once
#include "pch.h"

struct GCGameObjectTransform: public GCTransform
{
friend class GCGameObject;
friend class Component;

public:
	void Translate( const GCVEC3& translation );
	void SetPosition( const GCVEC3& position );
	
	void Scale( const GCVEC3& scale );
	void Scale( const float scale );
	void SetScale( const GCVEC3& scale );
	
	void Rotate( float yaw, float pitch, float roll );
	void RotateYaw( float angle );
	void RotatePitch( float angle );
	void RotateRoll( float angle );
	
    void SetRotation( float yaw, float pitch, float roll );
	void SetRotationX( float angle ) {};
	void SetRotationY( float angle ) {}; //! FAIS LE
	void SetRotationZ( float angle ) {};

	GCVEC3& GetWorldPosition();
	GCVEC3& GetWorldScale();
	GCMATRIX& GetWorldRotationMatrix();
	GCMATRIX& GetWorldMatrix();

protected:
	void UpdateWorldMatrixFromParent();
	void UpdateLocalMatrixFromWorld();
    void UpdateChildren();

protected:
	GCVEC3 m_worldScale;
	GCVEC3 m_worldPosition;
	GCMATRIX m_worldRotationMatrix;
	
	GCMATRIX m_worldMatrix;
	GCGameObject* m_pGameObject;
};