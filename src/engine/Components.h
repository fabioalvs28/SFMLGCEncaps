#pragma once
#include "../core/framework.h"

class GCGameObject;
struct GCVEC2;

// TODO Adding lots of stuff to the components



class Component
{
friend class GCGameObject;

public: enum { TYPE = 0 };

public:
    virtual ~Component() {};
    
    virtual int GetType() = 0;
    
    virtual void Init() = 0;
    virtual void Update() = 0;
    virtual void Destroy() = 0;
    
    void SetActive( bool active ) { m_active = active; }
    
    bool IsActive() { return m_active; }

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
    ~SpriteRenderer() override {}
    
    int GetType() override { return TYPE; }
    
    void Init() override {};
    void Update() override {};
    void Destroy() override {};
    
    void Render() {}
    
    void SetSprite();
    void SetColor( GCColor& color ) { m_color = color; }
    
    void GetSprite();
    GCColor& GetColor() { return m_color; }

protected:
    GCColor m_color;

};



class Collider : public Component
{

public:
    Collider();
    ~Collider() override {}
    
    void SetTrigger( bool trigger ) { m_trigger = trigger; }
    
    bool IsTrigger() { return m_trigger; }

protected:
    bool m_trigger;

};



class BoxCollider : public Collider
{
public: enum { TYPE = 2 };

public:
    ~BoxCollider() override {}
    
    int GetType() override { return TYPE; }
    
    void Init() override {}
    void Update() override {}
    void Destroy() override {}

};



class CircleCollider : public Collider
{
public: enum { TYPE = 3 };

public:
    ~CircleCollider() override {}
    
    int GetType() override { return TYPE; }
    
    void Init() override {}
    void Update() override {}
    void Destroy() override {}

};



class RigidBody : public Component
{
public: enum { TYPE = 4 };

public:
    ~RigidBody() override {}
    
    int GetType() override { return TYPE; }
    
    void Init() override {}
    void Update() override {}
    void Destroy() override {}
    
    void AddForce( GCVEC2 force ) {}

};



class Animator : public Component
{
public: enum { TYPE = 5 };

public:
    ~Animator() override {}
    
    int GetType() override { return TYPE; }
    
    void Init() override {}
    void Update() override {}
    void Destroy() override {}

};



class SoundMixer : public Component
{
public: enum { TYPE = 6 };

public:
    ~SoundMixer() override {}
    
    int GetType() override { return TYPE; }
    
    void Init() override {}
    void Update() override {}
    void Destroy() override {}

};



class Script : public Component
{
public: enum { TYPE = 7 };

public:
    ~Script() override {};
    
    int GetType() override { return TYPE; }
    
    void Init() override {}
    void Update() override {}
    void Destroy() override {}

};