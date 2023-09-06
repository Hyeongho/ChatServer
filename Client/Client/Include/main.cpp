#include "Chat/ChatManager.h"

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	if (!CChatManager::GetInst()->Init(hInstance))
	{
		CChatManager::DestroyInst();
		return 0;
	}

	CChatManager::GetInst()->CreateDefaultSceneMode();

	int Ret = CChatManager::GetInst()->Run();

	CChatManager::DestroyInst();

	return Ret;
}
