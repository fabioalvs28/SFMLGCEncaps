#pragma once
#include "pch.h"
#include "../Render/pch.h"

using namespace DirectX;

// TODO Adding lots of stuff to the components
// TODO Transforms for colliders
// TODO Make sure IDs are handled differently
// TODO À faire dans le Duplicate() : Start()

class Animation;

enum FLAGS
{
    NONE            = 0,
	UPDATE          = 1 << 0,
	FIXED_UPDATE    = 1 << 1,
    RENDER          = 1 << 2,
};
inline FLAGS operator|( FLAGS a, FLAGS b ) { return static_cast<FLAGS>(static_cast<int>(a) | static_cast<int>(b)); }



class Component
{
friend class GCGameObject;
friend class GCUpdateManager;
friend class GCSceneManager;
friend class GCPhysicManager;
friend class GCRenderManager;

public:
    virtual const int GetID() = 0;
    
    void Activate();
    void Deactivate();
    void SetActive( bool active );
    bool IsActive() { return m_globalActive && m_selfActive; }

    GCGameObject* GetGameObject() { return m_pGameObject; }

protected:
    Component();
    virtual ~Component() = default;
    
    virtual void RegisterToManagers();
    virtual void UnregisterFromManagers();
    
    virtual Component* Duplicate() = 0;
    virtual void CopyTo( Component *pDestination );
    
    virtual void Start() {}
    virtual void Update() {}
    virtual void FixedUpdate() {}
    virtual void Render() {}
    virtual void OnActivate() {}
    virtual void OnDeactivate() {}
    virtual void Destroy() {}
    
    virtual FLAGS GetFlags() = 0;
    bool IsFlagSet( FLAGS flag ) { return ( GetFlags() & flag ) != 0; }
    
    void ActivateGlobal();
    void DeactivateGlobal();

    virtual int GetComponentLayer() { return 0; }

protected:
    inline static int componentCount = 0;
    GCGameObject* m_pGameObject;
    bool m_globalActive;
    bool m_selfActive;
    
    bool m_created;
    
    GCListNode<Component*>* m_pUpdateNode;
    GCListNode<Component*>* m_pPhysicsNode;
    GCListNode<Component*>* m_pRenderNode;

};



class SpriteRenderer : public Component
{
friend class GCGameObject;
friend class GCUpdateManager;
friend class GCSceneManager;
friend class GCPhysicManager;
friend class GCRenderManager;
friend class Animator;

public:
    static const int GetIDStatic() { return m_ID; }
    const int GetID() override { return m_ID; }
    
    void SetSprite( std::string fileName);
    
    void GetSprite() {};

protected:
	SpriteRenderer();
    ~SpriteRenderer() override {}

    SpriteRenderer* Duplicate() override { return new SpriteRenderer(); }
    void CopyTo( Component* pDestination ) override;
    
    void Render() override;
    
    FLAGS GetFlags() override { return RENDER; }

    int GetComponentLayer() override { return 5; }

    void SetAnimatedSprite(GCGeometry* pGeometry, GCTexture* pTexture);

protected:
    inline static const int m_ID = ++Component::componentCount;

    GCGeometry* m_pGeometry;
    GCMesh* m_pMesh;
    GCMaterial* m_pMaterial;

};



class Collider : public Component
{
friend class GCGameObject;
friend class GCUpdateManager;
friend class GCSceneManager;
friend class GCPhysicManager;
friend class GCRenderManager;

public:
    Collider();
    ~Collider() override {}
    
    void RegisterToManagers() override;
    void UnregisterFromManagers() override;
    
    void CopyTo( Component* pDestination ) override;

    void SetTrigger( bool trigger ) { m_trigger = trigger; }
    void SetVisible( bool showCollider ) { m_visible = showCollider; }
    
    bool IsTrigger() { return m_trigger; }
    bool IsVisible() { return m_visible; }

protected:
    FLAGS GetFlags() override { return FIXED_UPDATE | RENDER; }

    virtual int GetComponentLayer() override { return 10; }

protected:
    bool m_trigger;
    bool m_visible;
    
    GCListNode<Collider*>* m_pColliderNode;

    GCGeometry* m_pGeometry;
    GCMesh* m_pMesh;
    GCMaterial* m_pMaterial;
};



class BoxCollider : public Collider
{
friend class GCGameObject;
friend class GCUpdateManager;
friend class GCSceneManager;
friend class GCPhysicManager;
friend class GCRenderManager;

public:
    static const int GetIDStatic() { return m_ID; }
    const int GetID() override { return m_ID; }

protected:
    BoxCollider();
    ~BoxCollider() override {}

    BoxCollider* Duplicate() override { return new BoxCollider(); }
    void CopyTo( Component* pDestination ) override;
    
    void Render() override;

protected:
    inline static const int m_ID = ++Component::componentCount;

};



class CircleCollider : public Collider
{
friend class GCGameObject;
friend class GCUpdateManager;
friend class GCSceneManager;
friend class GCPhysicManager;
friend class GCRenderManager;

public:
    static const int GetIDStatic() { return m_ID; }
    const int GetID() override { return m_ID; }

protected:
    CircleCollider() {}
    ~CircleCollider() override {}
    
    CircleCollider* Duplicate() override { return new CircleCollider(); }
    void CopyTo( Component* pDestination ) override;

protected:
    inline static const int m_ID = ++Component::componentCount;

};



class RigidBody : public Component
{
friend class GCGameObject;
friend class GCUpdateManager;
friend class GCSceneManager;
friend class GCPhysicManager;
friend class GCRenderManager;

public:
    static const int GetIDStatic() { return m_ID; }
    const int GetID() override { return m_ID; }
    
    void AddForce( GCVEC2 force ) {}

protected:
    RigidBody();
    ~RigidBody() override {}
    
    RigidBody* Duplicate() override { return new RigidBody(); }
    void CopyTo( Component* pDestination ) override;
    
    void FixedUpdate() override;
    
    FLAGS GetFlags() override { return FIXED_UPDATE; }

protected:
    inline static const int m_ID = ++Component::componentCount;
    GCVEC3 m_velocity;

};



class Animator : public Component
{
    friend class GCGameObject;
    friend class GCUpdateManager;
    friend class GCSceneManager;
    friend class GCPhysicManager;
    friend class GCRenderManager;

public:
    static const int GetIDStatic() { return m_ID; }
    const int GetID() override { return m_ID; }

    void PlayAnimation( std::string animationName, bool isLoop );
    void StopAnimation();

    void LoadSpriteSheet( std::string fileName ,int spriteSheetID );
    Animation* CreateAnimation( std::string animationName , int firstFrame , int frameNumber , float frameDisplayTime = 0.1f );
    Animation* CreateAnimationWithCustomFrames( std::string animationName , std::vector<int> frameList , float frameDisplayTime = 0.1f );

    std::string GetActiveAnimation() { return m_activeAnimationName; }

protected:
    Animator();
    ~Animator() override {}

    Animator* Duplicate() override { return new Animator(); }
    void CopyTo( Component* pDestination ) override;
    
    void Start() override;
    void Update() override;
    
    FLAGS GetFlags() override { return UPDATE; }

protected:
    inline static const int m_ID = ++Component::componentCount;

private:
    std::string m_spritesheetName;
    SpriteRenderer* m_pSpriteRenderer;
    int m_spriteSheetID;
    std::string m_activeAnimationName;
    Animation* m_currentAnimation;

    bool m_isLoop;
    int m_lastFrameIndex;
    int m_currentFrameIndex;
    float m_currentFrameTime;
};



class SoundMixer : public Component
{
friend class GCGameObject;
friend class GCUpdateManager;
friend class GCSceneManager;
friend class GCPhysicManager;
friend class GCRenderManager;

public:
    static const int GetIDStatic() { return m_ID; }
    const int GetID() override { return m_ID; }

protected:
	SoundMixer() {}
    ~SoundMixer() override {}
    
    SoundMixer* Duplicate() override { return new SoundMixer(); }
    void CopyTo( Component* pDestination ) override;
    
    void Update() override {}
    
    FLAGS GetFlags() override { return UPDATE; }

protected:
    inline static const int m_ID = ++Component::componentCount;

};



class Camera : public Component
{
friend class GCGameObject;
friend class GCUpdateManager;
friend class GCSceneManager;
friend class GCPhysicManager;
friend class GCRenderManager;

public:
    static const int GetIDStatic() { return m_ID; }
    const int GetID() override { return m_ID; }

protected:
    Camera();
    ~Camera() override {}
    
    Camera* Duplicate() override { return new Camera(); }
    void CopyTo( Component* pDestination ) override;
    
    void Update() override;
    
    FLAGS GetFlags() override { return NONE; }

protected:
    inline static const int m_ID = ++Component::componentCount;
    
    GCVEC3 m_position;
    GCVEC3 m_target;
    GCVEC3 m_up;
    
    float m_nearZ;
    float m_farZ;
    float m_viewWidth;
    float m_viewHeight;
    
    GCMATRIX m_viewMatrix;
    GCMATRIX m_projectionMatrix;

};



class Script : public Component
{
friend class GCGameObject;
friend class GCScene;
friend class GCUpdateManager;
friend class GCSceneManager;
friend class GCPhysicManager;
friend class GCRenderManager;
friend class GCGameManager;

protected:
    Script() {}
    virtual ~Script() = default;
    
    virtual void OnTriggerEnter( Collider* collider ) {}
    virtual void OnTriggerStay( Collider* collider ) {}
    virtual void OnTriggerExit( Collider* collider ) {}
    
    FLAGS GetFlags() override { return UPDATE | FIXED_UPDATE; }

protected:
    inline static int scriptCount = (1<<15)-1;

};

#define CREATE_SCRIPT_INHERIT_START( CLASS_NAME, INHERITANCE ) \
    class Script##CLASS_NAME : public Script##INHERITANCE \
    { \
    friend class GCGameObject; \
    friend class GCScene; \
    friend class GCUpdateManager; \
    friend class GCSceneManager; \
    friend class GCPhysicManager; \
    friend class GCRenderManager; \
    friend class GCGameManager; \
    public: \
        static const int GetIDStatic() { return m_ID; } \
        const int GetID() override { return m_ID; } \
     \
    protected: \
        Script##CLASS_NAME() = default; /* Calling Method to be overritten */ \
        ~Script##CLASS_NAME() = default; \
         \
        Script##CLASS_NAME* Duplicate() override { return new Script##CLASS_NAME(); } \
        void CopyTo( Component* pDestination ) override; \
         \
        /*void Start() override; \
        void Update() override; \
        void FixedUpdate() override; \
        void Destroy() override; \
         \
        void OnTriggerEnter( Collider* collider ) override; \
        void OnTriggerStay( Collider* collider ) override; \
        void OnTriggerExit( Collider* collider ) override;*/ \
     \
    protected: \
        inline static const int m_ID = ++Script::scriptCount; \
     \
    private:

#define CREATE_SCRIPT_START( CLASS_NAME ) CREATE_SCRIPT_INHERIT_START( CLASS_NAME,  )

#define CREATE_SCRIPT_END };
