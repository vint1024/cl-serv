// sockclie.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "winsock2.h"
#include "..\fsend.h"
#include <iostream>
#include <fstream>
#include <stdio.h>
using namespace std;

void main(int argc, char **argv) {

    // Initialize Winsock.
    WSADATA wsaData;
    int iResult = WSAStartup( MAKEWORD(2,2), &wsaData );
    if ( iResult != NO_ERROR )
	{
        cout<<"Error at WSAStartup()\n";
        system("pause");
		return;
	}

	// Create a socket.
    SOCKET m_socket;
    m_socket = socket( AF_INET, SOCK_STREAM, IPPROTO_TCP );

    if ( m_socket == INVALID_SOCKET ) {
        printf( "Error at socket(): %ld\n", WSAGetLastError() );
        WSACleanup();
		system("pause");
        return;
    }

    // Connect to a server.
    sockaddr_in clientService;

    clientService.sin_family = AF_INET;
	if (argc > 1)
	{
		clientService.sin_addr.s_addr = inet_addr( argv[1] );
		cout << "Connecting to "<< argv[1]<< ":";
	}
	else
	{
		clientService.sin_addr.s_addr = inet_addr( "127.0.0.1" );
		cout << "Connecting to "<< "127.0.0.1:";
	}
	if (argc > 2)
	{
		clientService.sin_port = htons( atoi(argv[2]) );
		cout << argv[2] << endl;
	}
	else
	{
		clientService.sin_port = htons( 27015 );
		cout << 27015 << endl;
	}
    if ( connect( m_socket, (SOCKADDR*) &clientService, sizeof(clientService) ) == SOCKET_ERROR) {
        cout<<"Failed to connect.\n";
        WSACleanup();
		system("pause");
        return;
    }

	// Get file size (using MS functions)
	WIN32_FIND_DATA FindFileData;
	HANDLE hFind;
	char tmp[1024];
	
	if (argc <3 )
	{
		cout << "Fine name --> ";
		cin.getline(tmp,1024);
	}
	if (argc > 3 ) cout<<argv[3]<<endl;
	if (argc > 3 ) 
		hFind = FindFirstFile(argv[3], &FindFileData);
	else
		hFind = FindFirstFile(tmp, &FindFileData);
	if (hFind == INVALID_HANDLE_VALUE){
		cout<<"Invalid File Handle. GetLastError reports "<<GetLastError()<<endl;
		system("pause");
		return;
	} 
	FindClose(hFind);

	ifstream ifs;
	if (argc > 3) 
		ifs.open(argv[3],ios_base::binary);
	else
		ifs.open(tmp,ios_base::binary);
	if (!ifs.is_open())	{
		cout<<"Error: file not opened!"<<endl;
		system("pause");
		return;
	}

	FPacket fp(FindFileData.nFileSizeLow);
	int bytesSent = send( m_socket,(const char *) &fp, sizeof(fp), 0 );
    cout<<"Bytes Sent: "<<bytesSent<<endl;
	if (argc >3 )
		fp=FPacket(argv[3]);
	else
		fp=FPacket(tmp);
    bytesSent = send( m_socket,(const char *) &fp, sizeof(fp), 0 );
    cout<<"Bytes Sent: "<<bytesSent<<endl;
	if (argc >3 )
		cout<<"file name "<<argv[3]<<endl;
	else
		cout<<"file name "<<tmp<<endl;

	char *buf=new char[FindFileData.nFileSizeLow];
	if (buf!=0)
	{
		int k=0;
        while (ifs.peek()!=EOF)
		{
			if (ifs.peek()==EOF)
					break;
			buf[k]=ifs.get();
			k++;
		}
		bytesSent = send( m_socket,buf, k, 0 );
	    cout<<"Bytes Sent: "<<bytesSent<<endl;
	}
	else
	{
		cout<<"can't send file"<<endl;
		system("pause");
	}
	delete[] buf;
    return;
}
