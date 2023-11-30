#include <WinSock2.h>	// socket lib  , ws2_32.lib
#include <cstring>
#include <iostream>
#include <thread>

using namespace std;

int server_init();
void proc_recvs();

#define PORT 9999
#define PACKET_SIZE 1024
SOCKET server_socket, client_socket;

int main()
{
	server_socket = server_init();
	if (server_socket < 0) {
		cout << "server init error" << endl;
		return 0;
	}

	cout << "server init" << endl;

	// 클라이언트 RECV
	SOCKADDR_IN tClntAddr = {};
	int iClntSize = sizeof(tClntAddr);
	client_socket = accept(server_socket, (SOCKADDR*)&tClntAddr, &iClntSize);		// accept(소켓, 소켓 구성요소 구조체의 주소, 크기를 담고 있는 변수의 주소)

	thread cproc(proc_recvs);

	char cMsg[PACKET_SIZE] = {};

	while (!WSAGetLastError())
	{
		cout << "입력 메시지 : " << endl;
		cin >> cMsg;

		send(client_socket, cMsg, strlen(cMsg), 0);
	}

	cproc.join();
	closesocket(client_socket);
	closesocket(server_socket);
	WSACleanup();

	return 0;
}

int server_init()
{
	WSADATA wsaData;		// WSADATA :: Windows 소켓 구조체
	SOCKET s;
	SOCKADDR_IN socktAddr;

	if(WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)		// 소켓버전, WSADATA 구조체 주소
	{
		cout << "WSAStartup Error" << endl;
		return -1;
	}

//	if ((s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
	if ((s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
		cout << "socket Error" << endl;
		return -1;
	}
	memset(&socktAddr, 0, sizeof(socktAddr));
	socktAddr.sin_family = AF_INET;			//  #define AF_INET 2 // internetwork: UDP, TCP, etc.
	socktAddr.sin_port = htons(PORT);
	socktAddr.sin_addr.s_addr = htonl(INADDR_ANY);

	// bind(소켓, 소켓 구성요소 구조체주소, 구조체크기)
	if (bind(s, (SOCKADDR*)&socktAddr, sizeof(socktAddr)) < 0) {
		cout << "bind Error" <<  endl;
		return -2;
	}
	// 소켓 리슨 상태
	if (listen(s, SOMAXCONN) < 0) {
		cout << "listen Error" << endl;
		return -3;
	}

	return s;
}

void proc_recvs() 
{
	char cbuffer[PACKET_SIZE] = {};
	
	while (!WSAGetLastError())
	{
		memset(&cbuffer, 0, sizeof(cbuffer));			// 클라리언트측 정보를 수신하기 위한 버퍼
		recv(client_socket, cbuffer, PACKET_SIZE, 0);	// recv(소켓, 수신 정보를 담을 배열주소, 배열의 크기, flag)
		cout << "상대메시지 : " << cbuffer << endl;
	}

}