#include "pch.h"
#include "Transform.h"

GCTransform::GCTransform( const GCVEC3& position, const GCVEC3& scale, const GCVEC3& rotation )
{
    m_position = position;
    m_scale = scale;
    m_rotationVector = rotation;
}