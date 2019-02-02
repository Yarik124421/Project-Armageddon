/*
AUTHOR: SlonoBoyko
LiteHack.Ru
21.05.2018
*/

#include <menu.h>
#include <INI.h>
#include "main.h"

struct cheats				menu;
struct ini_file				ini;

void ImGui_Teleport()
{
	if (*(int *)0xBA6774 != 0)
	{
		float coords[3];
		if (gta_map_marker_get(coords)) {
			pCRMP->getChat()->addMessageToChat(COLOR_MSG_SUCCESS,
				"Вы были телепортированы на координаты: %0.2f %0.2f %0.2f",
				coords[0], coords[1], coords[2]);
			gta_teleport(coords);
		}
	}
	else pCRMP->getChat()->addMessageToChat(COLOR_MSG_ERROR, "Ошибка! Поставьте метку на карте.");
}
void ImGui_ToggleEngine()
{
	if (trainer.state == CHEAT_STATE_VEHICLE) {
		vehicle_info *vinfo = vehicle_info_get(VEHICLE_SELF);
		if (vinfo != NULL) {
			vinfo->m_nVehicleFlags.bEngineOn ^= true;
			int iVehicleID = pCRMP->getVehicles()->getSAMPVehicleID(vinfo);
			if (!pCRMP->getVehicles()->isBadVehicle(iVehicleID))
				pCRMP->getVehicles()->pSAMP_Vehicle[iVehicleID]->iIsMotorOn = vinfo->m_nVehicleFlags.bEngineOn;
		}
	}
	else pCRMP->getChat()->addMessageToChat(COLOR_MSG_ERROR, "Ошибка! У вас нет автомобиля.");

}
void ImGui_ToggleLight()
{
	if (trainer.state == CHEAT_STATE_VEHICLE) {
		vehicle_info *vinfo = vehicle_info_get(VEHICLE_SELF);
		if (vinfo != NULL) {
			vinfo->m_nVehicleFlags.bLightsOn ^= true;
			int VehicleID = pCRMP->getVehicles()->getSAMPVehicleID(vinfo);
			if (!pCRMP->getVehicles()->isBadVehicle(VehicleID))
				pCRMP->getVehicles()->pSAMP_Vehicle[VehicleID]->iIsLightsOn = vinfo->m_nVehicleFlags.bLightsOn;
		}
	}
	else pCRMP->getChat()->addMessageToChat(COLOR_MSG_ERROR, "Ошибка! У вас нет автомобиля.");
}
void ImGui_Unfrese()
{
	pCRMP->getMisc()->toggleSAMPCursor(true, CURSORMODE_NO_LOCKING);

	//	pCRMP->getPlayers()->pLocalPlayer->toggleControllable(true);
	pPedSelf->SetIsLocked(false);
	GTAfunc_TogglePlayerControllable(false);

	if (trainer.state == CHEAT_STATE_ACTOR)
		GTAfunc_DisembarkInstantly();

	pGame->GetCamera()->RestoreWithJumpCut();
	pCRMP->getMisc()->toggleSAMPCursor(true);
}
void ImGui_RepairVehicle()
{
	if (trainer.state == CHEAT_STATE_VEHICLE) {
		vehicle_info *vinfo = vehicle_info_get(VEHICLE_SELF);
		if (vinfo != NULL) {
			GTAfunc_RepairVehicle(vinfo);
			vinfo->m_nVehicleFlags.bIsDamaged = false;
		}
	}
	else pCRMP->getChat()->addMessageToChat(COLOR_MSG_ERROR, "Ошибка! У вас нет автомобиля.");
}
void ImGui_SpawnCars()
{
	for (int i = 0; i < SAMP_MAX_VEHICLES; i++)
	{
		if (pCRMP->getVehicles()->iIsListed[i] != 1)
			continue;
		if (pCRMP->getVehicles()->pSAMP_Vehicle[i] == nullptr)
			continue;
		if (pCRMP->getVehicles()->pSAMP_Vehicle[i]->pGTA_Vehicle == nullptr)
			continue;
		if (pCRMP->getVehicles()->pSAMP_Vehicle[i]->isSelfVehicle())
			continue;

		pCRMP->getRakClient()->SendVehicleDestroyed(i);
		Sleep(5);
	}

	return;
}
void ImGui_NewSpawnCars()
{
	for (int i = 0; i < SAMP_MAX_VEHICLES; i++)
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

		pCRMP->getRakClient()->SendVehicleDeleter(i);
		Sleep(5);
	}
	return;
}
void ImGui_ObjectDump()
{
	FILE *file; SYSTEMTIME time;
	GetLocalTime(&time);

	char filename[MAX_PATH];
	sprintf(filename, "%s\\CRMP_Objects.txt", szWorkingDirectory);

	if (file = fopen(filename, "a")) {
		fprintf(file, "\n\n\nPlayer: %s[%d]\n", pCRMP->getPlayers()->strLocalPlayerName.c_str(), pCRMP->getPlayers()->sLocalPlayerID);
		fprintf(file, "Server: %s [%s:%d]\n", pCRMP->getInfo()->szHostname, pCRMP->getInfo()->szIP, pCRMP->getInfo()->ulPort);
		fprintf(file, "Date: %0.2d.%0.2d.%0.2d Time: %0.2d:%0.2d:%0.2d\n", time.wDay, time.wMonth, time.wYear, time.wHour, time.wMinute, time.wSecond);
		fprintf(file, "Position: %.2f, %.2f, %.2f\n\n\n", pPedSelf->GetPosition()->fX, pPedSelf->GetPosition()->fY, pPedSelf->GetPosition()->fZ);

		for (int i = 0; i < SAMP_MAX_OBJECTS; i++) {
			if (pCRMP->getObjects()->iIsListed[i] != 1) continue;
			if (pCRMP->getObjects()->object[i] == NULL) continue;
			if (vect3_near_zero(pCRMP->getObjects()->object[i]->fPos)) continue;
			stObject *pObject = pCRMP->getObjects()->object[i];
			fprintf(file, "CreateObject(%d, %.2f, %.2f, %.2f, %.2f, %.2f, %.2f);\n", pObject->pGTA_Entity->model_alt_id,
				pObject->fPos[0], pObject->fPos[1], pObject->fPos[2], pObject->fRot[0], pObject->fRot[1], pObject->fRot[2]);
		}
		fclose(file);
	}
	return pCRMP->getChat()->addMessageToChat(COLOR_MSG_SUCCESS, "Все объекты успешно скопированны в файл {FF9900}CRMP_Objects.txt");
}
void ImGui_OpenVehicles()
{
	if (pCRMP->getVehicles() == nullptr) return;

	for (int i = 0; i < SAMP_MAX_VEHICLES; i++)
	{
		if (pCRMP->getVehicles()->iIsListed[i] != 1) continue;
		if (pCRMP->getVehicles()->pSAMP_Vehicle[i] == nullptr) continue;
		if (pCRMP->getVehicles()->pSAMP_Vehicle[i]->pGTA_Vehicle == nullptr) continue;
		if (i == pCRMP->getPlayers()->pLocalPlayer->sCurrentVehicleID) continue;

		pCRMP->getVehicles()->pSAMP_Vehicle[i]->setDoorState(false);
	}
	return;
}
void ImGui_Menu_Draw()
{
	if (gta_menu_active() || !menu.active || menu.pizda) return;

	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(5.0f, 0.0f));
	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(3.0f, 1.0f));
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(5.0f, 3.0f));
	ImGui::PushStyleVar(ImGuiStyleVar_ButtonTextAlign, ImVec2(0.1f, 0.5f));
	ImGui::PushStyleVar(ImGuiStyleVar_GrabMinSize, 10.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowMinSize, ImVec2(180.0f, 250.0f));
	ImGui::SetNextWindowSize(ImVec2(240.0f, 350.0f), ImGuiCond_Once);
	ImGui::SetNextWindowPos(ImVec2(300.0f, 350.0f), ImGuiCond_Once);
	std::string temp = "Project Armageddon v3 | Yarik124";
	if(trainer.online > 0)
		temp = "Project Armageddon v3 | Online: " + std::to_string(trainer.online);

	if (ImGui::Begin(cp1251_to_utf8(temp).c_str(), &menu.active, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_ShowBorders))
	{
		float fItemWidth = ImGui::GetWindowWidth() - 10.0f;
		if (ImGui::GetScrollMaxY() > 5.0f)
			fItemWidth -= ImGui::GetStyle().ScrollbarSize;
		ImGui::Spacing();
		ImGui::Separator();
		if (ImGui::Button(cp1251_to_utf8("Coord-Master").c_str(), ImVec2(fItemWidth, 15.f)))
		{
			menu.cm_menu = !menu.cm_menu;
			Config->SetValue("CoordMaster", "cm_delay", std::make_unique<ConfigValue>(ini.cm_delay, CONFIG_INT));
			Config->SetValue("CoordMaster", "cm_dist", std::make_unique<ConfigValue>(ini.cm_dist, CONFIG_FLOAT));
			Config->SetValue("CoordMaster", "cm_height", std::make_unique<ConfigValue>(ini.cm_height, CONFIG_FLOAT));

			Config->Save(szIniFileName);
		}
		ImGui::Spacing();
		if (ImGui::CollapsingHeader(cp1251_to_utf8("Нопы").c_str()))
		{
			ImGui::Separator();
			ImGui::Checkbox(cp1251_to_utf8("Запретить выкидывать из авто").c_str(), &menu.noeject);
			ImGui::Checkbox(cp1251_to_utf8("Запретить глушить двигатель").c_str(), &menu.always_engine_on);
			ImGui::Checkbox(cp1251_to_utf8("Запретить показывать диалоги").c_str(), &menu.nodialog);
			ImGui::Checkbox(cp1251_to_utf8("Запретить замораживать себя").c_str(), &menu.nocontrol);
			ImGui::Separator();
		}
		ImGui::Spacing();
		if (ImGui::CollapsingHeader(cp1251_to_utf8("Основные").c_str()))
		{
			ImGui::Separator();
			if (ImGui::Button(cp1251_to_utf8("Телепорт на метку").c_str(), ImVec2(fItemWidth, 15.f))) ImGui_Teleport();
			if (ImGui::Button(cp1251_to_utf8("Разморозка").c_str(), ImVec2(fItemWidth, 15.f))) ImGui_Unfrese();
			if (ImGui::Button(cp1251_to_utf8("Починка автомобиля").c_str(), ImVec2(fItemWidth, 15.f))) ImGui_RepairVehicle();
			if (ImGui::Button(cp1251_to_utf8("Завести двигатель").c_str(), ImVec2(fItemWidth, 15.f))) ImGui_ToggleEngine();
			if (ImGui::Button(cp1251_to_utf8("Включить фары").c_str(), ImVec2(fItemWidth, 15.f))) ImGui_ToggleLight();
			if (ImGui::Button(cp1251_to_utf8("Открыть все машины").c_str(), ImVec2(fItemWidth, 15.f))) ImGui_OpenVehicles();
			ImGui::Checkbox(cp1251_to_utf8("Бессмертие персонажа").c_str(), &menu.god_mode_ped);
			ImGui::Checkbox(cp1251_to_utf8("Бессмертие авто").c_str(), &menu.god_mode_veh);
			ImGui::Checkbox(cp1251_to_utf8("Не падать с байка").c_str(), &menu.bike_fall);
			ImGui::Checkbox(cp1251_to_utf8("Каршот").c_str(), &menu.carshot);
			ImGui::Separator();

		}
		ImGui::Spacing();
		if (ImGui::CollapsingHeader(cp1251_to_utf8("WallHack").c_str()))
		{
			ImGui::Separator();
			ImGui::Checkbox(cp1251_to_utf8("Отрисовка игроков").c_str(), &menu.render.player);
			ImGui::Checkbox(cp1251_to_utf8("Отрисовка автомобилей").c_str(), &menu.render.vehicle);
			ImGui::Checkbox(cp1251_to_utf8("Отрисовка пикапов").c_str(), &menu.render.pickup);
			ImGui::Checkbox(cp1251_to_utf8("Отрисовка объектов").c_str(), &menu.render.object);
			ImGui::Checkbox(cp1251_to_utf8("Отрисовка 3D лейблов").c_str(), &menu.render.labels);
			ImGui::Separator();
		}
		ImGui::Spacing();
		if (ImGui::CollapsingHeader(cp1251_to_utf8("Продвинутые").c_str()))
		{
			ImGui::Separator();
			ImGui::Checkbox(cp1251_to_utf8("Нубо невидимка").c_str(), &menu.invisible);
			ImGui::Checkbox(cp1251_to_utf8("Андеграунд невидимка").c_str(), &menu.uinvisible);
			ImGui::Checkbox(cp1251_to_utf8("Пиздарванка").c_str(), &menu.pizdarvanka);
			ImGui::Checkbox(cp1251_to_utf8("Слапер").c_str(), &menu.slapper);
			ImGui::Checkbox(cp1251_to_utf8("Аимбот по скину").c_str(), &menu.skin_aim);
			ImGui::Checkbox(cp1251_to_utf8("Аимбот по пингу").c_str(), &menu.ping_aim);
			ImGui::Checkbox(cp1251_to_utf8("Автошот").c_str(), &menu.autoshot);
			ImGui::Checkbox(cp1251_to_utf8("Антиразброс пуль").c_str(), &menu.nospread);
			ImGui::Checkbox(cp1251_to_utf8("Стрельба без перезарядки").c_str(), &menu.noreload);
			ImGui::Checkbox(cp1251_to_utf8("Скилл мастер").c_str(), &menu.skillmaster);
			ImGui::Checkbox(cp1251_to_utf8("Авто +С баг").c_str(), &menu.pluscbug);
			ImGui::Checkbox(cp1251_to_utf8("Коллизия игроков").c_str(), &menu.play_col);
			ImGui::Checkbox(cp1251_to_utf8("Коллизия автомобилей").c_str(), &menu.veh_col);
			ImGui::Checkbox(cp1251_to_utf8("Коллизия обьектов").c_str(), &menu.obj_col);
			if (ImGui::Button(cp1251_to_utf8("Дамп объектов").c_str(), ImVec2(fItemWidth, 15.f))) ImGui_ObjectDump();
			ImGui::Separator();
		}
		ImGui::Spacing();
		if (ImGui::CollapsingHeader(cp1251_to_utf8("Игрок").c_str()))
		{
			ImGui::Separator();
			if (ImGui::Button(cp1251_to_utf8("Заспавниться").c_str(), ImVec2(fItemWidth, 15.f)))
				pCRMP->getPlayers()->pLocalPlayer->sendSpawn();
			if (ImGui::Button(cp1251_to_utf8("Восстановить здоровье").c_str(), ImVec2(fItemWidth, 15.f))) pPedSelf->SetHealth(100.0f);
			if (ImGui::Button(cp1251_to_utf8("Восстановить броню").c_str(), ImVec2(fItemWidth, 15.f))) pPedSelf->SetArmor(100.0f);
			ImGui::Checkbox(cp1251_to_utf8("Анти стан").c_str(), &menu.antistun);
			ImGui::Checkbox(cp1251_to_utf8("Банихоп").c_str(), &menu.banyhop);

			ImGui::Separator();
		}
		ImGui::Spacing();
		if (ImGui::CollapsingHeader(cp1251_to_utf8("Эксклюзив").c_str()))
		{
			ImGui::Separator();
			if (ImGui::Button(cp1251_to_utf8("Респануть все машины(OLD)").c_str(), ImVec2(fItemWidth, 15.f))) ImGui_SpawnCars();
			if (ImGui::Button(cp1251_to_utf8("Удалить все машины").c_str(), ImVec2(fItemWidth, 15.f))) ImGui_NewSpawnCars();
			ImGui::Checkbox(cp1251_to_utf8("Удалить все машины v2").c_str(), &menu.cardeleter);
			ImGui::Checkbox(cp1251_to_utf8("Удалить все машины v3").c_str(), &menu.carunocdeleter);
			ImGui::Checkbox(cp1251_to_utf8("Стрельба авто v2").c_str(), &menu.car_shooter);
			ImGui::Checkbox(cp1251_to_utf8("Таскалка авто").c_str(), &menu.pick_car);
			ImGui::Checkbox(cp1251_to_utf8("OnFoot Crasher(OLD)").c_str(), &menu.playercrasher);
			ImGui::Checkbox(cp1251_to_utf8("Обход каршота").c_str(), &menu.carshotbypass);

			ImGui::Separator();

		}
		ImGui::Spacing();
		if (ImGui::CollapsingHeader(cp1251_to_utf8("Патчи").c_str()))
		{
			ImGui::Separator();

			auto toggle_all = [](uint8_t type) {
				static bool state[5] = { false, false, false, false, false };
				for (size_t i = 0; i < ini.patch_cnt; i++) {
					if (ini.patch[i].type == type)
						ini.patch[i].enabled = !state[type];
				} state[type] ^= true;
			};

			for (int i = 1; i <= 4; i++)
			{
				static int iTreeId[4] = { false, false, false, false };
				if (ImGui::TreeNode(&iTreeId[i - 1], cp1251_to_utf8("%s %s").c_str(), i % 2 ? cp1251_to_utf8("Входящие").c_str() : cp1251_to_utf8("Исходящие").c_str(), i > 2 ? cp1251_to_utf8("Пакеты").c_str() : cp1251_to_utf8("RPC").c_str()))
				{
					if (ImGui::Button(cp1251_to_utf8("Переключить все").c_str(), ImVec2(fItemWidth - ImGui::GetStyle().IndentSpacing, 15.f)))
						toggle_all(i);

					ImGui::Spacing();

					for (size_t x = 0; x < ini.patch_cnt; x++)
					{
						if (ini.patch[x].id && ini.patch[x].name.length() && ini.patch[x].type == i) {
							string strName = to_string(ini.patch[x].id) + ". " + ini.patch[x].name;
							ImGui::Checkbox(strName.c_str(), &ini.patch[x].enabled);
						}
					}
					ImGui::TreePop();
				}
			}
			ImGui::Separator();
		}
		ImGui::Spacing();
		if (ImGui::CollapsingHeader(cp1251_to_utf8("Настройки").c_str())) {
			ImGui::Separator();
			ImGui::PushItemWidth(fItemWidth);
			ImGui::SliderInt(cp1251_to_utf8(" ").c_str(), &ini.exclusive_delay, 50, 1000, cp1251_to_utf8("Задержка стрельбы авто: %.0f мс.").c_str());
			ImGui::SliderFloat(cp1251_to_utf8("  ").c_str(), &ini.carshot_speed, 1.0f, 200.0f, cp1251_to_utf8("Скорость каршота: %.1f км/ч").c_str());
			ImGui::SliderInt(cp1251_to_utf8("   ").c_str(), &ini.reconnect_delay, 0, 25, cp1251_to_utf8("Задержка реконнекта: %.0f с.").c_str());
			ImGui::SliderFloat(cp1251_to_utf8("    ").c_str(), &ini.air_break_speed, 1.0f, 250.0f, cp1251_to_utf8("Скорость аир брейка: %.1f км/ч").c_str());
			ImGui::SliderFloat(cp1251_to_utf8("     ").c_str(), &ini.max_speed, 1.0f, 200.0f, cp1251_to_utf8("Скорость спидхака: %.1f км/ч").c_str());
			ImGui::SliderFloat(cp1251_to_utf8("      ").c_str(), &ini.acceleration, 1.0f, 50.0f, cp1251_to_utf8("Разгон спидхака: %.1f м/с").c_str(), 1.1f);
			ImGui::PopItemWidth();
			ImGui::Checkbox(cp1251_to_utf8("Чистый скриншот").c_str(), &menu.clear_screen);
			ImGui::Checkbox(cp1251_to_utf8("Ид игроков в чате").c_str(), &menu.chat_ids);
			ImGui::Checkbox(cp1251_to_utf8("Сохранять Aero Win7").c_str(), &menu.restore_aero);
			if (ImGui::Checkbox(cp1251_to_utf8("Работа в свернутом режиме").c_str(), &menu.aafk)) func_aafk();
			ImGui::Checkbox(cp1251_to_utf8("Автореконнект").c_str(), &menu.autoreconnect);
			ImGui::Checkbox(cp1251_to_utf8("Быстрый коннект").c_str(), &menu.fast_connect);
			ImGui::Checkbox(cp1251_to_utf8("Сохранять прицеп").c_str(), &menu.keep_trailer);
			ImGui::Checkbox(cp1251_to_utf8("FPS Unlimiter").c_str(), &menu.fps_unlimiter);


			ImGui::Separator();
		}
		ImGui::Spacing();
			ImGui::End();
			ImGui::PopStyleVar(8);
	}
		if (!menu.active) pCRMP->getMisc()->toggleSAMPCursor(/*trainer.imcursor*/ ImGui::GetIO().MouseDrawCursor = false);
}
void ImGui_CM_Draw()
{
	if (gta_menu_active() || !menu.cm_menu || !menu.active) return;

	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(5.0f, 0.0f));
	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(3.0f, 1.0f));
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(5.0f, 3.0f));
	ImGui::PushStyleVar(ImGuiStyleVar_ButtonTextAlign, ImVec2(0.1f, 0.5f));
	ImGui::PushStyleVar(ImGuiStyleVar_GrabMinSize, 10.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowMinSize, ImVec2(180.0f, 250.0f));
	ImGui::SetNextWindowSize(ImVec2(240.0f, 85.0f), ImGuiCond_Once);
	ImGui::SetNextWindowPos(ImVec2(700.0f, 350.0f), ImGuiCond_Once);
	if (ImGui::Begin(cp1251_to_utf8("Coord-master").c_str(), &menu.cm_menu, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_ShowBorders))
	{
		ImGui::Spacing();
		ImGui::Separator();
		//ImGui::PushItemWidth(fItemWidth);
		
		//ImGui::PopItemWidth();
		ImGui::PushItemWidth(85.0f);
		ImGui::InputInt(cp1251_to_utf8("Задержка").c_str(), &ini.cm_delay);
		ImGui::InputFloat(cp1251_to_utf8("Глубина").c_str(), &ini.cm_height);
		ImGui::InputFloat(cp1251_to_utf8("Скорость").c_str(), &ini.cm_dist);
		ImGui::PopItemWidth();
		if (ImGui::Button(cp1251_to_utf8("Отправиться к метке").c_str())) CoordMaster_Start(-1);
		if (ImGui::Button(cp1251_to_utf8("Отправиться к чекпоинту").c_str())) CoordMaster_Start(-2);
		for (size_t x = 0; x < ini.cm_cnt; x++)
			if (ini.cm[x].id && ini.cm[x].name.length()) 
				if (ImGui::Button(cp1251_to_utf8(ini.cm[x].name).c_str())) CoordMaster_Start(ini.cm[x].id);
		ImGui::Separator();
		ImGui::Spacing();
		ImGui::End();
		ImGui::PopStyleVar(8);
	}
	//if (!menu.active) pCRMP->getMisc()->toggleSAMPCursor(/*trainer.imcursor*/ ImGui::GetIO().MouseDrawCursor = false);
}
void CoordMaster_Start(int id)
{
	if (id == ini.cm_id)
	{
		float fPosition[3] = {
		pPedSelf->GetPosition()->fX + 1,
		pPedSelf->GetPosition()->fY + 1,
		pPedSelf->GetPosition()->fZ
		};
		fPosition[2] = pGame->GetWorld()->FindGroundZForPosition(fPosition[0], fPosition[1]) + 1.f;
		vect3_copy(fPosition, ini.cm_pos);
		ini.cm_id = -10;
		return;
	}
	else ini.cm_id = id;
	if (id == -1)
	{
		if (*(int *)0xBA6774 != 0)
		{
			gta_map_marker_get(ini.cm_pos);
			menu.coordmaster = 1;
		}
		else pCRMP->getChat()->addMessageToChat(COLOR_MSG_ERROR, "Маркер не установлен.");
	}
	else if (id == -2)
	{
		float markerPos[3];
		if (!gta_world_marker_get(markerPos))
			pCRMP->getChat()->addMessageToChat(COLOR_MSG_ERROR, "Чекпоинт не установлен.");
		else
		{
			gta_world_marker_get(ini.cm_pos);
			menu.coordmaster = 1;
		}
	}
	else if (!(ini.cm[id].Pos[0] == NULL && ini.cm[id].Pos[1] == NULL && ini.cm[id].Pos[2] == NULL))
	{
		vect3_copy(ini.cm[id].Pos, ini.cm_pos);
		menu.coordmaster = 1;
	}

}