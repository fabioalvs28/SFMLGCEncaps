// pch.h: This is a precompiled header file.
// Files listed below are compiled only once, improving build performance for future builds.
// This also affects IntelliSense performance, including code completion and many code browsing features.
// However, files listed here are ALL re-compiled if any one of them is updated between builds.
// Do not add files here that you will be updating frequently as this negates the performance advantage.

#ifndef ENGINE_PCH_H
#define ENGINE_PCH_H

#include "Define.h"

//C++ libraries
#include <iostream>
#include <string>
#include <typeinfo>
#include <functional>
#include <Windows.h>
#include <algorithm>
#include <tuple>

//Core Elements
#include "../core/framework.h"

//Render Elements
#include "../Render/pch.h" //! To change


// Gameplay classes

struct GCGameObjectTransform;
class GCSprite;

class GCComponent;
class GCSpriteRenderer;
class GCCollider;
class GCBoxCollider;
class GCCircleCollider;
class GCRigidBody;
class GCAnimator;
class GCSoundMixer;
class GCCamera;
class GCScript;

class GCGameObject;
class GCScene;



// Managers
class GCRenderManager;
class GCSceneManager;
class GCUpdateManager;
class GCPhysicManager;

class GCState;
class GCStateManager;

class GCFrame;
class GCAnimation;
class GCAnimationManager;



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
enum GCKEYBOARD;
enum GCMOUSE;
class GCINPUTS;

class GCTime;

class GCGameManager;

// Singleton
class GC;


#include "GameObjectTransform.h"
#include "Sprite.h"
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

//Intermediate file
#include "GCEngine.h"

#endif //ENGINE_PCH_H