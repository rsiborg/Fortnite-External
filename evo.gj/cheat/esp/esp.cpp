#include "esp.h"

void Esp::ActorLoop() {
	ImVec2 center = ImVec2(Width / 2, Height / 2);
	//I will link dumps.host to each offset, on the website it might be outdated but it gives you a understanding of the sdk
	
	//get address

	{
		LocalPtrs::Gworld = read<uint64_t>(BaseId + 0x11f26508); //https://dumpspace.spuckwaffel.com/Games/?hash=6b77eceb&type=offsets
		if (Debug::PrintPointers) Util::PrintPtr("Uworld: ", LocalPtrs::Gworld);
		if (!LocalPtrs::Gworld) return;

		uintptr_t OwningGameInstance = read<uint64_t>(LocalPtrs::Gworld + 0x1d8); //https://dumpspace.spuckwaffel.com/Games/?hash=6b77eceb&type=classes&idx=UWorld&member=OwningGameInstance
		if (Debug::PrintPointers) Util::PrintPtr("OwningGameInstance: ", OwningGameInstance);
		if (!OwningGameInstance) return;

		LocalPtrs::LocalPlayers = read<uint64_t>(read<uint64_t>(OwningGameInstance + 0x38)); //https://dumpspace.spuckwaffel.com/Games/?hash=6b77eceb&type=classes&idx=UGameInstance&member=LocalPlayers
		if (Debug::PrintPointers) Util::PrintPtr("LocalPlayers: ", LocalPtrs::LocalPlayers);
		if (!LocalPtrs::LocalPlayers) return;

		LocalPtrs::PlayerController = read<uint64_t>(LocalPtrs::LocalPlayers + 0x30); //https://dumpspace.spuckwaffel.com/Games/?hash=6b77eceb&type=classes&idx=UPlayer&member=PlayerController
		if (Debug::PrintPointers) Util::PrintPtr("PlayerController: ", LocalPtrs::PlayerController);
		if (!LocalPtrs::PlayerController) return;
		
		LocalPtrs::Player = read<uint64_t>(LocalPtrs::PlayerController + 0x330); //https://dumpspace.spuckwaffel.com/Games/?hash=6b77eceb&type=classes&idx=APlayerController&member=Player
		if (Debug::PrintPointers) Util::PrintPtr("Player: ", LocalPtrs::Player);
		if (!LocalPtrs::Player) return;

		LocalPtrs::RootComponent = read<uint64_t>(LocalPtrs::Player + 0x198); //https://dumpspace.spuckwaffel.com/Games/?hash=6b77eceb&type=classes&idx=AActor&member=RootComponent
		if (Debug::PrintPointers) Util::PrintPtr("RootComponent: ", LocalPtrs::RootComponent);
		if (!LocalPtrs::RootComponent) return;

	}

	//get player array then loop through it
	{
		uintptr_t GameState = read<uintptr_t>(LocalPtrs::Gworld + 0x160); //https://dumpspace.spuckwaffel.com/Games/?hash=6b77eceb&type=classes&idx=UWorld&member=GameState
		if (Debug::PrintPointers) Util::PrintPtr("GameState: ", GameState);
		if (!GameState) return;

		uintptr_t PlayerArray = read<uintptr_t>(GameState + 0x2a8); //https://dumpspace.spuckwaffel.com/Games/?hash=6b77eceb&type=classes&idx=AGameStateBase&member=PlayerArray

		int Num = read<int>(GameState + (0x2a8 + sizeof(uintptr_t))); //reads the total number of player states in this array
		
		for (int i = 0; i < Num; i++) {
		

			uintptr_t PlayerState = read<uintptr_t>(PlayerArray + (i * sizeof(uintptr_t))); //the size of the pointer in the array we are reading is the size of uintptr_t
			if (Debug::PrintPointers) Util::PrintPtr("Enemy PlayerState: ", PlayerState);
			if (!PlayerState) continue; // we say if the pointer is invalid to continue, this means it will continue onto the next loop

			//since we only have the player state, we use PawnPrivate to get to the player
			uintptr_t Player = read<uintptr_t>(PlayerState + 0x308); //https://dumpspace.spuckwaffel.com/Games/?hash=6b77eceb&type=classes&idx=APlayerState&member=PawnPrivate
			if (Debug::PrintPointers) Util::PrintPtr("Enemy Player: ", Player);
			if (!Player) continue;

			uintptr_t Mesh = read<uintptr_t>(Player + 0x318); //https://dumpspace.spuckwaffel.com/Games/?hash=6b77eceb&type=classes&idx=ACharacter&member=Mesh
			if (Debug::PrintPointers) Util::PrintPtr("Enemy Mesh: ", Mesh);
			if (!Mesh) continue;

			uintptr_t Persistentlevel = read<uintptr_t>(LocalPtrs::Gworld + 0x30);
			uintptr_t AActors = read<uintptr_t>(Persistentlevel + 0xA8);
			uintptr_t RootComponent = read<uintptr_t>(Player + 0x198);
			Vector3 LocalActorPos = read<Vector3>(RootComponent + 0x120);


			Vector3 Head3D = SDK::GetBoneWithRotation(Mesh, 110);
			Vector2 Head2D = SDK::ProjectWorldToScreen(Head3D);

			Vector3 Neck3D = SDK::GetBoneWithRotation(Mesh, 67);
			Vector2 Neck2D = SDK::ProjectWorldToScreen(Neck3D);

			Vector3 Pelvis3D = SDK::GetBoneWithRotation(Mesh, 2);
			Vector2 Pelvis2D = SDK::ProjectWorldToScreen(Pelvis3D);

			Vector3 RightShoulder3D = SDK::GetBoneWithRotation(Mesh, 9);
			Vector2 RightShoulder2D = SDK::ProjectWorldToScreen(RightShoulder3D);

			Vector3 RightElbow3D = SDK::GetBoneWithRotation(Mesh, 10); // 10
			Vector2 RightElbow2D = SDK::ProjectWorldToScreen(RightElbow3D);

			Vector3 RightWrist3D = SDK::GetBoneWithRotation(Mesh, 90); 
			Vector2 RightWrist2D = SDK::ProjectWorldToScreen(RightWrist3D);

			// Right Fingers
			// IndexFinger
			Vector2 rightIndexKnuckle2D = SDK::ProjectWorldToScreen(SDK::GetBoneWithRotation(Mesh, 13));
			Vector2 rightIndexFingerMCP2D = SDK::ProjectWorldToScreen(SDK::GetBoneWithRotation(Mesh, 14));
			Vector2 rightIndexFingerPIP2D = SDK::ProjectWorldToScreen(SDK::GetBoneWithRotation(Mesh, 15));
			// MiddleFinger
			Vector2 rightMiddleFingerKnuckle2D = SDK::ProjectWorldToScreen(SDK::GetBoneWithRotation(Mesh, 17));
			Vector2 rightMiddleFingerMCP2D = SDK::ProjectWorldToScreen(SDK::GetBoneWithRotation(Mesh, 18));
			Vector2 rightMiddleFingerPIP2D = SDK::ProjectWorldToScreen(SDK::GetBoneWithRotation(Mesh, 19));
			// RingFinger
			Vector2 rightRingFingerKnuckle2D = SDK::ProjectWorldToScreen(SDK::GetBoneWithRotation(Mesh, 21));
			Vector2 rightRingFingerMCP2D = SDK::ProjectWorldToScreen(SDK::GetBoneWithRotation(Mesh, 22));
			Vector2 rightRingFingerPIP2D = SDK::ProjectWorldToScreen(SDK::GetBoneWithRotation(Mesh, 23));
			// PinkyFinger
			Vector2 rightPinkyFingerKnuckle2D = SDK::ProjectWorldToScreen(SDK::GetBoneWithRotation(Mesh, 25));
			Vector2 rightPinkyFingerMCP2D = SDK::ProjectWorldToScreen(SDK::GetBoneWithRotation(Mesh, 26));
			Vector2 rightPinkyFingerPIP2D = SDK::ProjectWorldToScreen(SDK::GetBoneWithRotation(Mesh, 27));
			// ThumbFinger
			Vector2 rightThumbFingerKnuckle2D = SDK::ProjectWorldToScreen(SDK::GetBoneWithRotation(Mesh, 29));
			Vector2 rightThumbFingerMCP2D = SDK::ProjectWorldToScreen(SDK::GetBoneWithRotation(Mesh, 30));
			Vector2 rightThumbFingerPIP2D = SDK::ProjectWorldToScreen(SDK::GetBoneWithRotation(Mesh, 31));

			// Left Fingers
			// IndexFinger
			Vector2 leftIndexKnuckle2D = SDK::ProjectWorldToScreen(SDK::GetBoneWithRotation(Mesh, 42));
			Vector2 leftIndexFingerMCP2D = SDK::ProjectWorldToScreen(SDK::GetBoneWithRotation(Mesh, 43));
			Vector2 leftIndexFingerPIP2D = SDK::ProjectWorldToScreen(SDK::GetBoneWithRotation(Mesh, 44));
			// MiddleFinger
			Vector2 leftMiddleFingerKnuckle2D = SDK::ProjectWorldToScreen(SDK::GetBoneWithRotation(Mesh, 46));
			Vector2 leftMiddleFingerMCP2D = SDK::ProjectWorldToScreen(SDK::GetBoneWithRotation(Mesh, 47));
			Vector2 leftMiddleFingerPIP2D = SDK::ProjectWorldToScreen(SDK::GetBoneWithRotation(Mesh, 48));
			// RingFinger
			Vector2 leftRingFingerKnuckle2D = SDK::ProjectWorldToScreen(SDK::GetBoneWithRotation(Mesh, 50));
			Vector2 leftRingFingerMCP2D = SDK::ProjectWorldToScreen(SDK::GetBoneWithRotation(Mesh, 51));
			Vector2 leftRingFingerPIP2D = SDK::ProjectWorldToScreen(SDK::GetBoneWithRotation(Mesh, 52));
			// PinkyFinger
			Vector2 leftPinkyFingerKnuckle2D = SDK::ProjectWorldToScreen(SDK::GetBoneWithRotation(Mesh, 54));
			Vector2 leftPinkyFingerMCP2D = SDK::ProjectWorldToScreen(SDK::GetBoneWithRotation(Mesh, 55));
			Vector2 leftPinkyFingerPIP2D = SDK::ProjectWorldToScreen(SDK::GetBoneWithRotation(Mesh, 56));
			// ThumbFinger
			Vector2 leftThumbFingerKnuckle2D = SDK::ProjectWorldToScreen(SDK::GetBoneWithRotation(Mesh, 58));
			Vector2 leftThumbFingerMCP2D = SDK::ProjectWorldToScreen(SDK::GetBoneWithRotation(Mesh, 59));


			Vector3 RightHip3D = SDK::GetBoneWithRotation(Mesh, 71);
			Vector2 RightHip2D = SDK::ProjectWorldToScreen(RightHip3D);

			Vector3 RightKnee3D = SDK::GetBoneWithRotation(Mesh, 72);
			Vector2 RightKnee2D = SDK::ProjectWorldToScreen(RightKnee3D);

			Vector3 RightAnkle3D = SDK::GetBoneWithRotation(Mesh, 75);
			Vector2 RightAnkle2D = SDK::ProjectWorldToScreen(RightAnkle3D);

			Vector3 RightFoot3D = SDK::GetBoneWithRotation(Mesh, 76);
			Vector2 RightFoot2D = SDK::ProjectWorldToScreen(RightFoot3D);

			Vector3 LeftShoulder3D = SDK::GetBoneWithRotation(Mesh, 38);
			Vector2 LeftShoulder2D = SDK::ProjectWorldToScreen(LeftShoulder3D);

			Vector3 LeftElbow3D = SDK::GetBoneWithRotation(Mesh, 39); // 105 and 106
			Vector2 LeftElbow2D = SDK::ProjectWorldToScreen(LeftElbow3D);

			Vector3 LeftWrist3D = SDK::GetBoneWithRotation(Mesh, 40); // 107 and 112
			Vector2 LeftWrist2D = SDK::ProjectWorldToScreen(LeftWrist3D);

			Vector3 LeftHip3D = SDK::GetBoneWithRotation(Mesh, 78);
			Vector2 LeftHip2D = SDK::ProjectWorldToScreen(LeftHip3D);

			Vector3 LeftKnee3D = SDK::GetBoneWithRotation(Mesh, 79);
			Vector2 LeftKnee2D = SDK::ProjectWorldToScreen(LeftKnee3D);

			Vector3 LeftAnkle3D = SDK::GetBoneWithRotation(Mesh, 82);
			Vector2 LeftAnkle2D = SDK::ProjectWorldToScreen(LeftAnkle3D);

			Vector3 LeftFoot3D = SDK::GetBoneWithRotation(Mesh, 83);
			Vector2 LeftFoot2D = SDK::ProjectWorldToScreen(LeftFoot3D);

			if (Debug::PrintLocations) Util::Print3D("Head3D: ", Head3D);
			if (Debug::PrintLocations) Util::Print2D("Head2D: ", Head2D);

			Vector3 Bottom3D = SDK::GetBoneWithRotation(Mesh, 0);
			Vector2 Bottom2D = SDK::ProjectWorldToScreen(Bottom3D);

			float BoxHeight = (float)(Head2D.y - Bottom2D.y);
			float CornerHeight = abs(Head2D.y - Bottom2D.y);
			float CornerWidth = BoxHeight * 0.80;


			if (bCornerBox)
				Util::DrawCornerBox(Head2D.x - (CornerWidth / 2), Head2D.y, CornerWidth, CornerHeight, IM_COL32(0, 173, 237, 255), 1.5);

			if (bFov) {
				ImGui::GetOverlayDrawList()->AddCircle(center, FovSize, ImColor(0, 0, 0), 100, 2);
			}

			if (bLineEsp) {
				ImGui::GetOverlayDrawList()->AddLine(center, ImVec2(Head2D.x, Head2D.y), ImColor(0, 0, 0), 2);
			}

			if (bCrosshair) {
				ImGui::GetOverlayDrawList()->AddLine(ImVec2(Width / 2 - 11, Height / 2), ImVec2(Width / 2 + 1, Height / 2), ImColor(255, 255, 255), 2);
				ImGui::GetOverlayDrawList()->AddLine(ImVec2(Width / 2 + 12, Height / 2), ImVec2(Width / 2 + 1, Height / 2), ImColor(255, 255, 255), 2);
				ImGui::GetOverlayDrawList()->AddLine(ImVec2(Width / 2, Height / 2 -11 ), ImVec2(Width / 2, Height / 2 ), ImColor(255, 255, 255), 2);
				ImGui::GetOverlayDrawList()->AddLine(ImVec2(Width / 2, Height / 2 + 12), ImVec2(Width / 2, Height / 2), ImColor(255, 255, 255), 2);
			}
			auto dist = Util::GetCrossDistance(Head2D.x, Head2D.y, Width / 2, Height / 2);

			if (bDistanceESP) {
				/*std::string distance_string = "" + std::to_string((int)dist) + " M";*/

				char fpsinfo[64];
				Vector3 bonehere = SDK::GetBoneWithRotation(Mesh, 68);
				float distance = LocalActorPos.Distance(bonehere) / 100.f;

				char name[64];
				sprintf_s(name, "[%2.fm]", distance);

				ImGui::GetOverlayDrawList()->AddText(ImGui::GetFont(), 0, ImVec2(Bottom2D.x, Bottom2D.y), ImColor(0, 0, 0), name);
			}
			ImColor SkeleColor = ImColor(255, 255, 255);

			if (bSkeletonESP) {

				ImGui::GetOverlayDrawList()->AddLine(ImVec2(Head2D.x, Head2D.y), ImVec2(Neck2D.x, Neck2D.y), SkeleColor, 2);
				ImGui::GetOverlayDrawList()->AddLine(ImVec2(Neck2D.x, Neck2D.y), ImVec2(Pelvis2D.x, Pelvis2D.y), SkeleColor, 2);

				ImGui::GetOverlayDrawList()->AddLine(ImVec2(Neck2D.x, Neck2D.y), ImVec2(RightShoulder2D.x, RightShoulder2D.y), SkeleColor, 2);
				ImGui::GetOverlayDrawList()->AddLine(ImVec2(RightShoulder2D.x, RightShoulder2D.y), ImVec2(RightElbow2D.x, RightElbow2D.y), SkeleColor, 2);
				ImGui::GetOverlayDrawList()->AddLine(ImVec2(RightElbow2D.x, RightElbow2D.y), ImVec2(RightWrist2D.x, RightWrist2D.y), SkeleColor, 2);

				ImGui::GetOverlayDrawList()->AddLine(ImVec2(Neck2D.x, Neck2D.y), ImVec2(LeftShoulder2D.x, LeftShoulder2D.y), SkeleColor, 2);
				ImGui::GetOverlayDrawList()->AddLine(ImVec2(LeftShoulder2D.x, LeftShoulder2D.y), ImVec2(LeftElbow2D.x, LeftElbow2D.y), SkeleColor, 2);
				ImGui::GetOverlayDrawList()->AddLine(ImVec2(LeftElbow2D.x, LeftElbow2D.y), ImVec2(LeftWrist2D.x, LeftWrist2D.y), SkeleColor, 2);

				ImGui::GetOverlayDrawList()->AddLine(ImVec2(Pelvis2D.x, Pelvis2D.y), ImVec2(RightHip2D.x, RightHip2D.y), SkeleColor, 2);
				ImGui::GetOverlayDrawList()->AddLine(ImVec2(RightHip2D.x, RightHip2D.y), ImVec2(RightKnee2D.x, RightKnee2D.y), SkeleColor, 2);
				ImGui::GetOverlayDrawList()->AddLine(ImVec2(RightKnee2D.x, RightKnee2D.y), ImVec2(RightAnkle2D.x, RightAnkle2D.y), SkeleColor, 2);
				ImGui::GetOverlayDrawList()->AddLine(ImVec2(RightAnkle2D.x, RightAnkle2D.y), ImVec2(RightFoot2D.x, RightFoot2D.y), SkeleColor, 2);

				ImGui::GetOverlayDrawList()->AddLine(ImVec2(Pelvis2D.x, Pelvis2D.y), ImVec2(LeftHip2D.x, LeftHip2D.y), SkeleColor, 2);
				ImGui::GetOverlayDrawList()->AddLine(ImVec2(LeftHip2D.x, LeftHip2D.y), ImVec2(LeftKnee2D.x, LeftKnee2D.y), SkeleColor, 2);
				ImGui::GetOverlayDrawList()->AddLine(ImVec2(LeftKnee2D.x, LeftKnee2D.y), ImVec2(LeftAnkle2D.x, LeftAnkle2D.y), SkeleColor, 2);
				ImGui::GetOverlayDrawList()->AddLine(ImVec2(LeftAnkle2D.x, LeftAnkle2D.y), ImVec2(LeftFoot2D.x, LeftFoot2D.y), SkeleColor, 2);
			}
			if (bHandESP) {
				ImGui::GetOverlayDrawList()->AddLine(ImVec2(RightWrist2D.x, RightWrist2D.y), ImVec2(rightIndexKnuckle2D.x, rightIndexKnuckle2D.y), SkeleColor, 2);
				ImGui::GetOverlayDrawList()->AddLine(ImVec2(rightIndexKnuckle2D.x, rightIndexKnuckle2D.y), ImVec2(rightIndexFingerMCP2D.x, rightIndexFingerMCP2D.y), SkeleColor, 2);
				ImGui::GetOverlayDrawList()->AddLine(ImVec2(rightIndexFingerMCP2D.x, rightIndexFingerMCP2D.y), ImVec2(rightIndexFingerPIP2D.x, rightIndexFingerPIP2D.y), SkeleColor, 2);

				ImGui::GetOverlayDrawList()->AddLine(ImVec2(RightWrist2D.x, RightWrist2D.y), ImVec2(rightMiddleFingerKnuckle2D.x, rightMiddleFingerKnuckle2D.y), SkeleColor, 2);
				ImGui::GetOverlayDrawList()->AddLine(ImVec2(rightMiddleFingerKnuckle2D.x, rightMiddleFingerKnuckle2D.y), ImVec2(rightMiddleFingerMCP2D.x, rightMiddleFingerMCP2D.y), SkeleColor, 2);
				ImGui::GetOverlayDrawList()->AddLine(ImVec2(rightMiddleFingerMCP2D.x, rightMiddleFingerMCP2D.y), ImVec2(rightMiddleFingerPIP2D.x, rightMiddleFingerPIP2D.y), SkeleColor, 2);

				ImGui::GetOverlayDrawList()->AddLine(ImVec2(RightWrist2D.x, RightWrist2D.y), ImVec2(rightRingFingerKnuckle2D.x, rightRingFingerKnuckle2D.y), SkeleColor, 2);
				ImGui::GetOverlayDrawList()->AddLine(ImVec2(rightRingFingerKnuckle2D.x, rightRingFingerKnuckle2D.y), ImVec2(rightRingFingerMCP2D.x, rightRingFingerMCP2D.y), SkeleColor, 2);
				ImGui::GetOverlayDrawList()->AddLine(ImVec2(rightRingFingerMCP2D.x, rightRingFingerMCP2D.y), ImVec2(rightRingFingerPIP2D.x, rightRingFingerPIP2D.y), SkeleColor, 2);

				ImGui::GetOverlayDrawList()->AddLine(ImVec2(RightWrist2D.x, RightWrist2D.y), ImVec2(rightPinkyFingerKnuckle2D.x, rightPinkyFingerKnuckle2D.y), SkeleColor, 2);
				ImGui::GetOverlayDrawList()->AddLine(ImVec2(rightPinkyFingerKnuckle2D.x, rightPinkyFingerKnuckle2D.y), ImVec2(rightPinkyFingerMCP2D.x, rightPinkyFingerMCP2D.y), SkeleColor, 2);
				ImGui::GetOverlayDrawList()->AddLine(ImVec2(rightPinkyFingerMCP2D.x, rightPinkyFingerMCP2D.y), ImVec2(rightPinkyFingerPIP2D.x, rightPinkyFingerPIP2D.y), SkeleColor, 2);

				ImGui::GetOverlayDrawList()->AddLine(ImVec2(RightWrist2D.x, RightWrist2D.y), ImVec2(rightThumbFingerKnuckle2D.x, rightThumbFingerKnuckle2D.y), SkeleColor, 2);
				ImGui::GetOverlayDrawList()->AddLine(ImVec2(rightThumbFingerKnuckle2D.x, rightThumbFingerKnuckle2D.y), ImVec2(rightThumbFingerMCP2D.x, rightThumbFingerMCP2D.y), SkeleColor, 2);

				ImGui::GetOverlayDrawList()->AddLine(ImVec2(LeftWrist2D.x, LeftWrist2D.y), ImVec2(leftIndexKnuckle2D.x, leftIndexKnuckle2D.y), SkeleColor, 2);
				ImGui::GetOverlayDrawList()->AddLine(ImVec2(leftIndexKnuckle2D.x, leftIndexKnuckle2D.y), ImVec2(leftIndexFingerMCP2D.x, leftIndexFingerMCP2D.y), SkeleColor, 2);
				ImGui::GetOverlayDrawList()->AddLine(ImVec2(leftIndexFingerMCP2D.x, leftIndexFingerMCP2D.y), ImVec2(leftIndexFingerPIP2D.x, leftIndexFingerPIP2D.y), SkeleColor, 2);

				ImGui::GetOverlayDrawList()->AddLine(ImVec2(LeftWrist2D.x, LeftWrist2D.y), ImVec2(leftMiddleFingerKnuckle2D.x, leftMiddleFingerKnuckle2D.y), SkeleColor, 2);
				ImGui::GetOverlayDrawList()->AddLine(ImVec2(leftMiddleFingerKnuckle2D.x, leftMiddleFingerKnuckle2D.y), ImVec2(leftMiddleFingerMCP2D.x, leftMiddleFingerMCP2D.y), SkeleColor, 2);
				ImGui::GetOverlayDrawList()->AddLine(ImVec2(leftMiddleFingerMCP2D.x, leftMiddleFingerMCP2D.y), ImVec2(leftMiddleFingerPIP2D.x, leftMiddleFingerPIP2D.y), SkeleColor, 2);

				ImGui::GetOverlayDrawList()->AddLine(ImVec2(LeftWrist2D.x, LeftWrist2D.y), ImVec2(leftRingFingerKnuckle2D.x, leftRingFingerKnuckle2D.y), SkeleColor, 2);
				ImGui::GetOverlayDrawList()->AddLine(ImVec2(leftRingFingerKnuckle2D.x, leftRingFingerKnuckle2D.y), ImVec2(leftRingFingerMCP2D.x, leftRingFingerMCP2D.y), SkeleColor, 2);
				ImGui::GetOverlayDrawList()->AddLine(ImVec2(leftRingFingerMCP2D.x, leftRingFingerMCP2D.y), ImVec2(leftRingFingerPIP2D.x, leftRingFingerPIP2D.y), SkeleColor, 2);

				ImGui::GetOverlayDrawList()->AddLine(ImVec2(LeftWrist2D.x, LeftWrist2D.y), ImVec2(leftPinkyFingerKnuckle2D.x, leftPinkyFingerKnuckle2D.y), SkeleColor, 2);
				ImGui::GetOverlayDrawList()->AddLine(ImVec2(leftPinkyFingerKnuckle2D.x, leftPinkyFingerKnuckle2D.y), ImVec2(leftPinkyFingerMCP2D.x, leftPinkyFingerMCP2D.y), SkeleColor, 2);
				ImGui::GetOverlayDrawList()->AddLine(ImVec2(leftPinkyFingerMCP2D.x, leftPinkyFingerMCP2D.y), ImVec2(leftPinkyFingerPIP2D.x, leftPinkyFingerPIP2D.y), SkeleColor, 2);

				ImGui::GetOverlayDrawList()->AddLine(ImVec2(LeftWrist2D.x, LeftWrist2D.y), ImVec2(leftThumbFingerKnuckle2D.x, leftThumbFingerKnuckle2D.y), SkeleColor, 2);
				ImGui::GetOverlayDrawList()->AddLine(ImVec2(leftThumbFingerKnuckle2D.x, leftThumbFingerKnuckle2D.y), ImVec2(leftThumbFingerMCP2D.x, leftThumbFingerMCP2D.y), SkeleColor, 2);
			}


			if (dist < FovSize && dist < ClosestDistance) {
				ClosestDistance = dist;
				TargetPawn = Player;
			}
		}
	}
}
