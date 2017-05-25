#include <windows.h>
#include <Wtsapi32.h>
#include <string>
#include <iostream>
#pragma  comment(lib,"Wtsapi32.lib")
#include  "SessionInspector.h"

const LPCSTR TERMINAL_SERVER_KEY = "SYSTEM\\CurrentControlSet\\Control\\Terminal Server\\";
const LPCSTR GLASS_SESSION_ID = "GlassSessionId";

bool SessionInstector::IsCurrentSessionRemoteable()
{
	bool fIsRemoteable = false;

	if (GetSystemMetrics(SM_REMOTESESSION)) {
		fIsRemoteable = true;
	}
	else {
		HKEY hRegKey = NULL;
		LONG lResult;

		lResult = RegOpenKeyEx(HKEY_LOCAL_MACHINE, TERMINAL_SERVER_KEY,
			0, // ulOptions
			KEY_READ, &hRegKey);

		if (lResult == ERROR_SUCCESS) {
			DWORD dwGlassSessionId = 0;
			DWORD cbGlassSessionId = sizeof(dwGlassSessionId);
			DWORD dwType = 0;

			lResult = RegQueryValueEx(hRegKey, GLASS_SESSION_ID,
				NULL, // lpReserved
				&dwType, (BYTE *)&dwGlassSessionId,
				&cbGlassSessionId);

			if (lResult == ERROR_SUCCESS) {
				DWORD dwCurrentSessionId = 0;

				if (ProcessIdToSessionId(GetCurrentProcessId(), &dwCurrentSessionId)) {
					fIsRemoteable = (dwCurrentSessionId != dwGlassSessionId);
				}
			}
		}

		if (hRegKey) {
			RegCloseKey(hRegKey);
		}
	}

	return fIsRemoteable;
}

bool SessionInstector::DetectingSessionEvents()
{
	MSG msg;
	BOOL bRet;
	HWND dummyHWND = ::CreateWindowA("STATIC", "dummy", WS_MINIMIZE, 0, 0, 0, 0, NULL, NULL, NULL, NULL);
	::SetWindowTextA(dummyHWND, "Dummy Window!");

	::WTSRegisterSessionNotification(dummyHWND, NOTIFY_FOR_ALL_SESSIONS);

	while ((bRet = GetMessage(&msg, NULL, 0, 0)) != 0)
	{
		if (bRet == -1)
		{
			return false;
			// handle the error and possibly exit
		}
		else
		{
			if (msg.message == WM_WTSSESSION_CHANGE)
			{
				//int a = 2;
				std::cout << "Event happened";
			}

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	return true;
}

SessionInstector::SessionInstector()
{

}
