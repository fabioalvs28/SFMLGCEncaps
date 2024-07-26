// pch.h: This is a precompiled header file.
// Files listed below are compiled only once, improving build performance for future builds.
// This also affects IntelliSense performance, including code completion and many code browsing features.
// However, files listed here are ALL re-compiled if any one of them is updated between builds.
// Do not add files here that you will be updating frequently as this negates the performance advantage.

#ifndef CORE_PCH_H
#define CORE_PCH_H
#include "Define.h"






// Containers
template <typename T> class GCQueue;
template <typename T> class GCListNode;
template <typename T> class GCList;
template <typename T> class GCVector;
template <typename Key, typename Value> class GCMap;

// Math
struct GCVEC2;
struct GCVEC3;
struct GCQUATERNION;
struct GCMATRIX;
struct GCTransform;

// Logger
class Logger;

// Threading
class GCThread;
class WinThread;

// State Machine
class GCState;
class GCStateMachine;
// class GCStateManager;

// Other
class GCString;
struct GCColor;






// Containers
#include "Queue.h"
#include "List.h"
#include "Vector.h"
#include "Map.h"

// Math
#include "Vectors.h"
#include "Quaternion.h"
#include "Matrix.h"
#include "Transform.h"

// Logger
#include "Log.h"
#include "Logger.h"

// Threading
#include "Thread.h"

// State Machine
#include "State.h"
#include "StateMachine.h"

// Other
#include "GCString.h"
#include "GCColor.h"

#endif