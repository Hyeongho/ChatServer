#pragma once

#define	DIRECTINPUT_VERSION	0x0800
#define _CRT_SECURE_NO_WARNINGS // 구형 C 함수 사용 시 경고 끄기
#define _WINSOCK_DEPRECATED_NO_WARNINGS // 구형 소켓 API 사용 시 경고 끄기

#define SERVERPORT 9000
#define BUFSIZE 256

#include <winsock2.h> // 윈속 2 메인 헤더
#include <ws2tcpip.h> // 윈속2 확장 헤더

#include <Windows.h>
#include <list>
#include <vector>
#include <unordered_map>
#include <crtdbg.h>
#include <functional>
#include <string>
#include <math.h>
#include <algorithm>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <dinput.h>
#include <dwrite.h>
#include <dwrite_3.h>
#include <d2d1.h>
#include <stack>
#include <process.h>
#include <locale.h>
#include <WinUser.h>
#include <array>
#include <thread>
#include <mutex>
#include <iostream>

#include "Vector2.h"

#include "imgui.h"
#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dwrite.lib")
#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "ws2_32") // ws2_32.lib 링크

#define	SAFE_DELETE(p) if(p) { delete p; p = nullptr; }
#define	SAFE_DELETE_ARRAY(p) if(p) { delete[] p; p = nullptr; }
#define	SAFE_RELEASE(p)	if(p) { p->Release(); p = nullptr; }

#define	DECLARE_SINGLE(Type) \
private:\
	static Type* m_Inst;\
public:\
	static Type* GetInst()\
	{\
		if(!m_Inst)\
			{\
				m_Inst = new Type;\
			}\
		return m_Inst;\
	}\
	static void DestroyInst()\
	{\
		SAFE_DELETE(m_Inst);\
	}\
private:\
	Type();\
	~Type();

#define	DEFINITION_SINGLE(Type)	Type* Type::m_Inst = nullptr;

#define	ROOT_PATH "Root"
#define	SHADER_PATH "Shader"
#define	TEXTURE_PATH "Texture"
#define	FONT_PATH "Font"
#define	ANIMATION_PATH "Animation"
#define	SCENE_PATH "Scene"
#define	SOUND_PATH "Sound"
#define DATA_PATH "Data"

#define	SAFE_DELETE(p) if(p) { delete p; p = nullptr; }
#define	SAFE_DELETE_ARRAY(p) if(p) { delete[] p; p = nullptr; }
#define	SAFE_RELEASE(p)	if(p) { p->Release(); p = nullptr; }

struct Resolution
{
	unsigned int Width;
	unsigned int Height;
};

enum class Text_Type
{
	Multibyte,
	UTF8
};
