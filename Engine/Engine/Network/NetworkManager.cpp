#include "NetworkManager.hpp"
//#include <Windows.h>
#include <WinSock2.h>
#include <WS2tcpip.h>

namespace {

	WSADATA wsaData;

	NetStatus status = NetStatus::Disconnected;
	LobbyData* lobby;

}

void NetworkManager::Init() { }

void NetworkManager::Exit() { }

void NetworkManager::CreateLobby() { }

void NetworkManager::JoinLobby(const uint32 lobbyID) { }

void NetworkManager::ExitLobby() { }

NetStatus NetworkManager::GetStatus() {
	return status;
}

const LobbyData* NetworkManager::GetLobby() {
	return nullptr;
}
