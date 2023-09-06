#pragma once

#include "../IMGUIWindow.h"

class CChatWindow :
    public CIMGUIWindow
{
public:
	CChatWindow();
	virtual ~CChatWindow();

private:
	class CIMGUITextInput* m_InputIP;
	class CIMGUITextInput* m_InputPort;

	class CIMGUIButton* m_ConnectButton;

	class CIMGUITextInput* m_InputMessage;

	class CIMGUIButton* m_SendButton;

	class CIMGUIListBox* m_List;

private:
	char InputBuf[BUFSIZE];
	std::vector<std::string> m_SendMsg;
	std::vector<std::string> m_RecvMsg;

	SOCKET scok;
	sockaddr_in serverAddr;

	std::mutex m_mutex;

public:
	virtual bool Init();
	virtual void Update(float DeltaTime);

private:
	void ConnectCallback();
	void SendCallback();

	void CallBackIP();
	void CallBackPort();

private:
	void Send();
	void Recv();
};

