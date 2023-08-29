#include "Client.h"

CClient::CClient()
{
}

CClient::~CClient()
{
}

bool CClient::Init(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	return false;
}

void CClient::Run()
{
}

INT_PTR CClient::DlgProc(HWND, UINT, WPARAM, LPARAM)
{
	return INT_PTR();
}

LRESULT CClient::ChildWndProc(HWND, UINT, WPARAM, LPARAM)
{
	return LRESULT();
}

DWORD __stdcall CClient::ClientMain(LPVOID arg)
{
	return 0;
}

DWORD __stdcall CClient::ReadThread(LPVOID arg)
{
	return 0;
}

DWORD __stdcall CClient::WriteThread(LPVOID arg)
{
	return 0;
}
