#include <WinSock2.h>
#include <WS2tcpip.h>
#include <iostream>
#include <conio.h>
#include <chrono>
#include <iomanip>
#include <ctime>
#include <windows.h>


using namespace std;

#pragma comment(lib, "winmm.lib") 
#pragma comment( lib, "ws2_32.lib")

char buffer[512] = {};
char recvBuf[512] = {};
const char* pServerIP = "192.168.219.173";
const int port = 30001;
SOCKET					hClientSocket;
SOCKADDR_IN				servAddr;
int type, packetSize;
int period;

void RunTimerChrono();
void RunTimerClock();
void RunTimerTimeGetTime(); 

int main()
{
	WSADATA  wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) return 0;
	srand(time(nullptr));

	hClientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	memset(&servAddr, 0, sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	inet_pton(AF_INET, pServerIP, &servAddr.sin_addr);
	servAddr.sin_port = htons(port);

	if (connect(hClientSocket, (SOCKADDR*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR)
	{
		int error = WSAGetLastError();
		printf("connect �����ڵ� : %d\n", error);
		return false;
	}
	
	printf("��ŶŸ��(��ȣ) : ");
	cin >> type;
	*((unsigned short*)&buffer[2])= (unsigned short)type;
	cout << "��Ŷũ��(�ּ� 4) : ";
	cin >> packetSize;
	for (int i = 4; i < packetSize; ++i)
	{
		buffer[i] = rand() % 255 + 1;
	}
	*((unsigned short*)buffer) = (unsigned short)packetSize;

	cout << "���� �ֱ�(�и��ʴ���) : ";
	cin >> period;
	cout << "�ƹ� Ű�� �Է��ϸ� �����\n";

	RunTimerChrono();
	//RunTimerClock();
	//RunTimerTimeGetTime();

	if (hClientSocket != INVALID_SOCKET) closesocket(hClientSocket);

	return 0;
}

void RunTimerChrono()
{
	std::cout << std::fixed << std::setprecision(3);
	int cnt = 0;
	double totalMilliseconds = 0;
	while (1)
	{
		if (_kbhit()) break;

		Sleep(period);
		send(hClientSocket, buffer, packetSize, 0);
		auto start = std::chrono::high_resolution_clock::now();

		if (recv(hClientSocket, recvBuf, 512, 0) == SOCKET_ERROR) break;
		auto end = std::chrono::high_resolution_clock::now();
		double milliseconds = std::chrono::duration<double, std::milli>(end - start).count();
		totalMilliseconds += milliseconds;
		++cnt;

		std::cout << "��� �ð�: " << milliseconds << "�и��� (��� " << totalMilliseconds / cnt << ")" << std::endl;
	}
}

void RunTimerClock()
{
	while (1)
	{
		if (_kbhit()) break;

		Sleep(period);
		send(hClientSocket, buffer, packetSize, 0); 
		clock_t start = clock();

		recv(hClientSocket, recvBuf, 512, 0);
		clock_t end = clock();
		double milliseconds = ((double)(end - start)) / CLOCKS_PER_SEC * 1000;

		std::cout << "��� �ð�: " << milliseconds << "ms" << std::endl;
	}
}

void RunTimerTimeGetTime()
{
	while (1)
	{
		if (_kbhit()) break;

		Sleep(period);
		send(hClientSocket, buffer, packetSize, 0);
		DWORD start = timeGetTime();

		recv(hClientSocket, recvBuf, 512, 0);
		DWORD end = timeGetTime();
		DWORD milliseconds = end - start;

		std::cout << "��� �ð�: " << milliseconds << "ms" << std::endl;
	}
}