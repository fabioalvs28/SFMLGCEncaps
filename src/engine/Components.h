#pragma once
#include "../core/framework.h"
#include "../Render/pch.h"

#include "GCColor.h"

using namespace DirectX;

// TODO Adding lots of stuff to the components
// todo 2 transforms for colliders (self & wold)
// todo Enable children of components

class GCGameObject;
class GCUpdateManager;
class GCPhysicsManager;
class GCRenderManager;



enum FLAGS
{
	UPDATE          = 1 << 0,
	FIXED_UPDATE    = 1 << 1,
    RENDER          = 1 << 2,
};
inline FLAGS operator|(FLAGS a, FLAGS b) { return static_cast<FLAGS>(static_cast<int>(a) | static_cast<int>(b)); }



class Component
{
friend class GCGameObject;
friend class GCUpdateManager;
friend class GCPhysicsManager;
friend class GCRenderManager;
public: virtual const int GetID() = 0;

public:
    Component();
    Component( int flags );
    virtual ~Component() = default;
    
    void Init();
    virtual void Update() {}
    virtual void FixedUpdate() {}
    virtual void Render() {}
    virtual void Destroy() = 0;
    
    void SetActive( bool active ) { m_active = active; }
    
    bool IsActive() { return m_active; }

    GCGameObject* GetGameObject() { return m_pGameObject; }

    bool IsFlagSet( FLAGS flag ) { return ( m_flags & flag ) != 0; }

protected:
    inline static int componentCount = 0;
    GCGameObject* m_pGameObject;
    bool m_active;
	int m_flags;
    
    GCListNode<Component*>* m_pUpdateNode;
    GCListNode<Component*>* m_pPhysicsNode;
    GCListNode<Component*>* m_pRenderNode;

};



class SpriteRenderer : public Component
{
protected: inline static const int m_ID = ++Component::componentCount;
public:
    static const int GetIDStatic() { return m_ID; }
    const int GetID() { return m_ID; }

public:
    SpriteRenderer();
    ~SpriteRenderer() override; 
    
    void Render() override;
    void Destroy() override {}
    
    
    void SetSprite( std::string texturePath );
    void SetColor(); 
    
    void GetSprite() {};
    GCColor& GetColor() { return m_color; }

protected:

    GCColor m_color;

    GCMesh* m_pMesh;
    GCMaterial* m_pMaterial;

    XMMATRIX* m_worldMatrix;

};



class Collider : public Component
{

public:
    Collider();
    ~Collider() override {}

    void SetTrigger( bool trigger ) { m_trigger = trigger; }
    void SetVisible( bool showCollider ) { m_visible = showCollider; }
    
    bool IsTrigger() { return m_trigger; }
    bool IsVisible() { return m_visible; }

protected:

    bool m_trigger;
    bool m_visible;

    GCMesh* m_pMesh;
    GCMaterial* m_pMaterial;

    XMMATRIX* m_worldMatrix;

};



class BoxCollider : public Collider
{
protected: inline static const int m_ID = ++Component::componentCount;
public:
    static const int GetIDStatic() { return m_ID; }
    const int GetID() { return m_ID; }

private:
    GCVEC2 m_size;


public:
    BoxCollider(); 
    ~BoxCollider() override {}
    
    void FixedUpdate() override {}
    void Render() override;
    void Destroy() override {}

    inline GCVEC2 GetSize() { return m_size; }
    inline void SetSize( GCVEC2 size ) { m_size = size; }

};



class CircleCollider : public Collider
{
protected: inline static const int m_ID = ++Component::componentCount;
public:
    static const int GetIDStatic() { return m_ID; }
    const int GetID() { return m_ID; }

private:
    float m_radius;

public:
    CircleCollider();
    ~CircleCollider() override {}

    void FixedUpdate() override {}
    void Render() override;
    void Destroy() override {}

    inline float GetRadius() { return m_radius; }
    inline void SetRadius( float radius ) { m_radius = radius; }

};



class RigidBody : public Component
{
protected: inline static const int m_ID = ++Component::componentCount;
public:
    static const int GetIDStatic() { return m_ID; }
    const int GetID() { return m_ID; }

private:
    GCVEC3 m_velocity;

public:
	RigidBody() : Component( FIXED_UPDATE ), m_velocity(0, 0, 0) {}
    ~RigidBody() override {}
    
    void FixedUpdate() override;
    void Destroy() override {}
    
    void AddForce( GCVEC2 force ) {}

};



class Animator : public Component
{
protected: inline static const int m_ID = ++Component::componentCount;
public:
    static const int GetIDStatic() { return m_ID; }
    const int GetID() { return m_ID; }

public:
	Animator() : Component( UPDATE ) {}
    ~Animator() override {}
    
    void Update() override {}
    void Destroy() override {}

};



class SoundMixer : public Component
{
protected: inline static const int m_ID = ++Component::componentCount;
public:
    static const int GetIDStatic() { return m_ID; }
    const int GetID() { return m_ID; }

public:
	SoundMixer() : Component( UPDATE ) {}
    ~SoundMixer() override {}
    
    void Update() override {}
    void Destroy() override {}

};



class ScriptList : public Component
{
protected: inline static const int m_ID = ++Component::componentCount;
public:
    static const int GetIDStatic() { return m_ID; }
    const int GetID() { return m_ID; }

public:
	ScriptList() {}
    ~ScriptList() override {};
    
    void Destroy() override {}

};