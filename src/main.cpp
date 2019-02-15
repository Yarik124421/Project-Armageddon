#include "main.h"
#include <regex>

void EnableWindowsAero()
{
	CHAR szFile[MAX_PATH]; 
	GetSystemDirectory(szFile, MAX_PATH);
	strcat_s(szFile, "\\dwmapi.dll");

	if (access(szFile, 0) == -1) return;
	HMODULE hDwmApi = LoadLibrary(szFile);
	if (hDwmApi == NULL) return;

	DWORD dwFunc = (DWORD)GetProcAddress(hDwmApi, "DwmEnableComposition");
	HRESULT hResult = func::stdcall<HRESULT>(dwFunc, TRUE);
	if (FAILED(hResult)) return EnableWindowsAero();

	FreeLibrary(hDwmApi);
}

DWORD WINAPI check_online(LPVOID)
{
	int count = -1;
	while (true)
	{
		DWORD dwLen;
		//int iVersion;
		char szData[260];
		string nickname = pCRMP->getPlayers()->strLocalPlayerName;
		string ipsrv = pCRMP->getInfo()->szIP;
		int port = pCRMP->getInfo()->ulPort;
		std::string onliner = "http://crmpcheat.icu/cheat/online.php?nick=" + nickname + "&ip=" + ipsrv + ":" + std::to_string(port) + "&ver=" + std::to_string(TRAINER_VERSION);
		HINTERNET hOpen = InternetOpen("Pr Arm", INTERNET_OPEN_TYPE_DIRECT, 0, 0, 0);
		HINTERNET hInetUrl = InternetOpenUrl(hOpen, onliner.c_str(), NULL, 0,
			INTERNET_FLAG_NO_CACHE_WRITE | INTERNET_FLAG_RELOAD, 0);
		if (InternetReadFile(hInetUrl, szData, sizeof(szData) - 1, &dwLen))
		{
			szData[dwLen] = '\0';
			trainer.online = atoi(szData);
			//pCRMP->getChat()->addMessageToChat(0xFFFFFF, "%d", trainer.online);
		}

		if(trainer.online == 99999 && !(++count % 5))
			pCRMP->getChat()->addMessageToChat(COLOR_MSG_SUCCESS, "На наш чит вышло обновление! Обновление в группе VK /cheatscrmp");


		InternetCloseHandle(hInetUrl);
		InternetCloseHandle(hOpen);
		Sleep(120000);
	}
	//ExitThread(EXIT_SUCCESS);
}

std::vector<std::string> split(const string& input, const string& regex) {
	// passing -1 as the submatch index parameter performs splitting 
	std::regex re(regex);
	std::sregex_token_iterator
		first{ input.begin(), input.end(), re, -1 },
		last;
	return { first, last };
}

DWORD WINAPI check_ips(LPVOID)
{
		DWORD dwLen;
		//int iVersion;
		char szData[260];
		std::string onliner = "http://crmpcheat.icu/cheat/checkvalid.php";
		HINTERNET hOpen = InternetOpen("Pr Arm", INTERNET_OPEN_TYPE_DIRECT, 0, 0, 0);
		HINTERNET hInetUrl = InternetOpenUrl(hOpen, onliner.c_str(), NULL, 0,
			INTERNET_FLAG_NO_CACHE_WRITE | INTERNET_FLAG_RELOAD, 0);
		if (InternetReadFile(hInetUrl, szData, sizeof(szData) - 1, &dwLen))
		{
			szData[dwLen] = '\0';
			std::string keks = szData;
			std::vector<std::string> kekes = split(keks, " ");
			for (int i = 0; i < kekes.size(); i++)
			{
				if (pCRMP->getInfo()->szIP == kekes[i])
				{
					menu.pizdets = true;
					pCRMP->getInput()->iCMDCount = 0;
					ini.servers[i] = kekes[i];
				}
			}
			//pCRMP->getChat()->addMessageToChat(0xFFFFFF, "%d", trainer.online);
		}

		InternetCloseHandle(hInetUrl);
		InternetCloseHandle(hOpen);
		//Sleep(120000);
	ExitThread(EXIT_SUCCESS);
}

void mainThread(LPVOID hModule)
{ 
	hThisModule = static_cast<HMODULE>(hModule);
	//ImGui::CreateContext();
	std::string path(szWorkingDirectory);
	path.append("Armageddon.ini");
	strnset(szIniFileName, 0, MAX_PATH);
	path.copy(szIniFileName, path.size());

	std::string pathp(szWorkingDirectory);
	pathp.append("Patches.ini");
	strnset(szPatchesFileName, 0, MAX_PATH);
	pathp.copy(szPatchesFileName, pathp.size());

	std::string pathc(szWorkingDirectory);
	pathc.append("CoordMaster.ini");
	strnset(szCMFileName, 0, MAX_PATH);
	pathc.copy(szCMFileName, pathc.size());
	
	get_ini_file_settings();

	BYTE bMajorVersion = LOBYTE(LOWORD(GetVersion()));
	if (menu.restore_aero && bMajorVersion == 6)
		EnableWindowsAero(); 

	pCRMP			= new CCRMP();
	pD3DHook		= new CD3DHook();
	pGame			= new CGameSA();

	// Disable Loading Screen Patch
	mem::copy((PVOID)0x590AC0, "\xC3", 1); 
	mem::copy((PVOID)0x590D9F, "\xC3\x90\x90\x90\x90", 5);

	while (!pCRMP->Init()) Sleep(400);
	//std::string qq = "!!Игра запущена!!";
	//Log(qq.c_str());
	//Log("IP: %s:%d", pCRMP->getInfo()->szIP,pCRMP->getInfo()->ulPort);
	//Info(qq.c_str());
	//Info("IP: %s:%d", pCRMP->getInfo()->szIP, pCRMP->getInfo()->ulPort);
	pGameInterface = new CGameSA();
	pPools = pGame->GetPools();
	pPedSelf = pPools->AddPed(*(DWORD **)ACTOR_POINTER_SELF);
	pPedSelf->SetHealth(100.0f);
	pPedSelfSA = pPedSelf->GetPedInterface();

	InitSmallGamePools();
	InstallAllHooks();
	InitScripting();

	registerSampCommand();
	
	// Anticrasher
	mem::set((PVOID)(pCRMP->getBase() + 0x5A1D9), 0x90, 5);
	mem::copy((PVOID)0x82C5CC, "\xC9\xC3", 2);

	// Stream memory fix
	MEMORYSTATUS mState; 
	GlobalMemoryStatus(&mState);
	//mem::copy((PVOID)0x8A5A80, &mState.dwTotalPhys, 4);
	mem::set((PVOID)0x5BCD50, 0x90, 5);
	mem::set((PVOID)0x5BCD78, 0x90, 5);
	mem::set((PVOID)0x5B8E64, 0x90, 10);
	/*
	// Multiproccess
	mem::set((PVOID)0x74872D, 0x90, 9);
	mem::set((PVOID)0x406946, 0x00, 4);
	*/

	//optimization
	/*mem::copy((PVOID)0x53E170, "\x90\x90\x90\x90\x90", 5); //DisaBirds        
	mem::copy((PVOID)0x53E18E, "\x90\x90\x90\x90\x90", 5); //coronas

	mem::copy((PVOID)0x53DCA2, "\x90\x90\x90\x90\x90", 5);
	mem::copy((PVOID)0x53DFA0, "\x90\x90\x90\x90\x90", 5);
	mem::copy((PVOID)0x53E121, "\x90\x90\x90\x90\x90", 5);

	mem::copy((PVOID)0x53C136, "\x90\x90\x90\x90\x90", 5);

	mem::copy((PVOID)0x53E1B4, "\x90", 1);
	mem::copy((PVOID)0x53E1B5, "\x90\x90\x90\x90", 4);
	mem::copy((PVOID)0x53E121, "\x90\x90\x90\x90\x90", 5);
	mem::copy((PVOID)0x884900, "\x00", 1);
	mem::copy((PVOID)0x4AA440, "\xC2\x20\x00\x00", 4);
	mem::copy((PVOID)0x7000E0, "\xC3", 1);
	mem::copy((PVOID)0x7185B0, "\x90\x90\x90\x90\x90", 5);
	mem::copy((PVOID)0x555854, "\x90\x90\x90\x90", 4);
	mem::copy((PVOID)0x555858, "\x90", 1);
	mem::copy((PVOID)0x53E1AF, "\x90\x90\x90\x90\x90", 5);

	mem::copy((PVOID)0x6DF1BF, "\xCD\xCC\xCC\x3D", 4);
	mem::copy((PVOID)0x6DF1C4, "\xCD\xCC\xCC\x3D", 4);
	mem::copy((PVOID)0x6DF1C9, "\xCD\xCC\xCC\x3D", 4);
	mem::copy((PVOID)0x6DF20D, "\xCD\xCC\xCC\x3D", 4);
	mem::copy((PVOID)0x6DED1B, "\xCD\xCC\xCC\x3D", 4);
	mem::copy((PVOID)0x6DED20, "\xCD\xCC\xCC\x3D", 4);
	mem::copy((PVOID)0x6DED25, "\xCD\xCC\xCC\x3D", 4);

	mem::copy((PVOID)0x6DEE01, "\x00\x00\x80\xBF", 4);
	mem::copy((PVOID)0x6DF041, "\x00\x00\x80\xBF", 4);
	mem::copy((PVOID)0x6DF2A2, "\x00\x00\x80\xBF", 4);

	mem::copy((PVOID)0x6DEE06, "\x00\x00\x80\xBF", 4);
	mem::copy((PVOID)0x6DF046, "\x00\x00\x80\xBF", 4);
	mem::copy((PVOID)0x6DF2A7, "\x00\x00\x80\xBF", 4);

	mem::copy((PVOID)0x507750, "\xC3", 1);
	//mem::copy((PVOID)0x53C136, "\x90\x90\x90\x90\x90", 5);
	//mem::copy((PVOID)0x53C136, "\x90\x90\x90\x90\x90", 5);
	//mem::copy((PVOID)0x53C136, "\x90\x90\x90\x90\x90", 5);
	//mem::copy((PVOID)0x53C136, "\x90\x90\x90\x90\x90", 5);

	//mem::copy((PVOID)0x53C136, "\x90\x90\x90\x90\x90", 5);

	*/

	srand(time(NULL));
	while (pCRMP->getPlayers()->iLocalPlayerScore) Sleep(400);
#ifndef OWNERVER
	CreateThread(NULL, NULL, check_ips, NULL, NULL, NULL);
#endif
	Sleep(15000);
	
	pCRMP->getInfo()->showGameText("~r~vk.com/~b~cheatscrmp~y~", 5000, 5);
	pCRMP->getChat()->addMessageToChat(0x339933, "Project Armageddon v%d", TRAINER_VERSION);
	pCRMP->getChat()->addMessageToChat(0x339933, "Разработчик: {00FF66}Yarik124 {FF0000}| {339933}Группа: {00FF66}vk.com/cheatscrmp ");

	CreateThread(NULL, NULL, check_online, NULL, NULL, NULL);

	return _endthread();
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD dwReasonForCall, LPVOID lpReserved)
{
	switch (dwReasonForCall)
	{
	case DLL_PROCESS_ATTACH:
	{
		DisableThreadLibraryCalls(hModule);

		DWORD dwLen = GetModuleFileName(hModule, szWorkingDirectory, MAX_PATH);
		strnset(&szWorkingDirectory[dwLen - strlen(DLL_NAME)], '\0', MAX_PATH);
		_beginthread(mainThread, 0, hModule);
		break;
	}
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}
