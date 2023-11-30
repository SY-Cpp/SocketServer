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

	// Ŭ���̾�Ʈ RECV
	SOCKADDR_IN tClntAddr = {};
	int iClntSize = sizeof(tClntAddr);
	client_socket = accept(server_socket, (SOCKADDR*)&tClntAddr, &iClntSize);		// accept(����, ���� ������� ����ü�� �ּ�, ũ�⸦ ��� �ִ� ������ �ּ�)

	thread cproc(proc_recvs);

	char cMsg[PACKET_SIZE] = {};

	while (!WSAGetLastError())
	{
		cout << "�Է� �޽��� : " << endl;
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
	WSADATA wsaData;		// WSADATA :: Windows ���� ����ü
	SOCKET s;
	SOCKADDR_IN socktAddr;

	if(WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)		// ���Ϲ���, WSADATA ����ü �ּ�
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

	// bind(����, ���� ������� ����ü�ּ�, ����üũ��)
	if (bind(s, (SOCKADDR*)&socktAddr, sizeof(socktAddr)) < 0) {
		cout << "bind Error" <<  endl;
		return -2;
	}
	// ���� ���� ����
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
		memset(&cbuffer, 0, sizeof(cbuffer));			// Ŭ�󸮾�Ʈ�� ������ �����ϱ� ���� ����
		recv(client_socket, cbuffer, PACKET_SIZE, 0);	// recv(����, ���� ������ ���� �迭�ּ�, �迭�� ũ��, flag)
		cout << "���޽��� : " << cbuffer << endl;
	}

}