#pragma once

#define GC

#ifdef GC
#include "GC.h"
typedef LEWindowGC LEWindow;
typedef LETextureGC LETexture;
typedef LESpriteGC LESprite;

#else
#include "SFML.h"
typedef SFMLWindow LEWindow;
typedef SFMLTexture LETexture;
typedef SFMLSprite LESprite;

#endif
