#include "Server.h"

CServer::CServer()
{
}

CServer::~CServer()
{
}

bool CServer::AddSocketInfo(SOCKET sock, bool isIPv6, bool isUDP, std::string Port)
{
	if (m_TotalSocket >= FD_SETSIZE) 
	{
		printf("[오류] 소켓 정보를 추가할 수 없습니다!\n");
		return false;
	}

	std::shared_ptr<SOCKETINFO> ptr = std::make_shared<SOCKETINFO>();

	if (ptr == NULL) 
	{
		printf("[오류] 메모리가 부족합니다!\n");
		return false;
	}

	ptr->sock = sock;
	ptr->isIPv6 = isIPv6;
	ptr->isUDP = isUDP;
	ptr->recvbytes = 0;
	ptr->Port = Port;
	m_vSocketInfo.push_back(ptr);

	m_TotalSocket++;

	return true;
}

void CServer::RemoveSocketInfo(int index)
{
	SOCKETINFO* ptr = m_vSocketInfo[index].get();

	if (!ptr->isIPv6)
	{
		m_addrlen = sizeof(m_clientaddr4);

		getpeername(ptr->sock, (struct sockaddr*)&m_clientaddr4, &m_addrlen);

		char addr[INET_ADDRSTRLEN];

		inet_ntop(AF_INET, &m_clientaddr4.sin_addr, addr, sizeof(addr));

		printf("[TCP/IPv4 서버] 클라이언트 종료: IP 주소=%s, 포트 번호=%d\n", addr, ntohs(m_clientaddr4.sin_port));
	}

	// 소켓 닫기
	closesocket(ptr->sock);

	m_vSocketInfo.erase(m_vSocketInfo.begin() + index);

	m_TotalSocket--;
}

bool CServer::Start()
{
	if (WSAStartup(MAKEWORD(2, 2), &m_wsa) != 0)
	{
		return false;
	}

	m_listen_sock4 = socket(AF_INET, SOCK_STREAM, 0);

	if (m_listen_sock4 == INVALID_SOCKET)
	{
		err_quit("socket()");

		return false;
	}

	memset(&m_serveraddr, 0, sizeof(m_serveraddr));

	m_serveraddr.sin_family = AF_INET;
	m_serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	m_serveraddr.sin_port = htons(SERVERPORT);

	m_retval = bind(m_listen_sock4, (struct sockaddr*)&m_serveraddr, sizeof(m_serveraddr));

	if (m_retval == SOCKET_ERROR)
	{
		err_quit("bind()");
	}

	m_retval = listen(m_listen_sock4, SOMAXCONN);

	if (m_retval == SOCKET_ERROR)
	{
		err_quit("listen()");

		return false;
	}

	u_long mode = 1;
	m_retval = ioctlsocket(m_listen_sock4, FIONBIO, &mode);

	m_listen_sock6 = socket(AF_INET6, SOCK_STREAM, 0);

	if (m_listen_sock6 == INVALID_SOCKET)
	{
		err_quit("socket()");

		return false;
	}

	memset(&m_serveraddr6, 0, sizeof(m_serveraddr6));
	m_serveraddr6.sin6_family = AF_INET6;
	m_serveraddr6.sin6_addr = in6addr_any;
	m_serveraddr6.sin6_port = htons(SERVERPORT);

	m_retval = bind(m_listen_sock6, (struct sockaddr*)&m_serveraddr6, sizeof(m_serveraddr6));

	if (m_retval == SOCKET_ERROR)
	{
		err_quit("bind()");
	}

	m_retval = listen(m_listen_sock6, SOMAXCONN);

	if (m_retval == SOCKET_ERROR)
	{
		err_quit("listen()");
	}

	return true;
}

void CServer::Run(int argc, char* argv[])
{
	while (1)
	{
		FD_ZERO(&m_rset);
		FD_SET(m_listen_sock4, &m_rset);
		FD_SET(m_listen_sock6, &m_rset);

		for (int i = 0; i < m_TotalSocket; i++)
		{
			FD_SET(m_vSocketInfo[i]->sock, &m_rset);
		}

		m_retval = select(0, &m_rset, NULL, NULL, NULL);

		if (m_retval == SOCKET_ERROR)
		{
			err_quit("select()");
		}

		if (FD_ISSET(m_listen_sock4, &m_rset))
		{
			m_addrlen = sizeof(m_clientaddr4);

			m_client_sock = accept(m_listen_sock4, (struct sockaddr*)&m_clientaddr4, &m_addrlen);

			if (m_client_sock == INVALID_SOCKET)
			{
				err_display("accept()");

				break;
			}

			else
			{
				char addr[INET_ADDRSTRLEN];

				inet_ntop(AF_INET, &m_clientaddr4.sin_addr, addr, sizeof(addr));

				printf("\n[TCP/IPv4 서버] 클라이언트 접속: IP 주소=%s, 포트 번호=%d\n", addr, ntohs(m_clientaddr4.sin_port));

				std::string port = std::to_string(ntohs(m_clientaddr4.sin_port));

				if (!AddSocketInfo(m_client_sock, false, false, port))
				{
					closesocket(m_client_sock);
				}

				m_retval = send(m_client_sock, port.c_str(), port.size(), 0);
			}
		}

		for (int i = 0; i < m_TotalSocket; i++)
		{
			SOCKETINFO* ptr = m_vSocketInfo[i].get();

			if (FD_ISSET(ptr->sock, &m_rset))
			{
				m_retval = recv(ptr->sock, ptr->buf + ptr->recvbytes, BUFSIZE - ptr->recvbytes, 0);

				if (m_retval == 0 || m_retval == SOCKET_ERROR)
				{
					RemoveSocketInfo(i);

					continue;
				}

				printf("\n포트 번호=%s: %s\n", ptr->Port.c_str(), ptr->buf);

				ptr->recvbytes += m_retval;

				if (ptr->recvbytes == BUFSIZE)
				{
					ptr->recvbytes = 0;

					for (int j = 0; j < m_TotalSocket; j++)
					{
						SOCKETINFO* ptr2 = m_vSocketInfo[j].get();

						std::string Recv = "[" + ptr->Port + "]" + ptr->buf;

						m_retval = send(ptr2->sock, Recv.c_str(), BUFSIZE, 0);

						if (m_retval == SOCKET_ERROR)
						{
							err_display("send()");

							RemoveSocketInfo(j);
							j--;

							continue;
						}
					}
				}

				memset(ptr->buf, NULL, BUFSIZE);
			}
		}
	}

	closesocket(m_listen_sock4);

	WSACleanup();
}

void CServer::err_quit(const char* msg)
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

void CServer::err_display(const char* msg)
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

void CServer::err_display(int errcode)
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
