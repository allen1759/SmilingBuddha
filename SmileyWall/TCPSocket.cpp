//CONNECT TO REMOTE HOST (CLIENT APPLICATION)
//Include the needed header files.
//Don't forget to link libws2_32.a to your program as well
#include <winsock.h>
#include "TCPSocket.h"

SOCKET s; //Socket handle

//CONNECTTOHOST ¡V Connects to a remote host
bool TCPSocket::ConnectToHost(int PortNo, char* IPAddress)
{
	//Start up Winsock¡K
	WSADATA wsadata;

	int error = WSAStartup(0x0202, &wsadata);

	//Did something happen?
	if (error)
		return false;

	//Did we get the right Winsock version?
	if (wsadata.wVersion != 0x0202)
	{
		WSACleanup(); //Clean up Winsock
		return false;
	}

	//Fill out the information needed to initialize a socket¡K
	SOCKADDR_IN target; //Socket address information

	target.sin_family = AF_INET; // address family Internet
	target.sin_port = htons (PortNo); //Port to connect on
	target.sin_addr.s_addr = inet_addr (IPAddress); //Target IP

	s = socket (AF_INET, SOCK_STREAM, IPPROTO_TCP); //Create socket
	if (s == INVALID_SOCKET)
	{
		return false; //Couldn't create the socket
	}  

	//Try connecting...

	if (connect(s, (SOCKADDR *)&target, sizeof(target)) == SOCKET_ERROR)
	{
		return false; //Couldn't connect
	}
	else
		return true; //Success
}

//CLOSECONNECTION ¡V shuts down the socket and closes any connection on it
void TCPSocket::CloseConnection ()
{
	//Close the socket if it exists
	if (s)
		closesocket(s);

	WSACleanup(); //Clean up Winsock
}