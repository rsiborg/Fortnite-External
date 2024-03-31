#pragma once 
#include "../../util/util.h"
#include "../sdk/sdk.h"
#include "../aimbot/aimbot.h"
#include <iostream>
class Esp {
public:
	static void ActorLoop(Vector3* out_headbone);
};
inline bool bCornerBox = true;
inline bool bBoxESP = false;
inline bool bSelfESP = false;
inline bool SelfEsp = true;
inline bool bLineEsp = false;
inline bool bCrosshair = false;
static const char* bCurry = NULL;

namespace Debug {
	static bool PrintPointers = false;
	static bool PrintLocations = false;
}