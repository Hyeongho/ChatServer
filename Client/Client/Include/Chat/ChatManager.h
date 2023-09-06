#pragma once

#include "../ClientInfo.h"

class CChatManager
{
private:
	class CChatWindow* m_ChatWindow;

private:
	WSADATA m_wsa;
	SOCKET m_sock; 

	struct sockaddr_in m_serveraddr;
	int clntAddrSize;

	int m_retval;

	bool m_IsConnect;

public:
	void SetSocket(SOCKET sock)
	{
		m_sock = sock;
	}

	SOCKET GetScoket() const
	{
		return m_sock;
	}

	void SetSockaddr(sockaddr_in serveraddr)
	{
		m_serveraddr = serveraddr;
	}

	struct sockaddr_in GetSockaddr() const
	{
		return m_serveraddr;
	}

	void SetConnect(bool Connect)
	{
		m_IsConnect = Connect;
	}

	bool GetConnet() const
	{
		return m_IsConnect;
	}

public:
	void Connect(std::string IP, std::string Port);

public:
	// 소켓 함수 오류 출력 후 종료
	void err_quit(const char* msg)
	{
		LPVOID lpMsgBuf;
		FormatMessageA(
			FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
			NULL, WSAGetLastError(),
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			(char*)&lpMsgBuf, 0, NULL);
		MessageBoxA(NULL, (const char*)lpMsgBuf, msg, MB_ICONERROR);
		LocalFree(lpMsgBuf);
		exit(1);
	}

	// 소켓 함수 오류 출력
	void err_display(const char* msg)
	{
		LPVOID lpMsgBuf;
		FormatMessageA(
			FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
			NULL, WSAGetLastError(),
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			(char*)&lpMsgBuf, 0, NULL);
		printf("[%s] %s\n", msg, (char*)lpMsgBuf);
		LocalFree(lpMsgBuf);
	}

	// 소켓 함수 오류 출력
	void err_display(int errcode)
	{
		LPVOID lpMsgBuf;
		FormatMessageA(
			FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
			NULL, errcode,
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			(char*)&lpMsgBuf, 0, NULL);
		printf("[오류] %s\n", (char*)lpMsgBuf);
		LocalFree(lpMsgBuf);
	}

public:
	bool Init(HINSTANCE hInst);
	void CreateDefaultSceneMode();
	int Run();

	DECLARE_SINGLE(CChatManager)
};

