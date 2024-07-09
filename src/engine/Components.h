#pragma once
#include "../core/framework.h"
#include "GCColor.h"


enum FLAGS
{
	UPDATE          = 1 << 0,
	FIXED_UPDATE    = 1 << 1,
    RENDER          = 1 << 2,
};
inline FLAGS operator|(FLAGS a, FLAGS b)
{
	return static_cast<FLAGS>(static_cast<int>(a) | static_cast<int>(b));
}


class GCGameObject;

// TODO Adding lots of stuff to the components
// todo 2 transforms for colliders (self & wold)



class Component
{
friend class GCGameObject;

public: enum { TYPE = 0 };

protected:
	int m_flags;

public:
    Component();
    Component(int flags);
    virtual ~Component() {};
    
    virtual int GetType() = 0;
    
    virtual void Init() = 0;
    virtual void Update() = 0;
    virtual void FixedUpdate() = 0;
    virtual void Render() = 0;
    virtual void Destroy() = 0;
    
    void SetActive( bool active ) { m_active = active; }
    
    bool IsActive() { return m_active; }

    inline GCGameObject* GetGameObject() { return m_pGameObject; }

    inline bool IsFlagSet(FLAGS flag) { return (m_flags & flag) != 0; }

protected:
    void SetGameObject( GCGameObject* pGameObject ) { m_pGameObject = pGameObject; };

protected:
    GCGameObject* m_pGameObject;
    bool m_active;

};



class SpriteRenderer : public Component
{
public: enum { TYPE = 1 };

public:
	SpriteRenderer() : Component(RENDER) {}
    ~SpriteRenderer() override {}
    
    int GetType() override { return TYPE; }
    
    void Init() override {}
    void Update() override {}
    void FixedUpdate() override {}
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
public: enum { TYPE = 2 };

private:
    GCVEC2 m_size;


public:
    ~BoxCollider() override {}
    
    int GetType() override { return TYPE; }
    
	void Init() override {}
    void Update() override {}
    void FixedUpdate() override {}
    void Render() override {}
    void Destroy() override {}

    inline GCVEC2 GetSize() { return m_size; }
    inline void SetSize( GCVEC2 size ) { m_size = size; }

};



class CircleCollider : public Collider
{
public: enum { TYPE = 3 };

private:
    float m_radius;

public:
    ~CircleCollider() override {}
    
    int GetType() override { return TYPE; }

    void Init() override {}
    void Update() override {}
    void FixedUpdate() override {}
    void Render() override {}
    void Destroy() override {}

    inline float GetRadius() { return m_radius; }
    inline void SetRadius( float radius ) { m_radius = radius; }

};



class RigidBody : public Component
{
public: enum { TYPE = 4 };

private:
    GCVEC3 m_velocity;

public:
	RigidBody() : Component(FIXED_UPDATE), m_velocity(0, 0, 0) {}
    ~RigidBody() override {}
    
    int GetType() override { return TYPE; }
    
    void Init() override {}
    void Update() override {}
    void FixedUpdate() override;
    void Render() override {}
    void Destroy() override {}
    
    void AddForce( GCVEC2 force ) {}

};



class Animator : public Component
{
public: enum { TYPE = 5 };

public:
	Animator() : Component(UPDATE) {}
    ~Animator() override {}
    
    int GetType() override { return TYPE; }
    
    void Init() override {}
    void Update() override {}
    void FixedUpdate() override {}
    void Render() override {}
    void Destroy() override {}

};



class SoundMixer : public Component
{
public: enum { TYPE = 6 };

public:
	SoundMixer() : Component(UPDATE) {}
    ~SoundMixer() override {}
    
    int GetType() override { return TYPE; }
    
    void Init() override {}
    void Update() override {}
    void FixedUpdate() override {}
    void Render() override {}
    void Destroy() override {}

};



class Script : public Component
{
public: enum { TYPE = 7 };

public:
	Script() : Component(UPDATE | FIXED_UPDATE) {}
    ~Script() override {};
    
    int GetType() override { return TYPE; }
    
    void Init() override {}
    void Update() override {}
    void FixedUpdate() override {}
    void Render() override {}
    void Destroy() override {}

};