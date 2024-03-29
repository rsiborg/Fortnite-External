#pragma once 
#include "../../util/util.h"
#include "../sdk/sdk.h"
#include "../aimbot/aimbot.h"
#include <iostream>
class Esp {
public:
	static void ActorLoop();
};
inline bool bCornerBox = true;
inline bool SelfEsp = true;
inline bool bLineEsp = false;
inline bool bCrosshair = false;
static const char* bCurry = NULL;

namespace Debug {
	static bool PrintPointers = true;
	static bool PrintLocations = false;
}