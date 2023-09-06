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

CChatWindow::CChatWindow()
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
	m_InputIP->SetCallback(this, &CChatWindow::CallBackIP);

	Line = AddWidget<CIMGUISameLine>("Line");

	Label = AddWidget<CIMGUILabel>("Port", 100.f, 30.f);
	Label->SetColor(0, 0, 0);
	Label->SetAlign(0.5f, 0.f);

	Line = AddWidget<CIMGUISameLine>("Line");

	m_InputPort = AddWidget<CIMGUITextInput>("InputPort", 100.f, 100.f);
	m_InputPort->SetHideName(true);
	m_InputPort->AddFlag(ImGuiInputTextFlags_::ImGuiInputTextFlags_None);
	m_InputPort->SetTextType(ImGuiText_Type::String);
	m_InputPort->SetCallback(this, &CChatWindow::CallBackPort);

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
	std::string IP = m_InputIP->GetTextMultibyte();
	std::string Port = m_InputPort->GetTextMultibyte();

	CChatManager::GetInst()->Connect(IP, Port);
}

void CChatWindow::SendCallback()
{
	
}

void CChatWindow::CallBackIP()
{
}

void CChatWindow::CallBackPort()
{
}

void CChatWindow::Send()
{
}

void CChatWindow::Recv()
{
}
