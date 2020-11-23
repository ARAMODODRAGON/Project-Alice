#include "NetworkManager.hpp"
//#include <Windows.h>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include "../Core/Debugger.hpp"
#include <future>
#include <atomic>
#include "../General/Serialization.hpp"
#include <iostream>

// prevents linkage
namespace {

	// contains data about this WSA instance
	WSADATA wsaData;

	std::atomic<NetStatus> status = NetStatus::Disconnected;
	LobbyData* lobby = nullptr;
	uint32 userID = -1;

	// socket connection to server 
	SOCKET connectSocket = INVALID_SOCKET;

	std::future<void> connectToServerFut;

	// buffer to recieve data
	constexpr size_t bufferLen = 512;
	char buffer[bufferLen];

	// some defines
	#define DEFAULT_PORT "27115"
}

void NetworkManager::Init() {
	// initialize WSA
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
		DEBUG_ERROR("Failed to initialize WSA. id:" + WSAGetLastError());
		throw;
	}
}

void NetworkManager::Exit() {
	// close WSA
	WSACleanup();
}

void ConnectToServer(const uint32 lobbyID = -1) {
	status = NetStatus::Loading;

	struct addrinfo* result = nullptr;
	struct addrinfo hints;
	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	// create socket to server
	if (getaddrinfo("localhost", DEFAULT_PORT, &hints, &result) != 0) {
		DEBUG_ERROR("Failed to get address info!");
		status = NetStatus::Failed;
		return;
	}

	// try to find a socket to connect through
	for (struct addrinfo* ptr = result; ptr != NULL; ptr = ptr->ai_next) {

		// create a socket for connecting to server
		connectSocket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
		if (connectSocket == INVALID_SOCKET) {
			DEBUG_ERROR("Failed to connect to socket!");
			freeaddrinfo(result);
			status = NetStatus::Failed;
			return;
		}

		// connect to server
		if (connect(connectSocket, ptr->ai_addr, (int)ptr->ai_addrlen)
			== SOCKET_ERROR) {
			closesocket(connectSocket);
			connectSocket = INVALID_SOCKET;
			continue;
		}

		// connection made
		break;
	}

	// cleanup address info
	freeaddrinfo(result);

	// extra check in case we failed
	if (connectSocket == INVALID_SOCKET) {
		DEBUG_ERROR("unable to connect to server");
		status = NetStatus::Failed;
		return;
	}

	// connection has been made

	// create a lobby
	if (lobbyID == -1) {

		status = NetStatus::CreatingLobby;

		// request to create a lobby
		json j = { { "type", "create lobby" } };

		// get as string to send to server
		string s = j.dump(-1);

		std::cout << "test1" << std::endl;
		// try to send data over
		if (send(connectSocket, s.c_str(), s.size() + 1, 0) == SOCKET_ERROR) {
			DEBUG_ERROR("Failed to create lobby");
			closesocket(connectSocket);
			status = NetStatus::Failed;
			return;
		}

		std::cout << "test2" << std::endl;
		// wait for server response
		int32 bytesrecv = recv(connectSocket, buffer, bufferLen, 0);
		if (bytesrecv == SOCKET_ERROR) {
			DEBUG_ERROR("Failed to receive data from server!");
			closesocket(connectSocket);
			status = NetStatus::Failed;
			return;
		}

		// read back into json object
		j = buffer;

		// read data about the created lobby

		// should look like
		/* "type": "lobby created", "ID": number, "userID": number */

		s = j["type"];


		std::cout << "test3" << std::endl;
		if (s != "lobby created") {
			DEBUG_LOG("Got incorrect response from server? response: " + j.get<string>());
			closesocket(connectSocket);
			status = NetStatus::Failed;
			return;
		}

		// store the userID
		userID = j["userID"];

		std::cout << "test4" << std::endl;
		// create and store the lobby data
		LobbyData* data = new LobbyData();
		data->ID = j["ID"];
		data->thisIsOwner = true;
		lobby = data;

	}
	// try to join a lobby with the given ID
	else {

		status = NetStatus::JoiningLobby;

		// create request to join lobby with given ID
		json j = {
			{ "type", "join lobby" },
			{ "ID", lobbyID }
		};
		string s = j.dump(-1);

		// send to server
		if (send(connectSocket, s.c_str(), s.size() + 1, 0) == SOCKET_ERROR) {
			DEBUG_ERROR("Failed to create lobby");
			closesocket(connectSocket);
			status = NetStatus::Failed;
			return;
		}

		// wait for server response
		int32 bytesrecv = recv(connectSocket, buffer, bufferLen, 0);
		if (bytesrecv == SOCKET_ERROR) {
			DEBUG_ERROR("Failed to receive data from server!");
			closesocket(connectSocket);
			status = NetStatus::Failed;
			return;
		}

		// read back into json object
		j = buffer;

		// should look like
		/* "type": "joined lobby", "ID": number, "userID": number */
		// or
		/* "type": "failed to join lobby", "info": message */

		if (!j["type"].is_string()) {
			DEBUG_LOG("Got incorrect response from server? response: " + j.get<string>());
			closesocket(connectSocket);
			status = NetStatus::Failed;
			return;
		}

		s = j["type"];

		// joined lobby
		if (s == "joined lobby") {
			// store lobby data
			LobbyData* data = new LobbyData();
			data->ID = j["ID"].get<uint32>();
			data->thisIsOwner = false;
			lobby = data;

			// store the userID
			userID = j["userID"];
		}
		// failed to join
		else if (s == "failed to join lobby") {
			DEBUG_LOG("Cannot connect to lobby with given ID");
			closesocket(connectSocket);
			status = NetStatus::Disconnected;
			return;
		}

	}

	// clean up
	closesocket(connectSocket);
	connectSocket = INVALID_SOCKET;

	// we are connnected if the function didnt exit early
	status = NetStatus::Connected;
}

void NetworkManager::CreateLobby() {
	if (connectToServerFut.valid() || lobby) {
		DEBUG_WARNING("Connection already running!");
		return;
	}

	// start the ConnectToServer function
	connectToServerFut = std::async(ConnectToServer, -1);
}

void NetworkManager::JoinLobby(const uint32 lobbyID) {
	if (connectToServerFut.valid() || lobby) {
		DEBUG_WARNING("Connection already running!");
		return;
	}

	// start the ConnectToServer function
	connectToServerFut = std::async(ConnectToServer, lobbyID);
}

void NetworkManager::ExitLobby() {
	DEBUG_ERROR("Implement ExitLobby");
}

NetStatus NetworkManager::GetStatus() {
	return status;
}

const LobbyData* NetworkManager::GetLobby() {
	return lobby;
}

uint32 NetworkManager::GetUserID() {
	return userID;
}
