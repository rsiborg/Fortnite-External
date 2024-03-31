#include "esp.h";
#include <iostream>;

void Esp::ActorLoop(Vector3* out_headbone) {
	ImVec2 center = ImVec2(Width / 2, Height / 2);
	float time = ImGui::GetTime();


	float frequency = 1.0f;
	float intensity = 128.0f;


	int red = static_cast<int>(128.0f + 128.0f * cos(frequency * time));
	int green = static_cast<int>(128.0f + 128.0f * cos(frequency * time + 2.0f));
	int blue = static_cast<int>(128.0f + 128.0f * cos(frequency * time + 4.0f));

	ImColor SkeleColor = ImColor(red, green, blue);


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
			if (Debug::PrintPointers) 
			if (!Player) continue;
			if (Player == LocalPtrs::Player) continue;

			uintptr_t Mesh = read<uintptr_t>(Player + 0x318); //https://dumpspace.spuckwaffel.com/Games/?hash=6b77eceb&type=classes&idx=ACharacter&member=Mesh
			if (Debug::PrintPointers) Util::PrintPtr("Enemy Mesh: ", Mesh);
			if (!Mesh) continue;

			uintptr_t PlayerContrller = read<uintptr_t>(LocalPtrs::LocalPlayers + 0x30);
			//Util::PrintPtr("PlayerContrller: ", PlayerContrller);

			uintptr_t AcknowledgedPawn = read<uintptr_t>(PlayerContrller + 0x338);
			//Util::PrintPtr("AcknowledgedPawn: ", AcknowledgedPawn);

			uintptr_t CurrentWeaponList = read<uintptr_t>(AcknowledgedPawn + 0xa78);
			//Util::PrintPtr("CurrentWeaponList: ", CurrentWeaponList);

			uintptr_t CurrentWeapon = read<uintptr_t>(AcknowledgedPawn + 0xa68);


			for (int i = 0; i < sizeof(CurrentWeaponList); i++) {

			}

			int IsCached = read<int>(Mesh + 0x5F8);
			FTransform bonetrans[120] = {};

			driver::read_virtual_memory((void*)read<uintptr_t>(Mesh + 0x10 * IsCached + 0x5b0), bonetrans, sizeof(bonetrans));

			Vector3 Head3D = SDK::GetBoneWithRotation(Mesh, 110, bonetrans);
			Vector2 Head2D = SDK::ProjectWorldToScreen(Head3D);
			
			Vector3 Neck3D = SDK::GetBoneWithRotation(Mesh, 67, bonetrans);
			Vector2 Neck2D = SDK::ProjectWorldToScreen(Neck3D);

			Vector3 Pelvis3D = SDK::GetBoneWithRotation(Mesh, 2, bonetrans);
			Vector2 Pelvis2D = SDK::ProjectWorldToScreen(Pelvis3D);

			Vector3 RightShoulder3D = SDK::GetBoneWithRotation(Mesh, 9, bonetrans);
			Vector2 RightShoulder2D = SDK::ProjectWorldToScreen(RightShoulder3D);

			Vector3 RightElbow3D = SDK::GetBoneWithRotation(Mesh, 10, bonetrans); // 10
			Vector2 RightElbow2D = SDK::ProjectWorldToScreen(RightElbow3D);

			Vector3 RightWrist3D = SDK::GetBoneWithRotation(Mesh, 90, bonetrans);
			Vector2 RightWrist2D = SDK::ProjectWorldToScreen(RightWrist3D);

			Vector3 RightHip3D = SDK::GetBoneWithRotation(Mesh, 71, bonetrans);
			Vector2 RightHip2D = SDK::ProjectWorldToScreen(RightHip3D);

			Vector3 RightKnee3D = SDK::GetBoneWithRotation(Mesh, 72, bonetrans);
			Vector2 RightKnee2D = SDK::ProjectWorldToScreen(RightKnee3D);

			Vector3 RightAnkle3D = SDK::GetBoneWithRotation(Mesh, 75, bonetrans);
			Vector2 RightAnkle2D = SDK::ProjectWorldToScreen(RightAnkle3D);

			Vector3 RightFoot3D = SDK::GetBoneWithRotation(Mesh, 76, bonetrans);
			Vector2 RightFoot2D = SDK::ProjectWorldToScreen(RightFoot3D);

			Vector3 LeftShoulder3D = SDK::GetBoneWithRotation(Mesh, 38, bonetrans);
			Vector2 LeftShoulder2D = SDK::ProjectWorldToScreen(LeftShoulder3D);

			Vector3 LeftElbow3D = SDK::GetBoneWithRotation(Mesh, 39, bonetrans); // 105 and 106
			Vector2 LeftElbow2D = SDK::ProjectWorldToScreen(LeftElbow3D);

			Vector3 LeftWrist3D = SDK::GetBoneWithRotation(Mesh, 40, bonetrans); // 107 and 112
			Vector2 LeftWrist2D = SDK::ProjectWorldToScreen(LeftWrist3D);

			Vector3 LeftHip3D = SDK::GetBoneWithRotation(Mesh, 78, bonetrans);
			Vector2 LeftHip2D = SDK::ProjectWorldToScreen(LeftHip3D);

			Vector3 LeftKnee3D = SDK::GetBoneWithRotation(Mesh, 79, bonetrans);
			Vector2 LeftKnee2D = SDK::ProjectWorldToScreen(LeftKnee3D);

			Vector3 LeftAnkle3D = SDK::GetBoneWithRotation(Mesh, 82, bonetrans);
			Vector2 LeftAnkle2D = SDK::ProjectWorldToScreen(LeftAnkle3D);

			Vector3 LeftFoot3D = SDK::GetBoneWithRotation(Mesh, 83, bonetrans);
			Vector2 LeftFoot2D = SDK::ProjectWorldToScreen(LeftFoot3D);

			if (Debug::PrintLocations) Util::Print3D("Head3D: ", Head3D);
			if (Debug::PrintLocations) Util::Print2D("Head2D: ", Head2D);

			Vector3 Bottom3D = SDK::GetBoneWithRotation(Mesh, 0, bonetrans);
			Vector2 Bottom2D = SDK::ProjectWorldToScreen(Bottom3D);

			float BoxHeight = (float)(Head2D.y - Bottom2D.y);
			float CornerHeight = abs(Head2D.y - Bottom2D.y);
			float CornerWidth = BoxHeight * 0.4;




			if (bFov) {
				ImGui::GetOverlayDrawList()->AddCircle(center, FovSize, ImColor(0, 0, 0), 100, 2);
			}

			if (bLineEsp) {
				ImGui::GetOverlayDrawList()->AddLine(center, ImVec2(Head2D.x, Head2D.y), ImColor(255, 255, 255), 2);
			}

			if (bCornerBox) {
				Util::DrawCornerBox(Head2D.x - (CornerWidth / 2), Head2D.y - 85, CornerWidth, CornerHeight + 100, IM_COL32(0, 0, 0, 255), 2);
			}

				if (bBoxESP) {
					ImGui::GetOverlayDrawList()->AddRectFilled(ImVec2(Head2D.x - (CornerWidth / 2), Head2D.y - 75), ImVec2(Head2D.x + (CornerWidth / 2), Head2D.y + (CornerHeight + 20)), ImColor(red, green, blue, 125), 1, 100);
				}

				if (bCrosshair) {
					ImGui::GetOverlayDrawList()->AddLine(ImVec2(Width / 2 - 11, Height / 2), ImVec2(Width / 2 + 1, Height / 2), ImColor(255, 255, 255), 2);
					ImGui::GetOverlayDrawList()->AddLine(ImVec2(Width / 2 + 12, Height / 2), ImVec2(Width / 2 + 1, Height / 2), ImColor(255, 255, 255), 2);
					ImGui::GetOverlayDrawList()->AddLine(ImVec2(Width / 2, Height / 2 - 11), ImVec2(Width / 2, Height / 2), ImColor(255, 255, 255), 2);
					ImGui::GetOverlayDrawList()->AddLine(ImVec2(Width / 2, Height / 2 + 12), ImVec2(Width / 2, Height / 2), ImColor(255, 255, 255), 2);
				}
				if (bDistanceESP) {
					/*std::string distance_string = "" + std::to_string((int)dist) + " M";*/

					char fpsinfo[64];
					Vector3 bonehere = SDK::GetBoneWithRotation(Mesh, 106, bonetrans);

					Camera camera;
					float distance = camera.Location.Distance(Head3D) / 100.f;

					char name[64];
					sprintf_s(name, "[%2.fm]", distance);

					ImGui::GetOverlayDrawList()->AddText(ImGui::GetFont(), 0, ImVec2(Bottom2D.x, Bottom2D.y), ImColor(0, 0, 0), name);
				}

				if (bSkeletonESP) {


					ImGui::GetOverlayDrawList()->AddCircle(ImVec2(Head2D.x, Head2D.y - 15), 15, SkeleColor, 102, 2);


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
			
			auto dist = Util::GetCrossDistance(Head2D.x, Head2D.y, Width / 2, Height / 2);

			if (dist < FovSize && dist < ClosestDistance) {
				ClosestDistance = dist;
				TargetPawn = Player;
			}
		}
	}
}
