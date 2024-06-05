#pragma once



class Component
{

public:
    virtual ~Component() {};
    
    virtual void Init() = 0;
    virtual void Update() = 0;
    virtual void Destroy() = 0;

};



class SpriteRenderer : public Component
{

public:
    ~SpriteRenderer() override {};
    
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

public:
    ~BoxCollider() override {};
    
    void Init() override {};
    void Update() override {};
    void Destroy() override {};

};



class CircleCollider : public Collider
{

public:
    ~CircleCollider() override {};
    
    void Init() override {};
    void Update() override {};
    void Destroy() override {};

};



class RigidBody : public Component
{

public:
    ~RigidBody() override {};
    
    void Init() override {};
    void Update() override {};
    void Destroy() override {};

};



class Animator : public Component
{

public:
    ~Animator() override {};
    
    void Init() override {};
    void Update() override {};
    void Destroy() override {};

};



class SoundMixer : public Component
{

public:
    ~SoundMixer() override {};
    
    void Init() override {};
    void Update() override {};
    void Destroy() override {};

};



class Script : public Component
{

public:
    ~Script() override {};
    
    void Init() override {};
    void Update() override {};
    void Destroy() override {};

};