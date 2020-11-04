#include <iostream>
#include <nlohmann/json.hpp>
using nlohmann::json;

#include <vector>
#include <string>
using std::string;

#include <WinSock2.h>
#include <WS2tcpip.h>

struct Lobby {
	uint32_t ID;
	std::vector<uint32_t> connections;
};

// prevents linkage
namespace {

	// state
	bool isRunning = true;

	// contains data about this WSA instance
	WSADATA wsaData;

	// sockets
	SOCKET listenSocket = INVALID_SOCKET;
	SOCKET clientSocket = INVALID_SOCKET;

	// buffer to recieve data
	constexpr size_t bufferLen = 512;
	char buffer[bufferLen];

	// some defines
	#define DEFAULT_PORT "27115"

	// the lobbies
	std::vector<Lobby> lobbies;

	// counters
	uint32_t lobbyCounter = 0; // counts for each lobby created
	uint32_t userID = 0; // counts for each user, can be used to check if a user is still connected

}

// listens and responds
void MainLoop() {

	while (isRunning) {

		clientSocket = INVALID_SOCKET;

		if (listen(listenSocket, SOMAXCONN) == SOCKET_ERROR) {
			printf("listen failed with error: %d\n", WSAGetLastError());
			return;
		}

		printf("waiting for a connection request...\n");
		// accept a client socket
		clientSocket = accept(listenSocket, NULL, NULL);
		if (clientSocket == INVALID_SOCKET) {
			printf("accept failed with error: %d\n", WSAGetLastError());
			continue;
		}

		// get message from client
		int iresult = recv(clientSocket, buffer, bufferLen, 0);
		if (iresult == 0) {
			printf("connection is closing...\n");
		} else {
			printf("recv failed with error: %d\n", WSAGetLastError());
			closesocket(clientSocket);
			continue;
		}

		// convert to json object
		json j = buffer;

		// should look like one of the following

		/* "type": "create lobby" */
		/* "type": "join lobby", "ID": lobbyID */

		if (!j["type"].is_string()) {
			closesocket(clientSocket);
			continue;
		}

		string s = j["type"];

		// create a lobby
		if (s == "create lobby") {



		}
		// join an already existing lobby
		else if (s == "join lobby") {

		}

		// close and loop back around
		closesocket(clientSocket);
	}
}

int main(int argc, char* argv[]) {

	// initialize WSA
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
		printf("Failed to initialize WSA. id:%i", WSAGetLastError());
		return 1;
	}

	struct addrinfo* result = nullptr;
	struct addrinfo hints;
	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;

	// resolve the server address and port
	if (getaddrinfo(NULL, DEFAULT_PORT, &hints, &result) != 0) {
		printf("getaddrinfo failed: %d\n", WSAGetLastError());
		WSACleanup();
		return 1;
	}

	// create a socket for connecting
	listenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	if (listenSocket == INVALID_SOCKET) {
		printf("Socket failed with error: %d\n", WSAGetLastError());
		freeaddrinfo(result);
		WSACleanup();
		system("pause");
		return 1;
	}

	// setup the TCP listening socket
	if (bind(listenSocket, result->ai_addr, (int)result->ai_addrlen) == SOCKET_ERROR) {
		printf("bind failed with error: %d\n", WSAGetLastError());
		freeaddrinfo(result);
		closesocket(listenSocket);
		WSACleanup();
		system("pause");
		return 1;
	}

	// free adress info
	freeaddrinfo(result);

	// start the main loop
	MainLoop();

	// close WSA
	WSACleanup();


	return 0;
}