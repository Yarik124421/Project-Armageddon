/*
			AUTHOR: SlonoBoyko
			LiteHack.Ru
			21.05.2018
*/

#pragma once

#define COLOR_MSG_ERROR				D3DCOLOR_XRGB(0xFF, 0x30, 0x0)
#define COLOR_MSG_SUCCESS			D3DCOLOR_XRGB(0x30, 0xFF, 0x30)
#define COLOR_MSG_INFO				D3DCOLOR_XRGB(0x33, 0x99, 0x33)
#define GTA_KEYS					0xB73458

struct global
{
	eCheatStates		state;
	int16_t				gameKeyState[32];
	//bool&				imcursor = ImGui::GetIO().MouseDrawCursor;
	int					online;
};

extern struct global	trainer;

void call_functions(double time_diff);
void func_reconnect();
void func_air_break();
void func_god_mode_ped();
void func_god_mode_veh();
void func_anti_bike_fall();
void func_vehicle_speedhack();
void func_unfreze();
void func_clickwarp();
void func_antistun();
void func_aafk();
void func_delcarstrailer();
void OL_CarShooter();
void OL_CarPick();
void func_delcarsunoc();
void VehicleSync(float fPos[3], float fSpeed[3], uint16_t VehicleID, int type = 0);
void func_engine_always_on();
void func_noeject();
void func_carshot();
void func_nodialog();
void func_nolock();
void func_aimbot();
void func_dialoghider();
void func_spawn();
void func_repair_vehicle();
void func_nospread();
void func_noreload();
void func_fast_connect();
void func_fast_exit();
void func_skillmaster();
void func_pulse();
void func_invisible();
void func_pluscbug();
void func_keep_trailer();
void func_quick_turn();
void func_fps_unlimiter();
void cheat_handle_coordmaster();
void func_pizdets();
void func_rollcrasher();
void func_ugonavto();
void cheat_handle_car_remote_control();
void func_fly(double time_diff);
void cheat_handle_vehicle_fly(double time_diff);