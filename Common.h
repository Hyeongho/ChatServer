#pragma once
#define _CRT_SECURE_NO_WARNINGS // ���� C �Լ� ��� �� ��� ����
#define _WINSOCK_DEPRECATED_NO_WARNINGS // ���� ���� API ��� �� ��� ����

#include <winsock2.h> // ���� 2 ���� ���
#include <ws2tcpip.h> // ����2 Ȯ�� ���

#include <Windows.h>
#include <iostream>
#include <string>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <thread>
#include <mutex>
#include <memory>

#pragma comment(lib, "ws2_32") // ws2_32.lib ��ũ
