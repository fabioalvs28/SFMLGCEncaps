#pragma once
#include "GameObject.h"



class Component
{

public:
    virtual ~Component() {};
    
    virtual void Init() = 0;
    virtual void Update() = 0;
    virtual void Destroy() = 0;
    
    void SetGameObject( GameObject* pGameObject );

protected:
    GameObject* m_pGameObject;

};



class SpriteRenderer : public Component
{
public: enum { TYPE = 1 };

public:
    ~SpriteRenderer() override {};
    
    static int GetType() { return TYPE; };
    
    void Init() override {};
    void Update() override {};
    void Destroy() override {};

};



class Collider : public Component
{

public:
    ~Collider() override {};

};



class BoxCollider : public Collider
{
public: enum { TYPE = 2 };

public:
    ~BoxCollider() override {};
    
    static int GetType() { return TYPE; };
    
    void Init() override {};
    void Update() override {};
    void Destroy() override {};

};



class CircleCollider : public Collider
{
public: enum { TYPE = 3 };

public:
    ~CircleCollider() override {};
    
    static int GetType() { return TYPE; };
    
    void Init() override {};
    void Update() override {};
    void Destroy() override {};

};



class RigidBody : public Component
{
public: enum { TYPE = 4 };

public:
    ~RigidBody() override {};
    
    static int GetType() { return TYPE; };
    
    void Init() override {};
    void Update() override {};
    void Destroy() override {};

};



class Animator : public Component
{
public: enum { TYPE = 5 };

public:
    ~Animator() override {};
    
    static int GetType() { return TYPE; };
    
    void Init() override {};
    void Update() override {};
    void Destroy() override {};

};



class SoundMixer : public Component
{
public: enum { TYPE = 6 };

public:
    ~SoundMixer() override {};
    
    static int GetType() { return TYPE; };
    
    void Init() override {};
    void Update() override {};
    void Destroy() override {};

};



class Script : public Component
{
public: enum { TYPE = 7 };

public:
    ~Script() override {};
    
    static int GetType() { return TYPE; };
    
    void Init() override {};
    void Update() override {};
    void Destroy() override {};

};