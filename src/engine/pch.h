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






// Singleton

class GC;



// Managers

class GCGameManager;
class GCTime;

class GCInputSystem;
class GCInputManager;
class GCKeyboardInputManager;
class GCMouseInputManager;
// class GCControllerManager;
// class GCControllerInputManager;

class GCEvent;
class GCEventManager;

class GCPhysicManager;
class GCUpdateManager;
class GCSceneManager;
class GCRenderManager;



// Gameplay classes

class GCScene;
class GCGameObject;

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

class GCGameObjectTransform;






#include "GC.h"

#include "GameManager.h"
#include "Timer.h"
#include "InputManager.h"
#include "Event.h"
#include "EventManager.h"
#include "PhysicManager.h"
#include "UpdateManager.h"
#include "SceneManager.h"
#include "RenderManager.h"

#include "Scene.h"
#include "GameObject.h"
#include "Components.h"
#include "GameObjectTransform.h"

#endif //ENGINE_PCH_H