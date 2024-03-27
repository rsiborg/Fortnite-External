#pragma once 
#include "../../util/util.h"
#include "../sdk/sdk.h"
#include "../aimbot/aimbot.h"
class Esp {
public:
	static void ActorLoop();
};
inline bool bCornerBox = true;
inline bool SelfEsp = true;
inline bool bLineEsp = true;
inline bool bCrosshair = false;

namespace Debug {
	static bool PrintPointers = true;
	static bool PrintLocations = false;
}