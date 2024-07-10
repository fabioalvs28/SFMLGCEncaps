#pragma once
#include "../core/framework.h"
#include "GCColor.h"

// TODO Adding lots of stuff to the components
// todo 2 transforms for colliders (self & wold)

class GCGameObject;



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

public:
    Component();
    Component( int flags );
    virtual ~Component() = default;
    
    virtual void Update() {}
    virtual void FixedUpdate() {}
    virtual void Render() {}
    virtual void Destroy() = 0;
    
    void SetActive( bool active ) { m_active = active; }
    
    bool IsActive() { return m_active; }

    GCGameObject* GetGameObject() { return m_pGameObject; }

    bool IsFlagSet( FLAGS flag ) { return ( m_flags & flag ) != 0; }

protected:
    static int GetComponentCount() { return ++componentCount; }
    void SetGameObject( GCGameObject* pGameObject ) { m_pGameObject = pGameObject; };

protected:
    inline static int componentCount = 0;
    GCGameObject* m_pGameObject;
    bool m_active;
	int m_flags;

};



class SpriteRenderer : public Component
{
public: static const int GetID() { return m_ID; }
protected: inline static const int m_ID = Component::GetComponentCount();

public:
	SpriteRenderer() : Component( RENDER ) {}
    ~SpriteRenderer() override {}
    
    void Render() override {}
    void Destroy() override {}
    
    void SetSprite() {};
    void SetColor( GCColor& color ) { m_color = color; }
    
    void GetSprite() {};
    GCColor& GetColor() { return m_color; }

protected:
    GCColor m_color;

};



class Collider : public Component
{

public:
    Collider();
    ~Collider();

    void SetTrigger( bool trigger ) { m_trigger = trigger; }
    void SetVisible( bool showCollider ) { m_visible = showCollider; }
    
    bool IsTrigger() { return m_trigger; }
    bool IsVisible() { return m_visible; }

protected:
    bool m_trigger;
    bool m_visible;

};



class BoxCollider : public Collider
{
public: static const int GetID() { return m_ID; }
protected: inline static const int m_ID = Component::GetComponentCount();

private:
    GCVEC2 m_size;


public:
    ~BoxCollider() override {}
    
    void FixedUpdate() override {}
    void Render() override {}
    void Destroy() override {}

    inline GCVEC2 GetSize() { return m_size; }
    inline void SetSize( GCVEC2 size ) { m_size = size; }

};



class CircleCollider : public Collider
{
public: static const int GetID() { return m_ID; }
protected: inline static const int m_ID = Component::GetComponentCount();

private:
    float m_radius;

public:
    ~CircleCollider() override {}

    void FixedUpdate() override {}
    void Render() override {}
    void Destroy() override {}

    inline float GetRadius() { return m_radius; }
    inline void SetRadius( float radius ) { m_radius = radius; }

};



class RigidBody : public Component
{
public: static const int GetID() { return m_ID; }
protected: inline static const int m_ID = Component::GetComponentCount();

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
public: static const int GetID() { return m_ID; }
protected: inline static const int m_ID = Component::GetComponentCount();

public:
	Animator() : Component( UPDATE ) {}
    ~Animator() override {}
    
    void Update() override {}
    void Destroy() override {}

};



class SoundMixer : public Component
{
public: static const int GetID() { return m_ID; }
protected: inline static const int m_ID = Component::GetComponentCount();

public:
	SoundMixer() : Component( UPDATE ) {}
    ~SoundMixer() override {}
    
    void Update() override {}
    void Destroy() override {}

};



class ScriptList : public Component
{
public: static const int GetID() { return m_ID; }
protected: inline static const int m_ID = Component::GetComponentCount();

public:
	ScriptList() {}
    ~ScriptList() override {};
    
    void Destroy() override {}

};