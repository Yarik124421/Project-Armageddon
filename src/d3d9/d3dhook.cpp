/*
		AUTHOR: SlonoBoyko
		LiteHack.Ru
		21.05.2018
*/

#include <main.h>

CD3DHook *pD3DHook = nullptr;

CD3DHook::CD3DHook()
{
	InstallHook();

	pRender			= new CD3DRender(128);
	pD3DFont		= new CD3DFont("Tahoma", 10, FCR_BORDER);
	pD3DFontNick	= new CD3DFont("Microsoft MHei", 10, FCR_BOLD | FCR_BORDER);
	pD3DFontFix		= new CD3DFont("Tahoma", 8, FCR_BORDER);
	pD3DFontBig		= new CD3DFont("Arial", 11, FCR_BOLD | FCR_BORDER);
}

void CD3DHook::InstallHook()
{
	/*char szPath[MAX_PATH];
	GetSystemDirectory(szPath, MAX_PATH);
	strcat_s(szPath, "\\d3d9.dll");
	HMODULE hDirect = nullptr;
	
	while (hDirect == nullptr)
		hDirect = GetModuleHandle(szPath);
	Sleep(100);

	uint32_t dll_base = uint32_t(hDirect);
	while (dll_base++ < dll_base + 0x128000)
	{
		if (*(uint16_t*)(dll_base + 0x00) == 0x06C7 &&
			*(uint16_t*)(dll_base + 0x06) == 0x8689 &&
			*(uint16_t*)(dll_base + 0x0C) == 0x8689) {
			dll_base += 2; break;
		}
	}
	eDirectXVtable *vTable = *reinterpret_cast <eDirectXVtable**>(dll_base);*/

	uint32_t *vTable = nullptr;
	while (vTable == nullptr) vTable = reinterpret_cast<uint32_t *>(SAMP_DIRECT3DDEVICE9);
	pPresentParam = reinterpret_cast<D3DPRESENT_PARAMETERS*>(GAME_PRESENT_PARAMETERS);

	orig_Reset =	hook::func((uint32_t)&vTable[eDirectXVtable::VTABLE_Reset], Reset);
	orig_Present =	hook::func((uint32_t)&vTable[eDirectXVtable::VTABLE_Present], Present);
	orig_End =		hook::func((uint32_t)&vTable[eDirectXVtable::VTABLE_EndScene], End);
}
ImVec2 window_pos;
ImVec2 window_size;
bool isWork = false;
void CD3DHook::Render()
{

	if (pCRMP->getInfo() == NULL || gta_menu_active())
	{
	}
	else 
	{
		if (menu.render.player) {
			mem::copy((PVOID)SAMP_DISABLE_NAMETAGS_HP, "\xC3", 1, 1, "\x55");
			mem::copy((PVOID)SAMP_DISABLE_NAMETAGS, "\xC3", 1, 1, "\xA1");
		} else if (!mem::compare((PVOID)SAMP_DISABLE_NAMETAGS, "\xC3", 1)) {
			mem::set((PVOID)SAMP_DISABLE_NAMETAGS_HP, 0x55, 1);
			mem::set((PVOID)SAMP_DISABLE_NAMETAGS, 0xA1, 1);
		}
		if (menu.pizdets) return;
		if (menu.render.player)
		{
			for (int i = 0; i < SAMP_MAX_PLAYERS; i++)
			{
				if (pCRMP->getPlayers()->isBadPlayer(i)) continue;
				if (pCRMP->getPlayers()->pRemotePlayer[i]->pPlayerData == NULL) continue;
				if (pCRMP->getPlayers()->pRemotePlayer[i]->pPlayerData->pSAMP_Actor == NULL) continue;
				if (pCRMP->getPlayers()->pRemotePlayer[i]->pPlayerData->pSAMP_Actor->pGTA_Ped == NULL) continue;

				CVector vecScreen;
				CVector vecBone = *pCRMP->getPlayers()->getBonePos(i, 6);
				if (vecBone.IsNearZero()) continue;
				CalcScreenCoors(&vecBone, &vecScreen);
				if (vecScreen.fZ < 1.0f) continue;

				//sprintf(name, "%s[%d]", pCRMP->getPlayers()->getPlayerName(i), i);
				pD3DFontNick->Print(vecScreen.fX - 50.0f, vecScreen.fY - 65.0f, pCRMP->getPlayers()->getPlayerColor(i),
					pCRMP->getPlayers()->pRemotePlayer[i]->strPlayerName + "[" + to_string(i) + "]");

				float fHealth = pCRMP->getPlayers()->pRemotePlayer[i]->pPlayerData->fActorHealth;
				float fArmor = pCRMP->getPlayers()->pRemotePlayer[i]->pPlayerData->fActorArmor;

				if (fHealth >= 100.0f) fHealth = 100.0f;
				if (fArmor >= 100.0f) fArmor = 100.0f;
				pRender->D3DBox(vecScreen.fX - 51.0f, vecScreen.fY - 46.0f, 52.0f, 7.0f, 0x77000000);
				pRender->D3DBox(vecScreen.fX - 50.0f, vecScreen.fY - 45.0f, fHealth / 2, 5.0f, 0xFFFF0000);
				if (fArmor >= 5.0f)
				{
					pRender->D3DBox(vecScreen.fX - 51.0f, vecScreen.fY - 38.0f, 52.0f, 7.0f, 0x77000000);
					pRender->D3DBox(vecScreen.fX - 50.0f, vecScreen.fY - 37.0f, fArmor / 2, 5.0f, 0xFFF0F0F0);
				}
			}
		}

		if (menu.render.vehicle)
		{
			for (int i = 0; i < SAMP_MAX_VEHICLES; i++)
			{
				if (pCRMP->getVehicles()->iInitiated == 0) break;
				if (pCRMP->getVehicles()->iIsListed[i] != 1) continue;
				if (pCRMP->getVehicles()->pSAMP_Vehicle[i] == NULL) continue;
				if (pCRMP->getVehicles()->pGTA_Vehicle[i] == NULL) continue;
				vehicle_info *info = pCRMP->getVehicles()->getGTAVehicle(i);
				if (isBadPtr_GTA_pVehicle(info)) continue;

				CVector vecScreen; char text[128];
				CVector vecWorld = info->base.m_CMatrix->vPos;
				if (vecWorld.IsNearZero()) continue;
				vecWorld.fZ += 1.0f;
				CalcScreenCoors(&vecWorld, &vecScreen);

				if (vecScreen.fZ < 1.0f) continue;

				uint16_t uVehicleID = info->base.model_alt_id;
				const vehicle_entry *vehicle = gta_vehicle_get_by_id(uVehicleID);

				RwColor color = getVehicleColorRGB(info->color[0]);
				sprintf(text, "(%d) %s [Модель: %d]", i, vehicle->name, uVehicleID); vecScreen.fY -= 30.0f;
				pD3DFontNick->PrintShadow(vecScreen.fX - 30.0f, vecScreen.fY - 18.0f, D3DCOLOR_ARGB(color.a, color.r, color.g, color.b), text);

				float health_pos = 0.6f * (info->hitpoints - 250.0f) / 7.5f;
				pRender->D3DBox(vecScreen.fX - 31.0f, vecScreen.fY, 62.0f, 7.0f, 0x77000000);
				pRender->D3DBox(vecScreen.fX - 30.0f, vecScreen.fY, health_pos, 5.0f, D3DCOLOR_XRGB(255, 0, 0));

				if (info->door_status == 2)
					pD3DFontFix->PrintShadow(vecScreen.fX + 35.0f, vecScreen.fY - 4.0f, D3DCOLOR_XRGB(255, 55, 0), "Закрыта");

			}
		}

		if (menu.render.pickup)
		{
			for (int i = 0; i < SAMP_MAX_PICKUPS; i++)
			{
				if (pCRMP->getInfo()->pPools.pPickup == NULL) break;
				if (pCRMP->getInfo()->pPools.pPickup->count == 0) break;
				if (pCRMP->getInfo()->pPools.pPickup->ul_GTA_PickupID[i] == NULL) continue;
				if (pCRMP->getInfo()->pPools.pPickup->pickup[i].iModelID == NULL) continue;
				if (vect3_near_zero(pCRMP->getInfo()->pPools.pPickup->pickup[i].fPosition)) continue;

				CVector vecScreen; char text[128];
				CVector vecWorld = pCRMP->getInfo()->pPools.pPickup->pickup[i].fPosition;
				CalcScreenCoors(&vecWorld, &vecScreen);

				if (vecScreen.fZ < 1.f) continue;

				sprintf(text, "Пикап: %d Модель: %d", i, pCRMP->getInfo()->pPools.pPickup->pickup[i].iModelID);
				pD3DFontFix->Print(vecScreen.fX - 30.0f, vecScreen.fY - 12.0f, 0x00FF00, text);
				sprintf(text, "%.2f %.2f %.2f", vecWorld.fX, vecWorld.fY, vecWorld.fZ);
				pD3DFontFix->Print(vecScreen.fX - 30.0f, vecScreen.fY, 0x00FF00, text);
			}
		}

		if (menu.render.object)
		{
			for (int i = 0; i < SAMP_MAX_OBJECTS; i++)
			{
				if (pCRMP->getObjects()->iIsListed[i] != 1) continue;
				if (pCRMP->getObjects()->object[i] == NULL) continue;
				if (pCRMP->getObjects()->object[i]->pGTA_Entity == NULL) continue;
				if (vect3_near_zero(pCRMP->getObjects()->object[i]->fPos)) continue;

				CVector vecScreen; char text[64];
				CVector vecWorld = pCRMP->getObjects()->object[i]->fPos;
				CalcScreenCoors(&vecWorld, &vecScreen);

				if (vecScreen.fZ < 1.0f) continue;

				sprintf(text, "Объект: %d Модель: %d", i, pCRMP->getObjects()->object[i]->pGTA_Entity->model_alt_id);
				pD3DFontFix->Print(vecScreen.fX - 30.0f, vecScreen.fY - 12.0f, 0x00FF00, text);
				sprintf(text, "%0.2f %0.2f %0.2f", vecWorld.fX, vecWorld.fY, vecWorld.fZ);
				pD3DFontFix->Print(vecScreen.fX - 30.0f, vecScreen.fY, 0x00FF00, text);
			}
		}

		if (menu.render.labels)
		{
			for (int i = 0; i < SAMP_MAX_3DTEXTS; i++)
			{
				if (pCRMP->getInfo()->pPools.pText3D->iIsListed[i] != 1) continue;
				if (pCRMP->getInfo()->pPools.pText3D->textLabel[i].pText == NULL) continue;
				if (vect3_near_zero(pCRMP->getInfo()->pPools.pText3D->textLabel[i].fPosition)) continue;

				CVector vecScreen;
				CVector vecWorld = pCRMP->getInfo()->pPools.pText3D->textLabel[i].fPosition;
				CalcScreenCoors(&vecWorld, &vecScreen);
				if (vecScreen.fZ < 1.0f) continue;

				char *text = pCRMP->getInfo()->pPools.pText3D->textLabel[i].pText;

				vecScreen.fX -= pD3DFontFix->DrawLength(text) / 2;
			
				char *substr = strtok(text, "\n");
				while (substr != NULL) {
					vecScreen.fY -= pD3DFontFix->DrawHeight() / 2;
					pD3DFontFix->PrintShadow(vecScreen.fX, vecScreen.fY, pCRMP->getInfo()->pPools.pText3D->textLabel[i].color, substr);
					vecScreen.fY += pD3DFontFix->DrawHeight();
					substr = strtok(NULL, "\n");
				}
			}
		}
	}
	return;
}
void CD3DHook::UnInitRender(void)
{
	if (bD3DRenderInit == true) 
	{
		pRender->Invalidate();
		pD3DFont->Invalidate();
		pD3DFontFix->Invalidate();
		pD3DFontNick->Invalidate();
		pD3DFontBig->Invalidate();

		ImGui_ImplDX9_InvalidateDeviceObjects();

		bD3DRenderInit = false;
	}
	return;
}

void CD3DHook::InitRender(IDirect3DDevice9 *pDevice)
{
	if (pDevice == nullptr) return;
	
	if (bD3DRenderInit == false) 
	{
		pRender->Initialize(pDevice);
		pD3DFont->Initialize(pDevice);
		pD3DFontFix->Initialize(pDevice);
		pD3DFontNick->Initialize(pDevice);
		pD3DFontBig->Initialize(pDevice);
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();
		ImGuiStyle& style = ImGui::GetStyle();
		io.IniFilename = NULL;
		io.DeltaTime = 1.0f / 60.0f;
		//ImFont* pFont = io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\framd.ttf", 15);
		SetCursor(io.MouseDrawCursor ? NULL : LoadCursor(NULL, IDC_ARROW));
		io.Fonts->AddFontDefault();
		style.AntiAliasedLines = false;
		style.AntiAliasedFill = false;
		style.WindowBorderSize = 0.0f;
		ImGui_ImplWin32_Init(GetActiveWindow());
		ImGui_ImplDX9_Init(pPresentParam->hDeviceWindow, pDevice);
		ImGui_ImplDX9_CreateDeviceObjects();

		bD3DRenderInit = true;
	}
} 
uint64_t					__time_current;
double						g_timeDiff;

HRESULT CALLBACK CD3DHook::Present(IDirect3DDevice9 *pDevice, const RECT *pSrcRect, const RECT *pDestRect, HWND hDestWindow, const RGNDATA *pDirtyRegion)
{
	if (pDevice == nullptr)
		return pD3DHook->orig_Present(pDevice, pSrcRect, pDestRect, hDestWindow, pDirtyRegion);
	static uint64_t time_last;
	__time_current = __time_get();
	g_timeDiff = TIME_TO_DOUBLE(time_get() - time_last);
	if (pD3DHook->bD3DRenderInit) {
		static bool key_pressed = false;
		static DWORD clearFrameCount = 0;
		pD3DHook->bDrawingAllowed = true;
		if (menu.clear_screen) 
		{
			mem::copy((PVOID)SAMP_DISABLE_SCREENSHOT, "\x00", 1, 1, "\x01");
			if (GetAsyncKeyState(VK_F8) < 0) 
			{
				if (key_pressed == false) {
					pCRMP->getInfo()->takeScreenShot();
					clearFrameCount = 5;
					key_pressed = true;
				}
			} else if (key_pressed) key_pressed = false;

			if (clearFrameCount > 0) {
				clearFrameCount--;
				pD3DHook->bDrawingAllowed = false;
			}
		} else mem::set((PVOID)SAMP_DISABLE_SCREENSHOT, 1, 1);

		if (pD3DHook->bDrawingAllowed)
		{
			//SaveState(pDevice);
			ImGui_ImplDX9_NewFrame();
			ImGui_ImplWin32_NewFrame();
			ImGui_Menu_Draw();
			ImGui_CM_Draw();
			ImGui::EndFrame();
			ImGui::Render();
			ImGui_ImplDX9_RenderDrawLists(ImGui::GetDrawData());
			//RestoreState(pDevice);
			pD3DHook->Render();

		}
		
		if ((menu.active) && pCRMP->getMisc()->iCursorMode == CURSORMODE_DISABLE)
			pCRMP->getMisc()->toggleSAMPCursor(/*trainer.imcursor*/ ImGui::GetIO().MouseDrawCursor = true);
		if ((!menu.active) && pCRMP->getMisc()->iCursorMode == CURSORMODE_DISABLE)
			pCRMP->getMisc()->toggleSAMPCursor(/*trainer.imcursor*/ ImGui::GetIO().MouseDrawCursor = false);
		
		if (isKeyPressed(ini.key.menu)) {
			save_all_settings();
			menu.active = !menu.active;
			/*trainer.imcursor*/ ImGui::GetIO().MouseDrawCursor = (menu.active);
			pCRMP->getMisc()->toggleSAMPCursor(menu.active);
		}
		if (pCRMP->Init()) call_functions(g_timeDiff);
	} else if (pCRMP->Init()) pD3DHook->InitRender(pDevice);
	time_last = time_get();
		
	return pD3DHook->orig_Present(pDevice, pSrcRect, pDestRect, hDestWindow, pDirtyRegion);
}

HRESULT CALLBACK CD3DHook::End(IDirect3DDevice9 * pDevice)
{
	return pD3DHook->orig_End(pDevice);
}

HRESULT CALLBACK CD3DHook::Reset(IDirect3DDevice9 *pDevice, D3DPRESENT_PARAMETERS *pPresentParams)
{
	if (pDevice == nullptr)
		return pD3DHook->orig_Reset(pDevice, pPresentParams);
		
	if (pD3DHook->bD3DRenderInit) pD3DHook->UnInitRender();

	HRESULT hReturn = pD3DHook->orig_Reset(pDevice, pPresentParams);

	if (hReturn == D3D_OK) {
		pD3DHook->pD3DDevice = pDevice;
		pD3DHook->pPresentParam = pPresentParams;
		pD3DHook->InitRender(pDevice);
	}
	return hReturn;
}