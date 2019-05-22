#include <main.h>
#include "INI.h"
#include <Windows.h>
#include <String>
#pragma comment(lib, "user32.lib")


string				file::ini::fname;
vector<string>		file::ini::strbuf;

struct ConfigKey
{
	ConfigKey() {}

	ConfigKey(std::string Name, std::shared_ptr<ConfigValue> Value)
	{
		this->Name = Name;
		this->Value = std::move(Value);
	}

	std::string Name;
	std::shared_ptr<ConfigValue> Value;
};

struct ConfigSection
{
	ConfigSection() {}

	ConfigSection(std::string Name)
	{
		this->Name = Name;
	}

	std::string Name;
	std::vector<std::unique_ptr<ConfigKey>> Keys;
};

void CConfig::SetValue(std::string SectionName, std::string KeyName, std::unique_ptr<ConfigValue> Value)
{
	this->Sections.emplace_back(std::make_unique<ConfigSection>());

	bool FoundSection = false;
	bool FoundKey = false;

	for (const auto& Section : this->Sections)
	{
		if (Section->Name == SectionName)
		{
			FoundSection = true;

			for (const auto& Key : Section->Keys)
			{
				if (Key->Name == KeyName)
				{
					FoundKey = true;
					Key->Value = std::move(Value);
				}
			}

			if (!FoundKey)
			{
				auto Key = std::make_unique<ConfigKey>(KeyName, std::move(Value));
				Section->Keys.emplace_back(std::move(Key));
			}
		}
	}

	if (!FoundSection)
	{
		auto Section = std::make_unique<ConfigSection>(SectionName);
		auto Key = std::make_unique<ConfigKey>(KeyName, std::move(Value));

		Section->Keys.emplace_back(std::move(Key));
		this->Sections.emplace_back(std::move(Section));
	}
}

int CConfig::GetIntValue(const std::string& SectionName, const std::string& KeyName)
{
	char* tempString = new char[64];
	GetPrivateProfileStringA(SectionName.c_str(), KeyName.c_str(), NULL, tempString, 64, szIniFileName);
	std::string ReturnedString = tempString;
	delete tempString;
	return atoi(ReturnedString.c_str());
}
bool CConfig::GetBoolValue(const std::string& SectionName, const std::string& KeyName)
{
	char* tempString = new char[64];
	GetPrivateProfileStringA(SectionName.c_str(), KeyName.c_str(), NULL, tempString, 64, szIniFileName);
	std::string ReturnedString = tempString;
	delete tempString;
	return (atoi(ReturnedString.c_str()) != 0);
}
float CConfig::GetFloatValue(const std::string& SectionName, const std::string& KeyName)
{
	char* tempString = new char[64];
	GetPrivateProfileStringA(SectionName.c_str(), KeyName.c_str(), NULL, tempString, 64, szIniFileName);
	std::string ReturnedString = tempString;
	delete tempString;
	return atof(ReturnedString.c_str());
}

bool CConfig::Save(std::string PathToINI)
{
	for (const auto& Section : this->Sections)
	{
		for (const auto& Key : Section->Keys)
		{
			switch (Key->Value->Type)
			{
			case CONFIG_INT:
				if (!WritePrivateProfileStringA(Section->Name.c_str(), Key->Name.c_str(), std::to_string(Key->Value->Int).c_str(), PathToINI.c_str()))
					return false;
				break;
			case CONFIG_FLOAT:
				if (!WritePrivateProfileStringA(Section->Name.c_str(), Key->Name.c_str(), std::to_string(Key->Value->Float).c_str(), PathToINI.c_str()))
					return false;
				break;
			case CONFIG_BOOL:
				if (!WritePrivateProfileStringA(Section->Name.c_str(), Key->Name.c_str(), Key->Value->Bool ? std::to_string(1).c_str() : std::to_string(0).c_str(), PathToINI.c_str()))
					return false;
				break;
			case CONFIG_STRING:
				if (!WritePrivateProfileStringA(Section->Name.c_str(), Key->Name.c_str(), Key->Value->String.c_str(), PathToINI.c_str()))
					return false;
				break;
			default:
				break;
			}
		}
	}

	return true;
}

std::unique_ptr<CConfig> Config = std::make_unique<CConfig>();

void create_ini_file()
{
	Config->SetValue("Keys", "enable_menu", std::make_unique<ConfigValue>(113, CONFIG_INT));
	Config->SetValue("Keys", "dialoghider", std::make_unique<ConfigValue>(222, CONFIG_INT));
	Config->SetValue("Keys", "god_mode_veh", std::make_unique<ConfigValue>(0, CONFIG_INT));
	Config->SetValue("Keys", "god_mode_ped", std::make_unique<ConfigValue>(0, CONFIG_INT));
	Config->SetValue("Keys", "air_break", std::make_unique<ConfigValue>(161, CONFIG_INT));
	Config->SetValue("Keys", "reconnect", std::make_unique<ConfigValue>(48, CONFIG_INT));
	Config->SetValue("Keys", "skin_aimbot", std::make_unique<ConfigValue>(90, CONFIG_INT));
	Config->SetValue("Keys", "ping_aimbot", std::make_unique<ConfigValue>(88, CONFIG_INT));
	Config->SetValue("Keys", "speedhack", std::make_unique<ConfigValue>(164, CONFIG_INT));
	Config->SetValue("Keys", "repair", std::make_unique<ConfigValue>(49, CONFIG_INT));
	Config->SetValue("Keys", "unfreze", std::make_unique<ConfigValue>(220, CONFIG_INT));
	Config->SetValue("Keys", "fast_exit", std::make_unique<ConfigValue>(0, CONFIG_INT));
	Config->SetValue("Keys", "click_warp", std::make_unique<ConfigValue>(4, CONFIG_INT));
	Config->SetValue("Keys", "turn_right", std::make_unique<ConfigValue>(221, CONFIG_INT));
	Config->SetValue("Keys", "turn_left", std::make_unique<ConfigValue>(219, CONFIG_INT));
	Config->SetValue("Keys", "turn_back", std::make_unique<ConfigValue>(8, CONFIG_INT));
	Config->SetValue("Keys", "unflip", std::make_unique<ConfigValue>(46, CONFIG_INT));
	Config->SetValue("Keys", "fly", std::make_unique<ConfigValue>(190, CONFIG_INT));

	Config->SetValue("Changes", "exclusive_delay", std::make_unique<ConfigValue>(50, CONFIG_INT));
	Config->SetValue("Changes", "exclusive_speed", std::make_unique<ConfigValue>(2.5f, CONFIG_FLOAT));
	Config->SetValue("Changes", "exclusive_dist", std::make_unique<ConfigValue>(200.0f, CONFIG_FLOAT));
	Config->SetValue("Changes", "reconnect_delay", std::make_unique<ConfigValue>(5, CONFIG_INT));
	Config->SetValue("Changes", "carshot_speed", std::make_unique<ConfigValue>(50.0f, CONFIG_FLOAT));
	Config->SetValue("Changes", "air_break_speed", std::make_unique<ConfigValue>(50.0f, CONFIG_FLOAT));
	Config->SetValue("Changes", "acceleration", std::make_unique<ConfigValue>(5.0f, CONFIG_FLOAT));
	Config->SetValue("Changes", "max_speed", std::make_unique<ConfigValue>(150.0f, CONFIG_FLOAT));
	Config->SetValue("Changes", "fly_player_speed", std::make_unique<ConfigValue>(1.0f, CONFIG_FLOAT));
	Config->SetValue("Changes", "fly_player_accel_multiplier", std::make_unique<ConfigValue>(1.0f, CONFIG_FLOAT));
	Config->SetValue("Changes", "fly_player_decel_multiplier", std::make_unique<ConfigValue>(2.0f, CONFIG_FLOAT));

	Config->SetValue("StartUP", "restore_windows_aero", std::make_unique<ConfigValue>(0, CONFIG_INT));
	Config->SetValue("StartUP", "enable_anti_bike_fall", std::make_unique<ConfigValue>(0, CONFIG_INT));
	Config->SetValue("StartUP", "enable_banyhop", std::make_unique<ConfigValue>(0, CONFIG_INT));
	Config->SetValue("StartUP", "enable_anti_afk", std::make_unique<ConfigValue>(0, CONFIG_INT));
	Config->SetValue("StartUP", "enable_clear_screen", std::make_unique<ConfigValue>(0, CONFIG_INT));
	Config->SetValue("StartUP", "enable_chat_ids", std::make_unique<ConfigValue>(0, CONFIG_INT));
	Config->SetValue("StartUP", "enable_fast_connect", std::make_unique<ConfigValue>(0, CONFIG_INT));
	Config->SetValue("StartUP", "enable_auto_reconnect", std::make_unique<ConfigValue>(0, CONFIG_INT));
	Config->SetValue("StartUP", "enable_keep_trailer", std::make_unique<ConfigValue>(0, CONFIG_INT));
	Config->SetValue("StartUP", "enable_antifreeze", std::make_unique<ConfigValue>(0, CONFIG_INT));

	Config->SetValue("StartUP", "enable_always_engine_on", std::make_unique<ConfigValue>(0, CONFIG_INT));
	Config->SetValue("StartUP", "enable_antistun", std::make_unique<ConfigValue>(0, CONFIG_INT));
	Config->SetValue("StartUP", "enable_autoshot", std::make_unique<ConfigValue>(0, CONFIG_INT));
	Config->SetValue("StartUP", "enable_god_mode_veh", std::make_unique<ConfigValue>(0, CONFIG_INT));
	Config->SetValue("StartUP", "enable_nocontrol", std::make_unique<ConfigValue>(0, CONFIG_INT));
	Config->SetValue("StartUP", "enable_nodialog", std::make_unique<ConfigValue>(0, CONFIG_INT));
	Config->SetValue("StartUP", "enable_noeject", std::make_unique<ConfigValue>(0, CONFIG_INT));
	Config->SetValue("StartUP", "enable_noreload", std::make_unique<ConfigValue>(0, CONFIG_INT));
	Config->SetValue("StartUP", "enable_nospread", std::make_unique<ConfigValue>(0, CONFIG_INT));
	Config->SetValue("StartUP", "enable_obj_col", std::make_unique<ConfigValue>(0, CONFIG_INT));
	Config->SetValue("StartUP", "enable_ping_aim", std::make_unique<ConfigValue>(0, CONFIG_INT));
	Config->SetValue("StartUP", "enable_play_col", std::make_unique<ConfigValue>(0, CONFIG_INT));
	Config->SetValue("StartUP", "enable_pluscbug", std::make_unique<ConfigValue>(0, CONFIG_INT));
	Config->SetValue("StartUP", "enable_render.player", std::make_unique<ConfigValue>(0, CONFIG_INT));
	Config->SetValue("StartUP", "enable_render.vehicle", std::make_unique<ConfigValue>(0, CONFIG_INT));
	Config->SetValue("StartUP", "enable_skin_aim", std::make_unique<ConfigValue>(0, CONFIG_INT));
	Config->SetValue("StartUP", "enable_veh_col", std::make_unique<ConfigValue>(0, CONFIG_INT));
	Config->SetValue("StartUP", "enable_fps_unlimiter", std::make_unique<ConfigValue>(0, CONFIG_INT));
	Config->SetValue("StartUP", "enable_bottom_bar", std::make_unique<ConfigValue>(0, CONFIG_INT));
	Config->SetValue("StartUP", "enable_fly_vehicle_heliMode", std::make_unique<ConfigValue>(0, CONFIG_INT));

	Config->SetValue("CoordMaster", "cm_delay", std::make_unique<ConfigValue>(200, CONFIG_INT));
	Config->SetValue("CoordMaster", "cm_dist", std::make_unique<ConfigValue>(30.0f, CONFIG_FLOAT));
	Config->SetValue("CoordMaster", "cm_height", std::make_unique<ConfigValue>(-10.0f, CONFIG_FLOAT));

	/*
	ini.rvanka.time = Config->GetIntValue("Rvanks", "rvanka_time");
	ini.rvanka.speed[0] = Config->GetFloatValue("Rvanks", "rvanka_x");
	ini.rvanka.speed[1] = Config->GetFloatValue("Rvanks", "rvanka_y");
	ini.rvanka.speed[2] = Config->GetFloatValue("Rvanks", "rvanka_z");
	ini.rvanka.spread[0] = Config->GetFloatValue("Rvanks", "rvanka_spread1");
	ini.rvanka.spread[1] = Config->GetFloatValue("Rvanks", "rvanka_spread2");*/

	Config->SetValue("Rvanks", "rvanka_time", std::make_unique<ConfigValue>(5000, CONFIG_INT));
	Config->SetValue("Rvanks", "rvanka_x", std::make_unique<ConfigValue>(100.0f, CONFIG_FLOAT));
	Config->SetValue("Rvanks", "rvanka_y", std::make_unique<ConfigValue>(100.0f, CONFIG_FLOAT));
	Config->SetValue("Rvanks", "rvanka_z", std::make_unique<ConfigValue>(100.0f, CONFIG_FLOAT));

	Config->Save(szIniFileName);
}

void get_ini_file_settings()
{
	if (!FileExists(szIniFileName))
		create_ini_file();

	ini.key.air_break = Config->GetIntValue("Keys", "air_break");
	ini.key.click_warp = Config->GetIntValue("Keys", "click_warp");
	ini.key.dialoghider = Config->GetIntValue("Keys", "dialoghider");
	ini.key.fast_exit = Config->GetIntValue("Keys", "fast_exit");
	ini.key.god_mode_veh = Config->GetIntValue("Keys", "god_mode_veh");
	ini.key.god_mode_ped = Config->GetIntValue("Keys", "god_mode_ped");
	ini.key.menu = Config->GetIntValue("Keys", "enable_menu");
	ini.key.ping_aimbot = Config->GetIntValue("Keys", "ping_aimbot");
	ini.key.reconnect = Config->GetIntValue("Keys", "reconnect");
	ini.key.repair = Config->GetIntValue("Keys", "repair");
	ini.key.skin_aimbot = Config->GetIntValue("Keys", "skin_aimbot");
	ini.key.turn_back = Config->GetIntValue("Keys", "turn_back");
	ini.key.turn_left = Config->GetIntValue("Keys", "turn_left");
	ini.key.turn_right = Config->GetIntValue("Keys", "turn_right");
	ini.key.speedhack = Config->GetIntValue("Keys", "speedhack");
	ini.key.unflip = Config->GetIntValue("Keys", "unflip");
	ini.key.unfreze = Config->GetIntValue("Keys", "unfreze");
	ini.key.fly = Config->GetIntValue("Keys", "fly");

	ini.acceleration = Config->GetFloatValue("Changes", "acceleration");
	ini.air_break_speed = Config->GetFloatValue("Changes", "air_break_speed");
	ini.carshot_speed = Config->GetFloatValue("Changes", "carshot_speed");
	ini.max_speed = Config->GetFloatValue("Changes", "max_speed");
	ini.reconnect_delay = Config->GetIntValue("Changes", "reconnect_delay");
	ini.exclusive_delay = Config->GetIntValue("Changes", "exclusive_delay");
	ini.exclusive_speed = Config->GetIntValue("Changes", "exclusive_speed");
	ini.exclusive_dist = Config->GetIntValue("Changes", "exclusive_dist");
	ini.fly_player_speed = Config->GetIntValue("Changes", "fly_player_speed");
	ini.fly_player_decel_multiplier = Config->GetIntValue("Changes", "fly_player_decel_multiplier");
	ini.fly_player_accel_multiplier = Config->GetIntValue("Changes", "fly_player_accel_multiplier");


	menu.restore_aero = Config->GetBoolValue("StartUP", "restore_windows_aero");
	menu.bike_fall = Config->GetBoolValue("StartUP", "enable_anti_bike_fall");
	menu.banyhop = Config->GetBoolValue("StartUP", "enable_banyhop");
	menu.aafk = Config->GetBoolValue("StartUP", "enable_anti_afk");
	menu.clear_screen = Config->GetBoolValue("StartUP", "enable_clear_screen");
	menu.chat_ids = Config->GetBoolValue("StartUP", "enable_chat_ids");
	menu.fast_connect = Config->GetBoolValue("StartUP", "enable_fast_connect");
	menu.autoreconnect = Config->GetBoolValue("StartUP", "enable_auto_reconnect");
	menu.keep_trailer = Config->GetBoolValue("StartUP", "enable_keep_trailer");
	menu.antifreeze = Config->GetBoolValue("StartUP", "enable_antifreeze");

	menu.always_engine_on = Config->GetBoolValue("StartUP", "enable_always_engine_on");
	menu.antistun = Config->GetBoolValue("StartUP", "enable_antistun");
	menu.autoshot = Config->GetBoolValue("StartUP", "enable_autoshot");
	menu.god_mode_veh = Config->GetBoolValue("StartUP", "enable_god_mode_veh");
	menu.nocontrol = Config->GetBoolValue("StartUP", "enable_nocontrol");
	menu.nodialog = Config->GetBoolValue("StartUP", "enable_nodialog");
	menu.noeject = Config->GetBoolValue("StartUP", "enable_noeject");
	menu.noreload = Config->GetBoolValue("StartUP", "enable_noreload");
	menu.nospread = Config->GetBoolValue("StartUP", "enable_nospread");
	menu.obj_col = Config->GetBoolValue("StartUP", "enable_obj_col");
	menu.ping_aim = Config->GetBoolValue("StartUP", "enable_ping_aim");
	menu.play_col = Config->GetBoolValue("StartUP", "enable_play_col");
	menu.pluscbug = Config->GetBoolValue("StartUP", "enable_pluscbug");
	menu.render.labels = Config->GetBoolValue("StartUP", "enable_render.labels");
	menu.render.player = Config->GetBoolValue("StartUP", "enable_render.player");
	menu.render.vehicle = Config->GetBoolValue("StartUP", "enable_render.vehicle");
	menu.render.pickup = Config->GetBoolValue("StartUP", "enable_render.pickup");
	menu.render.object = Config->GetBoolValue("StartUP", "enable_render.object");
	menu.skin_aim = Config->GetBoolValue("StartUP", "enable_skin_aim");
	menu.veh_col = Config->GetBoolValue("StartUP", "enable_veh_col");
	menu.fps_unlimiter = Config->GetBoolValue("StartUP", "enable_fps_unlimiter");
	menu.bottom_bar = Config->GetBoolValue("StartUP", "enable_bottom_bar");
	menu.fly_vehicle_heliMode = Config->GetIntValue("StartUP", "enable_fly_vehicle_heliMode");

	ini.cm_delay = Config->GetIntValue("CoordMaster", "cm_delay");
	ini.cm_dist = Config->GetFloatValue("CoordMaster", "cm_dist");
	ini.cm_height = Config->GetFloatValue("CoordMaster", "cm_height");

	ini.rvanka.time = Config->GetIntValue("Rvanks", "rvanka_time");
	ini.rvanka.speed[0] = Config->GetFloatValue("Rvanks", "rvanka_x");
	ini.rvanka.speed[1] = Config->GetFloatValue("Rvanks", "rvanka_y");
	ini.rvanka.speed[2] = Config->GetFloatValue("Rvanks", "rvanka_z");

	get_patches();
	get_CM();
}
void save_all_settings()
{
	Config->SetValue("Changes", "exclusive_delay", std::make_unique<ConfigValue>(ini.exclusive_delay, CONFIG_INT));
	Config->SetValue("Changes", "exclusive_dist", std::make_unique<ConfigValue>(ini.exclusive_dist, CONFIG_FLOAT));
	Config->SetValue("Changes", "exclusive_speed", std::make_unique<ConfigValue>(ini.exclusive_speed, CONFIG_FLOAT));
	Config->SetValue("Changes", "reconnect_delay", std::make_unique<ConfigValue>(ini.reconnect_delay, CONFIG_INT));
	Config->SetValue("Changes", "carshot_speed", std::make_unique<ConfigValue>(ini.carshot_speed, CONFIG_FLOAT));
	Config->SetValue("Changes", "air_break_speed", std::make_unique<ConfigValue>(ini.air_break_speed, CONFIG_FLOAT));
	Config->SetValue("Changes", "acceleration", std::make_unique<ConfigValue>(ini.acceleration, CONFIG_FLOAT));
	Config->SetValue("Changes", "max_speed", std::make_unique<ConfigValue>(ini.max_speed, CONFIG_FLOAT));
	Config->SetValue("Changes", "fly_player_speed", std::make_unique<ConfigValue>(ini.fly_player_speed, CONFIG_FLOAT));
	Config->SetValue("Changes", "fly_player_accel_multiplier", std::make_unique<ConfigValue>(ini.fly_player_accel_multiplier, CONFIG_FLOAT));
	Config->SetValue("Changes", "fly_player_decel_multiplier", std::make_unique<ConfigValue>(ini.fly_player_decel_multiplier, CONFIG_FLOAT));

	Config->SetValue("StartUP", "restore_windows_aero", std::make_unique<ConfigValue>(menu.restore_aero, CONFIG_BOOL));
	Config->SetValue("StartUP", "enable_anti_bike_fall", std::make_unique<ConfigValue>(menu.bike_fall, CONFIG_BOOL));
	Config->SetValue("StartUP", "enable_banyhop", std::make_unique<ConfigValue>(menu.banyhop, CONFIG_BOOL));
	Config->SetValue("StartUP", "enable_anti_afk", std::make_unique<ConfigValue>(menu.aafk, CONFIG_BOOL));
	Config->SetValue("StartUP", "enable_clear_screen", std::make_unique<ConfigValue>(menu.clear_screen, CONFIG_BOOL));
	Config->SetValue("StartUP", "enable_chat_ids", std::make_unique<ConfigValue>(menu.chat_ids, CONFIG_BOOL));
	Config->SetValue("StartUP", "enable_fast_connect", std::make_unique<ConfigValue>(menu.fast_connect, CONFIG_BOOL));
	Config->SetValue("StartUP", "enable_auto_reconnect", std::make_unique<ConfigValue>(menu.autoreconnect, CONFIG_BOOL));
	Config->SetValue("StartUP", "enable_keep_trailer", std::make_unique<ConfigValue>(menu.keep_trailer, CONFIG_BOOL));
	Config->SetValue("StartUP", "enable_antifreeze", std::make_unique<ConfigValue>(menu.antifreeze, CONFIG_BOOL));
	Config->SetValue("StartUP", "enable_render.player", std::make_unique<ConfigValue>(menu.render.player, CONFIG_BOOL));
	Config->SetValue("StartUP", "enable_render.vehicle", std::make_unique<ConfigValue>(menu.render.vehicle, CONFIG_BOOL));
	Config->SetValue("StartUP", "enable_bottom_bar", std::make_unique<ConfigValue>(menu.bottom_bar, CONFIG_BOOL));
	Config->SetValue("StartUP", "enable_fly_vehicle_heliMode", std::make_unique<ConfigValue>(menu.fly_vehicle_heliMode, CONFIG_BOOL));

	Config->SetValue("CoordMaster", "cm_delay", std::make_unique<ConfigValue>(ini.cm_delay, CONFIG_INT));
	Config->SetValue("CoordMaster", "cm_dist", std::make_unique<ConfigValue>(ini.cm_dist, CONFIG_FLOAT));
	Config->SetValue("CoordMaster", "cm_height", std::make_unique<ConfigValue>(ini.cm_height, CONFIG_FLOAT));

	Config->SetValue("Rvanks", "rvanka_time", std::make_unique<ConfigValue>(ini.rvanka.time, CONFIG_INT));
	Config->SetValue("Rvanks", "rvanka_x", std::make_unique<ConfigValue>(ini.rvanka.speed[0], CONFIG_FLOAT));
	Config->SetValue("Rvanks", "rvanka_y", std::make_unique<ConfigValue>(ini.rvanka.speed[1], CONFIG_FLOAT));
	Config->SetValue("Rvanks", "rvanka_z", std::make_unique<ConfigValue>(ini.rvanka.speed[2], CONFIG_FLOAT));

	Config->Save(szIniFileName);
}
void create_patches()
{
	file::ini::add("##### Формат: \"Название\" Ид Автовключение\n");

	file::ini::add("##### Входящие РПЦ #####");
	file::ini::add("patch_incoming_rpc = \"ConnectionRejected\"\t13    \t0");
	file::ini::add("patch_incoming_rpc = \"ClientMessage\"\t14    \t0");
	file::ini::add("patch_incoming_rpc = \"SetWorldTime\"\t15    \t0");
	file::ini::add("patch_incoming_rpc = \"SetPlayerWeather\"\t19    \t0");
	file::ini::add("patch_incoming_rpc = \"SetPlayerSkin\"\t20    \t0");
	file::ini::add("patch_incoming_rpc = \"SetPlayerName\"\t21    \t0");
	file::ini::add("patch_incoming_rpc = \"SetPlayerPos\"\t22    \t0");
	file::ini::add("patch_incoming_rpc = \"SetPlayerPosFindZ\"\t23    \t0");
	file::ini::add("patch_incoming_rpc = \"SetPlayerHealth\"\t24    \t0");
	file::ini::add("patch_incoming_rpc = \"PutPlayerInVehicle\"\t25    \t0");
	//file::ini::add("patch_incoming_rpc = \"RemovePlayerFromVehicle\"\t26    \t0");
	file::ini::add("patch_incoming_rpc = \"SetPlayerColor\"\t27    \t0");
	file::ini::add("patch_incoming_rpc = \"DisplayGameText\"\t28    \t0");
	file::ini::add("patch_incoming_rpc = \"ForceSpawnSelection\"\t29    \t0");
	file::ini::add("patch_incoming_rpc = \"AttachObjectToPlayer\"\t30    \t0");
	file::ini::add("patch_incoming_rpc = \"InitMenu\"\t31    \t0");
	file::ini::add("patch_incoming_rpc = \"ShowMenu\"\t32    \t0");
	file::ini::add("patch_incoming_rpc = \"HideMenu\"\t33    \t0");
	file::ini::add("patch_incoming_rpc = \"SetPlayerWantedLevel\"\t34    \t0");
	file::ini::add("patch_incoming_rpc = \"ShowTextDraw\"\t35    \t0");
	file::ini::add("patch_incoming_rpc = \"HideTextDraw\"\t36    \t0");
	file::ini::add("patch_incoming_rpc = \"EnableStuntBonus\"\t37    \t0");
	file::ini::add("patch_incoming_rpc = \"EditTextDraw\"\t38    \t0");
	file::ini::add("patch_incoming_rpc = \"AddGangZone\"\t39    \t0");
	file::ini::add("patch_incoming_rpc = \"PlayCrimeReportForPlayer\"\t43    \t0");
	file::ini::add("patch_incoming_rpc = \"PlayAudioStream\"\t45    \t0");
	file::ini::add("patch_incoming_rpc = \"StopAudioStreamForPlayer\"\t46    \t0");
	file::ini::add("patch_incoming_rpc = \"RemoveBuildingForPlayer\"\t47    \t0");
	file::ini::add("patch_incoming_rpc = \"CreateObject\"\t48    \t0");
	file::ini::add("patch_incoming_rpc = \"SetObjectPos\"\t49    \t0");
	file::ini::add("patch_incoming_rpc = \"SetObjectRotation\"\t50    \t0");
	file::ini::add("patch_incoming_rpc = \"DestroyObject\"\t51    \t0");
	file::ini::add("patch_incoming_rpc = \"CreateExplosion\"\t54    \t0");
	file::ini::add("patch_incoming_rpc = \"ShowNameTagForPlayer\"\t55    \t0");
	file::ini::add("patch_incoming_rpc = \"AttachCameraToObject\"\t56    \t0");
	file::ini::add("patch_incoming_rpc = \"ClickTextDraw\"\t58    \t0");
	file::ini::add("patch_incoming_rpc = \"GiveTakeDamage\"\t60    \t0");
	file::ini::add("patch_incoming_rpc = \"EditAttachedObject\"\t61    \t0");
	file::ini::add("patch_incoming_rpc = \"EditObject\"\t62    \t0");
	file::ini::add("patch_incoming_rpc = \"SelectObject\"\t63    \t0");
	file::ini::add("patch_incoming_rpc = \"CancelEdit\"\t64    \t0");
	file::ini::add("patch_incoming_rpc = \"WorldPlayerAdd\"\t69    \t0");
	file::ini::add("patch_incoming_rpc = \"SetShopName\"\t70    \t0");
	file::ini::add("patch_incoming_rpc = \"SetDrunkLevel\"\t72    \t0");
	file::ini::add("patch_incoming_rpc = \"SetChatBubble\"\t75    \t0");
	file::ini::add("patch_incoming_rpc = \"ShowDialog\"\t77    \t0");
	file::ini::add("patch_incoming_rpc = \"VehicleParams\"\t80    \t0");
	file::ini::add("patch_incoming_rpc = \"RemoveGangZone\"\t82    \t0");
	file::ini::add("patch_incoming_rpc = \"ClientJoin\"\t81    \t0");
	file::ini::add("patch_incoming_rpc = \"FlashGangZone\"\t83    \t0");
	file::ini::add("patch_incoming_rpc = \"StopFlashGangZone\"\t84    \t0");
	file::ini::add("patch_incoming_rpc = \"ApplyAnimation\"\t85    \t0");
	file::ini::add("patch_incoming_rpc = \"ClearAnimations\"\t86    \t0");
	file::ini::add("patch_incoming_rpc = \"SetSpecialAction\"\t87    \t0");
	file::ini::add("patch_incoming_rpc = \"SetFightingStyle\"\t88    \t0");
	file::ini::add("patch_incoming_rpc = \"SetPlayerVelocity\"\t89    \t0");
	file::ini::add("patch_incoming_rpc = \"SetVehicleVelocity\"\t90    \t0");
	file::ini::add("patch_incoming_rpc = \"MoveObject\"\t94    \t0");
	file::ini::add("patch_incoming_rpc = \"StopObject\"\t95    \t0");
	file::ini::add("patch_incoming_rpc = \"TogglePlayerSpectating\"\t97    \t0");
	file::ini::add("patch_incoming_rpc = \"SetPlayerSpectating\"\t98    \t0");
	file::ini::add("patch_incoming_rpc = \"PlayerSpectatePlayer\"\t99    \t0");
	file::ini::add("patch_incoming_rpc = \"PlayerSpectateVehicle\"\t100    \t0");
	file::ini::add("patch_incoming_rpc = \"UpdateScoresPingsIPs\"\t105    \t0");
	file::ini::add("patch_incoming_rpc = \"SetPlayerInterior\"\t106    \t0");
	file::ini::add("patch_incoming_rpc = \"SetCameraPos\"\t107    \t0");
	file::ini::add("patch_incoming_rpc = \"SetCameraLookAt\"\t108    \t0");
	file::ini::add("patch_incoming_rpc = \"SetVehiclePos\"\t109    \t0");
	file::ini::add("patch_incoming_rpc = \"SetVehicleZAngle\"\t110    \t0");
	file::ini::add("patch_incoming_rpc = \"SetVehicleParamsForPlayer\"\t111    \t0");
	file::ini::add("patch_incoming_rpc = \"SetCameraBehindPlayer\"\t112    \t0");
	file::ini::add("patch_incoming_rpc = \"TogglePlayerControllable\"\t113    \t0");
	file::ini::add("patch_incoming_rpc = \"PlaySound\"\t114    \t0");
	file::ini::add("patch_incoming_rpc = \"SetPlayerWorldBounds\"\t115    \t0");
	file::ini::add("patch_incoming_rpc = \"GivePlayerMoney\"\t116    \t0");
	file::ini::add("patch_incoming_rpc = \"SetPlayerFacingAngle\"\t117    \t0");
	file::ini::add("patch_incoming_rpc = \"ResetMoney\"\t118    \t0");
	file::ini::add("patch_incoming_rpc = \"ResetPlayerWeapons\"\t119    \t0");
	file::ini::add("patch_incoming_rpc = \"GivePlayerWeapon\"\t120    \t0");
	file::ini::add("patch_incoming_rpc = \"LinkVehicleToInterior\"\t122    \t0");
	file::ini::add("patch_incoming_rpc = \"SetPlayerArmour\"\t123    \t0");
	file::ini::add("patch_incoming_rpc = \"SetSpawnInfo\"\t125    \t0");
	file::ini::add("patch_incoming_rpc = \"SetPlayerTeam\"\t126    \t0");
	file::ini::add("patch_incoming_rpc = \"DeathMessage\"\t153    \t0");
	file::ini::add("patch_incoming_rpc = \"SetMapIcon\"\t154    \t0");
	file::ini::add("patch_incoming_rpc = \"DisableMapIcon\"\t155    \t0");
	file::ini::add("patch_incoming_rpc = \"SetGravity\"\t157    \t0");
	file::ini::add("patch_incoming_rpc = \"SetVehicleHealth\"\t158    \t0");
	file::ini::add("patch_incoming_rpc = \"AttachTrailerToVehicle\"\t159    \t0");
	file::ini::add("patch_incoming_rpc = \"DetachTrailerFromVehicle\"\t160    \t0");
	file::ini::add("patch_incoming_rpc = \"AddVehicleComponent\"\t161    \t0\n");

	file::ini::align(2);


	file::ini::add("##### Исходящие РПЦ #####");
	file::ini::add("patch_outcoming_rpc = \"ServerCommand\"\t6    \t0");
	file::ini::add("patch_outcoming_rpc = \"SetInteriorId\"\t7    \t0");
	file::ini::add("patch_outcoming_rpc = \"MapMarker\"\t8    \t0");
	file::ini::add("patch_outcoming_rpc = \"VehicleDestroyed\"\t9    \t0");
	file::ini::add("patch_outcoming_rpc = \"ServerJoin\"\t10    \t0");
	file::ini::add("patch_outcoming_rpc = \"ServerQuit\"\t11    \t0");
	file::ini::add("patch_outcoming_rpc = \"InitGame\"\t12    \t0");
	file::ini::add("patch_outcoming_rpc = \"ClickTextDraw\"\t58    \t0");
	file::ini::add("patch_outcoming_rpc = \"GiveTakeDamage\"\t60    \t0");
	file::ini::add("patch_outcoming_rpc = \"WorldPlayerAdd\"\t69    \t0");
	file::ini::add("patch_outcoming_rpc = \"DialogResponse\"\t78    \t0");
	file::ini::add("patch_outcoming_rpc = \"ClickPlayer\"\t79    \t0");
	file::ini::add("patch_outcoming_rpc = \"PlayerSpectatePlayer\"\t99    \t0");
	file::ini::add("patch_outcoming_rpc = \"PlayerSpectateVehicle\"\t100    \t0");
	file::ini::add("patch_outcoming_rpc = \"RequestClass\"\t101    \t0");
	file::ini::add("patch_outcoming_rpc = \"RequestSpawn\"\t102    \t0");
	file::ini::add("patch_outcoming_rpc = \"EnterVehicle\"\t103    \t0");
	file::ini::add("patch_outcoming_rpc = \"ExitVehicle\"\t104    \t0");
	file::ini::add("patch_outcoming_rpc = \"UpdateScoresPingsIPs\"\t105    \t0");
	file::ini::add("patch_outcoming_rpc = \"WorldPlayerRemove\"\t127    \t0");
	file::ini::add("patch_outcoming_rpc = \"WorldVehicleAdd\"\t128    \t0");
	file::ini::add("patch_outcoming_rpc = \"WorldVehicleRemove\"\t129    \t0");
	file::ini::add("patch_outcoming_rpc = \"WorldPlayerDeath\"\t130    \t0");
	file::ini::add("patch_outcoming_rpc = \"DamageVehicle\"\t131    \t0");
	file::ini::add("patch_outcoming_rpc = \"GameModeRestart\"\t136    \t0");
	file::ini::add("patch_outcoming_rpc = \"Chat\"\t138    \t0");
	file::ini::add("patch_outcoming_rpc = \"ScmEvent\"\t140    \t0");
	file::ini::add("patch_outcoming_rpc = \"PickedUpPickup\"\t142    \t0");
	file::ini::add("patch_outcoming_rpc = \"MenuSelect\"\t143    \t0");
	file::ini::add("patch_outcoming_rpc = \"MenuQuit\"\t144    \t0");
	file::ini::add("patch_outcoming_rpc = \"Spawn\"\t150    \t0");
	file::ini::add("patch_outcoming_rpc = \"Death\"\t151    \t0");
	file::ini::add("patch_outcoming_rpc = \"NPCJoin\"\t152    \t0\n");

	file::ini::align(2);

	file::ini::add("##### Входящие пакеты #####");
	file::ini::add("patch_incoming_packet = \"Authentication\"\t12    \t0");
	file::ini::add("patch_incoming_packet = \"ConnectionAccepted\"\t34    \t0");
	file::ini::add("patch_incoming_packet = \"ConnectionAttemptFailed\"\t29    \t0");
	file::ini::add("patch_incoming_packet = \"InvalidPassword\"\t37    \t0");
	file::ini::add("patch_incoming_packet = \"ConnectionLost\"\t33    \t0");
	file::ini::add("patch_incoming_packet = \"ConnectionClosed\"\t32    \t0");
	file::ini::add("patch_incoming_packet = \"NoFreeConnections\"\t31    \t0");
	file::ini::add("patch_incoming_packet = \"ConnectionBanned\"\t35    \t0");
	file::ini::add("patch_incoming_packet = \"UnoccupiedData\"\t110    \t0");
	file::ini::add("patch_incoming_packet = \"MarkersData\"\t111    \t0");
	file::ini::add("patch_incoming_packet = \"WeaponData\"\t112    \t0");
	file::ini::add("patch_incoming_packet = \"TrailerData\"\t114    \t0");
	file::ini::add("patch_incoming_packet = \"PassengerData\"\t115    \t0");
	file::ini::add("patch_incoming_packet = \"PlayerData\"\t116    \t0");
	file::ini::add("patch_incoming_packet = \"AimData\"\t117    \t0");
	file::ini::add("patch_incoming_packet = \"VehicleData\"\t118    \t0\n");

	file::ini::align(2);

	file::ini::add("##### Исходящие пакеты #####");
	file::ini::add("patch_outcoming_packet = \"Authentication\"\t12    \t0");
	file::ini::add("patch_outcoming_packet = \"UnoccupiedData\"\t110    \t0");
	file::ini::add("patch_outcoming_packet = \"MarkersData\"\t111    \t0");
	file::ini::add("patch_outcoming_packet = \"WeaponData\"\t112    \t0");
	file::ini::add("patch_outcoming_packet = \"SpectatingData\"\t113    \t0");
	file::ini::add("patch_outcoming_packet = \"TrailerData\"\t114    \t0");
	file::ini::add("patch_outcoming_packet = \"PassengerData\"\t115    \t0");
	file::ini::add("patch_outcoming_packet = \"PlayerData\"\t116    \t0");
	file::ini::add("patch_outcoming_packet = \"AimData\"\t117    \t0");
	file::ini::add("patch_outcoming_packet = \"VehicleData\"\t118    \t0\n");

	file::ini::write();
}
void get_patches()
{
		file::ini(string(szPatchesFileName));

		if (!FileExists(szPatchesFileName) || !file::ini::read())
			create_patches();

		ini.patch_cnt = 0;
		while (file::ini::get("patch_incoming_rpc", ini.patch[ini.patch_cnt].name,
			ini.patch[ini.patch_cnt].id, ini.patch[ini.patch_cnt].enabled))
			ini.patch[ini.patch_cnt++].type = INCOMING_RPC;

		while (file::ini::get("patch_outcoming_rpc", ini.patch[ini.patch_cnt].name,
			ini.patch[ini.patch_cnt].id, ini.patch[ini.patch_cnt].enabled))
			ini.patch[ini.patch_cnt++].type = OUTCOMING_RPC;

		while (file::ini::get("patch_incoming_packet", ini.patch[ini.patch_cnt].name,
			ini.patch[ini.patch_cnt].id, ini.patch[ini.patch_cnt].enabled))
			ini.patch[ini.patch_cnt++].type = INCOMING_PACKET;

		while (file::ini::get("patch_outcoming_packet", ini.patch[ini.patch_cnt].name,
			ini.patch[ini.patch_cnt].id, ini.patch[ini.patch_cnt].enabled))
			ini.patch[ini.patch_cnt++].type = OUTCOMING_PACKET;
}
void get_CM()
{
	file::ini(string(szCMFileName));
	if (!FileExists(szCMFileName) || !file::ini::read())
		create_CM();
	ini.cm[0].id = 0;
	ini.cm[0].name = "";
	ini.cm[0].Pos[0] = 0.0f;
	ini.cm[0].Pos[1] = 0.0f;
	ini.cm[0].Pos[2] = 0.0f;
	ini.cm_cnt = 1;
	while	(file::ini::get("cm_point", 
			ini.cm[ini.cm_cnt].name,
			ini.cm[ini.cm_cnt].Pos[0],
			ini.cm[ini.cm_cnt].Pos[1],
			ini.cm[ini.cm_cnt].Pos[2]))
		ini.cm[ini.cm_cnt].id = ini.cm_cnt++;
}
void create_CM()
{
	file::ini::add("##### Формат: \"Название\" X Y Z\n");
	file::ini::add("cm_point = \"Центр карты\" \t0 \t0 \t5");
	file::ini::add("cm_point = \"Просто пример\" \t1035.86 \t22.343 \t66");

	file::ini::write();
}