// sockserv.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "winsock2.h"
#include "iphlpapi.h"
#include "..\fsend.h"
#include <iostream>
#include <fstream>
#include <queue>

using namespace std;

void main(int argc, char **argv) {
// Initialize Winsock.
    WSADATA wsaData;
    int iResult = WSAStartup( MAKEWORD(2,2), &wsaData );
    if ( iResult != NO_ERROR )
        cout<<"Error at WSAStartup()"<<endl;

// Create a socket.
    SOCKET m_socket, ferst_m_socket;
    m_socket = socket( AF_INET, SOCK_STREAM, IPPROTO_TCP );
	ferst_m_socket= socket( AF_INET, SOCK_STREAM, IPPROTO_TCP );
    if ( m_socket == INVALID_SOCKET ) {
        cout<<"Error at socket(): "<< WSAGetLastError() ;
        WSACleanup();
        return;
    }

// Bind the socket.
    sockaddr_in service;

    service.sin_family = AF_INET;
	if (argc > 1)
	{
		service.sin_addr.s_addr = inet_addr( argv[1] );
		cout << "Address to connect  " << argv[1] << ":";
	}
	else
	{
		service.sin_addr.s_addr = inet_addr( "127.0.0.1" );
		printf( "Address to connect  127.0.0.1:");
	}
	if (argc > 2)
	{
		service.sin_port = htons( atoi(argv[2]) );
		cout << argv[2] << endl;
	}
	else
	{
		service.sin_port = htons( 27015 );
		cout << 27015 << endl;
	}


    if ( bind( m_socket, (SOCKADDR*) &service, sizeof(service) ) == SOCKET_ERROR ) {
        printf( "bind() failed.\n" );
        closesocket(m_socket);
		system("pause");
        return;
    }
    
    // Listen on the socket.
    if ( listen( m_socket, 1 ) == SOCKET_ERROR )
		printf( "Error listening on socket.\n");
	// Accept connections.
		SOCKET AcceptSocket;
		ferst_m_socket = m_socket;
	while (1)
	{   
		printf( "Waiting for a client to connect...\n" );
		m_socket = ferst_m_socket;
		
		AcceptSocket = SOCKET_ERROR;
		while ( AcceptSocket == SOCKET_ERROR ) 
		{
			AcceptSocket = accept( m_socket, NULL, NULL );
		}
		printf( "Client Connected.\n");
		m_socket = AcceptSocket; 
    
		// receive data.
		int bytesRecv = SOCKET_ERROR;
		FPacket fp;
		DWORD filesize=0L,recvdsize=0L;
		char *filename=0;
		ofstream* ofs=0;

		while (1){
			bytesRecv = recv( m_socket,(char *)&fp, sizeof(fp), 0 );
			if (fp._type==FPSize)
			{
				filesize=fp._data._len;
				cout<<"Filesize "<<filesize<<endl;
				break;
			}
			else
			{
				cout<<"Didn't recieve file size"<<endl;
				system("pause");
				return;
			}
		}
		while (1)
		{
			bytesRecv = recv( m_socket,(char *)&fp, sizeof(fp), 0 );
			if (fp._type==FPName)
			{
				filename=new char[fp._size+1];
				//cout<<"Size of filename "<<fp._size<<endl;
				memcpy(filename,fp._data._binary,fp._size);
				filename[fp._size]='\0';
				cout<<"Filename "<<filename<<" "<<(int)filename[0]<<endl;
				break;
			}
		}
		if ( (filename!=0)&&(filesize!=0L) )
		{
			if (ofs==0)
			{
				ofs=new	ofstream;
				ofs->open(filename,ios_base::binary|ios_base::out|ios_base::trunc);
				if (!ofs->is_open())
				{
					cout<<"Can't create	file - exit!!"<<endl;
					system("pause");
					return;
				}
			}
			char *buf=new char[filesize];
			int	recvdsize=recv(	m_socket,buf, filesize,	0 );
			ofs->write((const char*)buf,filesize);
			ofs->close();
			cout<<"File	written: bytes recieved	"<<recvdsize<<endl;
		}
		else
		{
			cout<<"Not enough parameters recieved -	exit!!"<<endl;
			system("pause");
			return;
		}
	}
    return;
}

