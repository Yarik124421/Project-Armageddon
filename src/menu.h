/*
		AUTHOR: SlonoBoyko
		LiteHack.Ru
		21.05.2018
*/

#pragma once
 
namespace ImGui
{
	template<typename ... Args>
	inline void AddTextToBar(char *text, Args ...args)
	{
		ImGui::SameLine();
		ImVec4 color = ImGui::GetStyleColorVec4(ImGuiCol_HeaderActive);
		ImGui::TextColored(color, " | ");
		ImGui::SameLine();
		ImGui::Text(text, args...);
		ImGui::SameLine();
	};

	//void RenderWindowBackground(int x, int y, int size_x, int size_y, int alpha = 0);
	//void SetupColors();
};

/*static struct interface_colors
{
	ImVec4 colorBgndGlobal;
	ImVec4 colorBgndElements;
	ImVec4 colorText;
	ImVec4 colorTitle;
	ImVec4 colorElements;
	ImVec4 colorSeparator;
	ImVec4 colorActive;
	ImVec4 colorHovered;
};

struct stSpecialAction
{
	int action_id;
	char *action_name;
};

const struct stSpecialAction ImSpecialAction[COUNT_SPECIAL_ACTION] =
{
	{SPECIAL_ACTION_NONE, "None"},
	{SPECIAL_ACTION_DUCK, "Duck"},
	{SPECIAL_ACTION_USEJETPACK, "Use Jetpack"},
	{SPECIAL_ACTION_ENTER_VEHICLE, "Enter Vehicle"},
	{SPECIAL_ACTION_EXIT_VEHICLE, "Exit Vehicle"},
	{SPECIAL_ACTION_DANCE1, "Dance 1"},
	{SPECIAL_ACTION_DANCE2, "Dance 2"},
	{SPECIAL_ACTION_DANCE3, "Dance 3"},
	{SPECIAL_ACTION_DANCE4, "Dance 4"},
	{SPECIAL_ACTION_HANDSUP, "Hands Up"},
	{SPECIAL_ACTION_SITTING, "Sitting"},
	{SPECIAL_ACTION_USECELLPHONE, "Use Cellphone"},
	{SPECIAL_ACTION_STOPUSECELLPHONE, "Stop Use Cellphone"},
	{SPECIAL_ACTION_DRINK_BEER, "Drink Beer"},
	{SPECIAL_ACTION_SMOKE_CIGGY, "Smoke Ciggy"},
	{SPECIAL_ACTION_DRINK_WINE, "Drink Wine"},
	{SPECIAL_ACTION_DRINK_SPRUNK, "Drink Sprunk"},
	{SPECIAL_ACTION_CUFFED, "Cuffed"},
	{SPECIAL_ACTION_CARRY, "Carry"},
	{SPECIAL_ACTION_URINATE, "Urinate"}
};
*/
struct cheats
{
	bool active;
	bool nodialog;
	bool nocontrol;
	bool cardeleter;
	bool car_shooter;
	bool carunocdeleter;
	bool pick_car;
	bool playercrasher;
	bool carshot;
	bool always_engine_on;
	bool noeject;
	bool restore_aero;
	bool carshotbypass;
	bool fps_unlimiter;
	bool coordmaster;
	bool cm_menu;
	
	bool antifreeze;
	bool pulsecolor;
	bool pulsewheels;
	bool skillmaster;
	bool slapper;
	bool pizdarvanka;
	bool reconnect;
	bool autologin;
	bool aafk;
	bool invisible;
	bool uinvisible;
	bool skin_aim;
	bool ping_aim;
	bool air_break;
	bool god_mode_ped;
	bool god_mode_veh;
	bool banyhop;
	bool dhider;
	//bool buble;
	bool autoreconnect;
	bool bike_fall;
	//bool improved_overview;
	//bool full_overview;
	bool obj_col;
	bool play_col;
	bool veh_col;
	bool autoshot;
	bool clear_screen;
	bool chat_ids;
	bool nospread;
	bool cargun;
	bool noreload;
	bool fast_connect;
	bool clickwarp;
	bool pluscbug;
	bool keep_trailer;
	//bool flooder;
	bool antistun;
	bool pizdets;
	bool pizda;
	struct render 
	{
		bool pickup;
		bool object;
		bool player;
		bool vehicle;
		bool labels;
	};
	struct render render;
};
 

void							ImGui_Menu_Draw();
void							ImGui_CM_Draw();
void							CoordMaster_Start(int id);
//void func_bottom(ImVec2 window_pos, ImVec2 window_size);

extern struct cheats			menu;

