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
	char m_InputBuf[BUFSIZE];
	std::vector<std::string> m_SendMsg;
	std::vector<std::string> m_RecvMsg;

	SOCKET scok;
	sockaddr_in serverAddr;

	std::mutex m_mutex;
	std::thread  m_SendThread;
	std::thread  m_RecvThread;

	std::condition_variable m_cv;
	
	bool m_Press;
	static bool m_Stop;

public:
	virtual bool Init();
	virtual void Update(float DeltaTime);

private:
	void ConnectCallback();
	void SendCallback();

public:
	void StopThreads();

private:
	void Send();
	void Recv();
};

