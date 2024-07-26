// pch.h: This is a precompiled header file.
// Files listed below are compiled only once, improving build performance for future builds.
// This also affects IntelliSense performance, including code completion and many code browsing features.
// However, files listed here are ALL re-compiled if any one of them is updated between builds.
// Do not add files here that you will be updating frequently as this negates the performance advantage.

#ifndef ENGINE_PCH_H
#define ENGINE_PCH_H

typedef const char* cstr;
typedef const wchar_t* custr;
typedef __int8 I8;
typedef __int16 I16;
typedef __int32 I32;
typedef __int64 I64;
typedef unsigned __int8 UI8;
typedef unsigned __int16 UI16;
typedef unsigned __int32 UI32;
typedef unsigned __int64 UI64;






// Gameplay classes

struct GCGameObjectTransform;

class Component;
class SpriteRenderer;
class Collider;
class BoxCollider;
class CircleCollider;
class RigidBody;
class Animator;
class SoundMixer;
class Camera;
class Script;

class GCGameObject;
class GCScene;



// Managers

class GCRenderManager;

class GCSceneManager;

class GCState;
class GCStateManager;

class GCFrame;
class GCAnimation;
class GCAnimationManager;

class GCUpdateManager;

class GCPhysicManager;

class Layer;
class UILayer;
enum class GCEventType;
enum class GCMousebutton;
class GCEvent;
class GCEventDispatcher;
class GCMouseButtonPressed;
class GCMouseMoveEvent;
class GCWindowCloseEvent;
class GCWindowResizeEvent;
class GCKeyEvent;
class GCKeyPressedEvent;
class GCKeyReleasedEvent;
class ComponentAddedEvent;
class GCEventManager;

class GCInputManager;
class GCKeyboardInputManager;
class GCMouseInputManager;
// class GCControllerManager;
// class GCControllerInputManager;
class GCInputSystem;

class GCTime;

class GCGameManager;



// Singleton

class GC;






#include "GameObjectTransform.h"
#include "Components.h"
#include "GameObject.h"
#include "Scene.h"

#include "RenderManager.h"
#include "SceneManager.h"
#include "State.h"
#include "StateManager.h"
#include "Frame.h"
#include "Animation.h"
#include "AnimationManager.h"
#include "UpdateManager.h"
#include "PhysicManager.h"
#include "Layer.h"
#include "Event.h"
#include "EventManager.h"
#include "InputManager.h"
#include "Timer.h"
#include "GameManager.h"

#include "GC.h"

#endif //ENGINE_PCH_H