#ifndef NETWORK_NETWORK_MANAGER_HPP
#define NETWORK_NETWORK_MANAGER_HPP
#include "../General/Macros.hpp"
#include "../General/Types.hpp"
//#include <Windows.h>
#include <WinSock2.h>
#include <WS2tcpip.h>

// is this a client / server 
enum class NSystemType : uint8 {
	None,
	Client,
	Server
};

class NetworkManager {
	PRIVATE_SINGLETON(NetworkManager);

	NetworkManager();
	~NetworkManager();

	// server will listen to this port
	// client will try to connect to this port
	string targetPort;

public:

	// initializes the network manager
	static void Init(const string& targetPort_);

	// deinits the manager
	static void Exit();


};

#endif // !NETWORK_NETWORK_MANAGER_HPP