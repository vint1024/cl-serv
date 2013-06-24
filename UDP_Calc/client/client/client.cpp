// client.cpp: ���������� ����� ����� ��� ����������� ����������.
//
#include "stdafx.h"
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
 
// � ������ �������, � ������� Linker, � ������ Additional Dependancies ������� Ws2_32.lib
using namespace std;
int main(int argc, char **argv) 
{
	WSADATA wsaData;
	SOCKET SendRecvSocket;  // ����� ��� ������ � ��������
	sockaddr_in ServerAddr;  // ��� ����� ����� ������� � ��������
	int err, maxlen = 512;  // ��� ������, ������ ������� � ������ ��������� ������
	char* recvbuf=new char[maxlen];  // ����� ������
	char* query=new char[maxlen];  // ����� ��������
 
 
	// Initialize Winsock
	WSAStartup(MAKEWORD(2,2), &wsaData);
 
	// Create a SOCKET for connecting to server
	SendRecvSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
 
	ServerAddr.sin_family=AF_INET;
	if (argc > 1)
	{
		ServerAddr.sin_addr.s_addr = inet_addr( argv[1] );
		cout << "Connecting to "<< argv[1]<< ":";
	}
	else
	{
		ServerAddr.sin_addr.s_addr = inet_addr( "127.0.0.1" );
		cout << "Connecting to "<< "127.0.0.1:";
	}
	if (argc > 2)
	{
		ServerAddr.sin_port=htons( atoi(argv[2]) );
		cout << argv[2] << endl;
	}
	else
	{
		ServerAddr.sin_port=htons(12345);
		cout << 12345 << endl;
	}

	while (1)
	{
		cout << "--> ";
		cin.getline(query,1024);
		if (strcmp(query, "EXIT")==0)
			break;
		// ���������� ������ �� ������
		sendto(SendRecvSocket,query, strlen(query), 0, (sockaddr *)&ServerAddr,sizeof(ServerAddr));  
		printf("Sent: %s\n", query);
		// �������� ���������
		err = recvfrom(SendRecvSocket,recvbuf,maxlen,0,0,0);
		if (err > 0) {
			recvbuf[err]=0;
			printf("Result: %s\n", (char* )recvbuf);
		}
		else {
			printf("recv failed: %d\n", WSAGetLastError());
			closesocket(SendRecvSocket);
			WSACleanup();
			system("pause");
			return 1;
		}
	}
	closesocket(SendRecvSocket);
	return 0;
}

