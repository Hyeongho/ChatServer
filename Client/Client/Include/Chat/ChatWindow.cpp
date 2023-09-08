#include "ChatWindow.h"
#include "ChatManager.h"
#include "../IMGUIButton.h"
#include "../IMGUISameLine.h"
#include "../IMGUILabel.h"
#include "../IMGUIText.h"
#include "../IMGUITextInput.h"
#include "../IMGUIListBox.h"
#include "../IMGUIComboBox.h"
#include "../IMGUIMenuBar.h"
#include "../IMGUISeparator.h"
#include "../IMGUIModal.h"

bool CChatWindow::m_Stop = false;

CChatWindow::CChatWindow() : m_Press(false)
{
}

CChatWindow::~CChatWindow()
{
	
}

bool CChatWindow::Init()
{
	CIMGUIWindow::Init();

	//AddWindowFlag(ImGuiWindowFlags_MenuBar);

	// Input IP
	CIMGUILabel* Label = AddWidget<CIMGUILabel>("IP", 100.f, 30.f);
	Label->SetColor(0, 0, 0);
	Label->SetAlign(0.5f, 0.f);

	CIMGUISameLine* Line = AddWidget<CIMGUISameLine>("Line");

	m_InputIP = AddWidget<CIMGUITextInput>("InputIP", 100.f, 100.f);
	m_InputIP->SetHideName(true);
	m_InputIP->AddFlag(ImGuiInputTextFlags_::ImGuiInputTextFlags_None);
	m_InputIP->SetTextType(ImGuiText_Type::String);

	m_InputIP->SetText("127.0.0.1");

	Line = AddWidget<CIMGUISameLine>("Line");

	Label = AddWidget<CIMGUILabel>("Port", 100.f, 30.f);
	Label->SetColor(0, 0, 0);
	Label->SetAlign(0.5f, 0.f);

	Line = AddWidget<CIMGUISameLine>("Line");

	m_InputPort = AddWidget<CIMGUITextInput>("InputPort", 100.f, 100.f);
	m_InputPort->SetHideName(true);
	m_InputPort->AddFlag(ImGuiInputTextFlags_::ImGuiInputTextFlags_None);
	m_InputPort->SetTextType(ImGuiText_Type::String);

	m_InputPort->SetText("9000");

	Line = AddWidget<CIMGUISameLine>("Line");

	m_ConnectButton = AddWidget<CIMGUIButton>("Connect", 100.f, 30.f);
	m_ConnectButton->SetClickCallback(this, &CChatWindow::ConnectCallback);

	CIMGUISeparator* Separator = AddWidget<CIMGUISeparator>("Separator");

	// Input Message
	m_InputMessage = AddWidget<CIMGUITextInput>("Message", 425.f, 100.f);
	m_InputMessage->SetHideName(true);
	m_InputMessage->AddFlag(ImGuiInputTextFlags_::ImGuiInputTextFlags_None);
	m_InputMessage->SetTextType(ImGuiText_Type::String);

	Line = AddWidget<CIMGUISameLine>("Line");

	m_SendButton = AddWidget<CIMGUIButton>("Send", 100.f, 30.f);
	m_SendButton->SetClickCallback(this, &CChatWindow::SendCallback);

	m_List = AddWidget<CIMGUIListBox>("Chat", 450, 450);
	m_List->SetHideName(true);
	m_List->SetPageItemCount(15);

	return true;
}

void CChatWindow::Update(float DeltaTime)
{
	CIMGUIWindow::Update(DeltaTime);
}

void CChatWindow::ConnectCallback()
{
	if (CChatManager::GetInst()->GetConnet())
	{
		return;
	}

	std::string IP = m_InputIP->GetTextMultibyte();
	std::string Port = m_InputPort->GetTextMultibyte();

	CChatManager::GetInst()->Connect(IP, Port);

	m_SendThread = std::thread(&CChatWindow::Send, this);
	m_RecvThread = std::thread(&CChatWindow::Recv, this);
}

void CChatWindow::SendCallback()
{
	m_Press = true;

	m_cv.notify_all();
}

void CChatWindow::StopThreads()
{
	m_Stop = true;

	if (m_SendThread.joinable())
	{
		if (m_SendThread.joinable())
		{
			m_SendThread.join();
		}
	}

	if (m_RecvThread.joinable())
	{
		if (m_RecvThread.joinable())
		{
			m_RecvThread.join();
		}
	}
}

void CChatWindow::Send()
{
	while (m_Stop == false)
	{
		if (m_Press)
		{
			m_mutex.lock();

			int retval = CChatManager::GetInst()->GetRetval();

			std::string msg = m_InputMessage->GetTextMultibyte();

			SOCKET sock = CChatManager::GetInst()->GetScoket();

			int len = msg.size();

			strncpy(m_InputBuf, msg.c_str(), len);

			retval = send(sock, m_InputBuf, BUFSIZE, 0);

			m_InputMessage->SetText("");

			m_Press = false;

			memset(m_InputBuf, NULL, BUFSIZE);

			m_mutex.unlock();
		}
	}
}

void CChatWindow::Recv()
{
	while (m_Stop == false)
	{
		if (CChatManager::GetInst()->GetConnet())
		{
			SOCKET sock = CChatManager::GetInst()->GetScoket();

			char RecvMsg[BUFSIZE];

			int len = recv(sock, RecvMsg, BUFSIZE - 1, 0);

			if (len == SOCKET_ERROR)
			{
				continue;
			}

			m_mutex.lock();

			if (len >= 0)
			{
				RecvMsg[len] = '\0';
			}

			if (RecvMsg[0] != '\0')
			{
				m_List->AddItem(RecvMsg);
			}

			m_mutex.unlock();
		}
	}
}
