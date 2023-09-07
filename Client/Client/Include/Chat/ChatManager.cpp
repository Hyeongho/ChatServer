#include "ChatManager.h"
#include "../Engine.h"
#include "../resource.h"
#include "../IMGUIManager.h"
#include "ChatWindow.h"


DEFINITION_SINGLE(CChatManager)

CChatManager::CChatManager()
{
}

CChatManager::~CChatManager()
{
	m_ChatWindow->StopThreads();

	CEngine::DestroyInst();
}

void CChatManager::Connect(std::string IP, std::string Port)
{
	m_sock = socket(AF_INET, SOCK_STREAM, 0);

	if (m_sock == INVALID_SOCKET)
	{
		err_quit("socket()");

		return;
	}

	// connect()
	memset(&m_serveraddr, 0, sizeof(m_serveraddr));
	m_serveraddr.sin_family = AF_INET;
	inet_pton(AF_INET, IP.c_str(), &m_serveraddr.sin_addr);
	m_serveraddr.sin_port = htons(SERVERPORT);
	m_retval = connect(m_sock, (struct sockaddr*)&m_serveraddr, sizeof(m_serveraddr));

	if (m_retval == SOCKET_ERROR)
	{
		err_quit("connect()");

		return;
	}

	char PortNum[BUFSIZE];

	int len = recv(m_sock, PortNum, BUFSIZE - 1, 0);

	if (len >= 0)
	{
		PortNum[len] = '\0';
	}

	m_ChatWindow->SetName(PortNum);

	u_long mode = 1;
	m_retval = ioctlsocket(m_sock, FIONBIO, &mode);

	if (m_retval == SOCKET_ERROR)
	{
		err_quit("ioctlsocket()");
	}

	m_IsConnect = true;
}

bool CChatManager::Init(HINSTANCE hInst)
{
	CEngine::GetInst()->SetPlay(false);

	if (!CEngine::GetInst()->Init(hInst, TEXT("Client"), 600, 600, IDI_ICON1))
	{
		CEngine::DestroyInst();
		return false;
	}

	m_ChatWindow = CIMGUIManager::GetInst()->AddWindow<CChatWindow>("ChatWindow");

	if (WSAStartup(MAKEWORD(2, 2), &m_wsa) != 0)
	{
		return false;
	}

	return true;
}

void CChatManager::CreateDefaultSceneMode()
{
}

int CChatManager::Run()
{
	return CEngine::GetInst()->Run();
}
