#ifndef INCLUDED_TCPClient_H
#define INCLUDED_TCPClient_H

class TCPClient {
public:
	TCPClient(char* servername, char* port)
	{
		szServerName = servername;
		szServerPort = port;
		ConnectSocket = INVALID_SOCKET;
	}

	bool Start();

	// Free the resouces
	void Stop();

	// Send message to server
	bool Send(char* szMsg);

	// Receive message from server
	bool Recv();

private:
	char* szServerName;
	char* szServerPort;
	SOCKET ConnectSocket;
};

#endif