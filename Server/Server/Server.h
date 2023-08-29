#pragma once

#include "../../Common.h"

#include <vector>
#include <algorithm>
#include <thread>
#include <mutex>

#define SERVERPORT 9000
#define BUFSIZE 256

typedef struct _SOCKETINFO
{
	SOCKET sock;
	bool isIPv6;
	bool isUDP;
	char buf[BUFSIZE + 1];
	int recvbytes;
} SOCKETINFO;

class CServer
{
public:
	CServer();
	~CServer();

private:
	int m_TotalSocket;
	std::vector<std::shared_ptr<SOCKETINFO>> m_vSocketInfo;

private:
	int m_retval;
	WSADATA m_wsa;
	SOCKET m_listen_sock4;
	struct sockaddr_in m_serveraddr;

	SOCKET m_listen_sock6;
	struct sockaddr_in6 m_serveraddr6;

	// 데이터 통신에 사용할 변수
	fd_set m_rset;
	SOCKET m_client_sock;
	int m_addrlen;

	struct sockaddr_in m_clientaddr4;
	struct sockaddr_in6 m_clientaddr6;

public:
	bool AddSocketInfo(SOCKET sock, bool isIPv6, bool isUDP);
	void RemoveSocketInfo(int index);

public:
	bool Start();
	void Run(int argc, char* argv[]);

	void err_quit(const char* msg);
	void err_display(const char* msg);
	void err_display(int errcode);
	
};