#pragma once
#include "pch.h"

using namespace DirectX;

// TODO Transforms for colliders
// TODO Make sure IDs are handled differently

class GCAnimation;

enum FLAGS
{
    NONE            = 0,
	UPDATE          = 1 << 0,
	FIXED_UPDATE    = 1 << 1,
    RENDER          = 1 << 2,
    TRIGGER         = 1 << 3,
};
inline FLAGS operator|( FLAGS a, FLAGS b ) { return static_cast<FLAGS>(static_cast<int>(a) | static_cast<int>(b)); }



class GCComponent
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
    GCComponent();
    virtual ~GCComponent() = default;
    
    virtual void RegisterToManagers();
    virtual void UnregisterFromManagers();
    
    virtual GCComponent* Duplicate() = 0;
    virtual void CopyTo( GCComponent *pDestination );
    
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
    
    GCListNode<GCComponent*>* m_pUpdateNode;
    GCListNode<GCComponent*>* m_pPhysicsNode;
    GCListNode<GCComponent*>* m_pRenderNode;

};



class GCSpriteRenderer : public GCComponent
{
friend class GCGameObject;
friend class GCUpdateManager;
friend class GCSceneManager;
friend class GCPhysicManager;
friend class GCRenderManager;
friend class GCAnimator;

public:
    static const int GetIDStatic() { return m_ID; }
    const int GetID() override { return m_ID; }
    
    void SetSprite( GCSprite* pSprite );
    GCSprite* GetSprite() { return m_pSprite; };

protected:
	GCSpriteRenderer();
    ~GCSpriteRenderer() override {}

    GCSpriteRenderer* Duplicate() override { return new GCSpriteRenderer(); }
    void CopyTo( GCComponent* pDestination ) override;
    
    void Render() override;
    
    FLAGS GetFlags() override { return RENDER; }

    int GetComponentLayer() override { return 5; }

    void SetAnimatedSprite( GCGeometry* pGeometry );

protected:
    inline static const int m_ID = ++GCComponent::componentCount;
    GCSprite* m_pSprite;

};



class GCCollider : public GCComponent
{
friend class GCGameObject;
friend class GCUpdateManager;
friend class GCSceneManager;
friend class GCPhysicManager;
friend class GCRenderManager;

public:
    GCCollider();
    ~GCCollider() override {}
    
    void RegisterToManagers() override;
    void UnregisterFromManagers() override;
    
    void CopyTo( GCComponent* pDestination ) override;

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
    
    GCListNode<GCCollider*>* m_pColliderNode;
};



class GCBoxCollider : public GCCollider
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
    GCBoxCollider();
    ~GCBoxCollider() override {}

    GCBoxCollider* Duplicate() override { return new GCBoxCollider(); }
    void CopyTo( GCComponent* pDestination ) override;
    
    void Render() override;

protected:
    inline static const int m_ID = ++GCComponent::componentCount;

};



class GCCircleCollider : public GCCollider
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
    GCCircleCollider() {}
    ~GCCircleCollider() override {}
    
    GCCircleCollider* Duplicate() override { return new GCCircleCollider(); }
    void CopyTo( GCComponent* pDestination ) override;

protected:
    inline static const int m_ID = ++GCComponent::componentCount;

};



class GCRigidBody : public GCComponent
{
friend class GCGameObject;
friend class GCUpdateManager;
friend class GCSceneManager;
friend class GCPhysicManager;
friend class GCRenderManager;

public:
    static const int GetIDStatic() { return m_ID; }
    const int GetID() override { return m_ID; }
    
    GCVEC3 GetVelocity() { return m_velocity; }
    void SetVelocity( GCVEC3 velocity ) { m_velocity = velocity; }

protected:
    GCRigidBody();
    ~GCRigidBody() override {}
    
    GCRigidBody* Duplicate() override { return new GCRigidBody(); }
    void CopyTo( GCComponent* pDestination ) override;
    
    void Update() override;
    void FixedUpdate() override {};
    
    FLAGS GetFlags() override { return UPDATE | FIXED_UPDATE; }

protected:
    inline static const int m_ID = ++GCComponent::componentCount;
    GCVEC3 m_velocity;

};



class GCAnimator : public GCComponent
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

    void LoadSpriteSheet( std::string fileName , int row , int col , int width , int height );
    GCAnimation* CreateAnimation( std::string animationName , int firstFrame , int frameNumber , float frameDisplayTime = 0.1f );
    GCAnimation* CreateAnimationWithCustomFrames( std::string animationName , std::vector<int> frameList , float frameDisplayTime = 0.1f );

    std::string GetActiveAnimation() { return m_activeAnimationName; }

protected:
    GCAnimator();
    ~GCAnimator() override {}

    GCAnimator* Duplicate() override { return new GCAnimator(); }
    void CopyTo( GCComponent* pDestination ) override;
    
    void Start() override;
    void Update() override;
    
    FLAGS GetFlags() override { return UPDATE; }

protected:
    inline static const int m_ID = ++GCComponent::componentCount;

private:
    std::string m_spritesheetName;
    GC_SPRITESHEET_INFO* m_pSpriteSheetInfo;
    GCSpriteRenderer* m_pSpriteRenderer;

    std::string m_activeAnimationName;
    GCAnimation* m_pCurrentAnimation;

    bool m_isLoop;
    int m_lastFrameIndex;
    int m_currentFrameIndex;
    float m_currentFrameTime;
};



class GCSoundMixer : public GCComponent
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
	GCSoundMixer() {}
    ~GCSoundMixer() override {}
    
    GCSoundMixer* Duplicate() override { return new GCSoundMixer(); }
    void CopyTo( GCComponent* pDestination ) override;
    
    void Update() override {}
    
    FLAGS GetFlags() override { return UPDATE; }

protected:
    inline static const int m_ID = ++GCComponent::componentCount;

};



class GCCamera : public GCComponent
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
    GCCamera();
    ~GCCamera() override {}
    
    GCCamera* Duplicate() override { return new GCCamera(); }
    void CopyTo( GCComponent* pDestination ) override;
    
    void Update() override;
    
    FLAGS GetFlags() override { return NONE; }

protected:
    inline static const int m_ID = ++GCComponent::componentCount;
    
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



class GCScript : public GCComponent
{
friend class GCGameObject;
friend class GCScene;
friend class GCUpdateManager;
friend class GCSceneManager;
friend class GCPhysicManager;
friend class GCRenderManager;
friend class GCGameManager;

protected:
    GCScript();
    virtual ~GCScript() = default;
    
    void RegisterToManagers() override;
    void UnregisterFromManagers() override;
    
    virtual void OnTriggerEnter( GCCollider* collider ) {}
    virtual void OnTriggerStay( GCCollider* collider ) {}
    virtual void OnTriggerExit( GCCollider* collider ) {}
    
    FLAGS GetFlags() override { return UPDATE | FIXED_UPDATE | TRIGGER; }

protected:
    inline static int scriptCount = ( 1 << 15 ) - 1;
    GCListNode<GCScript*>* m_pTriggerNode;

};

#define CREATE_SCRIPT_INHERIT_START( CLASS_NAME, INHERITANCE ) \
    class GCScript##CLASS_NAME : public GCScript##INHERITANCE \
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
        GCScript##CLASS_NAME() = default; /* Calling Method to be overritten */ \
        ~GCScript##CLASS_NAME() = default; \
         \
        GCScript##CLASS_NAME* Duplicate() override { return new GCScript##CLASS_NAME(); } \
        void CopyTo( GCComponent* pDestination ) override; \
         \
        /*void Start() override; \
        void Update() override; \
        void FixedUpdate() override; \
        void Destroy() override; \
         \
        void OnTriggerEnter( GCCollider* collider ) override; \
        void OnTriggerStay( GCCollider* collider ) override; \
        void OnTriggerExit( GCCollider* collider ) override;*/ \
     \
    protected: \
        inline static const int m_ID = ++GCScript::scriptCount; \
     \
    private:

#define CREATE_SCRIPT_START( CLASS_NAME ) CREATE_SCRIPT_INHERIT_START( CLASS_NAME,  )

#define CREATE_SCRIPT_END };
