#pragma once
#include "Vectors.h"

class GCTransform
{

public:
    GCTransform() {}
    GCTransform( const GCVEC3& position, const GCVEC3& scale, const GCVEC3& rotation );
    ~GCTransform() {}
    
    GCVEC3& GetPosition() { return m_position; }
    GCVEC3& GetScale() { return m_scale; }
    GCVEC3& GetRotationVector() { return m_rotationVector; }
    
    void SetPosition( const GCVEC3& position );
    void SetScale( const GCVEC3& scale );
    void SetRotation( const GCVEC3& rotation );

protected:
    GCVEC3 m_position;
    GCVEC3 m_scale;
    GCVEC3 m_rotationVector;

};
