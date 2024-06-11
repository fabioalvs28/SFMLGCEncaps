#pragma once

class GCGameObject;



class Component
{
friend class GCGameObject;

public:
    virtual ~Component() {};
    
    virtual int GetType() = 0;
    
    virtual void Init() = 0;
    virtual void Update() = 0;
    virtual void Destroy() = 0;

protected:
    GCGameObject* m_pGameObject;
    void SetGameObject( GCGameObject* pGameObject ) { m_pGameObject = pGameObject; };

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