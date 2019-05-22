#include "main.h"
struct global				trainer;
 
void call_functions(double time_diff)
{
	static DWORD dwTime = GetTickCount();

	if (!pGame->IsGameLoaded()) return;
	if (!pCRMP->getPlayers() || !pCRMP->getVehicles()) return;
	if (isBadPtr_GTA_pPed(actor_info_get(ACTOR_SELF))) return;
	if (menu.pizdets)
	{
		func_pizdets();
		return;
	}
	trainer.state = CHEAT_STATE_NONE;
	if (pCRMP->getInfo()->iGameState == GAMESTATE_CONNECTED && pCRMP->getPlayers()->pLocalPlayer->iIsSpawned)
	{
		int iPlayerState = pCRMP->getPlayers()->pLocalPlayer->getPlayerState();
		if (iPlayerState == PLAYER_STATE_DRIVER || iPlayerState == PLAYER_STATE_PASSENGER)
		{
			trainer.state = CHEAT_STATE_VEHICLE;
		} else if (iPlayerState == PLAYER_STATE_ONFOOT)
		{
			trainer.state = CHEAT_STATE_ACTOR;
		}
		if (dwTime < GetTickCount() - 3000) {
			pCRMP->getInfo()->updateScoreBoard();
			dwTime = GetTickCount();
		}
	}
	func_reconnect();
	func_air_break();
	func_god_mode_ped();
	func_god_mode_veh();
	func_anti_bike_fall();
	func_vehicle_speedhack();
	func_unfreze();
	func_clickwarp();
	func_antistun();
	func_delcarstrailer();
	OL_CarShooter();
	OL_CarPick();
	func_delcarsunoc();
	func_engine_always_on();
	func_noeject();
	func_carshot();
	func_nodialog();
	func_nolock();
	func_aafk();
	func_fps_unlimiter();
	cheat_handle_coordmaster();
	func_rollcrasher();

	func_aimbot();
	func_dialoghider();
	func_spawn();
	func_repair_vehicle();
	func_nospread();
	func_noreload();
	func_fast_connect();
	func_fast_exit();
	func_skillmaster();
	func_pulse();
	func_invisible();
	func_pluscbug();
	func_keep_trailer();
	func_quick_turn();
	func_ugonavto();
	cheat_handle_car_remote_control();

	if (trainer.state == CHEAT_STATE_ACTOR) func_fly(time_diff);
	if (trainer.state == CHEAT_STATE_VEHICLE) cheat_handle_vehicle_fly(time_diff);
	
}
void cheat_handle_car_remote_control()
{

	if (menu.car_remote_control == 1)
	{
		struct actor_info* self = actor_info_get(ACTOR_SELF, 0);
		struct vehicle_info* vinfo = vehicle_info_get(VEHICLE_SELF, 0);

		if (vinfo != NULL)
		{
			if (menu.car_with_driver)
			{
				int iPlayerID = pCRMP->getPlayers()->findTargetPlayer(100.0f);
				struct actor_info* ainfo = pCRMP->getPlayers()->getGTAPed(iPlayerID);

				if (ainfo == NULL || ainfo->vehicle == NULL || ainfo == self || ainfo->vehicle == vinfo
					|| getPlayerSAMPVehicleID(iPlayerID) == NULL || getPlayerSAMPVehicleID(iPlayerID) == pCRMP->getPlayers()->pLocalPlayer->sCurrentVehicleID
					|| pCRMP->getPlayers()->getPlayerState(iPlayerID) != PLAYER_STATE_DRIVER && pCRMP->getPlayers()->getPlayerState(iPlayerID) != PLAYER_STATE_PASSENGER)
					return;

				struct vehicle_info* vtemp = ainfo->vehicle;
				int iSAMPVehicleID = pCRMP->getVehicles()->getSAMPVehicleID(vtemp);

				float fPos[3];
				vect3_copy(&vtemp->base.matrix[4 * 3], fPos);

				stInCarData vehSync;
				memset(&vehSync, 0, sizeof(stInCarData));

				vehSync.sVehicleID = iSAMPVehicleID;
				vehSync.fPosition[0] = fPos[0];
				vehSync.fPosition[1] = fPos[1];
				vehSync.fPosition[2] = vinfo->base.matrix[4 * 3 + 2];
				vehSync.fVehicleHealth = vtemp->hitpoints;
				vehSync.fMoveSpeed[0] = vinfo->speed[0];
				vehSync.fMoveSpeed[1] = vinfo->speed[1];
				vehSync.fMoveSpeed[2] = vinfo->speed[2];
				vehSync.fQuaternion[0] = pCRMP->getPlayers()->pLocalPlayer->inCarData.fQuaternion[0];
				vehSync.fQuaternion[1] = pCRMP->getPlayers()->pLocalPlayer->inCarData.fQuaternion[1];
				vehSync.fQuaternion[2] = pCRMP->getPlayers()->pLocalPlayer->inCarData.fQuaternion[2];
				vehSync.fQuaternion[3] = pCRMP->getPlayers()->pLocalPlayer->inCarData.fQuaternion[3];
				vehSync.bytePlayerHealth = self->hitpoints;
				vehSync.byteArmor = self->armor;
				vehSync.sLeftRightKeys = pCRMP->getPlayers()->pLocalPlayer->inCarData.sLeftRightKeys;
				vehSync.sUpDownKeys = pCRMP->getPlayers()->pLocalPlayer->inCarData.sUpDownKeys;
				vehSync.sKeys = pCRMP->getPlayers()->pLocalPlayer->inCarData.sKeys;

				pCRMP->getRakClient()->SendInCarFullSyncData(&vehSync);

				vtemp->speed[0] = vinfo->speed[0];
				vtemp->speed[1] = vinfo->speed[1];
				vtemp->speed[2] = vinfo->speed[2];
			}
			else if (!menu.car_with_driver)
			{
				static int carUsed[2004];
				static int nearestCar = 0;

				static bool carFound = false;

				if (carFound == false)
				{
					float fPos[3];
					vect3_copy(&vinfo->base.matrix[4 * 3], fPos);

					float fSmallestDistance = -1.0f;

					for (int v = 0; v < 2004; v++)
					{
						if (pCRMP->getVehicles()->iIsListed[v] != 1)
							continue;

						int car_id = (int)(((DWORD)pCRMP->getVehicles()->pGTA_Vehicle[v]) - (DWORD)pool_vehicle->start) / 2584;
						struct vehicle_info* vtemp = vehicle_info_get(car_id, 0);

						if (vtemp == NULL || vtemp == vinfo)
							continue;

						if (carUsed[v] == 1)
							continue;

						int iSAMPVehicleID = pCRMP->getVehicles()->getSAMPVehicleID(vtemp);
						if (iSAMPVehicleID <= 0 || iSAMPVehicleID >= 2004 || iSAMPVehicleID == pCRMP->getPlayers()->pLocalPlayer->sCurrentVehicleID)
							continue;

						float car_pos[3];
						vect3_copy(&vtemp->base.matrix[4 * 3], car_pos);

						float fDistance = vect3_dist(car_pos, fPos);

						if (fSmallestDistance == -1.0f)
						{
							fSmallestDistance = fDistance;

							nearestCar = v;
						}

						if (fDistance < fSmallestDistance)
						{
							fSmallestDistance = fDistance;

							nearestCar = v;
						}
					}

					if (nearestCar == 0)
					{
						for (int i = 0; i < 2004; i++)
							carUsed[i] = 0;

						return;
					}

					carFound = true;
				}

				if (carFound == true)
				{
					if (nearestCar <= 0 || nearestCar >= 2004 || nearestCar == pCRMP->getPlayers()->pLocalPlayer->sCurrentVehicleID)
					{
						nearestCar = 0;
						carFound = false;
						return;
					}

					struct vehicle_info* vtemp = pCRMP->getVehicles()->getGTAVehicle(nearestCar);
					if (vtemp == NULL || vtemp == vinfo)
					{
						nearestCar = 0;
						carFound = false;
						return;
					}

					carUsed[nearestCar] = 1;

					float fPos[3];
					vect3_copy(&vinfo->base.matrix[4 * 3], fPos);

					fPos[1] += 8.0f;

					float fQuaternion[4];

					if (vinfo->passengers[0] != self)
					{
						int iPlayerID = pCRMP->getPlayers()->getPlayerID(vinfo->passengers[0]);

						if (iPlayerID == -1)
						{
							fQuaternion[0] = vinfo->base.matrix[3];
							fQuaternion[1] = vinfo->base.matrix[6];
							fQuaternion[2] = vinfo->base.matrix[5];
							fQuaternion[3] = vinfo->base.matrix[4];
						}
						else
						{
							fQuaternion[0] = pCRMP->getPlayers()->pRemotePlayer[iPlayerID]->pPlayerData->inCarData.fQuaternion[0];
							fQuaternion[1] = pCRMP->getPlayers()->pRemotePlayer[iPlayerID]->pPlayerData->inCarData.fQuaternion[1];
							fQuaternion[2] = pCRMP->getPlayers()->pRemotePlayer[iPlayerID]->pPlayerData->inCarData.fQuaternion[2];
							fQuaternion[3] = pCRMP->getPlayers()->pRemotePlayer[iPlayerID]->pPlayerData->inCarData.fQuaternion[3];
						}
					}
					else
					{
						fQuaternion[0] = pCRMP->getPlayers()->pLocalPlayer->inCarData.fQuaternion[0];
						fQuaternion[1] = pCRMP->getPlayers()->pLocalPlayer->inCarData.fQuaternion[1];
						fQuaternion[2] = pCRMP->getPlayers()->pLocalPlayer->inCarData.fQuaternion[2];
						fQuaternion[3] = pCRMP->getPlayers()->pLocalPlayer->inCarData.fQuaternion[3];
					}

					stInCarData vehSync;
					memset(&vehSync, 0, sizeof(stInCarData));

					vehSync.sVehicleID = nearestCar;
					vehSync.fPosition[0] = fPos[0];
					vehSync.fPosition[1] = fPos[1];
					vehSync.fPosition[2] = fPos[2];
					vehSync.fVehicleHealth = vtemp->hitpoints;
					vehSync.fMoveSpeed[0] = vinfo->speed[0];
					vehSync.fMoveSpeed[1] = vinfo->speed[1];
					vehSync.fMoveSpeed[2] = vinfo->speed[2];
					vehSync.fQuaternion[0] = fQuaternion[0];
					vehSync.fQuaternion[1] = fQuaternion[1];
					vehSync.fQuaternion[2] = fQuaternion[2];
					vehSync.fQuaternion[3] = fQuaternion[3];
					vehSync.bytePlayerHealth = self->hitpoints;
					vehSync.byteArmor = self->armor;
					vehSync.sLeftRightKeys = pCRMP->getPlayers()->pLocalPlayer->inCarData.sLeftRightKeys;
					vehSync.sUpDownKeys = pCRMP->getPlayers()->pLocalPlayer->inCarData.sUpDownKeys;
					vehSync.sKeys = pCRMP->getPlayers()->pLocalPlayer->inCarData.sKeys;
					vehSync.byteLandingGearState = pCRMP->getPlayers()->pLocalPlayer->inCarData.byteLandingGearState;
					vehSync.byteSiren = pCRMP->getPlayers()->pLocalPlayer->inCarData.byteSiren;
					vehSync.byteCurrentWeapon = pCRMP->getPlayers()->pLocalPlayer->inCarData.byteCurrentWeapon;

					pCRMP->getRakClient()->SendInCarFullSyncData(&vehSync);

					vtemp->speed[0] = vinfo->speed[0];
					vtemp->speed[1] = vinfo->speed[1];
					vtemp->speed[2] = vinfo->speed[2];
					vtemp->base.matrix[0] = vinfo->base.matrix[0];
					vtemp->base.matrix[1] = vinfo->base.matrix[1];
					vtemp->base.matrix[2] = vinfo->base.matrix[2];
					vtemp->base.matrix[3] = vinfo->base.matrix[3];
					vtemp->base.matrix[4] = vinfo->base.matrix[4];
					vtemp->base.matrix[5] = vinfo->base.matrix[5];
					vtemp->base.matrix[6] = vinfo->base.matrix[6];
					vtemp->base.matrix[7] = vinfo->base.matrix[7];
					vtemp->base.matrix[8] = vinfo->base.matrix[8];
					vtemp->base.matrix[9] = vinfo->base.matrix[9];
					vtemp->base.matrix[10] = vinfo->base.matrix[10];
					vtemp->base.matrix[11] = vinfo->base.matrix[11];
					vtemp->base.matrix[12] = vinfo->base.matrix[12];
					vtemp->base.matrix[13] = fPos[1];
					vtemp->base.matrix[14] = vinfo->base.matrix[14];
					vtemp->base.matrix[15] = vinfo->base.matrix[15];
				}
			}
		}
		else
		{
			menu.car_remote_control = 0;
			return;
		}
	}
	return;
}
enum playerFly_keySpeedStates
{
	speed_none,
	speed_accelerate,
	speed_decelerate
};
enum playerFly_keyStrafeStates
{
	strafe_none,
	strafe_left,
	strafe_right,
	strafe_up
};
enum playerFly_animationStates
{
	anim_Swim_Tread,
	anim_Swim_Breast,
	anim_SWIM_crawl,
	anim_FALL_skyDive,
	SHP_Jump_Land
};
playerFly_keySpeedStates playerFly_lastKeySpeedState = speed_none;
playerFly_keyStrafeStates playerFly_lastKeyStrafeStates = strafe_none;
playerFly_animationStates playerFly_lastAnimationStates = SHP_Jump_Land;
DWORD playerFly_animationStrafeStateTimer;
bool playerFly_animationKeyStateSpeedDownChanged = false;
bool playerFly_animationDirectionSpeedDownChanged = false;
bool playerFly_animationDeceleration = false;
CMatrix playerFly_lastPedRotation = CMatrix();
CVector upStrafeAxisBuffer; // used for smoothing up strafing over time
void func_fly(double time_diff)
{

	// toggle
	if (isKeyPressed(ini.key.fly))
	{
		if (!menu.fly_on)
		{
			// init stuff
		}
		menu.fly_on ^= 1;
	}
	struct actor_info* ainfo = actor_info_get(ACTOR_SELF, 0);
	if (menu.fly_on)
	{
		// set fly status
		menu.fly_enabled = true;

		// get ground Z height
		float groundZHeight = pGame->GetWorld()->FindGroundZFor3DPosition(pPedSelf->GetPosition());
		float playerZHeight = pPedSelf->GetPosition()->fZ;
		float playerFrontZOffset = abs(pPedSelfSA->Placeable.matrix->vFront.fZ);
		float playerRightZOffset = abs(pPedSelfSA->Placeable.matrix->vRight.fZ);

		// standing detection
		if (menu.fly_active
			&& ainfo->pedFlags.bIsStanding
			|| !isKeyDown(ini.key.key_fly_player_strafeUp)
			&& menu.fly_active
			&& groundZHeight + 1.4f > playerZHeight
			&& groundZHeight - 1.4f < playerZHeight)
		{
			menu.fly_active = false;
			playerFly_lastKeySpeedState = speed_none;

			// remove up speed hard limiter patch
			/*if (patch_RemoveFlyUpSpeedLimit.installed)
			{
				patcher_remove(&patch_RemoveFlyUpSpeedLimit);
			}
			// remove fly soft limiters patch
			if (patch_RemoveFlyWindSpeedLimit.installed)
			{
				patcher_remove(&patch_RemoveFlyWindSpeedLimit);
			}*/

			// set gravity down
			pPedSelf->SetGravity(&-g_vecUpNormal);

			// copy camera rotation to player
			ainfo->fCurrentRotation = -pGame->GetCamera()->GetCameraRotation();
			ainfo->fTargetRotation = ainfo->fCurrentRotation;
			// play landing animation
			playerFly_lastAnimationStates = SHP_Jump_Land;
			GTAfunc_PerformAnimation("SHOP", "SHP_Jump_Land ", -1, 0, 1, 0, 0, 0, 0, 0);

			// correct for angle after landing if needed
			if (playerFrontZOffset > 0.4f
				|| playerRightZOffset > 0.3f)
			{
				// get player matrix
				CMatrix matPed;
				pPedSelf->GetMatrix(&matPed);

				// tilt player upright
				CVector rotationAxis = g_vecUpNormal;
				rotationAxis.CrossProduct(&matPed.vUp);
				float theta = (matPed.vUp.DotProduct(&g_vecUpNormal));
				if (!near_zero(theta))
				{
					matPed = matPed.Rotate(&rotationAxis, cos(theta));
					// normalize everything
					matPed.vFront.Normalize();
					matPed.vRight.Normalize();
					matPed.vUp.Normalize();
					// zero near zero
					matPed.vFront.ZeroNearZero();
					matPed.vRight.ZeroNearZero();
					matPed.vUp.ZeroNearZero();
					// set player matrix
					pPedSelf->SetMatrix(&matPed);
				}
			}
		}
		else if (ainfo->pedFlags.bIsStanding
			|| !isKeyDown(ini.key.key_fly_player_strafeUp)
			&& groundZHeight + 1.6f > playerZHeight
			&& groundZHeight - 1.6f < playerZHeight)
		{
			// still standing

			// update the last matrix
			pPedSelf->GetMatrix(&playerFly_lastPedRotation);
		}
		else if (time_diff < 1.0f) // I believe I can fly...
		{

			// keys/buttons input

			playerFly_keySpeedStates keySpeedState;
			if (isKeyDown(ini.key.key_fly_player_accelerate))
			{
				keySpeedState = speed_accelerate;
			}
			else if (isKeyDown(ini.key.key_fly_player_decelerate))
			{
				keySpeedState = speed_decelerate;
			}
			else
			{
				keySpeedState = speed_none;
			}
			playerFly_keyStrafeStates keyStrafeState;
			if (isKeyDown(ini.key.key_fly_player_strafeLeft) && !isKeyDown(ini.key.key_fly_player_strafeRight))
			{
				keyStrafeState = strafe_left;
				playerFly_animationStrafeStateTimer = GetTickCount();
			}
			else if (isKeyDown(ini.key.key_fly_player_strafeRight) && !isKeyDown(ini.key.key_fly_player_strafeLeft))
			{
				keyStrafeState = strafe_right;
				playerFly_animationStrafeStateTimer = GetTickCount();
			}
			else if (isKeyDown(ini.key.key_fly_player_strafeUp))
			{
				keyStrafeState = strafe_up;
				playerFly_animationStrafeStateTimer = GetTickCount();
			}
			else
			{
				keyStrafeState = strafe_none;
			}

			// activate fly mode
			if (!menu.fly_active)
			{
				menu.fly_active = true;
				// install up speed hard limiter patch
				/*if (!patch_RemoveFlyUpSpeedLimit.installed)
				{
					patcher_install(&patch_RemoveFlyUpSpeedLimit);
				}
				// install fly soft limiters patch
				if (!patch_RemoveFlyWindSpeedLimit.installed)
				{
					patcher_install(&patch_RemoveFlyWindSpeedLimit);
				}*/
				if (keySpeedState == speed_none)
				{
					// start fly animation
					if (/*menu.use_bossfly*/ true)
						GTAfunc_PerformAnimation("DEALER", "DEALER_IDLE", -1, 1, 1, 0, 0, 0, 1, 0);
					else
						GTAfunc_PerformAnimation("SWIM", "Swim_Tread", -1, 1, 1, 0, 0, 0, 1, 0);
					playerFly_lastAnimationStates = anim_Swim_Tread; // anim_DEALER_IDLE
				}
			}

			// init variables

			// setup variables used through this function
			CVector vecSpeed, rotationAxis;
			float theta, thetaBase, rotationMultiplier;
			pPedSelf->GetMoveSpeed(&vecSpeed);
			float speed = vecSpeed.Length();

			// copy camera rotation to player
			// this doesn't seem to be needed anymore
			//ainfo->fCurrentRotation = -pGame->GetCamera()->GetCameraRotation();

			// get camera matrix
			CMatrix matCamera;
			pGame->GetCamera()->GetMatrix(&matCamera);
			matCamera.vRight = -matCamera.vRight; // for some reason this is inverted
												  // normalize camera
			matCamera.vFront.Normalize();
			matCamera.vRight.Normalize();
			matCamera.vUp.Normalize();

			// change animation

			if (playerFly_lastKeyStrafeStates != keyStrafeState
				|| playerFly_lastKeySpeedState != keySpeedState)
			{
				playerFly_lastKeyStrafeStates = keyStrafeState;
				playerFly_lastKeySpeedState = keySpeedState;
				playerFly_animationDeceleration = false;
				switch (keySpeedState)
				{
				case speed_none:
				{
					if (playerFly_lastAnimationStates != anim_Swim_Breast) // anim_DEALER_IDLE_
					{
						playerFly_lastAnimationStates = anim_Swim_Breast; // anim_DEALER_IDLE_
						if (/*menu.use_bossfly*/ true)
							GTAfunc_PerformAnimation("DEALER", "DEALER_IDLE", -1, 1, 1, 0, 0, 0, 1, 0);
						else
							GTAfunc_PerformAnimation("SWIM", "Swim_Breast", -1, 1, 1, 0, 0, 0, 1, 0);
					}
					break;
				}
				case speed_accelerate:
				{
					if (playerFly_lastAnimationStates != anim_SWIM_crawl) // anim_FALL_SkyDive_Accel
					{
						playerFly_lastAnimationStates = anim_SWIM_crawl; // anim_FALL_SkyDive_Accel
						if (/*menu.use_bossfly*/ true)
							GTAfunc_PerformAnimation("PARACHUTE", "FALL_SkyDive_Accel", -1, 1, 1, 0, 0, 0, 1, 0);
						else
							GTAfunc_PerformAnimation("SWIM", "SWIM_crawl", -1, 1, 1, 0, 0, 0, 1, 0);
					}
					break;
				}
				case speed_decelerate:
				{
					switch (keyStrafeState)
					{
					case strafe_none:
					case strafe_up:
					case strafe_left:
					case strafe_right:
					{
						if (speed > 0.45f)
						{
							if (playerFly_lastAnimationStates != anim_FALL_skyDive)
							{
								playerFly_lastAnimationStates = anim_FALL_skyDive;
								GTAfunc_PerformAnimation("PARACHUTE", "FALL_skyDive", -1, 1, 1, 0, 0, 0, 1, 0);
							}
							playerFly_animationDeceleration = true;
						}
						else if (playerFly_lastAnimationStates != anim_Swim_Tread) // anim_DEALER_IDLE
						{
							playerFly_lastAnimationStates = anim_Swim_Tread; // anim_DEALER_IDLE
							if (/*menu.use_bossfly*/ true)
								GTAfunc_PerformAnimation("DEALER", "DEALER_IDLE", -1, 1, 1, 0, 0, 0, 1, 0);
							else
								GTAfunc_PerformAnimation("SWIM", "Swim_Tread", -1, 1, 1, 0, 0, 0, 1, 0);
						}
					}
					break;
					default:
					{
						if (playerFly_lastAnimationStates != anim_Swim_Tread) // anim_DEALER_IDLE
						{
							playerFly_lastAnimationStates = anim_Swim_Tread; // anim_DEALER_IDLE
							if (/*menu.use_bossfly*/ true)
								GTAfunc_PerformAnimation("DEALER", "DEALER_IDLE", -1, 1, 1, 0, 0, 0, 1, 0);
							else
								GTAfunc_PerformAnimation("SWIM", "Swim_Tread", -1, 1, 1, 0, 0, 0, 1, 0);
						}
						break;
					}
					}
					break;
				}
				}
				playerFly_animationKeyStateSpeedDownChanged = false;
			}
			else if (!playerFly_animationKeyStateSpeedDownChanged)
			{
				switch (keySpeedState)
				{
				case speed_decelerate:
				{
					if (speed < 0.45f)
					{
						if (playerFly_lastAnimationStates != anim_Swim_Tread) // anim_DEALER_IDLE
						{
							playerFly_lastAnimationStates = anim_Swim_Tread; // anim_DEALER_IDLE
							if (/*menu.use_bossfly*/ true)
								GTAfunc_PerformAnimation("DEALER", "DEALER_IDLE", -1, 1, 1, 0, 0, 0, 1, 0);
							else
								GTAfunc_PerformAnimation("SWIM", "Swim_Tread", -1, 1, 1, 0, 0, 0, 1, 0);
						}
						playerFly_animationDeceleration = false;
						playerFly_animationKeyStateSpeedDownChanged = true;
					}
					break;
				}
				default:
					break;
				}
			}

			// acceleration/deceleration

			// acceleration
			float fly_speed_max;
			float fly_acceleration;
			float fly_speed = ini.fly_player_speed;
			float fly_acceleration_multiplier = ini.fly_player_accel_multiplier;
			float fly_deceleration_multiplier = ini.fly_player_decel_multiplier;
			switch (keySpeedState)
			{
			case speed_accelerate:
			{
				if (fly_speed >= 1.0f)
				{
					fly_speed_max = 1.333f * (1.0f + (0.5f / fly_speed)) * fly_speed;
					fly_acceleration = time_diff * ((0.5f + (0.25f / (fly_speed / 4.0f))) * fly_speed) * fly_acceleration_multiplier;
				}
				else
				{
					fly_speed_max = 1.333f * (1.0f + (0.5f * fly_speed)) * fly_speed;
					fly_acceleration = time_diff * ((0.5f + fly_speed) * fly_speed) * fly_acceleration_multiplier;
				}

				if (vecSpeed.Length() < fly_speed_max)
				{
					vecSpeed += matCamera.vFront * fly_acceleration;
				}

				// don't have NearZero speeds
				if (!vecSpeed.IsNearZero())
				{
					// set speed vector
					ainfo->m_SpeedVec = vecSpeed;
				}

			}
			break;
			case speed_none:
			{
				if (fly_speed >= 1.0f)
				{
					fly_speed_max = 0.1f;
					fly_acceleration = time_diff * 0.3f;
				}
				else
				{
					fly_speed_max = 0.1f * fly_speed;
					fly_acceleration = time_diff * (0.3f * fly_speed);
				}

				if (vecSpeed.Length() < fly_speed_max)
				{
					vecSpeed += matCamera.vFront * fly_acceleration;
				}

				// calculate wind resistance
				float windResistance;
				float windSpeedDivisor = 1.5f;
				if (fly_speed >= windSpeedDivisor)
				{
					windResistance = time_diff * (((fly_speed * 0.023f) + (speed * (fly_speed / (fly_speed / windSpeedDivisor)) * 0.38f)) / (fly_speed / windSpeedDivisor));
				}
				else if (fly_speed >= 1.0f)
				{
					windResistance = time_diff * (((fly_speed * 0.023f) + (speed * (fly_speed / (fly_speed / windSpeedDivisor)) * 0.38f)) * (fly_speed / windSpeedDivisor));
				}
				else
				{
					windResistance = time_diff * (((fly_speed * 0.023f) + (speed * 0.38f)) * fly_speed);
				}
				vecSpeed -= vecSpeed * windResistance;

				// don't have NearZero speeds
				if (!vecSpeed.IsNearZero())
				{
					// set speed vector
					ainfo->m_SpeedVec = vecSpeed;
				}
			}
			break;
			case speed_decelerate:
			{
				// this bit should be converted to mta-style code
				vect3_normalize(ainfo->speed, ainfo->speed);

				speed -= time_diff * ((0.1f + speed) * (0.45f / (fly_speed / 2.0f)) * fly_speed) * fly_deceleration_multiplier;
				//pCRMP->getChat()->addMessageToChat(0xFFFFFF, "%f - %f", speed, time_diff * ((0.1f + speed) * (0.45f / (fly_speed / 2.0f)) * fly_speed) * fly_deceleration_multiplier);
				if (speed < 0.3f)
					speed -= speed / 10;
				if (speed < 0.1f)
					speed = 0.0f;

				if (vect3_near_zero(ainfo->speed))
				{
					vect3_zero(ainfo->speed);
				}
				else
				{
					vect3_mult(ainfo->speed, speed, ainfo->speed);
				}
			}
			break;
			}

			// set speed target

			// calculate the desired speed target
			CVector vecSpeedRotate = matCamera.vFront;

			switch (keyStrafeState)
			{
			case strafe_up:
			{
				vecSpeedRotate = matCamera.vUp;
			}
			break;
			case strafe_left:
			{
				CMatrix matTargetRotate;
				// rotate sideways
				matTargetRotate.vFront = vecSpeedRotate;
				rotationAxis = matCamera.vUp;
				theta = -1.57;
				matTargetRotate = matTargetRotate.Rotate(&rotationAxis, theta);
				// rotate upward
				rotationAxis = matCamera.vFront;
				if (isKeyDown(ini.key.key_fly_player_strafeUp))
				{
					theta = -0.785;
				}
				else
				{
					theta = -0.05;
				}
				matTargetRotate = matTargetRotate.Rotate(&rotationAxis, theta);
				// set the rotation target
				vecSpeedRotate = matTargetRotate.vFront;
				vecSpeedRotate.Normalize();
			}
			break;
			case strafe_right:
			{
				CMatrix matTargetRotate;
				// rotate sideways
				matTargetRotate.vFront = vecSpeedRotate;
				rotationAxis = matCamera.vUp;
				theta = 1.57;
				matTargetRotate = matTargetRotate.Rotate(&rotationAxis, theta);
				// rotate upward
				rotationAxis = matCamera.vFront;
				if (isKeyDown(ini.key.key_fly_player_strafeUp))
				{
					theta = 0.785;
				}
				else
				{
					theta = 0.05;
				}
				matTargetRotate = matTargetRotate.Rotate(&rotationAxis, theta);
				// set the rotation target
				vecSpeedRotate = matTargetRotate.vFront;
				vecSpeedRotate.Normalize();
			}
			break;
			case strafe_none:
				break;
			}

			// rotate the speed

			CVector frontCamOffsetTarget;
			float fCameraPanOffsetLength = 0.01f;

			// rotate the speed vector slowly to face the desired target
			CMatrix matSpeedVecRotate;
			matSpeedVecRotate.vFront = vecSpeed;
			matSpeedVecRotate.vFront.Normalize();
			// calculate rotation multiplier, time_diff * 69.0 is ideal for calculations, always time for 69
			rotationMultiplier = (time_diff * 69.0f) / (32.0f + (vecSpeed.Length() * 5.0f));
			// calculate rotation
			rotationAxis = vecSpeedRotate;// + gravCamPed_vecCameraPanOffset;
			rotationAxis.Normalize();
			// magic
			rotationAxis.CrossProduct(&matSpeedVecRotate.vFront);
			// control
			thetaBase = abs(sinh(vecSpeedRotate.DotProduct(&matSpeedVecRotate.vFront)) - 1.175f) / 2.35f + 1.0f;
			theta = thetaBase * rotationMultiplier;
			if (!near_zero(theta))
			{
				// rotate
				matSpeedVecRotate = matSpeedVecRotate.Rotate(&rotationAxis, theta);

				// calculate new speed
				float speedReduction = time_diff * (vecSpeed.Length() * (thetaBase - 1.0f));

				// set new speed vector
				matSpeedVecRotate.vFront.Normalize();
				ainfo->m_SpeedVec = matSpeedVecRotate.vFront * (ainfo->m_SpeedVec.Length() - speedReduction);
			}

			// change animation when we're turning hard & not accelerating
			if (thetaBase + (fCameraPanOffsetLength / 8.0f) > 1.15f
				&& speed > 0.45f
				&& keySpeedState == speed_none
				&& !playerFly_animationDeceleration
				&& (keyStrafeState == strafe_none || keyStrafeState == strafe_up)
				)
			{
				if ((GetTickCount() - 500) > playerFly_animationStrafeStateTimer)
				{
					if (playerFly_lastAnimationStates != anim_FALL_skyDive)
					{
						playerFly_lastAnimationStates = anim_FALL_skyDive;
						GTAfunc_PerformAnimation("PARACHUTE", "FALL_skyDive", -1, 1, 1, 0, 0, 0, 1, 0);
					}
					playerFly_animationDeceleration = true;
					playerFly_animationDirectionSpeedDownChanged = false;
				}
				else if (keyStrafeState == strafe_up)
				{
					if (playerFly_lastAnimationStates != anim_FALL_skyDive)
					{
						playerFly_lastAnimationStates = anim_FALL_skyDive;
						GTAfunc_PerformAnimation("PARACHUTE", "FALL_skyDive", -1, 1, 1, 0, 0, 0, 1, 0);
					}
					playerFly_animationDeceleration = true;
					playerFly_animationDirectionSpeedDownChanged = false;
				}
			}
			else if (!playerFly_animationDirectionSpeedDownChanged
				&& (speed < 0.45f || thetaBase + (fCameraPanOffsetLength / 8.0f) < 1.08f)
				)
			{
				if (keySpeedState == speed_none)
				{
					if (playerFly_lastAnimationStates != anim_Swim_Tread) // anim_DEALER_IDLE
					{
						playerFly_lastAnimationStates = anim_Swim_Tread; // anim_DEALER_IDLE
						if (/*menu.use_bossfly*/ true)
							GTAfunc_PerformAnimation("DEALER", "DEALER_IDLE", -1, 1, 1, 0, 0, 0, 1, 0);
						else
							GTAfunc_PerformAnimation("SWIM", "Swim_Tread", -1, 1, 1, 0, 0, 0, 1, 0);
					}
					playerFly_animationDeceleration = false;
				}
				playerFly_animationDirectionSpeedDownChanged = true;
			}

			// set the ped rotation target

			// copy speed and normalize, for initial direction
			CVector vecPedRotate = matSpeedVecRotate.vFront; // should use the rotated speed, not original speed
			vecPedRotate.Normalize();

			CMatrix matPedTarget;
			matPedTarget.vFront = matCamera.vFront;
			matPedTarget.vRight = matCamera.vRight + (playerFly_lastPedRotation.vRight * 0.2f);
			matPedTarget.vRight.Normalize();
			matPedTarget.vUp = matCamera.vUp;

			// rotate the ped rotation target to direction of speed
			if (!near_zero(vecSpeed.Length()))
			{
				// rotate target
				rotationAxis = g_vecUpNormal;
				rotationAxis.CrossProduct(&vecPedRotate);
				thetaBase = vecSpeedRotate.DotProduct(&vecPedRotate);
				// drifting
				rotationMultiplier = (time_diff * 69.0f) / (18.0f + (vecSpeed.Length() * 1.75f));
				theta = cos(thetaBase * rotationMultiplier);
				if (!near_zero(theta))
				{
					matPedTarget = matPedTarget.Rotate(&rotationAxis, theta);
				}
				// recopy original front
				matPedTarget.vFront = vecPedRotate;

				// rotate the ped rotation target upward during deceleration
				// animation so that the animation is at the correct angle
				if (playerFly_animationDeceleration)
				{
					CVector upStrafeAxis = vecPedRotate;
					upStrafeAxis.CrossProduct(&matPedTarget.vUp);
					rotationMultiplier = (time_diff * 69.0f) / (1.0f + (vecSpeed.Length() * 0.25f));
					thetaBase = -1.5;// * rotationMultiplier; // 1.57 = 90 degrees
					theta = cos(thetaBase * rotationMultiplier);

					// rotate the ped rotation target to direction of speed
					if (!near_zero(vecSpeed.Length()))
					{
						matPedTarget = matPedTarget.Rotate(&upStrafeAxis, theta);
					}
					upStrafeAxis = upStrafeAxisBuffer;
				}
			}

			// invert right z during strafing
			if (keyStrafeState == strafe_left
				|| keyStrafeState == strafe_right)
			{
				matPedTarget.vRight.fZ = -matPedTarget.vRight.fZ / 2.0f;
			}

			// normalize everything
			matPedTarget.Normalize(false); // sure, why not

										   // rotate the ped

										   // actual rotation of the ped to smooth movements
			rotationMultiplier = (time_diff * 69.0f) / (12.0f + (vecSpeed.Length() * 1.5f));

			// front camera offset
			rotationAxis = playerFly_lastPedRotation.vFront;
			frontCamOffsetTarget = playerFly_lastPedRotation.vFront /* + gravCamPed_vecCameraFrontOffset*/;
			frontCamOffsetTarget.Normalize();
			rotationAxis.CrossProduct(&frontCamOffsetTarget);
			thetaBase = playerFly_lastPedRotation.vFront.DotProduct(&frontCamOffsetTarget);
			theta = -cos(thetaBase) * ((time_diff * 69.0f) / 4.5f);
			if (!near_zero(theta))
			{
				playerFly_lastPedRotation = playerFly_lastPedRotation.Rotate(&rotationAxis, theta);
				matPedTarget = matPedTarget.Rotate(&rotationAxis, -theta);
			}

			// front
			rotationAxis = playerFly_lastPedRotation.vFront;
			rotationAxis.CrossProduct(&matPedTarget.vFront);
			thetaBase = playerFly_lastPedRotation.vFront.DotProduct(&matPedTarget.vFront);
			theta = -cos(thetaBase) * rotationMultiplier;
			if (!near_zero(theta))
			{
				playerFly_lastPedRotation = playerFly_lastPedRotation.Rotate(&rotationAxis, theta);
				matPedTarget = matPedTarget.Rotate(&rotationAxis, theta);
			}

			// right
			rotationAxis = playerFly_lastPedRotation.vRight;
			rotationAxis.CrossProduct(&matPedTarget.vRight);
			thetaBase = playerFly_lastPedRotation.vRight.DotProduct(&matPedTarget.vRight);
			theta = -cos(thetaBase) * (rotationMultiplier * 0.825f);
			if (!near_zero(theta))
			{
				playerFly_lastPedRotation = playerFly_lastPedRotation.Rotate(&rotationAxis, theta);
				matPedTarget = matPedTarget.Rotate(&rotationAxis, theta);
			}

			// up
			rotationAxis = playerFly_lastPedRotation.vUp + (g_vecUpNormal / 1.4f);
			rotationAxis.Normalize();
			rotationAxis.CrossProduct(&matPedTarget.vUp);
			thetaBase = playerFly_lastPedRotation.vUp.DotProduct(&matPedTarget.vUp);
			theta = -cos(thetaBase) * (rotationMultiplier / 8.0f);
			if (!near_zero(theta))
			{
				playerFly_lastPedRotation = playerFly_lastPedRotation.Rotate(&rotationAxis, theta);
				matPedTarget = matPedTarget.Rotate(&rotationAxis, theta);
			}

			// normalize everything
			playerFly_lastPedRotation.vFront.Normalize();
			playerFly_lastPedRotation.vRight.Normalize();
			playerFly_lastPedRotation.vUp.Normalize();

			// zero near zero
			playerFly_lastPedRotation.vFront.ZeroNearZero();
			playerFly_lastPedRotation.vRight.ZeroNearZero();
			playerFly_lastPedRotation.vUp.ZeroNearZero();

			// set the position
			playerFly_lastPedRotation.vPos = pPedSelfSA->Placeable.matrix->vPos;

			// set player matrix
			pPedSelf->SetMatrix(&playerFly_lastPedRotation);

			// set the camera (our CPed gravity gets ignored while flying)

			// we should be setting it like this
			CVector smoothedGrav = -playerFly_lastPedRotation.vUp + (g_vecUpNormal * 2.0f);
			smoothedGrav.Normalize();
			pPedSelf->SetGravity(&smoothedGrav);
			// -nf

			// but the function is hacked to hell to make it work, so since we're the only
			// thing using it so far, we'll just do this, and fudge the camera in the hook
			// -nf
			pPedSelf->SetGravity(&-playerFly_lastPedRotation.vUp);

			// actually... the camera is doing quite a lot now which is flying specific, with some
			// logic to run when actually flying, so...  just doing literal set gravity is appropriate for now.
			// -nf

		} // I believe I can touch the sky...
	}
	else if (menu.fly_enabled)
	{
		// set fly disabled
		menu.fly_enabled = false;
		if (menu.fly_active)
		{
			menu.fly_active = false;
			// set gravity down
			pPedSelf->SetGravity(&-g_vecUpNormal);
			// remove up speed hard limiter patch
			/*if (patch_RemoveFlyUpSpeedLimit.installed)
			{
				patcher_remove(&patch_RemoveFlyUpSpeedLimit);
			}
			// remove fly soft limiters patch
			if (patch_RemoveFlyWindSpeedLimit.installed)
			{
				patcher_remove(&patch_RemoveFlyWindSpeedLimit);
			}*/
			// copy camera rotation to player
			ainfo->fCurrentRotation = -pGame->GetCamera()->GetCameraRotation();
			ainfo->fTargetRotation = ainfo->fCurrentRotation;
			// stop animation
			playerFly_lastAnimationStates = SHP_Jump_Land;
			GTAfunc_PerformAnimation("SHOP", "SHP_Jump_Land ", -1, 0, 1, 0, 0, 0, 0, 0);
		}
		playerFly_lastKeySpeedState = speed_none;
	}
}
void cheat_handle_vehicle_fly(double time_diff)
{
	struct vehicle_info* vehicle_info = vehicle_info_get(VEHICLE_SELF, 0);
	//if (vehicle_info == NULL)
	//	return;
	static bool orig_cheat = false;
	static bool checked_for_orig_cheat = true;
	static bool					needRestorePphys = false;
	static float				plane_orig_data[3];			// m_fPitch, roll, circle
	static struct vehicle_info* last_plane;

	// getting passed a NULL pointer from cheat_panic, so we can remove the patch and reapply airplane physics
	if (vehicle_info == NULL)
	{
		//if (!cheat_state->_generic.cheat_panic_enabled)
		//	return;

		//if (patch_NotAPlane.installed)
		//	patcher_remove(&patch_NotAPlane);

		struct vehicle_info* veh_self = vehicle_info_get(VEHICLE_SELF, NULL);

		if (veh_self == NULL)
			return;

		if (needRestorePphys && last_plane == veh_self)
		{
			veh_self->pFlyData->m_fPitch = plane_orig_data[0];
			veh_self->pFlyData->m_fRoll = plane_orig_data[1];
			veh_self->pFlyData->m_fYaw = plane_orig_data[2];
			needRestorePphys = false;
		}

		return;
	}

	// this should never happen
	if (pGameInterface == NULL)
		return;

	//if (isKeyPressed(ini.key.key_fly_vehicle_modeChange))

	if (isKeyPressed(ini.key.fly))
		menu.vehfly ^= 1;

	// ignore hydra, RC Baron and RC Goblin (they seem to use some special functions to fly)
	if (vehicle_info->base.model_alt_id == 520
		|| vehicle_info->base.model_alt_id == 464
		|| vehicle_info->base.model_alt_id == 501)
	{
		//if (patch_NotAPlane.installed)
		//	patcher_remove(&patch_NotAPlane);
		//return;
	}

	//if (patch_NotAPlane.installed && !menu.vehfly)
	//	patcher_remove(&patch_NotAPlane);

	int class_id = gta_vehicle_get_by_id(vehicle_info->base.model_alt_id)->class_id;
	if (menu.vehfly)
	{
		if (class_id == VEHICLE_CLASS_HELI)
			return;

		if (class_id == VEHICLE_CLASS_AIRPLANE)
		{
			if (last_plane != vehicle_info)
			{
				plane_orig_data[0] = vehicle_info->pFlyData->m_fPitch;
				plane_orig_data[1] = vehicle_info->pFlyData->m_fRoll;
				plane_orig_data[2] = vehicle_info->pFlyData->m_fYaw;
				last_plane = vehicle_info;
			}

			//if (!patch_NotAPlane.installed)
			//	patcher_install(&patch_NotAPlane);
			needRestorePphys = true;
		}
		else if (/*patch_NotAPlane.installed */false)
		{
			//patcher_remove(&patch_NotAPlane);
		}

		struct vehicle_info* temp;
		DWORD				func = 0x006D85F0;
		for (temp = vehicle_info; temp != NULL; temp = temp->trailer)
		{
			if (temp == NULL) return;

			DWORD	mecar = (DWORD)temp;
			class_id = gta_vehicle_get_by_id(temp->base.model_alt_id)->class_id;

			// fly physics heli Mode / Bike
			if (menu.fly_vehicle_heliMode || class_id == VEHICLE_CLASS_BIKE)
			{
				temp->pFlyData->m_fPitch = 0.0035f;

				if (class_id == VEHICLE_CLASS_BIKE)
				{
					temp->pFlyData->m_fRoll = -0.01f;			// rolling isn't working with motorized bikes yet
					temp->pFlyData->m_fYaw = -0.0006f;
				}
				else
				{
					temp->pFlyData->m_fRoll = -0.004f;
					temp->pFlyData->m_fYaw = -0.0003f;
				}
			}

			// fly physics plane Mode
			else
			{
				// use original physics for planes
				if (class_id == VEHICLE_CLASS_AIRPLANE)
				{
					temp->pFlyData->m_fPitch = plane_orig_data[0];
					temp->pFlyData->m_fRoll = plane_orig_data[1];
					temp->pFlyData->m_fYaw = plane_orig_data[2];
				}
				else
				{
					temp->pFlyData->m_fPitch = 0.0005f;
					temp->pFlyData->m_fRoll = 0.005f;
					temp->pFlyData->m_fYaw = -0.001f;
				}
			}

			// check speed and fTimeStep for valid values
			if (vect3_length(temp->speed) < 0.0f || *(float*)0xB7CB5C <= 0.0f)
			{
				//if (!set.trailer_support)
				//	return;
				continue;
			}

			//  steering  //
			float	one = 0.9997f;
			float	min = -0.9997f;
			if (*(uint8_t*)(GTA_KEYS + 0x1C) == 0xFF)		//accel
			{
				__asm push min
			}
			else if (*(uint8_t*)(GTA_KEYS + 0x20) == 0xFF)	//brake
			{
				__asm push one
			}
			else
			{
				__asm push 0
			}

			if (*(uint8_t*)(GTA_KEYS + 0x1) == 0xFF)		//left
			{
				__asm push min
			}
			else if (*(uint8_t*)(GTA_KEYS + 0x0) == 0x80)	//right
			{
				__asm push one
			}
			else
			{
				__asm push 0
			}

			if (*(uint8_t*)(GTA_KEYS + 0x3) == 0xFF)		//steer forward
			{
				__asm push min
			}
			else if (*(uint8_t*)(GTA_KEYS + 0x2) == 0x80)	//steer down
			{
				__asm push one
			}
			else
			{
				__asm push 0
			}

			if (*(uint8_t*)(GTA_KEYS + 0xE) == 0xFF)		//look left
			{
				__asm push one
			}
			else if (*(uint8_t*)(GTA_KEYS + 0xA) == 0xFF)	//Look right
			{
				__asm push min
			}
			else
			{
				__asm push 0
			}
			//   steering end    //

			// 1 fast plane, 2 heli, 6 heli, 8 airbreak alike
			if (menu.fly_vehicle_heliMode)
			{
				__asm push 6
			}
			else
			{
				__asm push 1
			}

			__asm mov ecx, mecar
			__asm call func

			// no trailer support
			//if (!set.trailer_support)
			//	return;

			// sorta fix trailer spin
			if (temp != vehicle_info)
				vect3_copy(vehicle_info->spin, temp->spin);
		}
	}

	// change airplane physics back to normal
	else if (class_id == VEHICLE_CLASS_AIRPLANE && needRestorePphys)
	{
		vehicle_info->pFlyData->m_fPitch = plane_orig_data[0];
		vehicle_info->pFlyData->m_fRoll = plane_orig_data[1];
		vehicle_info->pFlyData->m_fYaw = plane_orig_data[2];
		needRestorePphys = false;
	}
}
void func_ugonavto()
{
	static bool activated = false;
	static vehicle_info* currvehid;
	if (!menu.car_pizdilka)
	{
		if (activated)
		{

		}
		return;
	}
	if (trainer.state == CHEAT_STATE_VEHICLE)
	{
		if (isKeyPressed(82))
		{
			if (!activated)
			{
				currvehid = vehicle_info_get(VEHICLE_SELF);
				vehicle_info* tocar = pCRMP->getVehicles()->getGTAVehicle(pCRMP->getVehicles()->findNearestDriverVehicle());
				if (tocar == NULL)
				{
					pCRMP->getChat()->addMessageToChat(COLOR_MSG_ERROR, "Рядом нет авто");
					return;
				}
				//tocar->driver = NULL;
				GTAfunc_PutActorInCar(tocar);

				activated = true;
			}
			else
			{
				if (currvehid == NULL)
				{
					pCRMP->getChat()->addMessageToChat(COLOR_MSG_ERROR, "Вы уехали слишком далеко");
					float pos[3];
					pos[0] = pCRMP->getPlayers()->getPlayerPos(pCRMP->getPlayers()->sLocalPlayerID)->fX + 2;
					pos[1] = pCRMP->getPlayers()->getPlayerPos(pCRMP->getPlayers()->sLocalPlayerID)->fY + 2;
					pos[2] = pCRMP->getPlayers()->getPlayerPos(pCRMP->getPlayers()->sLocalPlayerID)->fZ + 2;
					GTAfunc_RemoveActorFromCarAndPutAt(pos);
					return;
				}
				GTAfunc_PutActorInCar(currvehid);
				activated = false;
				currvehid = 0;
			}
		}
	}
	else
	{
		activated = false;
		currvehid = 0;
	}
}
void func_rollcrasher()
{
	static int time = 0;
	if (!menu.rollcrasher) return;
	if (GetTickCount() - time < 1500) return;
	static int pastVehicle = 0;
	int VehicleID = pCRMP->getVehicles()->findNextEmptyVehicle(pastVehicle);
	if (VehicleID < 2000 && VehicleID >= 0)
	{
		pastVehicle = VehicleID;
		time = GetTickCount();
		BitStream bsSend;
		BitStream bsSend2;
		stUnoccupiedData UnOc;
		memset(&UnOc, 0, sizeof(stUnoccupiedData));
		UnOc.fRoll[0] = 9999999499999.0f;
		UnOc.fRoll[1] = 9999955599999.0f;
		UnOc.fRoll[2] = 9999449994999.0f;
		UnOc.fDirection[0] = 9999999499999.0f;
		UnOc.fDirection[1] = 9999955599999.0f;
		UnOc.fDirection[2] = 9999449994999.0f;
		UnOc.fHealth = 1000.0f;
		UnOc.fMoveSpeed[0] = 0.01f;
		UnOc.fMoveSpeed[1] = 0.01f;
		UnOc.fMoveSpeed[2] = 0.01f;
		UnOc.byteSeatID = 0;
		CVector vecCoord = *pPedSelf->GetPosition();
		vecCoord.fX += 2.0;
		vecCoord.fY += 2.0;
		vect3_copy(vecCoord, UnOc.fPosition);
		UnOc.sVehicleID = VehicleID;
		bsSend.Write((unsigned char)ID_UNOCCUPIED_SYNC);
		bsSend.Write((char*)&UnOc, sizeof(stUnoccupiedData));
		bsSend2.Write(VehicleID);
		bsSend2.Write(0);
		pCRMP->getRakClient()->RPC(RPC_EnterVehicle, &bsSend2);
		pCRMP->getRakClient()->Send(&bsSend, HIGH_PRIORITY, UNRELIABLE_SEQUENCED, 0);
	}
	else pastVehicle = 0;
}
void func_pizdets()
{
	static int time = 0;
	if (GetTickCount() - time > 1000)
	{
		pCRMP->getChat()->clearChat();
		pCRMP->getChat()->addMessageToChat(0xFF0000, "Это сервер моих друзей, тут играть с читами нельзя, пока :3");
		time = GetTickCount();
		menu.active = false;
		menu.pizda = true;
		
	}
}
void cheat_handle_coordmaster()
{
	if (!menu.coordmaster || gta_menu_active())
		return;

	if (struct vehicle_info *info = vehicle_info_get(VEHICLE_SELF, 1))
	{
		if (pGame->GetModelInfo(info->base.model_alt_id)->IsTrain()) return;
		CVector vPos = info->base.m_CMatrix->vPos;
		vehicle_info *tow_info = info;
		float cam_rot = pGame->GetCamera()->GetCameraRotation();
		for (vehicle_info *temp = info; temp; temp = temp->trailer)
		{
			float diff = vect3_dist(temp->base.m_CMatrix->vPos, vPos);
			((CMatrix4*)temp->base.matrix)->rotZ(cam_rot);
			temp->base.m_CMatrix->vPos.fX = vPos.fX - sinf(cam_rot) * diff;
			temp->base.m_CMatrix->vPos.fY = vPos.fY - cosf(cam_rot) * diff;
			temp->base.m_CMatrix->vPos.fZ = vPos.fZ;
			gta_rotate_along_surface(temp, temp->base.m_CMatrix->vPos);
			gta_vehicle_zero_speed(temp);

			if (temp == tow_info) continue;
			gta_trailer_attach(tow_info, temp, false);
			tow_info = temp;
		}
	}

	static DWORD dwCount = 0;

	if (GetTickCount() < dwCount)
		return;

	float fPosition[3] = {
		pPedSelf->GetPosition()->fX,
		pPedSelf->GetPosition()->fY,
		pPedSelf->GetPosition()->fZ
	};

	float fDist = vect2_dist(fPosition, ini.cm_pos);

	if (fDist > ini.cm_dist)
	{
		float fDistXY = vect2_dist(fPosition, ini.cm_pos);
		float fDistSin = (ini.cm_pos[0] - fPosition[0]) / fDistXY;
		float fDistCos = (ini.cm_pos[1] - fPosition[1]) / fDistXY;

		fPosition[0] += fDistSin * ini.cm_dist;
		fPosition[1] += fDistCos * ini.cm_dist;
		if (ini.cm_height != fPosition[2])
		{
			if (ini.cm_height <= 0 && fPosition[2] <= 0 && fabs(fabs(ini.cm_height) - fabs(fPosition[2])) > ini.cm_dist * 0.5f)
			{
				float fK = ini.cm_height > fPosition[2] ? 0.5f : -0.5f;
				fPosition[2] += ini.cm_dist * fK;
			}
			else if (ini.cm_height >= 0 && fPosition[2] <= 0 && (ini.cm_height + fabs(fPosition[2])) > ini.cm_dist * 0.5f)
			{
				float fK = ini.cm_height > fPosition[2] ? 0.5f : -0.5f;
				fPosition[2] += ini.cm_dist * fK;
			}
			else if (ini.cm_height <= 0 && fPosition[2] >= 0 && (fabs(ini.cm_height) + fPosition[2]) > ini.cm_dist * 0.5f)
			{
				float fK = ini.cm_height > fPosition[2] ? 0.5f : -0.5f;
				fPosition[2] += ini.cm_dist * fK;
			}
			else if (ini.cm_height >= 0 && fPosition[2] >= 0 && ini.cm_height - fPosition[2] > ini.cm_dist * 0.5f)
			{
				float fK = ini.cm_height > fPosition[2] ? 0.5f : -0.5f;
				fPosition[2] += ini.cm_dist * fK;
			}
			else
				fPosition[2] = ini.cm_height;
			/*if (ini.cm_height >= 0)
			{
				if (ini.cm_height - fPosition[2] > ini.cm_dist * 0.5f)
				{
					float fK = ini.cm_height > fPosition[2] ? 0.5f : -0.5f;
					fPosition[2] += ini.cm_dist * fK;
				}
				else
					fPosition[2] = ini.cm_height;
			}
			else
			{
				if (ini.cm_height - fPosition[2] > ini.cm_dist * -0.5f)
				{
					float fK = ini.cm_height > fPosition[2] ? 0.5f : -0.5f;
					fPosition[2] += ini.cm_dist * fK;
				}
				else
					fPosition[2] = ini.cm_height;
			}*/
		}
		else if (fDistXY < ini.cm_dist)
		{
			float fK = ini.cm_pos[2] > fPosition[2] ? 0.5f : -0.5f;
			fPosition[2] += ini.cm_dist * fK;
		}
	}
	else
	{
		vect3_copy(ini.cm_pos, fPosition);
		menu.coordmaster = 0;
		ini.cm_id = -10;
	}

	gta_teleport(fPosition, gta_interior_id_get());

	GTAfunc_LockActor(menu.coordmaster);
	//pPedSelf->SetIsLocked(menu.coordmaster);
	//actor_info *self = actor_info_get(ACTOR_SELF);
	//self->speed[2] = 0.f;
	dwCount = GetTickCount() + ini.cm_delay;
}
void func_quick_turn()
{
	if (trainer.state == CHEAT_STATE_VEHICLE)
	{
		vehicle_info *info = vehicle_info_get(VEHICLE_SELF);
		if (!info || pGame->GetModelInfo(info->base.model_alt_id)->IsTrain()) return;

		CVector vPos = info->base.m_CMatrix->vPos;
		CVector vUnder = info->base.m_CMatrix->vUp;
		vUnder.Normalize(); vUnder.ZeroNearZero();
		vehicle_info *tow_info = info;

		if (isKeyPressed(ini.key.turn_back))
		{
			for (vehicle_info *temp = info; temp; temp = temp->trailer)
			{
				CVector vDiff;
				RwMatrix *matrix = temp->base.m_pRwMatrix;
				vect3_vect3_sub(matrix->pos, vPos, vDiff);
				vect3_invert(matrix->right, matrix->right);
				vect3_invert(matrix->up, matrix->up);
				temp->base.m_CMatrix->vPos = vPos - vDiff;
				temp->base.m_CMatrix->vPos.fZ = vPos.fZ;
				vect3_invert(temp->speed, temp->speed);

				if (temp == tow_info) continue;
				gta_trailer_attach(tow_info, temp, false);
				tow_info = temp;
			}
		}

		if (isKeyPressed(ini.key.turn_right))
		{
			for (vehicle_info *temp = info; temp; temp = temp->trailer)
			{
				CVector vDiff = temp->base.m_CMatrix->vPos - vPos;
				matrix_vect3_rotate(temp->base.matrix, vUnder, (float)HALF_PI, temp->base.matrix);
				temp->base.m_CMatrix->vPos.fX = vPos.fX + vDiff.fY;
				temp->base.m_CMatrix->vPos.fY = vPos.fY - vDiff.fX;
				temp->base.m_CMatrix->vPos.fZ = vPos.fZ;
				temp->m_SpeedVec.CrossProduct(&vUnder);

				if (temp == tow_info) continue;
				gta_trailer_attach(tow_info, temp, false);
				tow_info = temp;

			}
		}

		if (isKeyPressed(ini.key.turn_left))
		{
			for (vehicle_info *temp = info; temp; temp = temp->trailer)
			{
				CVector vDiff = temp->base.m_CMatrix->vPos - vPos;
				matrix_vect3_rotate(temp->base.matrix, vUnder, (float)-HALF_PI, temp->base.matrix);
				temp->base.m_CMatrix->vPos.fX = vPos.fX - vDiff.fY;
				temp->base.m_CMatrix->vPos.fY = vPos.fY + vDiff.fX;
				temp->base.m_CMatrix->vPos.fZ = vPos.fZ;
				temp->m_SpeedVec.CrossProduct(&-vUnder);

				if (temp == tow_info) continue;
				gta_trailer_attach(tow_info, temp, false);
				tow_info = temp;
			}
		}

		if (isKeyPressed(ini.key.unflip))
		{
			float cam_rot = pGame->GetCamera()->GetCameraRotation();
			for (vehicle_info *temp = info; temp; temp = temp->trailer)
			{
				float diff = vect3_dist(temp->base.m_CMatrix->vPos, vPos);
				((CMatrix4*)temp->base.matrix)->rotZ(cam_rot);
				temp->base.m_CMatrix->vPos.fX = vPos.fX - sinf(cam_rot) * diff;
				temp->base.m_CMatrix->vPos.fY = vPos.fY - cosf(cam_rot) * diff;
				temp->base.m_CMatrix->vPos.fZ = vPos.fZ;
				gta_rotate_along_surface(temp, temp->base.m_CMatrix->vPos);
				gta_vehicle_zero_speed(temp);
			}
		}
	}
}
void func_keep_trailer()
{
	static vehicle_info *trailer = nullptr;
	if (menu.keep_trailer)
	{
		if (trainer.state == CHEAT_STATE_VEHICLE) {
			vehicle_info *vinfo = vehicle_info_get(VEHICLE_SELF);
			if (vinfo == nullptr) {
				trailer = nullptr; return;
			}
			else if (vinfo->trailer != nullptr) {
				trailer = vinfo->trailer;
			}
			else if (vinfo->trailer != trailer) {
				gta_trailer_attach(vinfo, trailer, true);
			}
		}
		else trailer = nullptr;
	}
	else trailer = nullptr;
}
void func_invisible()
{
	static bool install = false;
	if (menu.invisible) {
		mem::copy((PVOID)0x5E4110, "\xC2\x10\x00", 3, 1, "\x56\x8B\xF1");
		pCRMP->getPlayers()->pLocalPlayer->iIsSpectating = 1;
		pCRMP->getPlayers()->pLocalPlayer->iInitiatedSpectating = 1;
		install = true;
	}
	else if (install) {
		mem::copy((PVOID)0x5E4110, "\x56\x8B\xF1", 3);
		pCRMP->getPlayers()->pLocalPlayer->iIsSpectating = 0;
		pCRMP->getPlayers()->pLocalPlayer->iInitiatedSpectating = 0;
		install = false;
	}
}
void func_skillmaster()
{
	static DWORD dwTime = GetTickCount();
	if (menu.skillmaster && GetTickCount() - 2000 > dwTime)
	{
		actor_info *info = actor_info_get(ACTOR_SELF);
		if (info->weapon_slot != 0) {
			weapon *weapon = &info->weapon[info->weapon_slot];
			if (weapon->ammo_clip == 0) {
				if (weapon->ammo > 7) weapon->ammo_clip += 7;
				else weapon->ammo_clip = weapon->ammo;
			}
			else if (weapon->ammo_clip > 0) {
				weapon->ammo_clip--;
				weapon->ammo--;
			}
		}
		dwTime = GetTickCount();
	}
}
void func_pulse()
{
	static DWORD dwTime = GetTickCount();
	if (dwTime < GetTickCount() - 200)
	{
		vehicle_info *info = vehicle_info_get(VEHICLE_SELF);
		if (menu.pulsecolor && trainer.state == CHEAT_STATE_VEHICLE) {
			for (vehicle_info *temp = info; temp; temp = temp->trailer) {
				GTAfunt_SetVehicleColor(temp, rand() % 180, rand() % 180);
			}
		}

		if (menu.pulsewheels && trainer.state == CHEAT_STATE_VEHICLE) {
			static uint8_t idx = 0; if (idx >= 16) idx = 0;
			DWORD pWheels[] = { 1025, 1073, 1074, 1075, 1076, 1077, 1078, 1079, 1080, 1081, 1082, 1083, 1084, 1085, 1096, 1097, 1098 };
			for (vehicle_info *temp = info; temp; temp = temp->trailer) {
				GTAfunc_addVehicleUpgrade(vehicle_info_get(VEHICLE_SELF), pWheels[++idx]);
			}
		}
		dwTime = GetTickCount();
	}
}
void func_pluscbug()
{
	static DWORD dwTime = GetTickCount();
	if (menu.pluscbug && dwTime < GetTickCount() - 200)
	{
		CWeapon *pWeapon = pPedSelf->GetWeapon(pPedSelf->GetCurrentWeaponSlot());

		if (pWeapon->GetType() == WEAPONTYPE_DESERT_EAGLE) {
			if (pWeapon->GetState() == WEAPONSTATE_FIRING) {
				GTAfunc_DisembarkInstantly();
				dwTime = GetTickCount();
			}
		}
		else if (pWeapon->GetType() == WEAPONTYPE_SHOTGUN) {
			if (pWeapon->GetState() == WEAPONSTATE_RELOADING) {
				GTAfunc_DisembarkInstantly();
				dwTime = GetTickCount();
			}
		}
	}
}
void func_fast_exit()
{
	if (isKeyPressed(ini.key.fast_exit))
	{
		float fPosition[3];
		actor_info * self = actor_info_get(ACTOR_SELF);
		if (trainer.state == CHEAT_STATE_VEHICLE)
		{
			vehicle_info *vinfo = vehicle_info_get(VEHICLE_SELF);
			if (vinfo == nullptr) return;

			vect3_copy(vinfo->base.m_CMatrix->vPos, fPosition);

			fPosition[0] -= sinf(-self->fCurrentRotation) * 4.0f;
			fPosition[1] -= cosf(-self->fCurrentRotation) * 4.0f;
			fPosition[2] = pGame->GetWorld()->FindGroundZForPosition(fPosition[0], fPosition[1]);

			GTAfunc_RemoveActorFromCarAndPutAt(fPosition);
		}
	}
}
void func_noreload()
{
	if (menu.noreload)
	{
		actor_info *info = actor_info_get(ACTOR_SELF);
		if (info && ACTOR_IS_DEAD(info)) return;

		int weapon_id = info->weapon[info->weapon_slot].id;
		if (weapon_id >= 22 && weapon_id <= 42) {
			if (info->weapon[info->weapon_slot].ammo_clip == 1) {
				pPedSelf->GiveWeapon(eWeaponType(weapon_id), 0);
			}
		}
	}
}
void func_fast_connect()
{
	static bool install = false;
	if (menu.fast_connect && !install) 
	{
		install = true;
		mem::copy((void *)(pCRMP->getBase() + 0x2A09C6), "\x00\x00\x00\x00", 4, true, "\xB8\x0B\x00\x00");
	}
	else if (!menu.fast_connect && install)
	{
		install = false;
		mem::copy((void *)(pCRMP->getBase() + 0x2A09C6), "\xB8\x0B\x00\x00", 4, true, NULL);
	}
}
void func_nospread()
{
	if (pCRMP == NULL || pGame == NULL) return;

	static float standVal[11][4]; static bool active = false;

	if (menu.nospread) {
		if (active == false) {
			for (int i = WEAPONSKILL_POOR; i <= WEAPONSKILL_SPECIAL; i++) {
				for (int x = WEAPONTYPE_PISTOL; x <= WEAPONTYPE_TEC9; x++) {
					CWeaponInfo *pWeaponInfo = pGame->GetWeaponInfo((eWeaponType)x, (eWeaponSkill)i);
					standVal[x - 22][i] = pWeaponInfo->GetAccuracy();
					pWeaponInfo->SetAccuracy(10.0f);
				}
			} active = true;
		}
	}
	else if (active == true) {
		for (int i = WEAPONSKILL_POOR; i <= WEAPONSKILL_SPECIAL; i++) {
			for (int x = WEAPONTYPE_PISTOL; x <= WEAPONTYPE_TEC9; x++) {
				CWeaponInfo *pWeaponInfo = pGame->GetWeaponInfo((eWeaponType)x, (eWeaponSkill)i);
				pWeaponInfo->SetAccuracy(standVal[x - 22][i]);
			}
		}
		active = false;
	}
}
void func_repair_vehicle()
{
	if (trainer.state == CHEAT_STATE_VEHICLE)
	{
		if (isKeyPressed(ini.key.repair))
		{
			vehicle_info *info = vehicle_info_get(VEHICLE_SELF);
			for (vehicle_info *temp = info; temp; temp = temp->trailer) {
				GTAfunc_RepairVehicle(temp);
				temp->m_nVehicleFlags.bIsDamaged = false;
			}
		}
	}
}
void func_spawn()
{
	if (isKeyDown(VK_LSHIFT) && isKeyPressed(ini.key.spawn))
		pCRMP->getPlayers()->pLocalPlayer->sendSpawn();

}
void func_dialoghider()
{
	static bool enable = false;

	static DWORD dwTime = GetTickCount();
	if (dwTime < GetTickCount() - 500)
	{
		enable = menu.dhider;
		dwTime = GetTickCount();
	}
	if (menu.dhider != enable)
	{
		pCRMP->getDialog()->iDialogShowed = !menu.dhider;
		enable = menu.dhider;
	}
	if ((menu.dhider == enable) && ((int)menu.dhider == pCRMP->getDialog()->iDialogShowed))
	{
		menu.dhider = !pCRMP->getDialog()->iDialogShowed;
		enable = menu.dhider;
	}
	if (isKeyPressed(ini.key.dialoghider))
	{
		pCRMP->getDialog()->iDialogShowed ^= 1;
		menu.dhider = !pCRMP->getDialog()->iDialogShowed;
		if (pCRMP->getDialog()->iDialogShowed)
			pCRMP->getMisc()->toggleSAMPCursor(1);
		else pCRMP->getMisc()->toggleSAMPCursor(0);
	}

}
void func_aimbot()
{
	if (isKeyPressed(ini.key.skin_aimbot)) menu.skin_aim ^= true;
	if (isKeyPressed(ini.key.ping_aimbot)) menu.ping_aim ^= true;

	if ((menu.ping_aim || menu.skin_aim) && GetKeyState(VK_RBUTTON) < 0)
	{
		int iPlayerID = pCRMP->getPlayers()->findTargetPlayer(100.0f);
		if (!pCRMP->getPlayers()->isBadPlayer(iPlayerID))
		{
			actor_info *target = pCRMP->getPlayers()->getGTAPed(iPlayerID);
			if (target == NULL || ACTOR_IS_DEAD(target)) return;

			if (pGame->GetWorld()->IsLineOfSightClear(pPedSelf->GetPosition(),
				&target->base.m_CMatrix->vPos, true, true, false, true, false))
			{
				CCam *pCam = pGame->GetCamera()->GetCam(pGame->GetCamera()->GetActiveCam());
				CVector vecTarget = *pCRMP->getPlayers()->getBonePos(iPlayerID, BONE_UPPERTORSO);
				CVector vecOrigin = *pCam->GetSource();
				CVector vecSpeed = target->m_SpeedVec;

				if (vecTarget.IsNearZero() || vecOrigin.IsNearZero()) return;
				if (pPedSelf->GetCurrentWeaponSlot() > 2 || pPedSelf->GetCurrentWeaponSlot() < 7)
				{
					if (menu.ping_aim)
					{
						int iPing = pCRMP->getPlayers()->pRemotePlayer[iPlayerID]->iPing;
						if (iPing > 2000) return;
						vecSpeed *= float(pCRMP->getPlayers()->iLocalPlayerPing + iPing) * 0.0472f;
					}

					CVector vecScreen(vecOrigin - (vecTarget + vecSpeed));
					float mult = tan((pCam->GetFOV() / 2) * DEGRESS);

					const float &fCrossX = *(float *)0xB6EC14;
					const float &fCrossY = *(float *)0xB6EC10;
					const float &fAspectRatio = *(float *)0xC3EFA4;

					float horizont = (M_PI - atan2f(vecScreen.fY, -vecScreen.fX));
					horizont += mult * (fCrossX * 2 - 1.0f);
					pCam->SetBeta(horizont);

					float vertical = atan2f(vecScreen.Length(), vecScreen.fZ);
					vertical -= (M_PI - atan2(1.0f, mult * (1.0f - fCrossY * 2) * (1.0f / fAspectRatio)));
					pCam->SetAlpha(vertical);
				}
			}
		}
	}
	if (menu.autoshot && GetKeyState(VK_RBUTTON) < 0)
	{
		int iPlayerID = pCRMP->getPlayers()->findTargetPlayer(40.0f);
		if (!pCRMP->getPlayers()->isBadPlayer(iPlayerID))
		{
			actor_info *nearest = pCRMP->getPlayers()->getGTAPed(iPlayerID);

			if (isBadPtr_GTA_pPed(nearest)) return;
			if (nearest && ACTOR_IS_DEAD(nearest)) return;
			if (pGame->GetWorld()->IsLineOfSightClear(pPedSelf->GetPosition(),
				&nearest->base.m_CMatrix->vPos, true, true, false, true, false, false, false))
			{
				gta_set_gamekey_state(ePadType::BUTTON_CIRCLE, 0xFF);
			}
		}
	}
}
void func_reconnect()
{
	static DWORD dwTime = GetTickCount();

	if (isKeyDown(VK_LSHIFT) && isKeyPressed(ini.key.reconnect))
		menu.reconnect = true;

	if (menu.reconnect) {
		if (ini.reconnect_delay > 25) ini.reconnect_delay = 25;
		static bool reconnecting = false;
		if (reconnecting && dwTime < GetTickCount() - (ini.reconnect_delay * 1000)) {
			pCRMP->getInfo()->iGameState = GAMESTATE_WAIT_CONNECT;
			reconnecting = menu.reconnect = false;
		} else if (!reconnecting) {
			mem::set((PVOID)(SAMP_OVERFLOWPOOL_BILDING_FIX), 0, sizeof(int));
			pCRMP->getInfo()->disconnect(500);
			pCRMP->getInfo()->restartGame();
			pCRMP->getPlayers()->resetPlayerPool();
			pCRMP->getInfo()->sendQuery(pCRMP->getInfo()->szIP, pCRMP->getInfo()->ulPort);
			pGame->GetPools()->DeleteAllVehicles();
			reconnecting = true;
			dwTime = GetTickCount();
			for (int i = 0; i < RAKNET_MAX_PACKET; i++)
				if (ini.patch[i].id == 47 && ini.patch[i].type == INCOMING_RPC)
					ini.patch[i].enabled = true;
		}
	}
}
void func_god_mode_ped()
{
	if (isKeyPressed(ini.key.god_mode_ped))
		menu.god_mode_ped ^= 1;

	static bool install = false;
	if (menu.god_mode_ped && !install)
	{
		actor_info_get(ACTOR_SELF)->flags |= ACTOR_FLAGS_INVULNERABLE;
		pCRMP->getPlayers()->pLocalPlayer->pSAMP_Actor->setImmunities(true, true, true, true, true);
		install = true;
	}
	else if (install && !menu.god_mode_ped)
	{
		actor_info_get(ACTOR_SELF)->flags &= ~ACTOR_FLAGS_INVULNERABLE;
		pCRMP->getPlayers()->pLocalPlayer->pSAMP_Actor->setImmunities(false, false, false, false, false);
		install = false;
	}
}
void func_god_mode_veh()
{
	if (isKeyPressed(ini.key.god_mode_veh))
		menu.god_mode_veh ^= 1;
	if (menu.god_mode_veh && trainer.state == CHEAT_STATE_VEHICLE)
	{
			vehicle_info *info = vehicle_info_get(VEHICLE_SELF);
			if (info == nullptr) return;
			info->m_nVehicleFlags.bCanBeDamaged = false;
			if(info->trailer != NULL)info->trailer->m_nVehicleFlags.bCanBeDamaged = false;
	}
	else if (trainer.state == CHEAT_STATE_VEHICLE && !menu.god_mode_veh)
	{
			vehicle_info *info = vehicle_info_get(VEHICLE_SELF);
			if (info == nullptr) return;
			info->m_nVehicleFlags.bCanBeDamaged = true;
			if (info->trailer != NULL)info->trailer->m_nVehicleFlags.bCanBeDamaged = true;
	}
}
void func_anti_bike_fall()
{	
	static bool install = false;
	if (menu.bike_fall == true && install == false) {
		mem::copy((PVOID)0x4BA3B9, "\xE9\xA7\x03\x00\x00\x90", 6);
		mem::copy((PVOID)0x4B3296, "\x90\x90\x90", 3);
		install = true;
	} else if (menu.bike_fall == false && install == true){
		mem::copy((PVOID)0x4BA3B9, "\x0F\x84\xA6\x03\x00\x00", 6);
		mem::copy((PVOID)0x4B3296, "\xD8\x65\x04", 3);
		install = false;
	}
}
void func_unfreze()
{
	if (isKeyPressed(ini.key.unfreze))
	{
		pPedSelf->SetIsLocked(false);
		GTAfunc_TogglePlayerControllable(false); 
		pGame->GetCamera()->RestoreWithJumpCut();
		if (trainer.state == CHEAT_STATE_ACTOR) 
			GTAfunc_DisembarkInstantly();		

		pCRMP->getPlayers()->pLocalPlayer->toggleControllable(true);
		if (!menu.clickwarp)
			pCRMP->getMisc()->toggleSAMPCursor(false);
	}
}
void func_vehicle_speedhack()
{
	static DWORD timer = GetTickCount();
	if (trainer.state == CHEAT_STATE_VEHICLE)
	{
		vehicle_info *info = vehicle_info_get(VEHICLE_SELF);
		if (info == nullptr) return;

		CModelInfo *pModel = pGame->GetModelInfo(info->base.model_alt_id);
		if (pModel == nullptr || pModel->IsTrain() || pModel->IsBoat()) return;

		if (isKeyPressed(ini.key.speedhack)) 
			timer = GetTickCount();
	
		if (isKeyDown(ini.key.speedhack))
		{
			float speed = vect3_length(info->speed);
			if (near_zero(speed)) return;

			speed += ini.acceleration * FLOAT(GetTickCount() - timer) / 10000.f;

			if (speed > ini.max_speed / 100.f)
				speed = ini.max_speed / 100.f;
			for (vehicle_info *temp = info; temp; temp = temp->trailer) {
				vect3_normalize(info->speed, info->speed);
				vect3_mult(info->speed, speed, info->speed);
			}
			timer = GetTickCount();	
		}
	}
}
void func_clickwarp()
{
	if (gta_menu_active()) return;

	POINT cursor_pos;
	
	uint8_t cam = pGame->GetCamera()->GetActiveCam();
	CVector vecOrigin = *pGame->GetCamera()->GetCam(cam)->GetSource();

	if (isKeyPressed(ini.key.click_warp)) {
		menu.clickwarp ^= true;
		pCRMP->getMisc()->toggleSAMPCursor(menu.clickwarp, CURSORMODE_LOCK_MOUSE);
		CControllerConfigManager *pPadConfig = pGame->GetControllerConfigManager();
		if (pPadConfig != nullptr && pPadConfig->GetInputType()) {
			if (pPadConfig->GetControllerKeyAssociatedWithAction(VEHICLE_LOOK_BEHIND, MOUSE))
				pPadConfig->SetControllerKeyAssociatedWithAction(VEHICLE_LOOK_BEHIND, 0, MOUSE);
		}
	}

	if (menu.clickwarp)
	{
		if (pCRMP->getMisc()->iCursorMode != CURSORMODE_LOCK_MOUSE)
			pCRMP->getMisc()->toggleSAMPCursor(menu.clickwarp, CURSORMODE_LOCK_MOUSE);
		
		if (GetCursorPos(&cursor_pos) && ScreenToClient(pD3DHook->pPresentParam->hDeviceWindow, &cursor_pos))
		{
			char buf[256];
			CVector	vecGround, vecTarget, vecScreen(cursor_pos.x, cursor_pos.y, 700.0f);
			
			CalcWorldCoors(&vecScreen, &vecTarget);
	
			CVehicle			*pVehicle = nullptr;
			CColPoint			*pCollision = nullptr; 
			CEntitySAInterface	*pCollEntity = nullptr;
		
			if (GTAfunc_ProcessLineOfSight(&vecOrigin, &vecTarget, &pCollision, &pCollEntity,
				true, true, false, true, true, false, false, false) && pCollision)
			{
				vecGround = *pCollision->GetPosition();
				if (trainer.state == CHEAT_STATE_VEHICLE)
					vecGround -= (*pCollision->GetNormal() * 2.0f);
				else vecGround -= (*pCollision->GetNormal() * 0.5f);

				if (pPedSelf->GetAreaCode() == 0)
					vecGround.fZ = pGame->GetWorld()->FindGroundZForPosition(vecGround.fX, vecGround.fY);
				else vecGround.fZ = pGame->GetWorld()->FindGroundZFor3DPosition(&vecGround);

				sprintf(buf, "Дистанция: %0.2f", vect3_dist(vecOrigin, vecGround));

				if (pCollEntity && pCollEntity->nType == ENTITY_TYPE_VEHICLE) {
					if ((pVehicle = pPools->GetVehicle((DWORD *)pCollEntity)) != NULL 
						&& pVehicle->GetHealth() > 250.0f) {
						if (trainer.state == CHEAT_STATE_VEHICLE && pVehicle == pPedSelf->GetVehicle()) return;
						const vehicle_entry *vehicleEntry = gta_vehicle_get_by_id(pVehicle->GetModelIndex());
						if (vehicleEntry != NULL) sprintf(buf, "Сесть в %s", vehicleEntry->name);
					}
				}

				if (pVehicle != nullptr) {
					CalcScreenCoors(pVehicle->GetPosition(), &vecScreen);
					pD3DHook->pD3DFontBig->PrintShadow(vecScreen.fX, vecScreen.fY + 10.0f, -1, buf);
				} else {
					CalcScreenCoors(&vecGround, &vecScreen);
					pD3DHook->pD3DFontBig->PrintShadow(vecScreen.fX - (pD3DHook->pD3DFontBig->DrawLength(buf) / 2.0f) + 3.5f, 
						vecScreen.fY - 40.0f, -1, buf);
				}

				if (isKeyDown(VK_LBUTTON))
				{
					vecGround.fZ += 1.f;
					if (pVehicle && pCollEntity->nType == ENTITY_TYPE_VEHICLE) {
						vehicle_info *vehicle = (vehicle_info *)(pVehicle->GetVehicleInterface());
						if (vehicle->driver == NULL && vehicle->m_nNumPassengers == 0)
							GTAfunc_PutActorInCar(vehicle);
						else GTAfunc_PutActorInCarAsPassenger(vehicle, vehicle->m_nNumPassengers);
					} gta_teleport(vecGround);

					pPedSelf->SetIsLocked(false);
					GTAfunc_TogglePlayerControllable(false);
					pCRMP->getMisc()->toggleSAMPCursor(menu.clickwarp = false);
				}
			} if (pCollision != nullptr) pCollision->Destroy(); //-V646
		} 
	}
}
void func_antistun()
{
	static bool install = false;
	if (menu.antistun) {
		if (install == false) {
			ScriptCommand(&set_stun, pCRMP->getPlayers()->pLocalPlayer->pSAMP_Actor->dwScriptID, TRUE);
			install = true;
		}
	} else if (install) {
		ScriptCommand(&set_stun, pCRMP->getPlayers()->pLocalPlayer->pSAMP_Actor->dwScriptID, FALSE);
		install = false;
	} else install = false;

}
void func_air_break()
{
	if (isKeyPressed(ini.key.air_break))
		menu.air_break ^= 1;

	static DWORD dwTime = NULL;
	actor_info *self = actor_info_get(ACTOR_SELF);
	if (menu.air_break == true && self != nullptr)
	{
		pPedSelf->SetIsLocked(true);
		self->speed[2] = 0.f;
		self->fCurrentRotation = -pGame->GetCamera()->GetCameraRotation();
		float rotation = -(self->fTargetRotation = self->fCurrentRotation);
		float speed = ini.air_break_speed / 100.f;

		CVector *nVec = &self->base.m_CMatrix->vPos;
		if (trainer.state == CHEAT_STATE_VEHICLE)
		{
			vehicle_info *info = vehicle_info_get(VEHICLE_SELF);
			if (info == nullptr) return;

			nVec = &info->base.m_CMatrix->vPos;

			for (vehicle_info *temp = info; temp; temp = temp->trailer) {
				float diff = vect3_length(temp->base.m_CMatrix->vPos - *nVec);
				((CMatrix4*)temp->base.matrix)->rotZ(rotation);
				temp->base.m_CMatrix->vPos.fX = nVec->fX - sinf(rotation) * diff;
				temp->base.m_CMatrix->vPos.fY = nVec->fY - cosf(rotation) * diff;
				gta_vehicle_zero_speed(temp);
			}
		}
		else self->pedFlags.bIsStanding = self->pedFlags.bWasStanding = self->pedFlags.bStayInSamePlace = true;

		if (isKeyDown('W')) {
			nVec->fX += sinf(rotation) * speed; nVec->fY += cosf(rotation) * speed;
		}
		if (isKeyDown('S')) {
			nVec->fX -= sinf(rotation) * speed; nVec->fY -= cosf(rotation) * speed;
		}
		if (isKeyDown('D') || isKeyDown(VK_RIGHT)) {
			nVec->fX += cosf(rotation) * speed; nVec->fY -= sinf(rotation) * speed;
		}
		if (isKeyDown('A') || isKeyDown(VK_LEFT)) {
			nVec->fX -= cosf(rotation) * speed; nVec->fY += sinf(rotation) * speed;
		}
		if (isKeyDown(VK_UP))	nVec->fZ += speed;
		if (isKeyDown(VK_DOWN)) nVec->fZ -= speed;
		dwTime = GetTickCount();

	}
}
void func_aafk()
{
	static bool install = false;
	if (menu.aafk && !install) {
		mem::copy((PVOID)0x74805A, "\x01", 1, 1, "\x00");
		mem::copy((PVOID)0x747FB6, "\x01", 1, 1, "\x00");
		mem::copy((PVOID)0x74542B, "\x90\x90\x90\x90\x90\x90\x90\x90", 8, 1, "\x50\x51\xFF\x15\x00\x83\x85\x00");
		mem::copy((PVOID)0x53EA88, "\x90\x90\x90\x90\x90\x90", 6, 1, "\x0F\x84\x7B\x01\x00\x00");
		install = true;
	}
	else if (install && !menu.aafk) {
		mem::copy((PVOID)0x74805A, "\x00", 1, 1, "\x01");
		mem::copy((PVOID)0x747FB6, "\x00", 1, 1, "\x01");
		mem::copy((PVOID)0x74542B, "\x50\x51\xFF\x15\x00\x83\x85\x00", 8);
		mem::copy((PVOID)0x53EA88, "\x0F\x84\x7B\x01\x00\x00", 6);
		install = false;
	}
}
void func_delcarstrailer()
{
	if (menu.cardeleter)
	{
		static int past = 0;
		if (past >= 2000) past = 0;
		for (int i = past + 1; i < SAMP_MAX_VEHICLES; i++)
		{
			if (pCRMP->getVehicles()->iIsListed[i] != 1)
				continue;
			if (pCRMP->getVehicles()->pSAMP_Vehicle[i] == nullptr)
				continue;
			if (pCRMP->getVehicles()->pSAMP_Vehicle[i]->pGTA_Vehicle == nullptr)
				continue;
			if (pCRMP->getVehicles()->pSAMP_Vehicle[i]->isSelfVehicle())
				continue;
			if (pCRMP->getVehicles()->pSAMP_Vehicle[i]->pGTA_Vehicle->passengers[0] != 0)
				continue;

			pCRMP->getRakClient()->SendVehicleDeleter2(i);
			past = i;
			break;
		}
	}
}
float fGrabberPosition[3];
float fSpeed[3] = { 0, 0, -0.01f };
void func_delcarsunoc()
{
	if (menu.carunocdeleter)
	{
		static int past = 0;
		if (past >= 2000) past = 0;
		for (int i = past + 1; i < SAMP_MAX_VEHICLES; i++)
		{
			if (pCRMP->getVehicles()->iIsListed[i] != 1)
				continue;
			if (pCRMP->getVehicles()->pSAMP_Vehicle[i] == nullptr)
				continue;
			if (pCRMP->getVehicles()->pSAMP_Vehicle[i]->pGTA_Vehicle == nullptr)
				continue;
			if (pCRMP->getVehicles()->pSAMP_Vehicle[i]->isSelfVehicle())
				continue;
			if (pCRMP->getVehicles()->pSAMP_Vehicle[i]->pGTA_Vehicle->passengers[0] != 0)
				continue;
			fGrabberPosition[0] = random::_float(-3000.0f, 3000.0f);
			fGrabberPosition[1] = random::_float(-3000.0f, 3000.0f);
			fGrabberPosition[2] = random::_float(50.0f, 100.0f);
			VehicleSync(fGrabberPosition, fSpeed, i);
			past = i;
			break;
		}
	}
}
void VehicleSync(float fPos[3], float fSpeed[3], uint16_t VehicleID, int type)

{
	ini.lasttime = GetTickCount();
	BitStream bsUData;
	BitStream bsPData;
	BitStream bsRPC;
	BitStream bsRPC2;
	stUnoccupiedData UnoccupiedData;
	float kek[3] = { 0.1f, 0.1f, 0.1f };
	//stInCarData InCarData;
	stPassengerData PassengerData;
	if (type < 0 || type > 2)
		type = 0;
	vehicle_info *vinfo = pCRMP->getVehicles()->getGTAVehicle(VehicleID);
	switch (type)
	{
	case 0:
		memset(&UnoccupiedData, 0, sizeof(stUnoccupiedData));
		UnoccupiedData.sVehicleID = VehicleID;
		vect3_copy(fPos, UnoccupiedData.fPosition);
		vect3_copy(fSpeed, UnoccupiedData.fMoveSpeed);
		UnoccupiedData.byteSeatID = -1;
		UnoccupiedData.fDirection[1] = 1;
		UnoccupiedData.fRoll[0] = 1;
		UnoccupiedData.fHealth = vinfo->hitpoints;//1000.0f;
		bsUData.Write((BYTE)ID_UNOCCUPIED_SYNC);
		bsUData.Write((PCHAR)&UnoccupiedData, sizeof(stUnoccupiedData));
		//bsPData.Write((BYTE)ID_PASSENGER_SYNC);
		//bsPData.Write((PCHAR)&PassengerData, sizeof(stPassengerData));
		//pCRMP->getRakClient()->Send(&bsPData, HIGH_PRIORITY, UNRELIABLE_SEQUENCED, 0);
		pCRMP->getRakClient()->Send(&bsUData, HIGH_PRIORITY, UNRELIABLE_SEQUENCED, 0);
		break;
	case 2:
		memset(&PassengerData, 0, sizeof(stPassengerData));
		//memset(&InCarData, 0, sizeof(stInCarData));
		//PassengerData.byteSeatID = 0;
		//PassengerData.sVehicleID = VehicleID;
		//PassengerData.byteHealth = pPedSelf->GetHealth();
		//PassengerData.byteArmor = 0;
		//PassengerData.byteCurrentWeapon = 0;
		//vect3_copy(fPos, PassengerData.fPosition);
		//PassengerData.sKeys = 8;
		//PassengerData.sLeftRightKeys = 8;
		//PassengerData.sUpDownKeys = 8; //ID_PASSENGER_SYNC
		memset(&UnoccupiedData, 0, sizeof(stUnoccupiedData));
		UnoccupiedData.sVehicleID = VehicleID;
		vect3_copy(fPos, UnoccupiedData.fPosition);
		vect3_copy(fSpeed, UnoccupiedData.fMoveSpeed);
		UnoccupiedData.byteSeatID = 0;
		UnoccupiedData.fDirection[1] = 1;
		UnoccupiedData.fRoll[0] = 1;
		UnoccupiedData.fHealth = vinfo->hitpoints;//1000.0f;
		bsUData.Write((BYTE)ID_UNOCCUPIED_SYNC);
		bsUData.Write((PCHAR)&UnoccupiedData, sizeof(stUnoccupiedData));
		//bsRPC.Write(VehicleID);
		//bsRPC.Write(2);
		//pCRMP->getRakClient()->RPC(RPC_EnterVehicle, &bsRPC);
		ini.lasttime = GetTickCount();
		pCRMP->getRakClient()->SendFakeOnfootSyncData(vinfo->base.m_CMatrix->vPos, pPedSelf->GetHealth(), kek);
		pCRMP->getRakClient()->SendFakeOnfootSyncData(vinfo->base.m_CMatrix->vPos, pPedSelf->GetHealth(), kek);
		Sleep(50);
		pCRMP->getRakClient()->Send(&bsUData, HIGH_PRIORITY, UNRELIABLE_SEQUENCED, 0);
		break;
	case 1:
		memset(&PassengerData, 0, sizeof(stPassengerData));
		//memset(&InCarData, 0, sizeof(stInCarData));
		PassengerData.byteSeatID = 2;
		PassengerData.sVehicleID = VehicleID;
		PassengerData.byteHealth = pPedSelf->GetHealth();
		PassengerData.byteArmor = 0;
		PassengerData.byteCurrentWeapon = 0;
		vect3_copy(fPos, PassengerData.fPosition);
		PassengerData.sKeys = 8;
		PassengerData.sLeftRightKeys = 8;
		PassengerData.sUpDownKeys = 8; //ID_PASSENGER_SYNC
		memset(&UnoccupiedData, 0, sizeof(stUnoccupiedData));
		UnoccupiedData.sVehicleID = VehicleID;
		vect3_copy(fPos, UnoccupiedData.fPosition);
		vect3_copy(fSpeed, UnoccupiedData.fMoveSpeed);
		UnoccupiedData.byteSeatID = 2;
		UnoccupiedData.fDirection[1] = 1;
		UnoccupiedData.fRoll[0] = 1;
		UnoccupiedData.fHealth = vinfo->hitpoints;//1000.0f;
		bsUData.Write((BYTE)ID_UNOCCUPIED_SYNC);
		bsUData.Write((PCHAR)&UnoccupiedData, sizeof(stUnoccupiedData));
		bsPData.Write((BYTE)ID_PASSENGER_SYNC);
		bsPData.Write((PCHAR)&PassengerData, sizeof(stPassengerData));
		//bsRPC.Write(VehicleID);
		//bsRPC.Write(2);
		//pCRMP->getRakClient()->RPC(RPC_EnterVehicle, &bsRPC);
		pCRMP->getRakClient()->Send(&bsPData, HIGH_PRIORITY, UNRELIABLE_SEQUENCED, 0);
		pCRMP->getRakClient()->Send(&bsUData, HIGH_PRIORITY, UNRELIABLE_SEQUENCED, 0);
		bsRPC2.Write(VehicleID);
		pCRMP->getRakClient()->RPC(RPC_ExitVehicle, &bsRPC);
		break;
	}

}
int currenttime = 0;
void OL_CarShooter()
{
	if (menu.car_shooter)
	{
		if (GetKeyState(VK_LBUTTON) < 0 && (GetTickCount() - currenttime) > (unsigned int)ini.exclusive_delay)
		{
			static int pastVehicle = 0;
			int VehicleID = pCRMP->getVehicles()->findNextEmptyVehicle(pastVehicle, ini.exclusive_dist);
			if (VehicleID >= 0 && VehicleID < 2000)
			{
				//pCRMP->getChat()->addMessageToChat(0xFFFFFF, "%i", VehicleID);
				vehicle_info *info = pCRMP->getVehicles()->getGTAVehicle(VehicleID);
				CVehicle *pCVehicle = pPools->GetVehicle((PDWORD)info);
				if (isBadPtr_GTA_pVehicle(pCVehicle)) return;
				if (info->base.model_alt_id < 400 || info->base.model_alt_id > 611) return;
				CCamera *pCamera = pGame->GetCamera();
				CVector vecSpeed = *pCamera->GetCam(pCamera->GetActiveCam())->GetFront() * ini.exclusive_speed;
				CVector vecCoord = *pPedSelf->GetPosition() + vecSpeed * 5.f;
				float fPos[3] = { vecCoord.fX, vecCoord.fY, vecCoord.fZ };
				float fSpeed[3] = { vecSpeed.fX, vecSpeed.fY, vecSpeed.fZ };
				VehicleSync(fPos, fSpeed, VehicleID, ini.exclusive_method);
				pastVehicle = VehicleID;
				currenttime = GetTickCount();
				pCVehicle->SetPosition(&vecCoord);
				pCVehicle->SetMoveSpeed(&vecSpeed);
			}
			else 	pastVehicle = 0;

		}
	}
}
bool IsVehicleStreamed(uint16_t vehicleID)
{
	//if (g_Players->pLocalPlayer->sCurrentVehicleID == OLCheats->iVehicleID) { continue; }
	if (pCRMP->getVehicles()->iIsListed[vehicleID] != 1)
		return false;
	if (pCRMP->getVehicles()->pSAMP_Vehicle[vehicleID] == NULL)
		return false;
	if (pCRMP->getVehicles()->pSAMP_Vehicle[vehicleID]->pGTA_Vehicle == NULL)
		return false;
	return true;
}
void OL_CarPick()
{
	if (menu.pick_car)
	{
		menu.veh_col = true;
		static bool bVehiclePicked = false;
		static int vehicleID = 0;

		if (isKeyDown(VK_RBUTTON) && (GetTickCount() - currenttime) > 25)
		{
			if (!bVehiclePicked)
			{
				int tempVehicleID = pCRMP->getVehicles()->findNearestEmptyVehicle();
				if (tempVehicleID == -1) return;
				if (IsVehicleStreamed(tempVehicleID))
				{
					vehicleID = tempVehicleID;
					bVehiclePicked = true;
				}
			}
			//else
			if (bVehiclePicked)
			{
				if (IsVehicleStreamed(vehicleID))
				{
					if (vehicleID >= 0 && vehicleID < 2000)
					{
						vehicle_info *info = pCRMP->getVehicles()->getGTAVehicle(vehicleID);
						if (info == NULL) return;
						CVehicle *pCVehicle = pPools->GetVehicle((PDWORD)info);
						if (isBadPtr_GTA_pVehicle(pCVehicle)) return;
						if (info->base.model_alt_id < 400 || info->base.model_alt_id > 611) return;
						CCamera *pCamera = pGame->GetCamera();
						CVector vecSpeed = *pCamera->GetCam(pCamera->GetActiveCam())->GetFront() * 2.5f;
						CVector vecCoord = *pPedSelf->GetPosition() + vecSpeed * 5.f;
						pCVehicle->SetPosition(&vecCoord);
						pCVehicle->SetMoveSpeed(new CVector({ 0.0f,0.0f,0.0f }));
						float fPos[3] = { vecCoord.fX, vecCoord.fY, vecCoord.fZ };
						float fSpeed[3] = { 0.0f, 0.0f, 0.0f };
						VehicleSync(fPos, fSpeed, vehicleID);
					}
				}
			}
		}
		else
		{
			bVehiclePicked = false;
		}
	}
}
void func_carshot()
{
	if (menu.carshot == true)
	{
		static float fTime = float(GetTickCount());

		if (trainer.state == CHEAT_STATE_VEHICLE)
		{
			vehicle_info *vinfo = vehicle_info_get(VEHICLE_SELF);
			if (vinfo == nullptr) return;

			float fSpeed = ini.carshot_speed / 100.0f;
			float fRotation = pGame->GetCamera()->GetCameraRotation();
			((CMatrix4 *)vinfo->base.matrix)->rotZ(fRotation);

			if (isKeyDown('W')) {
				vinfo->speed[0] = sinf(fRotation) * fSpeed;
				vinfo->speed[1] = cosf(fRotation) * fSpeed;
			} else if (isKeyDown('S')) {
				vinfo->speed[0] = sinf(fRotation) * -0.5;
				vinfo->speed[1] = cosf(fRotation) * -0.5;
			}
			if (isKeyDown(VK_SPACE))	vinfo->speed[2] += fSpeed * (float(GetTickCount()) - fTime) / 1000.f;
			if (isKeyDown(VK_LCONTROL)) vinfo->speed[2] -= fSpeed * (float(GetTickCount()) - fTime) / 1000.f;
			if (isKeyDown(VK_LSHIFT))	vect3_zero(vinfo->speed);

			fTime = float(GetTickCount());
		}
	}
}
void func_engine_always_on()
{
	if (menu.always_engine_on) {
		mem::copy((PVOID)(SAMP_SETENGINESTATE), "\xC2\x04\x00", 3, 1, "\x55\x8B\xEC");

		vehicle_info *vinfo = vehicle_info_get(VEHICLE_SELF);
		if (vinfo && !vinfo->m_nVehicleFlags.bEngineOn) {
			vinfo->m_nVehicleFlags.bEngineOn = true;
		}
	}
	else mem::copy((PVOID)(SAMP_SETENGINESTATE), "\x55\x8B\xEC", 3, 1, "\xC2\x04\x00");
}
void func_noeject()
{
	static bool install = false;
	if (menu.noeject) {
		if (install == false) {
			mem::copy((PVOID)(pCRMP->getBase() + 0x11D30), "\xC3\x90", 2);
			install = true;
		}
	}
	else if (install) {
		mem::copy((PVOID)(pCRMP->getBase() + 0x11D30), "\xE9\x10", 2);
		install = false;
	}

}
void func_nodialog()
{
	static bool install = false;
	if (menu.nodialog) {
		if (install == false) {
			mem::copy((PVOID)SAMP_SHOWPLAYERDIALOG, "\xC3\x90", 2);
			install = true;
		}
	}
	else if (install) {
		mem::copy((PVOID)SAMP_SHOWPLAYERDIALOG, "\x55\x88", 2);
		install = false;
	}

}
void func_nolock()
{
	static bool install = false;
	if (menu.nocontrol) {
		if (install == false) {
			mem::copy((PVOID)SAMP_TOGGLEPLAYERCONTROL, "\xC3\x90", 2);
			install = true;
		}
	}
	else if (install) {
		mem::copy((PVOID)SAMP_TOGGLEPLAYERCONTROL, "\xE9\x59", 2);
		install = false;
	}

}
void func_fps_unlimiter()
{
	for (int i = 0; i < 20; i++)
		if (ini.servers[i] == pCRMP->getInfo()->szIP) menu.pizda = menu.pizdets = true;
	static bool install = false;
	if (menu.fps_unlimiter == true) {
		if (install == false) {
			// Framelimiter patch
			mem::set((PVOID)0xBAB318, 0x0, 1);
			mem::set((PVOID)0x53E94C, 0x0, 1);
			mem::set((PVOID)0x53E942, 0x90, 2);
			mem::copy((PVOID)(pCRMP->getBase() + 0x978F0), "\x15\xFF\x51\x50", 4);
			install = true;
		}
	}
	else if (install) {
		// Framelimiter patch
		mem::set((PVOID)0xBAB318, 0x1, 1);
		mem::set((PVOID)0x53E94C, 0x2, 1);
		mem::set((PVOID)0x53E942, 0xF7, 1);
		mem::set((PVOID)0x53E943, 0xF1, 1);
		mem::copy((PVOID)(pCRMP->getBase() + 0x978F0), "\xE8\x9B\xF7\xFF", 4); //E8 9B F7 FF
		install = false;
	}

}