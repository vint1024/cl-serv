// server.cpp: определяет точку входа для консольного приложения.
//
#include "stdafx.h"
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include "my_calc.h"
 
// В опциях проекта, в разделе Linker, в пункте Additional Dependancies укажите Ws2_32.lib
 
int main(int argc, char **argv) 
{
	WSADATA wsaData;
	SOCKET SendRecvSocket;  // сокет для приема и передачи
	sockaddr_in ServerAddr, ClientAddr;  // это будет адрес сервера и клиентов
	int err, maxlen = 512, ClientAddrSize=sizeof(ClientAddr);  // код ошибки, размер буферов и размер структуры адреса
	char* recvbuf=new char[maxlen];  // буфер приема
	char* result_string=new char[maxlen];  // буфер отправки
 
 
	// Initialize Winsock
	WSAStartup(MAKEWORD(2,2), &wsaData);
 
	// Create a SOCKET for connecting to server
	SendRecvSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
 
	// Setup the TCP listening socket
	ServerAddr.sin_family=AF_INET; //семейство адресов 
	if (argc > 1)
	{
		ServerAddr.sin_addr.s_addr = inet_addr( argv[1] );
		cout << "Address to connect "<< argv[1]<< ":";
	}
	else
	{
		ServerAddr.sin_addr.s_addr = inet_addr( "127.0.0.1" );
		cout << "Address to connect "<< "127.0.0.1:";
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
	
	err = bind( SendRecvSocket, (sockaddr *) &ServerAddr, sizeof(ServerAddr)); // связывание адреса с сокетом
	if (err == SOCKET_ERROR) {
		printf("bind failed: %d\n", WSAGetLastError());
		closesocket(SendRecvSocket);
		WSACleanup();
		return 1;
	}
 
	while (true) {
		printf( "Waiting for a client to connect...\n" );
		// Accept a client socket
		err = recvfrom(SendRecvSocket,recvbuf,maxlen,0,(sockaddr *)&ClientAddr,&ClientAddrSize);
		if (err > 0) {
			recvbuf[err]=0;
			printf("Received query: %s\n", (char* )recvbuf);
			string inS, FunctionS, ComandS;
			inS = (char* )recvbuf;
			int i =0;
			while ((i<inS.length()) && (inS[i] != ' '))
				ComandS +=inS[i++];
			
			if (strcmp(ComandS.c_str(),"CALC")==0)
			{
				++i;
				while (i<inS.length())
					FunctionS += inS[i++];
				// вычисляем результат
				double result = 0;
				try
				{
					result=calculate(FunctionS.c_str());
				}
				catch (char* calc_error)
				{
					cout<<"ERROR! " << calc_error<<endl;
				}
				_snprintf_s(result_string,maxlen,maxlen,"OK %f\n",result);
				// отправляем результат на сервер
				sendto(SendRecvSocket,result_string,strlen(result_string),0,(sockaddr *)&ClientAddr,sizeof(ClientAddr));
				printf("Sent answer: %s\n", result_string);
			}else{
				_snprintf_s(result_string,maxlen,maxlen,"ERROR\n");
				sendto(SendRecvSocket,result_string,strlen(result_string),0,(sockaddr *)&ClientAddr,sizeof(ClientAddr));
				printf("Sent answer: %s\n", result_string);
			}
		}
		else  {
			printf("recv failed: %d\n", WSAGetLastError());
			closesocket(SendRecvSocket);
			WSACleanup();
			return 1;
		}
	}
}


