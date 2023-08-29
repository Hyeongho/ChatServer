#pragma once

#include "ChatInfo.h"

class CClient
{
public:
	CClient();
	~CClient();

private:
	char dummy[SIZE_DAT];
	char msg[SIZE_DAT];

private:
	static HINSTANCE m_hInst;
	
	static HWND m_hBtnSendMsg;
	static HWND m_hEditStatus;

	static char m_ipaddr[64];
	static int m_port;

	static std::thread m_ClientThread;
	static volatile bool m_ComStarted;
	static SOCKET m_sock;
	
	static std::mutex m_mtx;
	static std::condition_variable m_ReadEvent;
	static std::condition_variable m_WriteEvent;

public:
	bool Init(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow);
	void Run();

private:
	INT_PTR CALLBACK DlgProc(HWND, UINT, WPARAM, LPARAM);
	LRESULT CALLBACK ChildWndProc(HWND, UINT, WPARAM, LPARAM);

	DWORD WINAPI ClientMain(LPVOID arg);
	DWORD WINAPI ReadThread(LPVOID arg);
	DWORD WINAPI WriteThread(LPVOID arg);
};

