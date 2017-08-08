class TCPSocket{

public:
	SOCKET s; //Socket handle

	//CONNECTTOHOST ¡V Connects to a remote host
	bool ConnectToHost(int PortNo, char* IPAddress);

	//CLOSECONNECTION ¡V shuts down the socket and closes any connection on it
	void CloseConnection ();

	
};